#pragma once
#include "display_tweaker.h"
#include "module_log.h"

namespace TweakerJni {
    void Initialize(JNIEnv* env, jclass clazz) {
        if (!DisplayTweaker::Init()) {
            ModuleLog::E("Failed to initialize!");
            return;
        }

        if (!DisplayTweaker::SetupResolution()) {
            ModuleLog::E("Failed to initialize functions!");
        }

        if (!DisplayTweaker::SetupTargetFrameRate()) {
            ModuleLog::E("Failed to initialize frame rate functions!");
        }
    }

    void SetResolution(JNIEnv* env, jclass clazz, jint width, jint height, jboolean lock) {
        if (!DisplayTweaker::PatchResolution(width, height, lock)) {
            ModuleLog::E("Failed to change resolution!");
        }
    }

    void SetFpsCap(JNIEnv* env, jclass clazz, jint fpsCap, jboolean lock) {
        if (!DisplayTweaker::PatchTargetFrameRate(fpsCap, lock)) {
            ModuleLog::E("Failed to change fps cap!");
        }
    }
}