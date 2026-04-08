#include <string>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include <mimem/mem_map.h>

using namespace MiMem;


static bool IsBlank(const std::string& s) {
    if (s.empty()) return true;
    return std::all_of(s.begin(), s.end(), [](unsigned char ch) {
        return std::isspace(ch);
    });
}

// start-end  perms  offset  dev  inode  pathname
// ?X-?X  [r-][w-][x-][ps]  8D  2X:2X  ?D  (...)
static void TryParseAndAppend(const std::string& line, std::vector<MemMap>& vec) {
    if (IsBlank(line))
        return;

    auto m = MemMap();

    std::string addrTmp;
    std::string devTmp;
    std::string permTmp;

    std::istringstream ss(line);
    if (!(ss >> addrTmp >> permTmp >> std::hex >> m.offset >> devTmp >> std::dec >> m.inode >> std::ws)) return;

    std::getline(ss, m.path);

    auto addrD = addrTmp.find('-');
    if (addrD == std::string::npos) return;
    addrTmp.replace(addrD, 1, " ");

    auto devD = devTmp.find(':');
    if (devD == std::string::npos) return;
    devTmp.replace(devD, 1, " ");

    m.readable = (permTmp[0] == 'r');
    m.writable = (permTmp[1] == 'w');
    m.executable = (permTmp[2] == 'x');
    m.shared = (permTmp[3] == 's');

    std::istringstream addrSS(addrTmp);
    if (!(addrSS >> std::hex >> m.start >> m.end)) return;

    std::istringstream devSS(devTmp);
    if (!(devSS >> std::hex >> m.devMajor >> m.devMinor)) return;

    vec.push_back(m);
}

std::vector<MemMap> MiMem::GetAllMaps() {
    std::ifstream maps("/proc/self/maps");
    if (!maps.is_open()) {
        throw std::runtime_error("failed to open memory maps.");
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(maps, line)) {
        lines.push_back(line);
    }

    std::vector<MemMap> out;
    for (const auto& l : lines) {
        TryParseAndAppend(l, out);
    }

    return out;
}