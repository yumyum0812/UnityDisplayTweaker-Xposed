package jp.miruku.unitydisplaytweaker.module;

import jp.miruku.unitydisplaytweaker.module.util.ModuleLog;

public final class UDTNative {
    public static void loadLib() {
        try {
            System.loadLibrary("udt-native");
        } catch (UnsatisfiedLinkError e) {
            ModuleLog.e(e.getClass().getName() + ": " + e.getMessage());
            throw e;
        }
    }

    public static native void initialize();
    public static native void setResolution(int width, int height);
    public static native void setFpsCap(int maxFps);
}
