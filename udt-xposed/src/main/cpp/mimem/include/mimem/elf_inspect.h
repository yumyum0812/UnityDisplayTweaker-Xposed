#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace MiMem {
    struct MemSegment {
        uintptr_t base = 0;
        uintptr_t size = 0;
        bool r = false;
        bool w = false;
        bool x = false;
    };

    struct MemModule {
        std::string name;
        uintptr_t base = 0;
        std::vector<MemSegment> segments;
    };

    std::vector<MemModule> GetLoadedModules();
    uintptr_t GetBaseAddress(const std::string& filename);
    bool IsLibLoaded(const std::string& filename);
    uintptr_t FindFirstCodePattern(const std::string& filename, const std::string& pattern);
}