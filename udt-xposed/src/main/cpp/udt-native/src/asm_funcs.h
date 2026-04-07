#pragma once

#include <cstdint>
#include <memory>

#include <mimem/mem_patch.h>

#include "armutils/arm32_util.h"
#include "armutils/arm64_util.h"
#include "armutils/thumb2_util.h"
#include "utils/logcat.h"
#include "target_arch.h"
#include "module_log.h"

namespace AsmFuncs {
    uintptr_t FindSubroutineCallT2(uintptr_t funAddr, int performs) {
        funAddr &= ~1;

        size_t offset = 0;

        for (int i = 0; i < performs; i++) {
            ModuleLog::D("T2: 0x{:x}", offset);

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
                    return (pc + off) | 1; // set LSB (continue Thumb mode)
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

            ModuleLog::D("A32: 0x{:x}", offset);

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
            ModuleLog::D("A64: 0x{:x}", offset);

            uintptr_t instrAddr = funAddr + offset;
            uint32_t instr = Arm64Util::ReadInstruction((uint8_t*) instrAddr);

            if (Arm64Util::IsInstrBL(instr)) {
                int32_t off = Arm64Util::ExtractOffsetBL(instr);
                return instrAddr + off;
            }
        }
        return 0;
    }

    uintptr_t FindNativeSubroutineCall(uintptr_t funAddr, int tries) {
#if TARGET_ARM64
        return FindSubroutineCallA64(funAddr, tries);
#elif TARGET_ARMV7
        if (funAddr & 1) { // Check T-bit (1 = Thumb, 0 = Arm)
            return FindSubroutineCallT2(funAddr, tries);
        } else {
            return FindSubroutineCallA32(funAddr, tries);
        }
#else
#error "the ABI is not supported!"
#endif
    }

    std::unique_ptr<MiMem::MemPatch> CreateNativeDisableVoidPatch(uintptr_t funAddr) {
        std::string code;

#if TARGET_ARM64
        // Arm64: RET
        code = "C0 03 5F D6";
#elif TARGET_ARMV7
        // Arm32: Check T-bit (1 = Thumb, 0 = Arm)
        if (funAddr & 1) {
            // Thumb: BX LR
            code = "70 47";
            funAddr &= ~1; // aligning to instruction address
        } else {
             // Arm: BX LR
            code = "1E FF 2F E1";
        }
#else
#error "the ABI is not supported!"
#endif

        return std::make_unique<MiMem::MemPatch>(MiMem::CreateHexPatch(funAddr, code));
    }
}