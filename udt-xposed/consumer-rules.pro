-dontwarn de.robv.android.xposed.**
-keep class de.robv.android.xposed.** { *; }

-keep class * implements de.robv.android.xposed.IXposedHookLoadPackage {
    public <init>(...);
}

-keep class * implements de.robv.android.xposed.IXposedHookZygoteInit {
    public <init>(...);
}

-keep class * implements de.robv.android.xposed.IXposedHookInitPackageResources {
    public <init>(...);
}