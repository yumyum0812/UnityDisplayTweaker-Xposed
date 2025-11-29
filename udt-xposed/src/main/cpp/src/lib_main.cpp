#include <string>
#include <chrono>
#include <thread>
#include <cstdint>
#include <jni.h>

#include <xdl.h>

#include "proc/MemPatch.h"
#include "utils/api_macros.h"
#include "utils/log.h"
#include "utils/lib_finder.h"
#include "unity/engine/screen_manager.h"
#include "unity/il2cpp/fullscreen_mode.h"
#include "unity/il2cpp/refresh_rate.h"
#include "apply_config.h"
#include "module_constants.h"
#include "asm_funcs.h"
#include "display_tweaker.h"

void TestMono() {
    Logging::Debug(ModuleConstants::LOG_TAG, "Mono");

    void* mono = xdl_open("libmonobdwgc-2.0.so", XDL_DEFAULT);
    if (!mono) mono = xdl_open("libmono.so", XDL_DEFAULT);

    if (!mono) {
        Logging::Debug(ModuleConstants::LOG_TAG, "A");
        return;
    }

    struct GPtrArray {
        void** data;
        int len;
    };

    auto mono_get_root_domain = (void* (*)()) xdl_sym(mono, "mono_get_root_domain", nullptr);
    auto mono_domain_get_assemblies = (GPtrArray* (*)(void*, bool)) xdl_sym(mono, "mono_domain_get_assemblies", nullptr);
    auto mono_assembly_get_image = (void* (*)(void*)) xdl_sym(mono, "mono_assembly_get_image", nullptr);
    auto mono_image_get_name = (const char* (*)(void*)) xdl_sym(mono, "mono_image_get_name", nullptr);
    auto mono_method_desc_new = (void* (*)(const char*)) xdl_sym(mono, "mono_method_desc_new", nullptr);
    auto mono_method_desc_search_in_image = (void* (*)(void*, void*)) xdl_sym(mono, "mono_method_desc_search_in_image", nullptr);
    auto mono_lookup_internal_call = (void* (*)(void*)) xdl_sym(mono, "mono_lookup_internal_call", nullptr);

    if (!mono_get_root_domain) {
        Logging::Debug(ModuleConstants::LOG_TAG, "mono_get_root_domain");
        return;
    }
    if (!mono_domain_get_assemblies) {
        Logging::Debug(ModuleConstants::LOG_TAG, "mono_domain_get_assemblies");
        return;
    }
    if (!mono_assembly_get_image) {
        Logging::Debug(ModuleConstants::LOG_TAG, "mono_assembly_get_image");
        return;
    }
    if (!mono_image_get_name) {
        Logging::Debug(ModuleConstants::LOG_TAG, "mono_image_get_name");
        return;
    }
    if (!mono_method_desc_new) {
        Logging::Debug(ModuleConstants::LOG_TAG, "mono_method_desc_new");
        return;
    }
    if (!mono_method_desc_search_in_image) {
        Logging::Debug(ModuleConstants::LOG_TAG, "mono_method_desc_search_in_image");
        return;
    }
    if (!mono_lookup_internal_call) {
        Logging::Debug(ModuleConstants::LOG_TAG, "mono_lookup_internal_call");
        return;
    }

    void* domain = mono_get_root_domain();
    if (!domain) {
        Logging::Debug(ModuleConstants::LOG_TAG, "mdga");
        return;
    }

    GPtrArray* asms = mono_domain_get_assemblies(domain, false);
    if (!asms) {
        Logging::Debug(ModuleConstants::LOG_TAG, "E");
        return;
    }

    //    Logging::Debug(ModuleConstants::LOG_TAG, "{:d}", asms->len);

//    void* desc = mono_method_desc_new("UnityEngine.Application::set_targetFrameRate");
    void* desc = mono_method_desc_new("UnityEngine.Screen::SetResolution");

    for (int i = 0; i < asms->len; i++) {
        void* a = asms->data[i];
        void* img = mono_assembly_get_image(a);
        auto name = mono_image_get_name(img);
        Logging::Debug(ModuleConstants::LOG_TAG, "{:d}: {}", i + 1, name);

        void* mtd = mono_method_desc_search_in_image(desc, img);
        if (mtd) {
            Logging::Debug(ModuleConstants::LOG_TAG, "mtd: 0x{:X}", (uintptr_t)mtd);
            void* ptr = mono_lookup_internal_call(mtd);
            Logging::Debug(ModuleConstants::LOG_TAG, "ptr: 0x{:X}", (uintptr_t)ptr);
//            if (ptr) reinterpret_cast<void(*)(int)>(ptr)(30);
            if (ptr) reinterpret_cast<void(*)(int, int, bool, int)>(ptr)(1080, 3080, true, 0);
            break;
        }
    }
}

void Apply(const ApplyConfig& config) {
    if (!DisplayTweaker::Init()) {
        Logging::Warn(ModuleConstants::LOG_TAG, "Failed to initialize!");
        return;
    }

    if (config.changeResolution) {
        if (!DisplayTweaker::SetupResolution()) {
            Logging::Warn(ModuleConstants::LOG_TAG, "Failed to initialize functions!");
            return;
        }

        if (!DisplayTweaker::PatchResolution(config.width, config.height)) {
            Logging::Warn(ModuleConstants::LOG_TAG, "Failed to change resolution!");
            return;
        }
    }

    if (config.changeMaxFps) {
        if (!DisplayTweaker::SetupTargetFrameRate()) {
            Logging::Warn(ModuleConstants::LOG_TAG, "Failed to initialize frame rate functions!");
            return;
        }

        if (!DisplayTweaker::PatchTargetFrameRate(config.maxFps)) {
            Logging::Warn(ModuleConstants::LOG_TAG, "Failed to change target frame rate!");
            return;
        }
    }
}

extern "C"
EXPORTSYM void LoadFromExternal() {
    std::thread([] {
        ApplyConfig cfg;
        Apply(cfg);
    }).detach();
}

extern "C"
JNIEXPORT void JNICALL
Java_jp_miruku_unitydisplaytweaker_module_MainModule_startApply(JNIEnv* env, jclass clazz, jfloat delay, jboolean changeResolution, jint width, jint height, jboolean changeMaxFps, jint maxFps) {
    std::thread([=] {
        Logging::Info(ModuleConstants::LOG_TAG, "ABI: " TARGET_ARCH_NAME);

        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delay * 1000)));

        ApplyConfig cfg;
        cfg.changeResolution = changeResolution;
        cfg.width = width;
        cfg.height = height;
        cfg.changeMaxFps = changeMaxFps;
        cfg.maxFps = maxFps;

        Apply(cfg);
    }).detach();
}

extern "C"
JNIEXPORT JNICALL jint JNI_OnLoad(JavaVM* vm, void*) {
    return JNI_VERSION_1_6;
}