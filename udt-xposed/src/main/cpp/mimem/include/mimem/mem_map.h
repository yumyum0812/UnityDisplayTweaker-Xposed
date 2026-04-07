#pragma once

#include <string>
#include <vector>
#include <cstdint>


namespace MiMem {
    struct MemMap {
        uint64_t start;
        uint64_t end;
        bool readable;
        bool writable;
        bool executable;
        bool shared;
        uint64_t offset;
        uint16_t devMajor;
        uint16_t devMinor;
        uint64_t inode;
        std::string path;

    public:
        explicit MemMap():
                start(0), end(0),
                readable(false), writable(false), executable(false), shared(false),
                offset(0), devMajor(0), devMinor(0), inode(0)
        {}
    };

    std::vector<MemMap> GetAllMaps();
}