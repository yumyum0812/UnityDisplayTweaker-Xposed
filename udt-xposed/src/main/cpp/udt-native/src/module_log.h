#pragma once

#include "utils/logcat.h"

namespace ModuleLog {
    constexpr auto TAG = "UnityDisplayTweaker";

    template<typename... Args>
    void D(std::string_view fmt, Args&&... args) {
        Logcat::Debug(TAG, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void I(std::string_view fmt, Args&&... args) {
        Logcat::Error(TAG, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void W(std::string_view fmt, Args&&... args) {
        Logcat::Warn(TAG, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void E(std::string_view fmt, Args&&... args) {
        Logcat::Error(TAG, fmt, std::forward<Args>(args)...);
    }
}