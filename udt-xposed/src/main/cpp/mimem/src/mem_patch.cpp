#include <mimem/mem_patch.h>
#include <mimem/mem_map.h>
#include <sstream>
#include <sys/mman.h>
#include <bits/sysconf.h>

using MemPatch = MiMem::MemPatch;
using MemMap = MiMem::MemMap;


static std::vector<uint8_t> ParseHexBytes(const std::string& hexBytes) {
    std::vector<uint8_t> bytes;
    std::istringstream ss(hexBytes);

    std::string bTmp;
    while (ss >> bTmp) {
        if (bTmp.size() != 2)
            throw std::invalid_argument("Invalid hex byte format.");

        auto b = (uint8_t) std::stoi(bTmp, nullptr, 16);
        bytes.push_back(b);
    }

    return bytes;
}

static size_t GetPageSize() {
    return sysconf(_SC_PAGE_SIZE);
}

static uintptr_t GetPageStart(uintptr_t address) {
    return address & ~(GetPageSize() - 1);
}

static void GetProts(uintptr_t base, uintptr_t len, std::map<uintptr_t, int>& out) {
    out.clear();

    std::vector<MemMap> maps = MiMem::GetAllMaps();
    size_t ps = GetPageSize();
    uintptr_t start = GetPageStart(base);
    uintptr_t end = GetPageStart(base + len - 1);

    for (uintptr_t page = start; page <= end; page += ps) {
        bool found = false;
        for (auto& m: maps) {
            if (page >= m.start && page < m.end) {
                int prot = 0;
                if (m.readable) prot |= PROT_READ;
                if (m.writable) prot |= PROT_WRITE;
                if (m.executable) prot |= PROT_EXEC;
                out[page] = prot;
                found = true;
                break;
            }
        }
        if (!found) {
            out[page] = PROT_READ | PROT_WRITE | PROT_EXEC;
        }
    }
}

static void SetProt(uintptr_t base, uintptr_t len, int prot) {
    size_t ps = GetPageSize();
    uintptr_t start = GetPageStart(base);
    uintptr_t end = GetPageStart(base + len - 1);

    for (uintptr_t page = start; page <= end; page += ps) {
        mprotect((void*) page, ps, prot);
    }
}

static void RestoreProts(const std::map<uintptr_t, int>& sets) {
    size_t ps = GetPageSize();
    for (const auto& [page, prot] : sets) {
        mprotect((void*) page, ps, prot);
    }
}

static void ReadData(uintptr_t base, size_t len, void* dest) {
    std::memcpy(dest, (void*) base, len);
}

static void WriteData(uintptr_t addr, size_t len, const void* data) {
    std::memcpy((void*) addr, data, len);
    __builtin___clear_cache((char*) addr, (char*)(addr + len));
}


MemPatch::MemPatch(uintptr_t baseAddress, const std::vector<uint8_t>& bytes) {
    if (bytes.empty())
        throw std::runtime_error("given bytes are empty");

    baseAddr = baseAddress;
    len = bytes.size();
    modBytes = bytes;
    origBytes.resize(len);
}

void MemPatch::Modify() {
    if (on) return;

    if (!cacheOk) {
        GetProts(baseAddr, len, origProts);
        ReadData(baseAddr, len, origBytes.data());
        cacheOk = true;
    }

    SetProt(baseAddr, len, PROT_READ | PROT_WRITE | PROT_EXEC);
    WriteData(baseAddr, len, modBytes.data());
    RestoreProts(origProts);
    on = true;
}

void MemPatch::Restore() {
    if (!on) return;

    SetProt(baseAddr, len, PROT_READ | PROT_WRITE | PROT_EXEC);
    WriteData(baseAddr, len, origBytes.data());
    RestoreProts(origProts);
    on = false;
}

void MemPatch::SetEnabled(bool state) {
    if (state)
        return MemPatch::Modify();
    else
        return MemPatch::Restore();
}


void MiMem::Patch(uintptr_t address, const std::vector<uint8_t>& bytes) {
    if (bytes.empty())
        throw std::runtime_error("given bytes are empty");

    auto len = bytes.size();
    std::map<uintptr_t, int> origProts;

    GetProts(address, len, origProts);
    SetProt(address, len, PROT_READ | PROT_WRITE | PROT_EXEC);
    WriteData(address, len, bytes.data());
    RestoreProts(origProts);
}

void MiMem::HexPatch(uintptr_t address, const std::string& hexBytes) {
    auto bytes = ParseHexBytes(hexBytes);
    MiMem::Patch(address, bytes);
}

MemPatch MiMem::CreateHexPatch(uintptr_t address, const std::string& hexBytes) {
    auto bytes = ParseHexBytes(hexBytes);
    return {address, bytes};
}