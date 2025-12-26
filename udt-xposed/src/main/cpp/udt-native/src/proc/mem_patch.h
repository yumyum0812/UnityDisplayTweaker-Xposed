#pragma once

#include <android/log.h>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <istream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <ranges>
#include <algorithm>
#include <sstream>

#include <sys/mman.h>
#include <bits/sysconf.h>

#include "proc_map.h"


class MemPatch {
    uintptr_t addr = 0;
    size_t length = 0;
    bool applied = false;
    std::vector<uint8_t> modBytes;
    bool cached = false;
    std::vector<uint8_t> origBytes;
    std::map<uintptr_t, int> origProts;

    static inline size_t GetPageSize() {
        return sysconf(_SC_PAGE_SIZE);
    }

    static uintptr_t GetPageStart(uintptr_t address) {
        return address & ~(GetPageSize() - 1);
    }

    bool SetProts(int prot) {
        return std::ranges::all_of(origProts.begin(), origProts.end(), [prot](const auto p) {
            return mprotect((void*) p.first, GetPageSize(), prot) == 0;
        });
    }

    bool ResetProts() {
        return std::ranges::all_of(origProts.begin(), origProts.end(), [](const auto p) {
            return mprotect((void*) p.first, GetPageSize(), p.second) == 0;
        });
    }

    void Write(bool mod) {
        std::memcpy((void*) addr, mod ? modBytes.data() : origBytes.data(), length);
        __builtin___clear_cache((char*) addr, (char*)(addr + length));
    }

    void CacheProts() {
        std::vector<ProcMap> maps = ProcMap::GetAllMaps();
        size_t pageSize = GetPageSize();
        uintptr_t startPage = GetPageStart(addr);
        uintptr_t endPage = GetPageStart(addr + length - 1);

        for (uintptr_t page = startPage; page <= endPage; page += pageSize) {
            bool found = false;
            for (ProcMap& m: maps) {
                if (page >= m.start && page < m.end) {
                    int prot = 0;
                    if (m.readable) prot |= PROT_READ;
                    if (m.writable) prot |= PROT_WRITE;
                    if (m.executable) prot |= PROT_EXEC;
                    origProts[page] = prot;
                    found = true;
                    break;
                }
            }
            if (!found) {
                origProts[page] = PROT_READ | PROT_WRITE | PROT_EXEC;
            }
        }
    }

    void SetupCache() {
        if (cached) return;
        cached = true;

        CacheProts();

        SetProts(PROT_READ | PROT_WRITE | PROT_EXEC);
        std::memcpy(origBytes.data(), (void*) addr, length);
        ResetProts();
    }

    void Init(uintptr_t address, const std::vector<uint8_t>& bytes) {
        addr = address;
        length = bytes.size();
        modBytes = bytes;
        origBytes = std::vector<uint8_t>(modBytes.size());
    }

public:
    MemPatch(uintptr_t address, const std::vector<uint8_t>& bytes) {
        Init(address, bytes);
    }

    MemPatch(uintptr_t address, const std::string& hexBytes) {
        std::vector<uint8_t> bytes;
        std::istringstream ss(hexBytes);
        std::string byteStr;

        while (ss >> byteStr) {
            if (byteStr.size() != 2)
                throw std::invalid_argument("Invalid hex byte format.");
            bytes.push_back((uint8_t) std::stoi(byteStr, nullptr, 16));
        }

        Init(address, bytes);
    }

    bool Modify() {
        if (applied) return true;
        if (length < 1) return false;

        SetupCache();

        if (!SetProts(PROT_READ | PROT_WRITE | PROT_EXEC)) return false;
        Write(true);
        if (!ResetProts()) return false;

        applied = true;
        return true;
    }

    bool Restore() {
        if (!applied || !cached) return true;

        if (!SetProts(PROT_READ | PROT_WRITE | PROT_EXEC)) return false;
        Write(false);
        if (!ResetProts()) return false;

        applied = false;
        return true;
    }

    ~MemPatch() {
        if (applied) Restore();
    }
};