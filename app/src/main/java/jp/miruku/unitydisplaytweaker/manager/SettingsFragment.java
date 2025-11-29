package jp.miruku.unitydisplaytweaker.manager;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;

import androidx.annotation.Nullable;
import androidx.preference.SwitchPreference;
import androidx.preference.SwitchPreferenceCompat;

import com.google.android.material.dialog.MaterialAlertDialogBuilder;

import jp.miruku.material.preference.PreferenceFragmentMaterial;
import jp.miruku.unitydisplaytweaker.R;
import jp.miruku.unitydisplaytweaker.module.ModuleConstants;

import java.util.Map;
import java.util.Objects;

public class SettingsFragment extends PreferenceFragmentMaterial implements SharedPreferences.OnSharedPreferenceChangeListener {
    private SharedPreferences mSp;
    private OnInitializationFailedListener mFailedListener;

    private void callInitilizationFailed() {
        if (mFailedListener != null) {
            mFailedListener.OnInitializationFailed();
        }
    }

    public void setInitializationFailedListener(@Nullable OnInitializationFailedListener listener) {
        mFailedListener = listener;
    }

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        getPreferenceManager().setSharedPreferencesMode(Context.MODE_PRIVATE);
        try {
            mSp = requireContext().getSharedPreferences("module_config", Context.MODE_WORLD_READABLE);
        } catch (SecurityException e) {
            callInitilizationFailed();
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
            }
        }
        editor.apply();
    }

    public interface OnInitializationFailedListener {
        void OnInitializationFailed();
    }
}