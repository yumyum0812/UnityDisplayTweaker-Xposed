#pragma once

#include <cstdint>

#include "bit_util.h"

namespace Arm64Util {
    uint32_t ReadInstruction(const uint8_t* p) {
        return uint32_t(p[0])        |
              (uint32_t(p[1]) << 8)  |
              (uint32_t(p[2]) << 16) |
              (uint32_t(p[3]) << 24);
    }

    bool IsInstrBL(uint32_t instr) {
        // BL
        if ((instr >> 26) == 0b100101)
            return true;

        return false;
    }

    int32_t ExtractOffsetBL(uint32_t instr) {
        // 1001 01II IIII IIII IIII IIII IIII IIII
        uint32_t imm26 = instr & BitUtil::BitMask<uint32_t, 26>();
        uint32_t raw = (imm26 << 2);

        // 28bit → 32bit
        raw = BitUtil::SignExtend<uint32_t, 28>(raw);

        return static_cast<int32_t>(raw);
    }
}