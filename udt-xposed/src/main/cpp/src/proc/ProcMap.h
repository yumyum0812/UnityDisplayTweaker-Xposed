#pragma once

#include <android/log.h>
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
    uint64_t length;
    std::string permission;
    bool readable;
    bool writable;
    bool executable;
    bool shared;
    uint64_t offset;
    int devMajor;
    int devMinor;
    int inode;
    std::string path;

private:
    explicit ProcMap(const std::string& line) {
        std::string addrTmp;
        std::string devTmp;

        std::istringstream ss(line);
        ss >> addrTmp >> permission >> std::hex >> offset >> devTmp >> std::dec >> inode >> std::ws;
        std::getline(ss, path);

        addrTmp.replace(addrTmp.find('-'), 1, " ");
        devTmp.replace(devTmp.find(':'), 1, " ");

        readable = (permission[0] == 'r');
        writable = (permission[1] == 'w');
        executable = (permission[2] == 'x');
        shared = (permission[3] == 's');

        std::istringstream addrSS(addrTmp);
        addrSS >> std::hex >> start >> end;
        length = end - start;

        std::istringstream devSS(devTmp);
        devSS >> std::dec >> devMajor >> devMinor;
    }

public:
    explicit ProcMap() : start(0), end(0), length(0), permission(""), readable(false), writable(false), executable(false), shared(false), offset(0), devMajor(0), devMinor(0), inode(0) {}
    
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