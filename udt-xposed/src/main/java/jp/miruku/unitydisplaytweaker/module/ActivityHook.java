package jp.miruku.unitydisplaytweaker.module;

import android.app.Activity;
import android.widget.Toast;

import androidx.annotation.NonNull;

import java.util.Objects;

import de.robv.android.xposed.XC_MethodHook;
import jp.miruku.unitydisplaytweaker.module.display.ActivityDisplay;
import jp.miruku.unitydisplaytweaker.module.util.ModuleLog;

public class ActivityHook extends XC_MethodHook {
    private String mEntryClassName;

    @Override
    protected void afterHookedMethod(@NonNull MethodHookParam param) {
        var className = param.thisObject.getClass().getName();
        if (mEntryClassName != null && !className.equals(mEntryClassName)) return;
        mEntryClassName = className;

        var activity = (Activity) Objects.requireNonNull(param.thisObject);
        var config = XposedEntry.getConfig();

        ModuleLog.i("Module loaded");

        if (config.showToast) {
            Toast.makeText(activity, "UDT loaded", Toast.LENGTH_SHORT).show();
        }

        var display = new ActivityDisplay(activity);
        var nativeRes = display.getNativeResolutionRotated();
        var nativeRR = display.getNativeRefreshRate();
        ModuleLog.i("Native resolution: " + nativeRes.x + "x" + nativeRes.y);
        ModuleLog.i("Native refresh rate: " + nativeRR);

        int w = config.useNativeResolution ? nativeRes.x : config.customWidth;
        int h = config.useNativeResolution ? nativeRes.y : config.customHeight;

        if (config.changeResolutionScale) {
            w = (int)(w * config.widthScale);
            h = (int)(h * config.heightScale);
        }

        var maxFps = config.useNativeRefreshRate ? Math.round(nativeRR) : config.maxFps;
        try {
            System.loadLibrary("udt-native");
            XposedEntry.startApply(config.delay, config.changeResolution, w, h, config.changeMaxFps, maxFps);
        } catch (UnsatisfiedLinkError e) {
            ModuleLog.e("Failed to apply", e);
        }
    }
}
