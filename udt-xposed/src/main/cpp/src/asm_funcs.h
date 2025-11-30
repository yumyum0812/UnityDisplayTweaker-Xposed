#pragma once

#include "utils/arm/arm32_util.h"
#include "utils/arm/arm64_util.h"
#include "utils/arm/thumb2_util.h"
#include "utils/log.h"
#include "proc/MemPatch.h"
#include "module_constants.h"
#include "target_arch.h"

namespace AsmFuncs {
    uintptr_t FindSubroutineCallT2(uintptr_t funAddr, int performs) {
        funAddr &= ~1;

        size_t offset = 0;

        for (int i = 0; i < performs; i++) {
            Logging::Debug(ModuleConstants::LOG_TAG, "Performing (T2): 0x{:X}", offset);

            uintptr_t instrAddr = funAddr + offset;
            uintptr_t pc = instrAddr + 4;
            uint8_t instrLen = Thumb2Util::GetInstructionLength((uint8_t*) instrAddr);

            if (instrLen == 4) {
                uint16_t first = Thumb2Util::ReadInstruction((uint8_t*) instrAddr);
                uint16_t second = Thumb2Util::ReadInstruction((uint8_t*)(instrAddr + 2));

                if (Thumb2Util::IsInstrImmBLX(first, second)) {
                    int32_t off = Thumb2Util::ExtractImmOffsetBLX(first, second);
                    return pc + off;
                } else if (Thumb2Util::IsInstrImmBL(first, second)) {
                    int32_t off = Thumb2Util::ExtractImmOffsetBL(first, second);
                    return (pc + off) | 1; // LSB を設定（Thumbモードを継続）
                }
            }

            offset += instrLen;
        }

        return 0;
    }

    uintptr_t FindSubroutineCallA32(uintptr_t funAddr, int performs) {
        funAddr &= ~0b11;

        for (int i = 0; i < performs; i++) {
            size_t offset = i * 4;

            Logging::Debug(ModuleConstants::LOG_TAG, "Performing (A32): 0x{:X}", offset);

            uintptr_t instrAddr = funAddr + offset;
            uintptr_t pc = instrAddr + 8;
            uint32_t instr = Arm32Util::ReadInstruction((uint8_t*) instrAddr);

            if (Arm32Util::IsInstrImmBL(instr)) {
                int32_t off = Arm32Util::ExtractImmOffsetBL(instr);
                return pc + off;
            } else if (Arm32Util::IsInstrImmBLX(instr)) {
                int32_t off = Arm32Util::ExtractImmOffsetBLX(instr);
                return pc + off;
            }
        }

        return 0;
    }

    uintptr_t FindSubroutineCallA64(uintptr_t funAddr, int performs) {
        funAddr &= ~0b11;

        for (int i = 0; i < performs; i++) {
            size_t offset = i * 4;
            Logging::Debug(ModuleConstants::LOG_TAG, "Performing (A64): 0x{:X}", offset);

            uintptr_t instrAddr = funAddr + offset;
            uint32_t instr = Arm64Util::ReadInstruction((uint8_t*) instrAddr);

            if (Arm64Util::IsInstrBL(instr)) {
                int32_t off = Arm64Util::ExtractOffsetBL(instr);
                return instrAddr + off;
            }
        }
        return 0;
    }

    uintptr_t FindSubroutineCall(uintptr_t funAddr, int performs) {
#if TARGET_ARM64
        return FindSubroutineCallA64(funAddr, performs);
#else
        if (funAddr & 1) { // Check T-bit (1 = Thumb, 0 = Arm)
            return FindSubroutineCallT2(funAddr, performs);
        } else {
            return FindSubroutineCallA32(funAddr, performs);
        }
#endif
    }

    void DisableVoidFunc(uintptr_t funAddr) {
#if TARGET_ARM64
        // Arm64: RET
        MemPatch::Patch(funAddr, "C0 03 5F D6");
#else
        // Arm32: Check the T-bit (1 = Thumb, 0 = Arm)
        if (funAddr & 1) {
            // Thumb: BX LR
            MemPatch::Patch(funAddr & ~1, "70 47");
        } else {
            // Arm: BX LR
            MemPatch::Patch(funAddr, "1E FF 2F E1");
        }
#endif
    }
}