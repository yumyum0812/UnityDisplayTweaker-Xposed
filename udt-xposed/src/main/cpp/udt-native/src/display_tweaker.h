#pragma once

#include <xdl.h>
#include "unity/engine/screen_manager.h"
#include "unity/il2cpp/fullscreen_mode.h"
#include "unity/il2cpp/refresh_rate.h"
#include "utils/logcat.h"
#include "proc/mem_patch.h"
#include "asm_funcs.h"
#include "module_log.h"

#define UDT_FORCE_OPCODE_METHOD 0


namespace DisplayTweaker {
    void* (* il2cpp_resolve_icall)(const char*) = nullptr;

    void (* Screen_SetResolution)(int width, int height, FullScreenMode mode, int rr) = nullptr;
    void (* Screen_SetResolution_Injected)(int width, int height, FullScreenMode mode, const RefreshRate& rr) = nullptr;
    void (* Application_set_targetFrameRate)(int value) = nullptr;
    ScreenManager* (* GetScreenManager)() = nullptr;

    bool resInit = false;
    bool frInit = false;

    bool Init();
    bool SetupResolution();
    bool SetupTargetFrameRate();

    struct {
        std::unique_ptr<MemPatch> setResPatch;
        std::unique_ptr<MemPatch> setResInjPatch;
        std::unique_ptr<MemPatch> setFrPatch;
        std::unique_ptr<MemPatch> reqResPatch;
    } hooks;

    bool Init() {
        if (il2cpp_resolve_icall) return true;

        void* handle = xdl_open("libil2cpp.so", XDL_DEFAULT);
        if (handle == nullptr) {
            ModuleLog::E("Couldn't obtain handle of libil2cpp.so");
            return false;
        }

        ModuleLog::D("libil2cpp.so found.");

        il2cpp_resolve_icall = (decltype(il2cpp_resolve_icall)) xdl_sym(handle, "il2cpp_resolve_icall", nullptr);
        if (il2cpp_resolve_icall == nullptr) {
            ModuleLog::E("Couldn't resolve symbol: il2cpp_resolve_icall");
            return false;
        }

        ModuleLog::D("il2cpp_resolve_icall: 0x{:x}", (uintptr_t) il2cpp_resolve_icall);

        return true;
    }

    bool SetupResolution() {
        if (resInit) return true;

#if !UDT_FORCE_OPCODE_METHOD
        Screen_SetResolution = (decltype(Screen_SetResolution)) il2cpp_resolve_icall("UnityEngine.Screen::SetResolution");
        Screen_SetResolution_Injected = (decltype(Screen_SetResolution_Injected)) il2cpp_resolve_icall("UnityEngine.Screen::SetResolution_Injected");

        if (Screen_SetResolution) {
            ModuleLog::D("SetResolution: 0x{:x}", (uintptr_t) Screen_SetResolution);
        }
        if (Screen_SetResolution_Injected) {
            ModuleLog::D("SetResolution_Injected: 0x{:x}", (uintptr_t) Screen_SetResolution_Injected);
        }
#endif

        if ((Screen_SetResolution == nullptr) && (Screen_SetResolution_Injected == nullptr)) {
            auto Screen_get_width = (int (*)()) il2cpp_resolve_icall("UnityEngine.Screen::get_width");
            if (Screen_get_width == nullptr) {
                ModuleLog::E("Couldn't find get_width function!");
                return false;
            }
            ModuleLog::D("get_width: 0x{:x}", (uintptr_t) Screen_get_width);

            // 先頭の命令4つから GetScreenManager の呼び出しを期待
            GetScreenManager = (decltype(GetScreenManager)) AsmFuncs::FindSubroutineCall((uintptr_t) Screen_get_width, 4);
            if (GetScreenManager == nullptr) {
                ModuleLog::E("Couldn't find call of GetScreenManager!");
                return false;
            }
            ModuleLog::D("GetScreenManager: 0x{:x}", (uintptr_t) GetScreenManager);
        }

        resInit = true;
        return true;
    }

    bool SetupTargetFrameRate() {
        if (frInit) return true;

        Application_set_targetFrameRate = (decltype(Application_set_targetFrameRate)) il2cpp_resolve_icall("UnityEngine.Application::set_targetFrameRate");
        if (Application_set_targetFrameRate == nullptr) {
            ModuleLog::E("Couldn't find set_targetFrameRate function!");
            return false;
        }

        ModuleLog::D("set_targetFrameRate: 0x{:x}", (uintptr_t) Application_set_targetFrameRate);
        frInit = true;
        return true;
    }

    bool PatchResolution(int width, int height) {
        if (!resInit) {
            ModuleLog::E("Resolution functions not init!");
            return false;
        }

        bool success = false;

        if (Screen_SetResolution) {
            if (hooks.setResPatch != nullptr) hooks.setResPatch->Restore();

            Screen_SetResolution(width, height, FullScreenMode::FullScreenWindow, 0);

            hooks.setResPatch = AsmFuncs::CreateDisableVoidPatch((uintptr_t) Screen_SetResolution);
            hooks.setResPatch->Modify();

            ModuleLog::I("Changed resolution successfully (1)");
            success = true;
        }

        if (Screen_SetResolution_Injected) {
            if (hooks.setResInjPatch != nullptr) hooks.setResInjPatch->Restore();

            Screen_SetResolution_Injected(width, height, FullScreenMode::FullScreenWindow, {0, 0});

            hooks.setResInjPatch = AsmFuncs::CreateDisableVoidPatch((uintptr_t) Screen_SetResolution_Injected);
            hooks.setResInjPatch->Modify();

            ModuleLog::I("Changed resolution successfully (2)");
            success = true;
        }

        if (!success && (GetScreenManager != nullptr)) {
            ScreenManager* sm = GetScreenManager();
            if (sm == nullptr) {
                ModuleLog::E("Couldn't obtain ScreenManager!");
                return false;
            }

            ModuleLog::D("ScreenManager: 0x{:x}", (uintptr_t) sm);

            if (hooks.reqResPatch != nullptr) hooks.reqResPatch->Restore();

            sm->RequestResolution(width, height, true, 0);

            hooks.reqResPatch = AsmFuncs::CreateDisableVoidPatch((uintptr_t) sm->vtable->RequestResolution);
            hooks.reqResPatch->Modify();

            ModuleLog::I("Changed resolution successfully (3)");
            return true;
        }

        return success;
    }

    bool PatchTargetFrameRate(int target) {
        if (!frInit) {
            ModuleLog::E("Framerate functions not init!");
            return false;
        }

        if (Application_set_targetFrameRate != nullptr) {
            if (hooks.setFrPatch != nullptr) hooks.setFrPatch->Restore();

            Application_set_targetFrameRate(target);

            hooks.setFrPatch = AsmFuncs::CreateDisableVoidPatch((uintptr_t) Application_set_targetFrameRate);
            hooks.setFrPatch->Modify();

            ModuleLog::I("Changed target framerate successfully (1)");
            return true;
        }
        return false;
    }
}