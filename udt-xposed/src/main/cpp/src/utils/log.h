#pragma once

#include <string>
#include <format>
#include <android/log.h>

#define DISABLE_DEBUG_RELEASE 0

namespace Logging {
    template<typename... Args>
    void Print(int priority, std::string_view tag, std::string_view fmt, Args&&... args) {
        std::string msg = std::vformat(fmt, std::make_format_args(args...));
        __android_log_print(priority, tag.data(), "%s", msg.c_str());
    }

#if defined(NDEBUG) && DISABLE_DEBUG_RELEASE
    template<typename... Args>
    constexpr void Debug(std::string_view tag, std::string_view fmt, Args&&... args) {}
#else
    template<typename... Args>
    void Debug(std::string_view tag, std::string_view fmt, Args&&... args) {
        Print(ANDROID_LOG_DEBUG, tag, fmt, std::forward<Args>(args)...);
    }
#endif

    template<typename... Args>
    void Verbose(std::string_view tag, std::string_view fmt, Args&&... args) {
        Print(ANDROID_LOG_VERBOSE, tag, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Info(std::string_view tag, std::string_view fmt, Args&&... args) {
        Print(ANDROID_LOG_INFO, tag, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Warn(std::string_view tag, std::string_view fmt, Args&&... args) {
        Print(ANDROID_LOG_WARN, tag, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Error(std::string_view tag, std::string_view fmt, Args&&... args) {
        Print(ANDROID_LOG_ERROR, tag, fmt, std::forward<Args>(args)...);
    }
}