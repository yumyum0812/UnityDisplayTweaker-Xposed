#pragma once

#include <string>
#include <format>
#include <android/log.h>

#define LOGGING_DISABLE_DEBUG_RELEASE 1


namespace Logging {
    template<typename... Args>
    void Print(int priority, std::string_view tag, std::string_view fmt, Args&&... args) {
        std::string msg = std::vformat(fmt, std::make_format_args(args...));
        __android_log_write(priority, tag.data(), msg.c_str());
    }

#if defined(NDEBUG) && LOGGING_DISABLE_DEBUG_RELEASE
    template<typename... Args>
    __attribute__((always_inline))
    inline void Debug(std::string_view tag, std::string_view fmt, Args&&... args) {}
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

    template<typename... Args>
    void Fatal(std::string_view tag, std::string_view fmt, Args&&... args) {
        Print(ANDROID_LOG_FATAL, tag, fmt, std::forward<Args>(args)...);
    }
}