#pragma once

#include <type_traits>
#include <cstdint>
#include <bit>

#include "bit_util.h"

namespace Arm32Util {
    uint32_t ReadInstruction(const uint8_t* p) {
        return uint32_t(p[0])        |
              (uint32_t(p[1]) <<  8) |
              (uint32_t(p[2]) << 16) |
              (uint32_t(p[3]) << 24);
    }

    bool IsInstrImmBL(uint32_t instr) {
        // CCCC 1011 IIII IIII IIII IIII IIII IIII

        if ((instr >> 28) == 0b1111)
            return false;

        if (((instr >> 24) & 0b1111) != 0b1011)
            return false;

        return true;
    }

    bool IsInstrImmBLX(uint32_t instr) {
        // 1111 101H IIII IIII IIII IIII IIII IIII

        if ((instr >> 28) != 0b1111)
            return false;

        if (((instr >> 25) & 0b111) != 0b101)
            return false;

        return true;
    }

    int32_t ExtractImmOffsetBL(uint32_t instr) {
        // CCCC 1011 IIII IIII IIII IIII IIII IIII

        uint32_t imm24 = instr & BitUtil::BitMask<uint32_t, 24>();

        uint32_t raw = imm24 << 2;

        // 26bit → 32bit
        raw = BitUtil::SignExtend<uint32_t, 26>(raw);

        return static_cast<int32_t>(raw);
    }

    int32_t ExtractImmOffsetBLX(uint32_t instr) {
        // 1111 101H IIII IIII IIII IIII IIII IIII

        uint32_t imm24 = instr & BitUtil::BitMask<uint32_t, 24>();
        bool h = (instr >> 24) & 1;

        uint32_t raw = (imm24 << 2) | (h << 1);

        // 26bit → 32bit
        raw = BitUtil::SignExtend<uint32_t, 26>(raw);

        return static_cast<int32_t>(raw);
    }
}