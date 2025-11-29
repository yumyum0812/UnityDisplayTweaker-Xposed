package jp.miruku.unitydisplaytweaker.module;

import android.app.Activity;
import android.content.Context;
import android.graphics.Point;
import android.util.Log;
import android.view.Display;
import android.view.Surface;
import android.view.WindowManager;
import android.widget.Toast;

import androidx.annotation.NonNull;

import java.util.Objects;

import de.robv.android.xposed.XC_MethodHook;

public class OnCreateHook extends XC_MethodHook {
    private boolean mAlreadyLoaded = false;

    private static Display getCurrentDisplay(@NonNull Context context) {
        var wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        return wm.getDefaultDisplay();
    }

    private static Point getNativeResolution(@NonNull Context context) {
        var display = getCurrentDisplay(context);
        var md = display.getMode();

        int physicalWidth = md.getPhysicalWidth();
        int physicalHeight = md.getPhysicalHeight();

        int rotation = display.getRotation();
        boolean isLandscape = (rotation == Surface.ROTATION_90 || rotation == Surface.ROTATION_270);

        Point point = new Point();
        point.x = isLandscape ? physicalHeight : physicalWidth;
        point.y = isLandscape ? physicalWidth : physicalHeight;
        return point;
    }

    private static float getNativeRefreshRate(@NonNull Context context) {
        var display = getCurrentDisplay(context);
        var md = display.getMode();
        return md.getRefreshRate();
    }

    @Override
    protected void afterHookedMethod(@NonNull MethodHookParam param) {
        if (mAlreadyLoaded) return;
        mAlreadyLoaded = true;

        var context = (Activity) Objects.requireNonNull(param.thisObject);
        var config = MainModule.config;

        if (config.showToast) {
            Toast.makeText(context, "UDT Loaded", Toast.LENGTH_SHORT).show();
        }

        var nativeRes = getNativeResolution(context);
        var nativeRR = getNativeRefreshRate(context);
        Log.i(ModuleConstants.LOG_TAG, "Native resolution: " + nativeRes.x + "x" + nativeRes.y);
        Log.i(ModuleConstants.LOG_TAG, "Native refresh rate: " + nativeRR);

        int w = config.useNativeResolution ? nativeRes.x : config.customWidth;
        int h = config.useNativeResolution ? nativeRes.y : config.customHeight;

        if (config.changeResolutionScale) {
            w = (int)(w * config.widthScale);
            h = (int)(h * config.heightScale);
        }

        var maxFps = config.useNativeRefreshRate ? Math.round(nativeRR) : config.maxFps;

        Log.i(ModuleConstants.LOG_TAG, "Target resolution: " + w + "x" + h);
        Log.i(ModuleConstants.LOG_TAG, "Target frame rate: " + maxFps);

        try {
            System.loadLibrary("udt-native");
            MainModule.startApply(config.delay, config.changeResolution, w, h, config.changeMaxFps, maxFps);
        } catch (UnsatisfiedLinkError e) {
            Log.e(ModuleConstants.LOG_TAG, e.getClass().getName() + ": " + e.getMessage());
        }
    }
}
