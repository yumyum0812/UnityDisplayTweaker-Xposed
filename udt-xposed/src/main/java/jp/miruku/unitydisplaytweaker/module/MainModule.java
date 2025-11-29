package jp.miruku.unitydisplaytweaker.module;
import android.os.Bundle;
import android.util.Log;

import androidx.annotation.NonNull;

import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.XposedHelpers;
import de.robv.android.xposed.callbacks.XC_LoadPackage;

public class MainModule implements IXposedHookLoadPackage {
    public static Config config;

    public static native void startApply(float delay, boolean changeResolution, int width, int height, boolean changeMaxFps, int maxFps);

    private void loadPrefs() {
        var prefs = new Preferences("module_config");
        config = new Config(prefs);
    }

    @Override
    public void handleLoadPackage(@NonNull XC_LoadPackage.LoadPackageParam param) {
        if (param.packageName.equals(ModuleConstants.MANAGER_PACKAGE)) return;
        if (!param.isFirstApplication) return;
        Log.d(ModuleConstants.LOG_TAG, "handleLoadPackage: " + param.packageName);

        loadPrefs();

        try {
            XposedHelpers.findAndHookMethod("android.app.Activity", param.classLoader, "onCreate", Bundle.class, new OnCreateHook());
            Log.d(ModuleConstants.LOG_TAG, "Hooked.");
        } catch (RuntimeException e) {
            Log.e(ModuleConstants.LOG_TAG, e.getClass().getName() + ": " + e.getMessage());
            throw new RuntimeException(e);
        }
    }
}
