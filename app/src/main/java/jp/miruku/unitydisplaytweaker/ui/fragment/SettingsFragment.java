package jp.miruku.unitydisplaytweaker.ui.fragment;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;

import androidx.annotation.Nullable;

import jp.miruku.material.preference.PreferenceFragmentMaterial;
import jp.miruku.unitydisplaytweaker.R;

import java.util.Map;
import java.util.Objects;
import java.util.Set;

public class SettingsFragment extends PreferenceFragmentMaterial implements SharedPreferences.OnSharedPreferenceChangeListener {
    private SharedPreferences mSp;
    private boolean mInitialized = false;

    public boolean isInitialized() {
        return mInitialized;
    }

    @Override
    @SuppressWarnings("deprecation")
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        getPreferenceManager().setSharedPreferencesMode(Context.MODE_PRIVATE);
        try {
            mSp = requireContext().getSharedPreferences("module_config", Context.MODE_WORLD_READABLE);
        } catch (SecurityException e) {
            mInitialized = true;
        }
        setPreferencesFromResource(R.xml.root_preferences, rootKey);
    }

    @Override
    public void onResume() {
        super.onResume();
        if (mSp != null) {
            Objects.requireNonNull(getPreferenceScreen().getSharedPreferences()).registerOnSharedPreferenceChangeListener(this);
        }
    }

    @Override
    public void onPause() {
        super.onPause();
        if (mSp != null) {
            Objects.requireNonNull(getPreferenceScreen().getSharedPreferences()).unregisterOnSharedPreferenceChangeListener(this);
        }
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, @Nullable String s) {
        var editor = mSp.edit();
        for (Map.Entry<String, ?> entry : sharedPreferences.getAll().entrySet()) {
            var key = entry.getKey();
            var value = entry.getValue();
            if (value instanceof String) {
                editor.putString(key, (String) value);
            } else if (value instanceof Integer) {
                editor.putInt(key, (Integer) value);
            } else if (value instanceof Boolean) {
                editor.putBoolean(key, (Boolean) value);
            } else if (value instanceof Float) {
                editor.putFloat(key, (Float) value);
            } else if (value instanceof Long) {
                editor.putLong(key, (Long) value);
            } else if (value instanceof Set<?>) {
                @SuppressWarnings("unchecked")
                Set<String> set = (Set<String>) value;
                editor.putStringSet(key, set);
            }
        }
        editor.apply();
    }
}