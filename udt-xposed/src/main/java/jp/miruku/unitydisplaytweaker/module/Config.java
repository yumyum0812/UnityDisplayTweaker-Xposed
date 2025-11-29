package jp.miruku.unitydisplaytweaker.module;

public class Config {
    public boolean showToast;
    public float delay;
    public boolean changeResolution;
    public boolean useNativeResolution;
    public int customWidth;
    public int customHeight;
    public boolean changeResolutionScale;
    public float widthScale;
    public float heightScale;
    public boolean changeMaxFps;
    public boolean useNativeRefreshRate;
    public int maxFps;

    public Config(Preferences prefs) {
        showToast = prefs.loadBoolean("show_toast", true);
        delay = prefs.loadFloatStr("delay", 5.f);
        changeResolution = prefs.loadBoolean("change_resolution", false);
        useNativeResolution = prefs.loadBoolean("use_native_resolution", false);
        customWidth = prefs.loadIntStr("custom_width", 1280);
        customHeight = prefs.loadIntStr("custom_height", 720);
        changeResolutionScale = prefs.loadBoolean("change_resolution_scale", false);
        widthScale = prefs.loadFloatStr("width_scale", 1.f);
        heightScale = prefs.loadFloatStr("height_scale", 1.f);
        changeMaxFps = prefs.loadBoolean("change_max_fps", false);
        useNativeRefreshRate = prefs.loadBoolean("use_native_refresh_rate", false);
        maxFps = prefs.loadIntStr("max_fps", 60);
    }
}
