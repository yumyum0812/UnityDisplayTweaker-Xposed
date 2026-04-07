# UnityDisplayTweaker
![Android](https://img.shields.io/badge/Android-3DDC84?logo=android&logoColor=white)
![Xposed: 93+](https://img.shields.io/badge/Xposed-93%2B-blue)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

[English](./README.md) | 日本語

## 概要
Android 上で動作する Unity（IL2CPP）で作成されたゲームの **解像度** や **最大FPS** を変更できる Xposed モジュールです。
解像度設定をより細かく調整したい場合に役立ちます。

## 注意
- すべての Unity ゲームで動作するわけではありません。
- Mono や x86/x86_64 ビルドには非対応です。
- ゲーム内部の処理に干渉するため、タイトルによっては **チート判定** や **アカウント制限** が発生する恐れがあります。
  自身の責任でご使用ください！

## 動作環境

### 対応デバイス
- **Android バージョン**：7.0（API 24）以上
- **Xposed API**：93 以上

### 対応ゲーム
- **ゲームエンジン**：Unity
- **ランタイム**：IL2CPP
- **ABI**：armeabi-v7a / arm64-v8a

## 導入手順
1. モジュールの APK を入手してインストールします。
2. お使いの Xposed マネージャーでこのモジュールを有効にします。
3. 対象ゲームをモジュールのスコープに追加します。
4. アプリを起動し、変更したい設定を調整します。
5. ゲームを起動して動作を確認してください。

## Galaxy 端末
一部のGalaxy端末では、Game Boosterの影響によりネイティブな解像度を基準にした解像度が正しく反映されないことがあります。
この場合は、該当の機能を無効化することで改善する可能性があります。
### 実行方法
以下のいずれかの環境でコマンドを実行してください：
- ADB：`adb shell ...`
- シェル権限：`su shell -c "..."`
### 無効化
#### Game Optimizing Service の無効化
```shell
pm disable-user --user 0 com.samsung.android.game.gos
```
#### Game Booster, Gaming Hub の削除
```shell
pm uninstall --user 0 com.samsung.android.game.gametools
pm uninstall --user 0 com.samsung.android.game.gamehome
```
### FPS制限の修正
- FPSをアンロックする場合は、開発者オプションにある **「Disable default frame rate for games」** を有効にしてください。

### 再有効化
#### Game Optimizing Service の有効化
```shell
pm enable --user 0 com.samsung.android.game.gos
```
#### Game Booster, Gaming Hub の復元
```shell
pm install-existing com.samsung.android.game.gametools
pm install-existing com.samsung.android.game.gamehome
```

## 参考
このプロジェクトは、以下のプロジェクトに感化されて開発されました：
- [UnityFPSUnlocker](https://github.com/hexstr/UnityFPSUnlocker/)

使用したライブラリ：
- [xDL](https://github.com/hexhacking/xDL)
- [XposedBridge](https://github.com/rovo89/XposedBridge/tree/art)