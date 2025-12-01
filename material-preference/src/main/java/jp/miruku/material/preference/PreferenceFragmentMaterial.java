package jp.miruku.material.preference;

import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.DialogFragment;
import androidx.fragment.app.Fragment;
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
        boolean handled = false;
        if (getCallbackFragment() instanceof OnPreferenceDisplayDialogCallback) {
            handled = ((OnPreferenceDisplayDialogCallback) getCallbackFragment()).onPreferenceDisplayDialog(this, preference);
        }

        Fragment callbackFragment = this;
        while (!handled && callbackFragment != null) {
            if (callbackFragment instanceof OnPreferenceDisplayDialogCallback) {
                handled = ((OnPreferenceDisplayDialogCallback) callbackFragment).onPreferenceDisplayDialog(this, preference);
            }
            callbackFragment = callbackFragment.getParentFragment();
        }
        if (!handled && getContext() instanceof OnPreferenceDisplayDialogCallback) {
            handled = ((OnPreferenceDisplayDialogCallback) getContext()).onPreferenceDisplayDialog(this, preference);
        }
        if (!handled && getActivity() instanceof OnPreferenceDisplayDialogCallback) {
            handled = ((OnPreferenceDisplayDialogCallback) getActivity()).onPreferenceDisplayDialog(this, preference);
        }

        if (handled) {
            return;
        }

        var fm = getChildFragmentManager();
        if (fm.findFragmentByTag(DIALOG_FRAGMENT_TAG) != null) {
            return;
        }

        DialogFragment f;
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
        //f.setTargetFragment(this, 0);
        f.show(fm, DIALOG_FRAGMENT_TAG);
    }
}