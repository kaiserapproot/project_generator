# クロスプラットフォーム プロジェクト自動生成ツール

このツールセットは、Visual Studio、VC++6.0、およびXcode用のプロジェクトファイルを自動生成するスクリプト集です。

## 概要

- **create_dual_dsp.bat**: VC++6.0用プロジェクト（.dsp/.dsw）自動生成
- **create_vcxproj.bat**: VS2022用プロジェクト（.vcxproj/.vcxproj.filters）自動生成
- **generate_xcodeproj.sh**: Xcode用プロジェクト（.xcodeproj）自動生成（macOS/Linux用）

Windows開発環境ではバッチスクリプト、macOS/iOS開発環境ではシェルスクリプトを使用し、各プラットフォームに最適化されたプロジェクトファイルを生成できます。全てのツールで、必要に応じてサンプルソースコードも自動生成します。

---

## create_vcxproj.bat（VS2022用）

### 基本的な使い方

```bat
cd vs2022
create_vcxproj.bat [オプション] プロジェクト名 [追加ソースファイル...]
```

### オプション

| オプション | 説明 |
|------------|------|
| `-src` | メインソースファイル（main.c または main.m）を自動生成 |
| `-objc` | Objective-Cプロジェクトとして生成（GNUStep対応） |
| `-path <パス>` | GNUStepのインストールパス（デフォルト: ./gnustep） |
| `-clean プロジェクト名` | 指定したプロジェクトの全ファイルを削除 |

### 使用例

#### 基本的なC/C++プロジェクト
```bat
create_vcxproj.bat MyProject
```

#### サンプルソース付きプロジェクト
```bat
create_vcxproj.bat -src MyProject
```

#### 追加ソースファイルを含むプロジェクト
```bat
create_vcxproj.bat -src MyProject utils.c network.c
```

#### Objective-Cプロジェクト（GNUStep使用）
```bat
create_vcxproj.bat -src -objc MyObjCProject
```

#### リポジトリ内蔵のGNUStepを使用（推奨）
```bat
create_vcxproj.bat -src -objc -path "./gnustep" MyObjCProject
```
このリポジトリには事前設定済みのGNUStep環境（./gnustepディレクトリ）が含まれているため、追加のインストール作業なしでObjective-Cプロジェクトを作成できます。

#### カスタムGNUStepパスでObjective-Cプロジェクト
```bat
create_vcxproj.bat -src -objc -path "C:\GNUstep" MyObjCProject
```

#### プロジェクトのクリーンアップ
```bat
create_vcxproj.bat -clean MyProject
```

### 生成されるファイル

| ファイル | 説明 |
|----------|------|
| `{プロジェクト名}.vcxproj` | メインプロジェクトファイル |
| `{プロジェクト名}.vcxproj.filters` | ソースファイルフィルタ設定 |
| `{プロジェクト名}.m` または `{プロジェクト名}_main.c` | メインソースファイル（-srcオプション時） |
| `{プロジェクト名}_build_all.bat` | 全構成ビルドバッチ |

### 対応する構成

- **文字セット**: MBCS, Unicode
- **プラットフォーム**: Win32 (x86), x64
- **構成**: Debug, Release
- **サブシステム**: Console, Windows（AppTypeプロパティで動的切替）

### Objective-C対応機能

- **ツールセット**: ClangCL（LLVM）
- **ランタイム**: GNUStep 2.0
- **対応拡張子**: .m（Objective-C）
- **自動設定**: GNUStepライブラリリンク、DLLコピー、コンパイラオプション
- **組み込み環境**: このリポジトリには事前設定済みのGNUStep環境（./gnustepディレクトリ）が含まれており、追加のインストールなしでObjective-C開発が可能

---

## create_dual_dsp.bat（VC++6.0用）

### 基本的な使い方

```bat
cd vs6
create_dual_dsp.bat [オプション] プロジェクト名
```

### オプション

| オプション | 説明 |
|------------|------|
| `-src` | メインソースファイル（{プロジェクト名}_main.c）を自動生成 |

### 使用例

#### 基本的なプロジェクト
```bat
create_dual_dsp.bat MyProject
```

#### サンプルソース付きプロジェクト
```bat
create_dual_dsp.bat -src MyProject
```

### 生成されるファイル

| ファイル | 説明 |
|----------|------|
| `win_{プロジェクト名}.dsp` | Windows用プロジェクトファイル |
| `con_{プロジェクト名}.dsp` | Console用プロジェクトファイル |
| `{プロジェクト名}.dsw` | ワークスペースファイル（両プロジェクトを含む） |
| `{プロジェクト名}_main.c` | メインソースファイル（-srcオプション時） |
| `build_{プロジェクト名}.bat` | 全構成ビルドバッチ |

### 対応する構成

- **プロジェクトタイプ**: Windows Application, Console Application
- **構成**: Debug, Release
- **プラットフォーム**: Win32

---

## generate_xcodeproj.sh（Xcode用）

### 基本的な使い方

```bash
./generate_xcodeproj.sh <プロジェクトディレクトリ名> <プラットフォーム>
```

### サポートするプラットフォーム

| プラットフォーム | 説明 | ターゲット |
|------------------|------|------------|
| `ios` | iOS用アプリケーション | iPhone/iPad（iOS 14.0以上） |
| `macos` | macOS用アプリケーション | macOS 10.14以上 |

### 使用例

#### iOS用プロジェクト生成
```bash
./generate_xcodeproj.sh MyiOSApp ios
```

#### macOS用プロジェクト生成
```bash
./generate_xcodeproj.sh MyMacApp macos
```

### 生成されるファイル

| ファイル | 説明 |
|----------|------|
| `{プロジェクト名}.xcodeproj/project.pbxproj` | Xcodeプロジェクトファイル |
| `Info.plist` | アプリケーション情報ファイル |
| `{プロジェクト名}_main.m` | メインソースファイル（Objective-C） |

### プロジェクト構成

#### iOS用
- **プロダクトタイプ**: iOS Application
- **デプロイメントターゲット**: iOS 14.0
- **対応デバイス**: iPhone/iPad（Universal）
- **SDK**: iOS SDK
- **バンドルID**: `com.example.{プロジェクト名}`

#### macOS用
- **プロダクトタイプ**: macOS Application
- **デプロイメントターゲット**: macOS 10.14
- **SDK**: macOS SDK
- **バンドルID**: `com.example.{プロジェクト名}`

### 使用方法の詳細

1. **プロジェクト生成**:
   ```bash
   ./generate_xcodeproj.sh MyApp ios
   ```

2. **生成されたディレクトリ構造**:
   ```
   MyApp/
   ├── MyApp.xcodeproj/
   │   └── project.pbxproj
   ├── Info.plist
   └── MyApp_main.m
   ```

3. **Xcodeで開く**:
   ```bash
   open MyApp/MyApp.xcodeproj
   ```

### 生成されるサンプルコード

#### iOS用main関数
```objc
#import <UIKit/UIKit.h>

int main(int argc, char * argv[]) {
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, nil);
    }
}
```

#### macOS用main関数
```objc
#import <Cocoa/Cocoa.h>

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        return NSApplicationMain(argc, argv);
    }
}
```

### 必要な環境

- **OS**: macOS
- **ツール**: Xcode Command Line Tools
- **シェル**: bash
- **テンプレートファイル**: 
  - `project_ios.pbxproj.template` (iOS用)
  - `project.pbxproj.template` (macOS用)

### エラーと対処法

#### テンプレートファイルが見つからない
```
エラー: project_ios.pbxproj.template が見つかりません
対処: カレントディレクトリにテンプレートファイルがあることを確認
```

#### 無効なプラットフォーム
```
エラー: サポートされていないプラットフォーム: android
対処: ios または macos を指定してください
```

#### 権限エラー
```
エラー: Permission denied
対処: chmod +x generate_xcodeproj.sh でスクリプトに実行権限を付与
```

---

## 生成されるサンプルソースコード

### C言語版（create_vcxproj.bat -src、create_dual_dsp.bat -src）

- **機能**: Windows/Console両対応
- **内容**: システム情報表示、ドライブ一覧、日時表示
- **互換性**: VC++6.0 C90準拠

### Objective-C版（create_vcxproj.bat -src -objc）

- **機能**: Win32ウィンドウアプリケーション
- **アーキテクチャ**: AppDelegate/View/ViewController パターン
- **内容**: ウィンドウ表示、マウスクリック処理、描画処理

---

## ビルド方法

### VS2022プロジェクト

#### 個別ビルド
```bat
MSBuild MyProject.vcxproj /p:Configuration="Debug MBCS" /p:Platform=x64 /p:AppType=WINDOWS
```

#### 全構成ビルド
```bat
MyProject_build_all.bat
```

### VC++6.0プロジェクト

#### 個別ビルド
```bat
msdev MyProject.dsw /make "win_MyProject - Win32 Release" /rebuild
```

#### 全構成ビルド
```bat
build_MyProject.bat
```

---

## 必要なファイル

### VS2022用（vs2022フォルダ）
- `template.vcxproj`: プロジェクトテンプレート
- `create_vcxproj.bat`: メインバッチスクリプト

### VC++6.0用（vs6フォルダ）
- `win_template.dsp`: Windows用プロジェクトテンプレート
- `con_template.dsp`: Console用プロジェクトテンプレート
- `create_dual_dsp.bat`: メインバッチスクリプト

---

## トラブルシューティング

### よくある問題

#### 1. エンコーディング問題
- **症状**: 日本語文字化け
- **対処**: UTF-8対応エディタで編集

#### 2. GNUStep関連エラー（Objective-C）
- **症状**: ヘッダファイルが見つからない
- **対処**: `-path`オプションで正しいGNUStepパスを指定

#### 3. ビルドエラー
- **症状**: MSBuildまたはmsdevが見つからない
- **対処**: Visual Studio Developer Command Promptを使用

#### 4. 古いファイルが残る
- **症状**: 設定変更が反映されない
- **対処**: `-clean`オプションでクリーンアップ後再生成

### エラーメッセージと対処法

```
エラー: プロジェクト名を指定してください
→ プロジェクト名引数が必要

エラー: -pathオプションに値が指定されていません
→ -pathの後にGNUStepパスを指定

エラー: *.m の作成に失敗しました
→ ディスク容量または権限を確認
```

---

## 高度な使用方法

### カスタム構成の追加

MSBuildプロパティを使用してカスタム構成を作成できます：

```bat
MSBuild MyProject.vcxproj /p:Configuration="Custom Debug" /p:Platform=x64 /p:AppType=CONSOLE /p:CharacterSet=MBCS
```

### プリプロセッサ定義の追加

```bat
MSBuild MyProject.vcxproj /p:PreprocessorDefinitions="CUSTOM_DEFINE;DEBUG_MODE"
```

### 複数プロジェクトの一括生成

```bat
for %%p in (Project1 Project2 Project3) do (
    create_vcxproj.bat -src %%p
)
```

---

## 更新履歴

- **2025年7月12日**: generate_xcodeproj.sh使用方法をREADME.mdに追加、エスケープシーケンス問題修正、Objective-C感嘆符エスケープ対応
- **2025年7月10日**: 初版作成、基本機能実装

---

## ライセンス

このツールセットは自由に使用・改変できます。商用利用も可能です。

---

## 作成者・連絡先

質問や改善要望がある場合は、プロジェクトの履歴ファイル（history.md）を参照してください。
