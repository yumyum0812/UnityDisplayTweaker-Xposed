package jp.miruku.unitydisplaytweaker.module;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import de.robv.android.xposed.XSharedPreferences;
import jp.miruku.unitydisplaytweaker.module.common.Constants;

public class Preferences {
    private final XSharedPreferences mPrefs;

    public Preferences(@NonNull String group) {
        mPrefs = new XSharedPreferences(Constants.MANAGER_PACKAGE_NAME, group);
        mPrefs.makeWorldReadable();
        mPrefs.reload();
    }

    public boolean loadBoolean(@NonNull String key, boolean def) {
        try {
            return mPrefs.getBoolean(key, def);
        } catch (ClassCastException e) {
            return def;
        }
    }

    public String loadStr(@NonNull String key, @Nullable String def) {
        try {
            return mPrefs.getString(key, def);
        } catch (ClassCastException e) {
            return def;
        }
    }

    public int loadIntStr(@NonNull String key, int def) {
        try {
            var s = mPrefs.getString(key, String.valueOf(def));
            return Integer.parseInt(s);
        } catch (ClassCastException e) {
            return def;
        }
    }

    public float loadFloatStr(@NonNull String key, float def) {
        try {
            var s = mPrefs.getString(key, String.valueOf(def));
            return Float.parseFloat(s);
        } catch (ClassCastException e) {
            return def;
        }
    }
}
