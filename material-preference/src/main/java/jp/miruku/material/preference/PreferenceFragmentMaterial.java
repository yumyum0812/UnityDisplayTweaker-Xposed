package jp.miruku.material.preference;

import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.preference.EditTextPreference;
import androidx.preference.ListPreference;
import androidx.preference.Preference;
import androidx.preference.PreferenceFragmentCompat;

public abstract class PreferenceFragmentMaterial extends PreferenceFragmentCompat {
    private static final String DIALOG_FRAGMENT_TAG = "preference_dialog";

    @Override
    public abstract void onCreatePreferences(@Nullable Bundle savedInstanceState, @Nullable String rootKey);

    @Override
    public void onDisplayPreferenceDialog(@NonNull Preference preference) {
        if (getChildFragmentManager().findFragmentByTag(DIALOG_FRAGMENT_TAG) == null) {
            PreferenceDialogFragmentMaterial f;
            if (preference instanceof EditTextPreference) {
                f = EditTextPreferenceDialogFragmentMaterial.newInstance(preference.getKey());
            } else if (preference instanceof ListPreference) {
                f = ListPreferenceDialogFragmentMaterial.newInstance(preference.getKey());
            } else {
                throw new IllegalArgumentException(
                        "Cannot display dialog for an unknown Preference type: "
                                + preference.getClass().getSimpleName()
                                + ". Make sure to implement onPreferenceDisplayDialog() to handle "
                                + "displaying a custom dialog for this Preference.");
            }
            f.show(getChildFragmentManager(), DIALOG_FRAGMENT_TAG);
        }
    }

    /*@Override
    public void onDisplayPreferenceDialog(@NonNull Preference preference) {
        if (preference instanceof EditTextPreference) {
            var etPref = (EditTextPreference) preference;
            var container = new FrameLayout(requireContext());
            getLayoutInflater().inflate(etPref.getDialogLayoutResource(), container);

            var msgText = (TextView) container.findViewById(android.R.id.message);
            var msg = etPref.getDialogMessage();
            if (msg != null && msg.length() > 0) {
                msgText.setText(msg);
                msgText.setVisibility(View.VISIBLE);
            } else {
                msgText.setVisibility(View.GONE);
            }

            var et = (EditText) container.findViewById(android.R.id.edit);
            et.setText(etPref.getText());
            et.requestFocus();

            var builder = new MaterialAlertDialogBuilder(requireContext());
            builder.setIcon(etPref.getDialogIcon());
            builder.setTitle(etPref.getDialogTitle());
            builder.setView(container);
            builder.setPositiveButton(android.R.string.ok, (dialog, which) -> {
                etPref.setText(String.valueOf(et.getText()));
            });
            builder.setNegativeButton(android.R.string.cancel, null);

            var dialog = builder.create();
            et.postDelayed(() -> {
                var imm = (InputMethodManager) requireContext().getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.showSoftInput(et, 0);
            }, 200);
            dialog.show();
        } else if (preference instanceof ListPreference) {
            var listPref = (ListPreference) preference;
            var entries = listPref.getEntries();
            var entryValues = listPref.getEntryValues();

            int curr = listPref.findIndexOfValue(listPref.getValue());

            var builder = new MaterialAlertDialogBuilder(requireContext());
            builder.setIcon(listPref.getDialogIcon());
            builder.setTitle(preference.getTitle());
            builder.setSingleChoiceItems(entries, curr, (di, which) -> {
                listPref.setValue(String.valueOf(entryValues[which]));
                di.dismiss();
            });
            builder.setPositiveButton(android.R.string.cancel, null);

            var dialog = builder.create();
            dialog.show();
        } else {
            super.onDisplayPreferenceDialog(preference);
        }
    }/**/
}