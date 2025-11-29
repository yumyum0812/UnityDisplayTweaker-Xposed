package jp.miruku.material.preference;

import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Build;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.view.Window;
import android.view.WindowInsets;
import android.widget.TextView;

import androidx.annotation.DoNotInline;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AlertDialog;
import androidx.fragment.app.DialogFragment;
import androidx.preference.DialogPreference;
import androidx.preference.PreferenceDialogFragmentCompat;

import com.google.android.material.dialog.MaterialAlertDialogBuilder;

public abstract class PreferenceDialogFragmentMaterial extends DialogFragment implements DialogInterface.OnClickListener {
    protected static final String ARG_KEY = "key";
    private int mWhichButtonClicked;
    private DialogPreference mPreference;
    private String mKey;
    private CharSequence mTitle;
    private CharSequence mPositiveButtonText;
    private CharSequence mNegativeButtonText;
    private CharSequence mDialogMessage;
    private int mDialogLayoutRes;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        var rawFragment = getParentFragment();
        if (!(rawFragment instanceof DialogPreference.TargetFragment)) {
            throw new IllegalStateException("Target fragment must implement TargetFragment interface");
        }

        var fragment = (DialogPreference.TargetFragment) rawFragment;
        mKey = requireArguments().getString(ARG_KEY);
        if (mKey == null) {
            throw new IllegalArgumentException("ARG_KEY is null");
        }

        mPreference = fragment.findPreference(mKey);
        if (mPreference == null) {
            throw new IllegalArgumentException("preference of " + mKey + " is null");
        }

        mTitle = mPreference.getDialogTitle();
        mPositiveButtonText = mPreference.getPositiveButtonText();
        mNegativeButtonText = mPreference.getNegativeButtonText();
        mDialogMessage = mPreference.getDialogMessage();
        mDialogLayoutRes = mPreference.getDialogLayoutResource();
    }

    @Override
    public void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);
    }

    @NonNull
    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        mWhichButtonClicked = DialogInterface.BUTTON_NEGATIVE;

        var builder = new MaterialAlertDialogBuilder(requireContext());
        builder.setTitle(mTitle);
        builder.setPositiveButton(mPositiveButtonText, this);
        builder.setNegativeButton(mNegativeButtonText, this);

        View contentView = onCreateDialogView(requireContext());
        if (contentView != null) {
            onBindDialogView(contentView);
            builder.setView(contentView);
        } else {
            builder.setMessage(mDialogMessage);
        }
        onPrepareDialogBuilder(builder);

        var dialog = builder.create();
        if (needInputMethod()) {
            requestInputMethod(dialog);
        }
        return dialog;
    }

    protected boolean needInputMethod() {
        return false;
    }

    protected void onPrepareDialogBuilder(@NonNull AlertDialog.Builder builder) {}

    private void requestInputMethod(@NonNull Dialog dialog) {
        Window window = dialog.getWindow();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            Api30Impl.showIme(window);
        } else {
            scheduleShowSoftInput();
        }
    }

    protected void scheduleShowSoftInput() {
    }

    protected void onBindDialogView(@NonNull View view) {
        View dialogMessageView = view.findViewById(android.R.id.message);

        if (dialogMessageView != null) {
            if (!TextUtils.isEmpty(mDialogMessage)) {
                if (dialogMessageView instanceof TextView) {
                    ((TextView) dialogMessageView).setText(mDialogMessage);
                }
                dialogMessageView.setVisibility(View.VISIBLE);
            } else {
                dialogMessageView.setVisibility(View.GONE);
            }
        }
    }

    @Nullable
    protected View onCreateDialogView(@NonNull Context context) {
        final int resId = mDialogLayoutRes;
        if (resId == 0) {
            return null;
        }

        return getLayoutInflater().inflate(resId, null);
    }

    @Override
    public void onClick(@NonNull DialogInterface dialog, int which) {
        mWhichButtonClicked = which;
    }

    @Override
    public void onDismiss(@NonNull DialogInterface dialog) {
        super.onDismiss(dialog);
        onDialogClosed(mWhichButtonClicked == DialogInterface.BUTTON_POSITIVE);
    }

    public DialogPreference getPreference() {
        if (mPreference == null) {
            var key = requireArguments().getString(ARG_KEY);
            var fragment = (DialogPreference.TargetFragment) getParentFragment();
            assert fragment != null;
            assert key != null;
            mPreference = fragment.findPreference(key);
        }
        return mPreference;
    }

    public abstract void onDialogClosed(boolean positiveResult);

    @RequiresApi(Build.VERSION_CODES.R)
    private static class Api30Impl {
        // Prevent instantiation.
        private Api30Impl() {}

        @DoNotInline
        static void showIme(@NonNull Window dialogWindow) {
            dialogWindow.getDecorView().getWindowInsetsController().show(WindowInsets.Type.ime());
        }
    }
}
