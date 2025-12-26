package jp.miruku.unitydisplaytweaker.module;

import android.app.Activity;
import android.view.View;
import android.view.Window;
import android.widget.Toast;

import androidx.annotation.NonNull;

import de.robv.android.xposed.XC_MethodHook;
import de.robv.android.xposed.XposedBridge;
import de.robv.android.xposed.XposedHelpers;
import de.robv.android.xposed.callbacks.XC_LoadPackage;
import jp.miruku.unitydisplaytweaker.module.display.ActivityDisplay;
import jp.miruku.unitydisplaytweaker.module.util.ModuleLog;

public class UnityPlayerHooks {
    private static final String UNITY_PLAYER_CLASS = "com.unity3d.player.UnityPlayer";

    private final XC_LoadPackage.LoadPackageParam mModuleParam;
    private final Config mConfig;

    public UnityPlayerHooks(@NonNull XC_LoadPackage.LoadPackageParam param) {
        this.mModuleParam = param;
        this.mConfig = Config.getConfig();
    }

    public void initialize() throws ClassNotFoundException {
        var clz = mModuleParam.classLoader.loadClass(UNITY_PLAYER_CLASS);
        XposedBridge.hookAllConstructors(clz, new ConstructorHook());
    }

    private class ConstructorHook extends XC_MethodHook {
        private int mWindowWidth, mWindowHeight = 0;
        private Activity mActivity;
        private View mContentView;
        private boolean mDelayInit = false;

        @Override
        protected void afterHookedMethod(@NonNull XC_MethodHook.MethodHookParam param) {
            mActivity = (Activity) XposedHelpers.getStaticObjectField(param.thisObject.getClass(), "currentActivity");
            var window = mActivity.getWindow();

            if (mConfig.showToastOnLoad) {
                var t = Toast.makeText(mActivity, "UDT loaded", Toast.LENGTH_SHORT);
                t.show();
            }

            mContentView = window.findViewById(Window.ID_ANDROID_CONTENT);
            mContentView.addOnLayoutChangeListener((
                    v,
                    left, top, right, bottom,
                    oldLeft, oldTop, oldRight, oldBottom) -> {
                updateResolution();
            });

            mContentView.postDelayed(this::delayInitialize, (int)(mConfig.delayApply * 1000));
        }

        private void delayInitialize() {
            mDelayInit = true;
            initizalizeApi();
            updateResolution();
            updateFps();
        }

        private void initizalizeApi() {
            UDTNative.initialize();
        }

        private void updateResolution() {
            if (!mDelayInit) return;
            if (!mConfig.changeResolution) return;

            int mw = mContentView.getMeasuredWidth();
            int mh = mContentView.getMeasuredHeight();
            if (mWindowWidth != mw || mWindowHeight != mh) {
                ModuleLog.i("Window size: " + mw + "x" + mh);
                mWindowWidth = mw;
                mWindowHeight = mh;

                int w = mConfig.useWindowResolution ? mw : mConfig.customWidth;
                int h = mConfig.useWindowResolution ? mh : mConfig.customHeight;
                if (mConfig.useResolutionScale) {
                    w = Math.round(w * mConfig.widthScale);
                    h = Math.round(h * mConfig.heightScale);
                }

                int targetWidth = w;
                int targetHeight = h;
                ModuleLog.i("Target resolution: " + w + "x" + h);
                UDTNative.setResolution(targetWidth, targetHeight);
            }
        }

        private void updateFps() {
            if (!mDelayInit) return;
            if (!mConfig.changeFpsCap) return;
            var ad = new ActivityDisplay(mActivity);
            int rr = Math.round(ad.getRefreshRate());
            ModuleLog.i("Display refresh rate: " + rr);

            int last = mConfig.useNativeRefreshRate ? rr : mConfig.fpsCap;
            ModuleLog.i("Target FPS cap: " + last);
            UDTNative.setFpsCap(last);
        }
    }
}
