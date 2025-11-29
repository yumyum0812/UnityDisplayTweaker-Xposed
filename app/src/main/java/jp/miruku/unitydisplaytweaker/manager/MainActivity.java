package jp.miruku.unitydisplaytweaker.manager;

import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import jp.miruku.unitydisplaytweaker.R;

public class MainActivity extends AppCompatActivity {
    private static final String INITIALIZATION_FAILED_DIALOG_TAG = "initialization_failed";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main2), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(v.getPaddingLeft(), v.getPaddingTop(), v.getPaddingRight(), systemBars.bottom);
            return insets;
        });

        var sf = new SettingsFragment();
        sf.setInitializationFailedListener(this::onSettingsInitializationFailed);

        getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, sf).commit();
        var toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
    }

    private void onSettingsInitializationFailed() {
        var f = new InitializationFailedDialogFragment();
        if (getSupportFragmentManager().findFragmentByTag(INITIALIZATION_FAILED_DIALOG_TAG) == null) {
            f.show(getSupportFragmentManager(), INITIALIZATION_FAILED_DIALOG_TAG);
        }
    }
}