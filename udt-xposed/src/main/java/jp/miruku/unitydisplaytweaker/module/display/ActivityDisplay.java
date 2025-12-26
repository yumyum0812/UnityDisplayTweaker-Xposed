package jp.miruku.unitydisplaytweaker.module.display;

import android.app.Activity;
import android.os.Build;
import android.view.Display;

import androidx.annotation.NonNull;

public class ActivityDisplay {
    private final Display mDisplay;

    public ActivityDisplay(@NonNull Activity activity) {
        mDisplay = getDisplayInternal(activity);
    }

    public float getRefreshRate() {
        float maxRefresh = 0f;

        for (var mode : mDisplay.getSupportedModes()) {
            if (mode.getRefreshRate() > maxRefresh) {
                maxRefresh = mode.getRefreshRate();
            }
        }

        return maxRefresh;
    }

    private static Display getDisplayInternal(@NonNull Activity activity) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            // Android 11 (API 30) or higher
            return activity.getDisplay();
        } else {
            // Android 10 (API 29) or lower
            @SuppressWarnings("deprecation")
            var display = activity.getWindowManager().getDefaultDisplay();
            return display;
        }
    }
}
