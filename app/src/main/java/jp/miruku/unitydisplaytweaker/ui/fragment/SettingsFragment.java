package jp.miruku.unitydisplaytweaker.ui.fragment;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.core.view.WindowInsetsCompat;
import androidx.recyclerview.widget.RecyclerView;

import jp.miruku.material.preference.PreferenceFragmentMaterial;
import jp.miruku.unitydisplaytweaker.R;

import java.util.Set;

public class SettingsFragment extends PreferenceFragmentMaterial implements SharedPreferences.OnSharedPreferenceChangeListener {
    private static final String INITIALIZATION_FAILED_DIALOG_TAG = "initialization_failed_dialog";

    private SharedPreferences mSp;

    @Override
    @NonNull
    public RecyclerView onCreateRecyclerView(@NonNull LayoutInflater inflater, @NonNull ViewGroup parent, @Nullable Bundle savedInstanceState) {
        var listView = super.onCreateRecyclerView(inflater, parent, savedInstanceState);
        listView.setClipToPadding(false);

        final int baseLeft = listView.getPaddingLeft();
        final int baseTop = listView.getPaddingTop();
        final int baseRight = listView.getPaddingRight();
        final int baseBottom = listView.getPaddingBottom();

        listView.setOnApplyWindowInsetsListener((v2, insets) -> {
            var insetsCompat = WindowInsetsCompat.toWindowInsetsCompat(insets);
            var systemInsets = insetsCompat.getInsets(
                    WindowInsetsCompat.Type.systemBars()
            );

            listView.setPadding(baseLeft, baseTop, baseRight, baseBottom + systemInsets.bottom);
            return insets;
        });

        return listView;
    }

    private void onInitializationFailed() {
        var fm = getParentFragmentManager();
        if (fm.findFragmentByTag(INITIALIZATION_FAILED_DIALOG_TAG) == null) {
            var df = new InitializationFailedDialogFragment();
            df.show(fm, INITIALIZATION_FAILED_DIALOG_TAG);
        }
    }

    @Override
    @SuppressWarnings("deprecation")
    @SuppressLint("WorldReadableFiles")
    public void onCreatePreferences(@Nullable Bundle savedInstanceState, String rootKey) {
        getPreferenceManager().setSharedPreferencesMode(Context.MODE_PRIVATE);
        setPreferencesFromResource(R.xml.root_preferences, rootKey);
        try {
            mSp = requireContext().getSharedPreferences("module_config", Context.MODE_WORLD_READABLE);
        } catch (SecurityException ignored) {
            onInitializationFailed();
            getPreferenceScreen().setEnabled(false);
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        if (mSp != null) {
            var sp = getPreferenceScreen().getSharedPreferences();
            if (sp != null) sp.registerOnSharedPreferenceChangeListener(this);
        }
    }

    @Override
    public void onPause() {
        super.onPause();
        if (mSp != null) {
            var sp = getPreferenceScreen().getSharedPreferences();
            if (sp != null) sp.unregisterOnSharedPreferenceChangeListener(this);
        }
    }

    @Override
    public void onSharedPreferenceChanged(@NonNull SharedPreferences sharedPreferences, @Nullable String key) {
        var editor = mSp.edit();
        for (var entry : sharedPreferences.getAll().entrySet()) {
            var k = entry.getKey();
            var v = entry.getValue();
            if (v instanceof String) {
                editor.putString(k, (String) v);
            } else if (v instanceof Integer) {
                editor.putInt(k, (Integer) v);
            } else if (v instanceof Boolean) {
                editor.putBoolean(k, (Boolean) v);
            } else if (v instanceof Float) {
                editor.putFloat(k, (Float) v);
            } else if (v instanceof Long) {
                editor.putLong(k, (Long) v);
            } else if (v instanceof Set<?>) {
                @SuppressWarnings("unchecked")
                var set = (Set<String>) v;
                editor.putStringSet(k, set);
            }
        }
        editor.apply();
    }
}