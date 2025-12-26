#pragma once

namespace JniImpls {
    void Initialize(JNIEnv* env, jclass clazz) {
        if (!DisplayTweaker::Init()) {
            ModuleLog::E("Failed to initialize!");
        }

        if (!DisplayTweaker::SetupResolution()) {
            ModuleLog::E("Failed to initialize functions!");
        }

        if (!DisplayTweaker::SetupTargetFrameRate()) {
            ModuleLog::E("Failed to initialize frame rate functions!");
        }
    }

    void SetResolution(JNIEnv* env, jclass clazz, jint width, jint height) {
        if (!DisplayTweaker::PatchResolution(width, height)) {
            ModuleLog::E("Failed to change resolution!");
        }
    }

    void SetFpsCap(JNIEnv* env, jclass clazz, jint fpsCap) {
        if (!DisplayTweaker::PatchTargetFrameRate(fpsCap)) {
            ModuleLog::E("Failed to change fps cap!");
        }
    }
}