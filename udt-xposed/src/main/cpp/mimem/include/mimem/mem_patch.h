#pragma once
#include <cstdint>
#include <vector>
#include <map>

namespace MiMem {
    class MemPatch {
        uintptr_t baseAddr = 0;
        size_t len = 0;
        std::vector<uint8_t> modBytes;

        bool on = false;
        bool cacheOk = false;
        std::vector<uint8_t> origBytes;
        std::map<uintptr_t, int> origProts;

    public:
        MemPatch() = delete;
        MemPatch(uintptr_t address, const std::vector<uint8_t>& bytes);

        void Modify();
        void Restore();
        void SetEnabled(bool state);
    };

    void Patch(uintptr_t address, const std::vector<uint8_t>& bytes);
    void HexPatch(uintptr_t address, const std::string& hexBytes);
    MemPatch CreateHexPatch(uintptr_t address, const std::string& hexBytes);
}