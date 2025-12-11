/*
 * Reference:
 *   Arm Architecture Reference Manual for A-profile Architecture
 *   (DDI 0487L.b)
 *   https://developer.arm.com/documentation/ddi0487/lb/
 */

#pragma once

#include <cstdint>

#include "internal/bit_util.h"

namespace Thumb2Util {
    uint16_t ReadInstruction(const uint8_t* p) {
        return uint16_t(p[0]) | (uint16_t(p[1]) << 8);
    }

    uint8_t GetInstructionLength(const uint8_t* data, size_t offset = 0) {
        uint16_t hw1 = ReadInstruction(data + offset);

        uint8_t op0 = hw1 >> 13;
        uint8_t op1 = (hw1 >> 11) & 0b11;

        // Thumb-2 32-bit
        if (op0 == 0b111 && op1 != 0b00)
            return 4;

        // Thumb 16-bit
        if (op0 != 0b111)
            return 2;

        // T2 B (16-bit) | OP0:OP1 = 11100
        return 2;
    }

    bool IsInstrImmBL(uint16_t hw1, uint16_t hw2) {
        // HW1: 1111 0SHH HHHH HHHH
        // HW2: 11J1 JLLL LLLL LLLL

        if ((hw1 >> 11) != 0b11110)
            return false;

        if (((hw2 >> 12) & 0b1101) != 0b1101)
            return false;

        return true;
    }

    bool IsInstrImmBLX(uint16_t hw1, uint16_t hw2) {
        // HW1: 1111 0SHH HHHH HHHH
        // HW2: 11J0 JLLL LLLL LLL0

        if ((hw1 >> 11) != 0b11110)
            return false;

        if (((hw2 >> 12) & 0b1101) != 0b1100)
            return false;

        return true;
    }

    int32_t ExtractImmOffsetBL(uint16_t hw1, uint16_t hw2) {
        // HW1: 1111 0SHH HHHH HHHH
        // HW2: 11J1 JHHH HHHH HHHH

        uint16_t imm10 = hw1 & BitUtil::BitMask<uint32_t, 10>();
        uint16_t imm11 = hw2 & BitUtil::BitMask<uint32_t, 11>();

        bool j1 = (hw2 >> 13) & 1;
        bool j2 = (hw2 >> 11) & 1;

        bool s = (hw1 >> 10) & 1;

        bool i1 = !(j1 ^ s);
        bool i2 = !(j2 ^ s);

        uint32_t raw =
                (s     << 24) |
                (i1    << 23) |
                (i2    << 22) |
                (imm10 << 12) |
                (imm11 << 1);

        // 25bit → 32bit
        raw = BitUtil::SignExtend<uint32_t, 25>(raw);

        return static_cast<int32_t>(raw);
    }

    int32_t ExtractImmOffsetBLX(uint16_t hw1, uint16_t hw2) {
        // HW1: 1111 0SHH HHHH HHHH
        // HW2: 11J0 JHHH HHHH HHHH

        uint16_t imm10h =  hw1 & BitUtil::BitMask<uint32_t, 10>();
        uint16_t imm10l = (hw2 & BitUtil::BitMask<uint32_t, 11>()) >> 1;

        bool j1 = (hw2 >> 13) & 1;
        bool j2 = (hw2 >> 11) & 1;

        bool s = (hw1 >> 10) & 1;

        bool i1 = !(j1 ^ s);
        bool i2 = !(j2 ^ s);

        uint32_t raw =
                (s      << 24) |
                (i1     << 23) |
                (i2     << 22) |
                (imm10h << 12) |
                (imm10l << 2);

        // 25bit → 32bit
        raw = BitUtil::SignExtend<uint32_t, 25>(raw);

        return static_cast<int32_t>(raw);
    }
}