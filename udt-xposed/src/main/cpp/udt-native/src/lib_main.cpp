#include <string>
#include <chrono>
#include <thread>
#include <cstdint>
#include <jni.h>

#include <xdl.h>

#include "proc/mem_patch.h"
#include "utils/api_macros.h"
#include "utils/logcat.h"
#include "utils/lib_finder.h"
#include "unity/engine/screen_manager.h"
#include "unity/il2cpp/fullscreen_mode.h"
#include "unity/il2cpp/refresh_rate.h"
#include "apply_config.h"
#include "asm_funcs.h"
#include "display_tweaker.h"
#include "module_log.h"
#include "jni_impls.h"

extern "C"
JNIEXPORT JNICALL jint JNI_OnLoad(JavaVM* vm, void*) {
    JNIEnv* env = nullptr;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        ModuleLog::E("Couldn't obtain JNIEnv!");
        return JNI_ERR;
    }

    jclass clazz = env->FindClass("jp/miruku/unitydisplaytweaker/module/XposedEntry");
    if (clazz == nullptr) {
        ModuleLog::E("Couldn't find entry class!");
        return JNI_ERR;
    }

    JNINativeMethod nativeMethods[] = {
            {"startApply", "(FZIIZI)V", (void*) StartApply},
    };

    if (jint rc = env->RegisterNatives(clazz, nativeMethods, sizeof nativeMethods / sizeof nativeMethods[0]); rc != JNI_OK) {
        ModuleLog::E("Failed to register native method(s): {:d}", rc);
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