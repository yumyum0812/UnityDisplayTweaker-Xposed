# UnityDisplayTweaker
![Android](https://img.shields.io/badge/Android-3DDC84?logo=android&logoColor=white)
![Xposed: 93+](https://img.shields.io/badge/Xposed-93%2B-blue)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

English | [日本語](./README-jaJP.md)

## Overview
This is an Xposed module that allows you to adjust the **rendering resolution** and **maximum FPS** of Unity (IL2CPP) games running on Android devices.  
It can be useful if you want more control over resolution settings.

## Warning
- This module does **not** work with every Unity game.
- Mono and x86/x86_64 builds are not supported.
- This module modifies internal game behavior and **may be detected** by some titles, which could result in **account restrictions**.
  Use at your own risk!

## Supported Requirements

### Device Requirements
- **Android Version**: 7.0 (API 24) or higher
- **Xposed API**: 93 or higher

### Game Requirements
- **Game Engine**: Unity
- **Runtime**: IL2CPP
- **ABI**: armeabi-v7a / arm64-v8a

## Installation
1. Download and install the module APK.
2. Enable the module in your Xposed manager.
3. Add your target game to the module scope.
4. Open the module app and adjust the settings to your liking.
5. Launch the game and enjoy!

## On Samsung Devices
On some Samsung devices, the resolution based on the native resolution may not be displayed correctly due to the effects of Game Booster.
In this case, disabling the feature may resolve the issue.
### Execution
Run the command in one of the following environments:
- ADB: `adb shell ...`
- Shell Privilege: `su shell -c "..."`
### Disable
#### Disable Game Optimizing Service
```shell
pm disable-user --user 0 com.samsung.android.game.gos
```
#### Uninstall Game Booster and Gaming Hub
```shell
pm uninstall --user 0 com.samsung.android.game.gametools
pm uninstall --user 0 com.samsung.android.game.gamehome
```
#### Fix FPS cap
- If you want to unlock FPS, turn on **"Disable default frame rate for games"** in the developer options.

### Re-Enable
#### Enable Game Optimizing Service
```shell
pm enable --user 0 com.samsung.android.game.gos
```
#### Restore Game Booster and Gaming Hub
```shell
pm install-existing com.samsung.android.game.gametools
pm install-existing com.samsung.android.game.gamehome
```

## Credits
This project was inspired by the following work:
- [UnityFPSUnlocker](https://github.com/hexstr/UnityFPSUnlocker/)

Libraries used:
- [xDL](https://github.com/hexhacking/xDL)
- [XposedBridge](https://github.com/rovo89/XposedBridge/tree/art)
