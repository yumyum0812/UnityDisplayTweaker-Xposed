package jp.miruku.unitydisplaytweaker.module;

public class Config {
    public boolean showToastOnLoad;
    public float delayApply;
    public boolean changeResolution;
    public boolean useNativeResolution;
    public int customWidth;
    public int customHeight;
    public boolean useResolutionScale;
    public float widthScale;
    public float heightScale;
    public boolean changeFpsCap;
    public boolean useNativeRefreshRate;
    public int fpsCap;

    public Config(Preferences prefs) {
        showToastOnLoad = prefs.loadBoolean("show_toast", true);
        delayApply = prefs.loadFloatStr("delay", 5.f);
        changeResolution = prefs.loadBoolean("change_resolution", false);
        useNativeResolution = prefs.loadBoolean("use_native_resolution", false);
        customWidth = prefs.loadIntStr("custom_width", 1280);
        customHeight = prefs.loadIntStr("custom_height", 720);
        useResolutionScale = prefs.loadBoolean("change_resolution_scale", false);
        widthScale = prefs.loadFloatStr("width_scale", 1.f);
        heightScale = prefs.loadFloatStr("height_scale", 1.f);
        changeFpsCap = prefs.loadBoolean("change_max_fps", false);
        useNativeRefreshRate = prefs.loadBoolean("use_native_refresh_rate", false);
        fpsCap = prefs.loadIntStr("max_fps", 60);
    }
}
