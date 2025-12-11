package jp.miruku.unitydisplaytweaker.module;
import android.os.Bundle;

import androidx.annotation.NonNull;

import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.XposedHelpers;
import de.robv.android.xposed.callbacks.XC_LoadPackage;
import jp.miruku.unitydisplaytweaker.module.common.Constants;
import jp.miruku.unitydisplaytweaker.module.util.ModuleLog;

public class XposedEntry implements IXposedHookLoadPackage {
    public static Config config;

    private void loadPrefs() {
        var prefs = new Preferences("module_config");
        config = new Config(prefs);
    }

    public static Config getConfig() {
        return config;
    }

    @Override
    public void handleLoadPackage(@NonNull XC_LoadPackage.LoadPackageParam param) {
        if (param.packageName.equals(Constants.MANAGER_PACKAGE_NAME)) return;
        if (!param.isFirstApplication) return;
        ModuleLog.i("handleLoadPackage: " + param.packageName);

        loadPrefs();

        try {
            XposedHelpers.findAndHookMethod("android.app.Activity", param.classLoader, "onCreate", Bundle.class, new ActivityHook());
            ModuleLog.i("Hooked.");
        } catch (RuntimeException e) {
            ModuleLog.e("Failed to hook", e);
            throw new RuntimeException(e);
        }
    }

    public static native void startApply(float delay, boolean changeResolution, int width, int height, boolean changeMaxFps, int maxFps);
}
