#include <mimem/safe_mem.h>

using SafeMem = MiMem::SafeMem;

SafeMem::SafeMem() {
    std::string memPath("/proc/self/mem");

    memFs = std::make_unique<std::fstream>(memPath, std::ios::in | std::ios::out | std::ios::binary);
    if (!memFs->is_open()) {
    throw std::runtime_error("failed to open process memory");
    }
}

std::vector<uint8_t> SafeMem::Read(uintptr_t address, size_t count) {
    std::vector<uint8_t> out(count);

    memFs->seekg((off_t) address, std::ios::beg);
    if (!memFs->good()) {
        throw std::runtime_error("failed to seek memory location");
    }

    memFs->read(reinterpret_cast<char*>(out.data()), (std::streamsize) count);
    if (!memFs->good()) {
        throw std::runtime_error("failed to read memory");
    }

    return out;
}

void SafeMem::Write(uintptr_t address, const std::vector<uint8_t>& data) {
    memFs->seekp((off_t) address, std::ios::beg);
    if (!memFs->good()) {
        throw std::runtime_error("failed to seek memory location");
    }

    memFs->write(reinterpret_cast<const char*>(data.data()), (std::streamsize) data.size());
    if (!memFs->good()) {
        throw std::runtime_error("failed to write memory");
    }
}