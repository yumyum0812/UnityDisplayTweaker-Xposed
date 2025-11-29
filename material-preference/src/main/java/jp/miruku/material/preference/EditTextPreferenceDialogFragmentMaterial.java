package jp.miruku.material.preference;

import android.content.Context;
import android.os.Bundle;
import android.os.SystemClock;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.preference.EditTextPreference;

public class EditTextPreferenceDialogFragmentMaterial extends PreferenceDialogFragmentMaterial {
    private static final String SAVE_STATE_TEXT = "edittext.text";

    private CharSequence mText;
    private EditText mEditText;

    private final Runnable mShowSoftInputRunnable = this::scheduleShowSoftInputInner;
    private long mShowRequestTime = -1;
    private static final int SHOW_REQUEST_TIMEOUT = 1000;

    public static EditTextPreferenceDialogFragmentMaterial newInstance(String key) {
        var args = new Bundle(1);
        args.putString(PreferenceDialogFragmentMaterial.ARG_KEY, key);

        var f = new EditTextPreferenceDialogFragmentMaterial();
        f.setArguments(args);
        return f;
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (savedInstanceState == null) {
            mText = getEditTextPreference().getText();
        } else {
            mText = savedInstanceState.getCharSequence(SAVE_STATE_TEXT);
        }
    }

    @Override
    public void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);
        outState.putCharSequence(SAVE_STATE_TEXT, mText);
    }

    @Override
    protected void onBindDialogView(@NonNull View view) {
        super.onBindDialogView(view);

        mEditText = view.findViewById(android.R.id.edit);
        if (mEditText == null) {
            throw new IllegalStateException("Dialog view must contain an EditText with id @android:id/edit");
        }

        mEditText.requestFocus();
        mEditText.setText(mText);
        mEditText.setSelection(mEditText.getText().length());
    }

    @Override
    protected boolean needInputMethod() {
        return true;
    }

    private EditTextPreference getEditTextPreference() {
        return (EditTextPreference) getPreference();
    }

    private boolean hasPendingShowSoftInputRequest() {
        return (mShowRequestTime != -1 && ((mShowRequestTime + SHOW_REQUEST_TIMEOUT) > SystemClock.currentThreadTimeMillis()));
    }

    private void setPendingShowSoftInputRequest(boolean pendingShowSoftInputRequest) {
        mShowRequestTime = pendingShowSoftInputRequest ? SystemClock.currentThreadTimeMillis() : -1;
    }

    void scheduleShowSoftInputInner() {
        if (hasPendingShowSoftInputRequest()) {
            if (mEditText == null || !mEditText.isFocused()) {
                setPendingShowSoftInputRequest(false);
                return;
            }
            final InputMethodManager imm = (InputMethodManager)
                    mEditText.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
            // Schedule showSoftInput once the input connection of the editor established.
            if (imm.showSoftInput(mEditText, 0)) {
                setPendingShowSoftInputRequest(false);
            } else {
                mEditText.removeCallbacks(mShowSoftInputRunnable);
                mEditText.postDelayed(mShowSoftInputRunnable, 50);
            }
        }
    }

    @Override
    public void onDialogClosed(boolean positiveResult) {
        if (positiveResult) {
            String value = mEditText.getText().toString();
            final EditTextPreference preference = getEditTextPreference();
            if (preference.callChangeListener(value)) {
                preference.setText(value);
            }
        }
    }
}
