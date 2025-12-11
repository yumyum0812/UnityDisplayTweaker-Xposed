package jp.miruku.unitydisplaytweaker.module.display;

import android.app.Activity;
import android.graphics.Point;
import android.os.Build;
import android.view.Display;
import android.view.Surface;

import androidx.annotation.NonNull;

public class ActivityDisplay {
    private final Display mDisplay;
    private final Display.Mode mMode;

    public ActivityDisplay(@NonNull Activity activity) {
        mDisplay = getDisplayInternal(activity);
        mMode = mDisplay.getMode();
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

    public Point getNativeResolution() {
        int physicalWidth = mMode.getPhysicalWidth();
        int physicalHeight = mMode.getPhysicalHeight();
        return new Point(physicalWidth, physicalHeight);
    }

    public Point getNativeResolutionRotated() {
        var size = getNativeResolution();
        int w = size.x;
        int h = size.y;

        int rotation = mDisplay.getRotation();
        if (rotation == Surface.ROTATION_90 || rotation == Surface.ROTATION_270) {
            return new Point(h, w);
        }
        return size;
    }

    public float getNativeRefreshRate() {
        return mMode.getRefreshRate();
    }
}
