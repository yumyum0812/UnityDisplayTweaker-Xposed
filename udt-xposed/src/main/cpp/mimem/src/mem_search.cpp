#include <mimem/mem_search.h>
#include <string>
#include <sstream>

static std::vector<int16_t> ParseHexPatterns(const std::string& hexBytes) {
    std::vector<int16_t> bytes;
    std::istringstream ss(hexBytes);

    std::string bTmp;
    while (ss >> bTmp) {
        if (bTmp == "?" || bTmp == "??") {
            bytes.push_back(-1);
            continue;
        }

        if (bTmp.size() != 2)
            throw std::invalid_argument("Invalid hex byte format.");

        auto b = (uint8_t) std::stoi(bTmp, nullptr, 16);
        bytes.push_back(b);
    }

    return bytes;
}

std::vector<uintptr_t> MiMem::MemorySearch(uintptr_t addressStart, uintptr_t addressEnd, const std::string& hexPatterns, int maxResults) {
    auto pat = ParseHexPatterns(hexPatterns);
    auto len = pat.size();
    auto pd = pat.data();

    if (addressEnd <= addressStart || len == 0) return {};

    std::vector<uintptr_t> founds;
    founds.reserve(maxResults);

    for (uintptr_t a = addressStart; a + len <= addressEnd; a++) {
        bool match = true;

        for (size_t i = 0; i < len; i++) {
            if (pd[i] == -1) continue; // ??

            bool bOk = *(uint8_t*)(a + i) == pd[i];
            if (!bOk) {
                match = false;
                break;
            }
        }

        if (match) {
            founds.push_back(a);
            if (maxResults > 0 && founds.size() >= maxResults)
                break;
        }
    }

    return founds;
}