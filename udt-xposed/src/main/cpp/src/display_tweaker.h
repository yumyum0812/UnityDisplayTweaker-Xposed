#pragma once

#include <xdl.h>
#include "unity/engine/screen_manager.h"
#include "unity/il2cpp/fullscreen_mode.h"
#include "unity/il2cpp/refresh_rate.h"
#include "module_constants.h"
#include "utils/logging.h"
#include "asm_funcs.h"

#define UDT_FORCE_OPCODE_METHOD 0


namespace DisplayTweaker {
    void* (* il2cpp_resolve_icall)(const char*) = nullptr;

    void (* Screen_SetResolution)(int width, int height, FullScreenMode mode, int rr) = nullptr;
    void (* Screen_SetResolution_Injected)(int width, int height, FullScreenMode mode, const RefreshRate& rr) = nullptr;
    int (* Screen_get_width)() = nullptr;
    void (* Application_set_targetFrameRate)(int value);

    bool Init() {
        void* handle = xdl_open("libil2cpp.so", XDL_DEFAULT);
        if (!handle) {
            Logging::Warn(ModuleConstants::LOG_TAG, "Couldn't obtain handle of libil2cpp.so");
            return false;
        }

        Logging::Debug(ModuleConstants::LOG_TAG, "libil2cpp.so found.");

        il2cpp_resolve_icall = (decltype(il2cpp_resolve_icall)) xdl_sym(handle, "il2cpp_resolve_icall", nullptr);
        if (!il2cpp_resolve_icall) {
            Logging::Warn(ModuleConstants::LOG_TAG, "Couldn't resolve symbol: il2cpp_resolve_icall");
            return false;
        }

        Logging::Debug(ModuleConstants::LOG_TAG, "il2cpp_resolve_icall: 0x{:X}", (uintptr_t) il2cpp_resolve_icall);

        return true;
    }

    bool SetupResolution() {
        if (!il2cpp_resolve_icall) {
            return false;
        }

        Screen_SetResolution = (decltype(Screen_SetResolution)) il2cpp_resolve_icall("UnityEngine.Screen::SetResolution");
        Screen_SetResolution_Injected = (decltype(Screen_SetResolution_Injected)) il2cpp_resolve_icall("UnityEngine.Screen::SetResolution_Injected");
        Screen_get_width = (decltype(Screen_get_width)) il2cpp_resolve_icall("UnityEngine.Screen::get_width");

        if (Screen_SetResolution) {
            Logging::Debug(ModuleConstants::LOG_TAG, "Screen_SetResolution: 0x{:X}", (uintptr_t) Screen_SetResolution);
        }
        if (Screen_SetResolution_Injected) {
            Logging::Debug(ModuleConstants::LOG_TAG, "Screen_SetResolution_Injected: 0x{:X}", (uintptr_t) Screen_SetResolution_Injected);
        }
        if (Screen_get_width) {
            Logging::Debug(ModuleConstants::LOG_TAG, "Screen_get_width: 0x{:X}", (uintptr_t) Screen_get_width);
        }

        if (Screen_SetResolution || Screen_SetResolution_Injected || Screen_get_width) {
            return true;
        }

        return false;
    }

    bool SetupTargetFrameRate() {
        if (!il2cpp_resolve_icall) {
            return false;
        }

        Application_set_targetFrameRate = (decltype(Application_set_targetFrameRate)) il2cpp_resolve_icall("UnityEngine.Application::set_targetFrameRate");

        if (Application_set_targetFrameRate) {
            Logging::Debug(ModuleConstants::LOG_TAG, "set_targetFrameRate: 0x{:X}", (uintptr_t) Application_set_targetFrameRate);
        }

        if (Application_set_targetFrameRate) {
            return true;
        }

        return false;
    }

    bool PatchResolution(int width, int height) {
        bool success = false;

#if !UDT_FORCE_OPCODE_METHOD
        if (Screen_SetResolution) {
            Screen_SetResolution(width, height, FullScreenMode::FullScreenWindow, 0);
            AsmFuncs::DisableVoidFunc((uintptr_t) Screen_SetResolution);

            Logging::Info(ModuleConstants::LOG_TAG, "Changed resolution successfully. (1)");
            success = true;
        }

        if (Screen_SetResolution_Injected) {
            Screen_SetResolution_Injected(width, height, FullScreenMode::FullScreenWindow, {0, 0});
            AsmFuncs::DisableVoidFunc((uintptr_t) Screen_SetResolution_Injected);

            Logging::Info(ModuleConstants::LOG_TAG, "Changed resolution successfully. (2)");
            success = true;
        }
#endif

        if (!success && Screen_get_width) {
            auto getWidthAddr = (uintptr_t) Screen_get_width;

            // 先頭の命令8つからGetScreenManagerの呼び出しを期待
            auto getScreenManager = (ScreenManager*(*)()) AsmFuncs::FindSubroutineCall(getWidthAddr, 8);
            if (!getScreenManager) {
                Logging::Debug(ModuleConstants::LOG_TAG, "Couldn't find call of GetScreenManager!");
                return false;
            }

            Logging::Debug(ModuleConstants::LOG_TAG, "GetScreenManager: 0x{:X}", (uintptr_t) getScreenManager);

            ScreenManager* sm = getScreenManager();
            if (!sm) {
                Logging::Debug(ModuleConstants::LOG_TAG, "Couldn't obtain ScreenManager!");
                return false;
            }

            Logging::Debug(ModuleConstants::LOG_TAG, "ScreenManager: 0x{:X}", (uintptr_t) sm);

            sm->RequestResolution(width, height, true, 0);
            AsmFuncs::DisableVoidFunc((uintptr_t) sm->vtable->RequestResolution);

            Logging::Info(ModuleConstants::LOG_TAG, "Changed resolution successfully. (3)");
            return true;
        }

        return success;
    }

    bool PatchTargetFrameRate(int target) {
        if (Application_set_targetFrameRate) {
            Application_set_targetFrameRate(target);
            AsmFuncs::DisableVoidFunc((uintptr_t) Application_set_targetFrameRate);
            Logging::Info(ModuleConstants::LOG_TAG, "Changed target framerate successfully. (1)");
            return true;
        }
        return false;
    }
}