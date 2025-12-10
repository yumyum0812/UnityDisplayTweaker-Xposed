#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>


struct ProcMap {
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

private:
    explicit ProcMap(const std::string& line) {
        std::string addrTmp;
        std::string devTmp;
        std::string permTmp;

        // address (start-end)  perms  offset  dev  inode  pathname
        // X-X [r-][w-][x-][ps] 8D 2X:2X D    .*
        std::istringstream ss(line);
        ss >> addrTmp >> permTmp >> std::hex >> offset >> devTmp >> std::dec >> inode >> std::ws;
        std::getline(ss, path);

        addrTmp.replace(addrTmp.find('-'), 1, " ");
        devTmp.replace(devTmp.find(':'), 1, " ");

        readable = (permTmp[0] == 'r');
        writable = (permTmp[1] == 'w');
        executable = (permTmp[2] == 'x');
        shared = (permTmp[3] == 's');

        std::istringstream addrSS(addrTmp);
        addrSS >> std::hex >> start >> end;

        std::istringstream devSS(devTmp);
        devSS >> std::dec >> devMajor >> devMinor;
    }

public:
    explicit ProcMap() : start(0), end(0), readable(false), writable(false), executable(false), shared(false), offset(0), devMajor(0), devMinor(0), inode(0) {}

    uint64_t GetLength() const {
        return end - start;
    }

    std::string GetFullPermission() const {
        std::string perm;
        perm += readable ? 'r' : '-';
        perm += writable ? 'w' : '-';
        perm += executable ? 'x' : '-';
        perm += shared ? 's' : 'p';
        return perm;
    }

    static std::vector<ProcMap> GetAllMaps(pid_t pid) {
        std::ifstream maps("/proc/" + std::to_string(pid) + "/maps");
        if (!maps.is_open()) {
            throw std::runtime_error("Failed to open memory maps.");
        }

        std::vector<ProcMap> out;
        std::string line;
        while (std::getline(maps, line)) {
            ProcMap map(line);
            out.push_back(map);
        }

        return out;
    }

    static std::vector<ProcMap> GetAllMaps() {
        std::ifstream maps("/proc/self/maps");
        if (!maps.is_open()) {
            throw std::runtime_error("Failed to open memory maps.");
        }

        std::vector<ProcMap> out;
        std::string line;
        while (std::getline(maps, line)) {
            ProcMap map(line);
            out.push_back(map);
        }

        return out;
    }
};