#pragma once
#include <fstream>
#include <memory>

namespace MiMem {
    class SafeMem {
        std::unique_ptr<std::fstream> memFs;

    public:
        explicit SafeMem();

        std::vector<uint8_t> Read(uintptr_t address, size_t count);
        void Write(uintptr_t address, const std::vector<uint8_t>& data);
    };
}