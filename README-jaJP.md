# UnityDisplayTweaker
![Android](https://img.shields.io/badge/Android-3DDC84?logo=android&logoColor=white)
![Xposed: 93+](https://img.shields.io/badge/Xposed-93%2B-blue)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

[English](./README.md) | 日本語

## 概要
これは Android 上で動作する Unity（IL2CPP）製ゲームの **解像度** や **最大FPS** を変更できる Xposed モジュールです。
ゲームの描画設定を細かく調整したい場合に役立ちます。

## 注意
- すべての Unity タイトルで動作するわけではありません。
- Mono 版や x86/x86_64 ビルドには非対応です。
- ゲーム内部の処理に干渉するため、タイトルによっては **不正行為判定** や **アカウント制限** が発生する恐れがあります。

## 動作環境

### 対応デバイス
- **Android バージョン**: 7.0 (API 24) 以上
- **Xposed API**: 93 以上

### 対応ゲーム
- **ゲームエンジン**: Unity
- **ランタイム**: IL2CPP
- **ABI**: armeabi-v7a / arm64-v8a

## 導入手順
1. モジュールの APK を入手してインストールします。
2. お使いの Xposed マネージャーで本モジュールを有効化します。
3. 対象ゲームをモジュールのスコープに追加します。
4. アプリを起動し、変更したい設定を調整します。
5. ゲームを起動して動作を確認してください。

## 参考
このプロジェクトは、以下のプロジェクトに感化されて開発されました：
- UnityFPSUnlocker（https://github.com/hexstr/UnityFPSUnlocker/tree/zygisk_module/UnityFPSUnlocker）
