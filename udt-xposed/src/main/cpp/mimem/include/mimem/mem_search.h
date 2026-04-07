#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace MiMem {
    std::vector<uintptr_t> MemorySearch(uintptr_t addressStart, uintptr_t addressEnd, const std::string& hexPattern, int maxResults = 0);
}