package jp.miruku.material.preference;

import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
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

import com.google.android.material.dialog.MaterialAlertDialogBuilder;

public abstract class PreferenceDialogFragmentMaterial extends DialogFragment implements DialogInterface.OnClickListener {
    protected static final String ARG_KEY = "key";
    private static final String SAVE_STATE_TITLE = "PreferenceDialogFragment.title";
    private static final String SAVE_STATE_POSITIVE_TEXT = "PreferenceDialogFragment.positiveText";
    private static final String SAVE_STATE_NEGATIVE_TEXT = "PreferenceDialogFragment.negativeText";
    private static final String SAVE_STATE_MESSAGE = "PreferenceDialogFragment.message";
    private static final String SAVE_STATE_LAYOUT = "PreferenceDialogFragment.layout";
    private static final String SAVE_STATE_ICON = "PreferenceDialogFragment.icon";

    private int mWhichButtonClicked;
    private DialogPreference mPreference;
    private CharSequence mDialogTitle;
    private CharSequence mPositiveButtonText;
    private CharSequence mNegativeButtonText;
    private CharSequence mDialogMessage;
    private int mDialogLayoutRes;
    private BitmapDrawable mDialogIcon;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        var rawFragment = getParentFragment();
        if (!(rawFragment instanceof DialogPreference.TargetFragment)) {
            throw new IllegalStateException("Target fragment must implement TargetFragment interface");
        }

        var fragment = (DialogPreference.TargetFragment) rawFragment;
        String key = requireArguments().getString(ARG_KEY);
        assert key != null;

        if (savedInstanceState == null) {
            mPreference = fragment.findPreference(key);
            if (mPreference == null) {
                throw new IllegalArgumentException("preference of " + key + " is null");
            }
            mDialogTitle = mPreference.getDialogTitle();
            mPositiveButtonText = mPreference.getPositiveButtonText();
            mNegativeButtonText = mPreference.getNegativeButtonText();
            mDialogMessage = mPreference.getDialogMessage();
            mDialogLayoutRes = mPreference.getDialogLayoutResource();

            final Drawable icon = mPreference.getDialogIcon();
            if (icon == null || icon instanceof BitmapDrawable) {
                mDialogIcon = (BitmapDrawable) icon;
            } else {
                final Bitmap bitmap = Bitmap.createBitmap(icon.getIntrinsicWidth(), icon.getIntrinsicHeight(), Bitmap.Config.ARGB_8888);
                final Canvas canvas = new Canvas(bitmap);
                icon.setBounds(0, 0, canvas.getWidth(), canvas.getHeight());
                icon.draw(canvas);
                mDialogIcon = new BitmapDrawable(getResources(), bitmap);
            }
        } else {
            mDialogTitle = savedInstanceState.getCharSequence(SAVE_STATE_TITLE);
            mPositiveButtonText = savedInstanceState.getCharSequence(SAVE_STATE_POSITIVE_TEXT);
            mNegativeButtonText = savedInstanceState.getCharSequence(SAVE_STATE_NEGATIVE_TEXT);
            mDialogMessage = savedInstanceState.getCharSequence(SAVE_STATE_MESSAGE);
            mDialogLayoutRes = savedInstanceState.getInt(SAVE_STATE_LAYOUT, 0);
            Bitmap bitmap = savedInstanceState.getParcelable(SAVE_STATE_ICON);
            if (bitmap != null) {
                mDialogIcon = new BitmapDrawable(getResources(), bitmap);
            }
        }
    }

    @Override
    public void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);

        outState.putCharSequence(SAVE_STATE_TITLE, mDialogTitle);
        outState.putCharSequence(SAVE_STATE_POSITIVE_TEXT, mPositiveButtonText);
        outState.putCharSequence(SAVE_STATE_NEGATIVE_TEXT, mNegativeButtonText);
        outState.putCharSequence(SAVE_STATE_MESSAGE, mDialogMessage);
        outState.putInt(SAVE_STATE_LAYOUT, mDialogLayoutRes);
        if (mDialogIcon != null) {
            outState.putParcelable(SAVE_STATE_ICON, mDialogIcon.getBitmap());
        }
    }

    @NonNull
    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        mWhichButtonClicked = DialogInterface.BUTTON_NEGATIVE;

        var builder = new MaterialAlertDialogBuilder(requireContext());
        builder.setTitle(mDialogTitle);
        builder.setIcon(mDialogIcon);
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
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            Window window = dialog.getWindow();
            assert window != null;
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
    private static final class Api30Impl {
        @DoNotInline
        static void showIme(@NonNull Window dialogWindow) {
            var dv = dialogWindow.getDecorView();
            var ic = dv.getWindowInsetsController();
            assert ic != null;

            ic.show(WindowInsets.Type.ime());
        }
    }
}
