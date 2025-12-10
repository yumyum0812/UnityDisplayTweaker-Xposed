#pragma once

class ScreenManager {
public:
    struct {
        void* unk1; // [0]
        void* unk2; // [1]
        void (* RequestResolution)(ScreenManager* thiz, int width, int height, bool fullscreen, int preferredRefreshRate); // [2]
    }* vtable;

    void RequestResolution(int width, int height, bool fullscreen, int preferredRefreshRate) {
        return vtable->RequestResolution(this, width, height, fullscreen, preferredRefreshRate);
    }
};