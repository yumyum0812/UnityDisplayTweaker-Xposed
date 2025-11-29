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

#include <sys/mman.h>
#include <bits/sysconf.h>

#include "ProcMap.h"


static inline size_t GetPageSize() {
    return sysconf(_SC_PAGE_SIZE);
}

static inline uintptr_t GetPageStart(uintptr_t address) {
    size_t pageSize = GetPageSize();
    return address & ~(pageSize - 1);
}


namespace MemPatch {
    void Patch(uintptr_t address, const std::vector<uint8_t>& bytes) {
        if (bytes.empty()) return;

        size_t length = bytes.size();

        uintptr_t pageSize = GetPageSize();
        uintptr_t pageStart = GetPageStart(address);

        std::vector<ProcMap> maps = ProcMap::GetAllMaps();

        std::map<uintptr_t, int> pageProts;
        for (uintptr_t page = pageStart; page < (address + length); page += pageSize) {
            for (ProcMap& m: maps) {
                if (page >= m.start && page < m.end) {
                    int prot = 0;
                    if (m.readable) prot |= PROT_READ;
                    if (m.writable) prot |= PROT_WRITE;
                    if (m.executable) prot |= PROT_EXEC;
                    pageProts.insert({page, prot});
                    break;
                }
            }
        }

        for (auto const& [page, prot] : pageProts)
            if (mprotect(reinterpret_cast<void*>(page), pageSize, PROT_READ | PROT_WRITE | PROT_EXEC) != 0)
                throw std::runtime_error("Failed to modify memory protection.");

        std::memcpy(reinterpret_cast<void*>(address), bytes.data(), length);

        for (auto const& [page, prot] : pageProts)
            if (mprotect(reinterpret_cast<void*>(page), pageSize, prot) != 0)
                throw std::runtime_error("Failed to recover memory protection.");
    }

    void Patch(uintptr_t address, const std::string& hexBytes) {
        std::vector<uint8_t> bytes;
        std::istringstream ss(hexBytes);
        std::string byteStr;

        while (ss >> byteStr) {
            if (byteStr.size() != 2)
                throw std::invalid_argument("Invalid hex byte format.");
            bytes.push_back(static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16)));
        }

        MemPatch::Patch(address, bytes);
    }
};