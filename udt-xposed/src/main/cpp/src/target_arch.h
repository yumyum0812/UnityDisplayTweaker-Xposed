#pragma once

#define TARGET_ARM64 0
#define TARGET_ARMV7 0
#define TARGET_ARCH_NAME "unknown"

#if defined(__aarch64__) || defined(_M_ARM64)
    #undef TARGET_ARM64
    #undef TARGET_ARCH_NAME
    #define TARGET_ARM64 1
    #define TARGET_ARCH_NAME "arm64"
#elif defined(__arm__) || defined(_M_ARM)
    #undef TARGET_ARMV7
    #undef TARGET_ARCH_NAME
    #define TARGET_ARMV7 1
    #define TARGET_ARCH_NAME "arm32"
#else
    #error "Tried to compile with unsupported ABI!"
#endif