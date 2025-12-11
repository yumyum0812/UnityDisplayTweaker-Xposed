#pragma once

#include <type_traits>
#include <bit>

namespace BitUtil {
    template<typename T, unsigned width>
    consteval typename std::enable_if<std::is_integral<T>::value, T>::type BitMask() {
        return (width >= sizeof(T) * 8) ? ~T(0) : (T(1) << width) - 1;
    }

    template<typename DstT, unsigned bits, typename SrcT>
    constexpr typename std::enable_if<
            std::is_integral<SrcT>::value &&
            std::is_integral<DstT>::value, DstT
            >::type SignExtend(SrcT value) {
        static_assert(sizeof(SrcT) <= sizeof(DstT), "DstT must be equal or larger than SrcT");
        static_assert(bits != 0 && bits <= sizeof(SrcT) * 8, "SignExtend: bits out of range");

        DstT mask = DstT(1) << (bits - 1); // 符号ビット
        return DstT(value ^ mask) - mask;  // 符号拡張
    }
}