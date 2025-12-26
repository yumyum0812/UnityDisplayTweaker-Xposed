package jp.miruku.unitydisplaytweaker.module.util;

import android.util.Log;

import de.robv.android.xposed.XposedBridge;

public class ModuleLog {
    private static final String TAG_LOGCAT = "UnityDisplayTweaker";
    private static final String TAG_XPOSED = "UDT";

    public static void d(String msg) {
        Log.d(TAG_LOGCAT, msg);
        logForXposed("D", msg);
    }

    public static void i(String msg) {
        Log.i(TAG_LOGCAT, msg);
        logForXposed("I", msg);
    }

    public static void e(String msg) {
        Log.e(TAG_LOGCAT, msg);
        logForXposed("E", msg);
    }

    public static void e(String msg, Throwable t) {
        Log.e(TAG_LOGCAT, msg, t);
        logForXposed("E", msg + ": " + t.getClass().getName() + ": " + t.getMessage());
    }

    private static void logForXposed(String level, String msg) {
        XposedBridge.log("[" + TAG_XPOSED + "][" + level + "] " + msg);
    }
}
