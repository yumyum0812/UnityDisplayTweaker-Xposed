#pragma once

#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <cstdio>
#include <cstdint>
#include <vector>


class ProcMem {
    std::unique_ptr<std::fstream> memFs;

public:
    explicit ProcMem(pid_t pid) {
        std::string memPath("/proc/" + std::to_string(pid) + "/mem");

        memFs = std::make_unique<std::fstream>(memPath, std::ios::in | std::ios::out | std::ios::binary);
        if (!memFs->is_open()) {
            throw std::runtime_error("Failed to open memory maps.");
        }
    }

    std::vector<uint8_t> Read(uintptr_t address, size_t count) {
        std::vector<uint8_t> out(count);

        memFs->seekg(address, std::ios::beg);
        if (!memFs->good()) {
            throw std::runtime_error("Failed to seek memory location.");
        }

        memFs->read(reinterpret_cast<char*>(out.data()), count);
        if (!memFs->good()) {
            throw std::runtime_error("Failed to read memory.");
        }

        return out;
    }

    void Write(uintptr_t address, const std::vector<uint8_t>& data) {
        memFs->seekp(address, std::ios::beg);
        if (!memFs->good()) {
            throw std::runtime_error("Failed to seek memory location.");
        }
        
        memFs->write(reinterpret_cast<const char*>(data.data()), data.size());
        if (!memFs->good()) {
            throw std::runtime_error("Failed to write memory.");
        }
    }
};