package jp.miruku.unitydisplaytweaker.manager;

import android.app.Dialog;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.fragment.app.DialogFragment;

import com.google.android.material.dialog.MaterialAlertDialogBuilder;

import jp.miruku.unitydisplaytweaker.R;

public class InitializationFailedDialogFragment extends DialogFragment {
    @NonNull
    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        var builder = new MaterialAlertDialogBuilder(requireContext());
        builder.setTitle(R.string.dialog_initialization_failed_title);
        builder.setMessage(R.string.dialog_initialization_failed_message);
        builder.setPositiveButton(android.R.string.ok, null);

        return builder.create();
    }
}
