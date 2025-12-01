package jp.miruku.unitydisplaytweaker.ui.activity;

import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import jp.miruku.unitydisplaytweaker.R;
import jp.miruku.unitydisplaytweaker.ui.fragment.InitializationFailedDialogFragment;
import jp.miruku.unitydisplaytweaker.ui.fragment.SettingsFragment;

public class MainActivity extends AppCompatActivity {
    private static final String INITIALIZATION_FAILED_DIALOG_TAG = "initialization_failed";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(v.getPaddingLeft(), v.getPaddingTop(), v.getPaddingRight(), systemBars.bottom);
            return insets;
        });

        var toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        var f = (SettingsFragment) getSupportFragmentManager().findFragmentById(R.id.main_fragment);
        if (f == null || f.isInitialized()) {
            onSettingsInitializationFailed();
        }
    }

    private void onSettingsInitializationFailed() {
        var f = new InitializationFailedDialogFragment();
        var fm = getSupportFragmentManager();
        if (fm.findFragmentByTag(INITIALIZATION_FAILED_DIALOG_TAG) == null) {
            f.show(fm, INITIALIZATION_FAILED_DIALOG_TAG);
        }
    }
}