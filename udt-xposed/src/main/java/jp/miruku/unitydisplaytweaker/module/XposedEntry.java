package jp.miruku.unitydisplaytweaker.module;

import androidx.annotation.NonNull;

import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.callbacks.XC_LoadPackage;
import jp.miruku.unitydisplaytweaker.module.common.Constants;
import jp.miruku.unitydisplaytweaker.module.util.ModuleLog;

public class XposedEntry implements IXposedHookLoadPackage {
    @Override
    public void handleLoadPackage(@NonNull XC_LoadPackage.LoadPackageParam param) {
        if (param.packageName.equals(Constants.MANAGER_PACKAGE_NAME)) return;
        if (!param.isFirstApplication) return;
        ModuleLog.i("Module loaded: " + param.packageName);

        UDTNative.loadLib();
        ModuleLog.i("Native library loaded");

        Config.load();
        ModuleLog.i("Configuration loaded");

        try {
            var hooks = new UnityPlayerHooks(param);
            hooks.initialize();
            ModuleLog.d("Hooked Activity methods successfully");
        } catch (RuntimeException | ClassNotFoundException e) {
            ModuleLog.e("Failed to hook Activity methods", e);
            throw new RuntimeException(e);
        }
    }
}
