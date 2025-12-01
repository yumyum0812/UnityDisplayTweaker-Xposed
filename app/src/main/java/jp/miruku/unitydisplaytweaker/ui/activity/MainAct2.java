package jp.miruku.unitydisplaytweaker.ui.activity;

import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;
import androidx.navigation.fragment.NavHostFragment;
import androidx.navigation.ui.NavigationUI;

import com.google.android.material.navigation.NavigationBarView;

import jp.miruku.unitydisplaytweaker.R;

public class MainAct2 extends AppCompatActivity {
    @Override
    public void onCreate(Bundle savedInstanceState) {
        setContentView(R.layout.act2);

        var navHostFragment = (NavHostFragment) getSupportFragmentManager().findFragmentById(R.id.nav_host_fragment);

        var navController = navHostFragment.getNavController();
        var nav = (NavigationBarView) findViewById(R.id.nav);
        NavigationUI.setupWithNavController(nav, navController);
    }
}
