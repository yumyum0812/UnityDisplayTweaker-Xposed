package jp.miruku.unitydisplaytweaker.module;

public class Config {
    public boolean showToastOnLoad;
    public float delayApply;
    public boolean changeResolution;
    public boolean useWindowResolution;
    public int customWidth;
    public int customHeight;
    public boolean useResolutionScale;
    public float widthScale;
    public float heightScale;
    public boolean changeFpsCap;
    public boolean useNativeRefreshRate;
    public int fpsCap;

    private static Config config;

    public static void load() {
        var prefs = new Preferences("module_config");
        config = new Config();
        config.showToastOnLoad = prefs.loadBoolean("show_toast", true);
        config.delayApply = prefs.loadFloatStr("delay", 5.f);
        config.changeResolution = prefs.loadBoolean("change_resolution", false);
        config.useWindowResolution = prefs.loadBoolean("use_window_resolution", false);
        config.customWidth = prefs.loadIntStr("custom_width", 1280);
        config.customHeight = prefs.loadIntStr("custom_height", 720);
        config.useResolutionScale = prefs.loadBoolean("change_resolution_scale", false);
        config.widthScale = prefs.loadFloatStr("width_scale", 1.f);
        config.heightScale = prefs.loadFloatStr("height_scale", 1.f);
        config.changeFpsCap = prefs.loadBoolean("change_max_fps", false);
        config.useNativeRefreshRate = prefs.loadBoolean("use_native_refresh_rate", false);
        config.fpsCap = prefs.loadIntStr("max_fps", 60);
    }

    public static Config getConfig() {
        return config;
    }
}
