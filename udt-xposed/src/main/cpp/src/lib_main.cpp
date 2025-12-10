#include <string>
#include <chrono>
#include <thread>
#include <cstdint>
#include <jni.h>

#include <xdl.h>

#include "proc/mem_patch.h"
#include "utils/api_macros.h"
#include "utils/logging.h"
#include "utils/lib_finder.h"
#include "unity/engine/screen_manager.h"
#include "unity/il2cpp/fullscreen_mode.h"
#include "unity/il2cpp/refresh_rate.h"
#include "apply_config.h"
#include "module_constants.h"
#include "asm_funcs.h"
#include "display_tweaker.h"

void Apply(const ApplyConfig& cfg) {
    if (!DisplayTweaker::Init()) {
        Logging::Warn(ModuleConstants::LOG_TAG, "Failed to initialize!");
        return;
    }

    if (cfg.changeResolution) {
        if (!DisplayTweaker::SetupResolution()) {
            Logging::Warn(ModuleConstants::LOG_TAG, "Failed to initialize functions!");
            return;
        }

        if (!DisplayTweaker::PatchResolution(cfg.width, cfg.height)) {
            Logging::Warn(ModuleConstants::LOG_TAG, "Failed to change resolution!");
            return;
        }
    }

    if (cfg.changeMaxFps) {
        if (!DisplayTweaker::SetupTargetFrameRate()) {
            Logging::Warn(ModuleConstants::LOG_TAG, "Failed to initialize frame rate functions!");
            return;
        }

        if (!DisplayTweaker::PatchTargetFrameRate(cfg.maxFps)) {
            Logging::Warn(ModuleConstants::LOG_TAG, "Failed to change target frame rate!");
            return;
        }
    }
}

void StartApply(JNIEnv* env, jclass clazz, jfloat delay, jboolean changeResolution, jint width, jint height, jboolean changeMaxFps, jint maxFps) {
    Logging::Info(ModuleConstants::LOG_TAG, "ABI: " TARGET_ARCH_NAME);

    ApplyConfig cfg;
    cfg.changeResolution = changeResolution;
    cfg.width = width;
    cfg.height = height;
    cfg.changeMaxFps = changeMaxFps;
    cfg.maxFps = maxFps;

    if (changeResolution) {
        Logging::Info(ModuleConstants::LOG_TAG, "Target resolution: {:d}x{:d}", width, height);
    }
    if (changeMaxFps) {
        Logging::Info(ModuleConstants::LOG_TAG, "Target frame rate: {:d}", maxFps);
    }

    std::thread([=] {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delay * 1000)));
        Apply(cfg);
    }).detach();
}

extern "C"
JNIEXPORT JNICALL jint JNI_OnLoad(JavaVM* vm, void*) {
    JNIEnv* env = nullptr;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    jclass clazz = env->FindClass("jp/miruku/unitydisplaytweaker/module/XposedEntry");
    if (clazz == nullptr) {
        return JNI_ERR;
    }

    JNINativeMethod nativeMethods[] = {
            {"startApply", "(FZIIZI)V", (void*) StartApply},
    };

    if (jint rc = env->RegisterNatives(clazz, nativeMethods, sizeof nativeMethods / sizeof nativeMethods[0]); rc != JNI_OK) {
        return rc;
    }

    return JNI_VERSION_1_6;
}

extern "C"
API_EXPORT
void LoadFromExternal() {
    std::thread([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));

        ApplyConfig cfg;
        cfg.changeResolution = true;
        cfg.width = 640;
        cfg.height = 480;
        cfg.changeMaxFps = true;
        cfg.maxFps = 30;
        Apply(cfg);
    }).detach();
}