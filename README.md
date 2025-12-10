# UnityDisplayTweaker
![Android](https://img.shields.io/badge/Android-3DDC84?logo=android&logoColor=white)
![Xposed: 93+](https://img.shields.io/badge/Xposed-93%2B-blue)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

English | [日本語](./README-jaJP.md)

## Overview
This is an Xposed module that allows you to adjust the **rendering resolution** and **maximum FPS** of Unity (IL2CPP) games running on Android devices.  
It can be useful if you want more control over graphics-related settings.

## Warning
- This module does **not** work with every Unity game.
- Mono and x86/x86_64 builds are not supported.
- This module modifies internal game behavior and **may be detected** by some titles, which could result in **account restrictions**.

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
5. Launch thd game and enjoy!

## Credits
This project was inspired by the following work:
- [UnityFPSUnlocker](https://github.com/hexstr/UnityFPSUnlocker/)

