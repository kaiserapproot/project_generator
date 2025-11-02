# gen.exe - プロジェクトジェネレーター

## 概要
gen.exeは、C/C++/Objective-Cプロジェクトを複数のプラットフォーム向けに自動生成するツールです。

## 基本的な使い方

```bash
gen.exe [オプション] <プロジェクト名>
```

## オプション一覧

### Visual C++ 6.0 プロジェクト
```bash
gen.exe -vs6 <プロジェクト名>
```
- `<プロジェクト名>/`フォルダを作成
- 以下のファイルを生成:
  - `win_<プロジェクト名>.dsp` (Windows GUIアプリ)
  - `con_<プロジェクト名>.dsp` (コンソールアプリ)
  - `<プロジェクト名>.dsw` (ワークスペース)
  - `build_<プロジェクト名>.bat` (ビルドバッチ)
  - `<プロジェクト名>_main.c` (サンプルコード)

**例:**
```bash
gen.exe -vs6 myproject
```

### Visual Studio (vcxproj) プロジェクト
```bash
gen.exe -vcxproj <プロジェクト名>
```
- `<プロジェクト名>/`フォルダを作成
- 以下のファイルを生成:
  - `win_<プロジェクト名>.vcxproj` (Windows GUIアプリ)
  - `con_<プロジェクト名>.vcxproj` (コンソールアプリ)
  - `win_<プロジェクト名>.vcxproj.user`
  - `con_<プロジェクト名>.vcxproj.user`
  - `win_<プロジェクト名>.vcxproj.filters`
  - `con_<プロジェクト名>.vcxproj.filters`
  - `<プロジェクト名>.sln` (ソリューション)
  - `<プロジェクト名>_main.c` (サンプルコード)

**例:**
```bash
gen.exe -vcxproj vs2022project
```

### Windows Objective-C プロジェクト (GNUstep)
```bash
gen.exe -winobjc [-gnustep <パス>] <プロジェクト名>
```
- `<プロジェクト名>/`フォルダを作成
- 以下のファイルを生成:
  - `<プロジェクト名>.vcxproj`
  - `<プロジェクト名>.vcxproj.filters`
  - `<プロジェクト名>.sln`
  - `main.m` (Objective-Cサンプルコード)
- GNUstepランタイムを使用したObjective-C対応

**オプション:**
- `-gnustep <パス>`: GNUstepのインストールパスを指定 (デフォルト: `../gnustep`)

**例:**
```bash
gen.exe -winobjc myobjcapp
gen.exe -winobjc -gnustep c:/gnustep myobjcapp
```

### Xcode プロジェクト
```bash
gen.exe -xcode [-platform macos|ios] <プロジェクト名>
```
- `<プロジェクト名>/`フォルダを作成
- 以下のファイルを生成:
  - `<プロジェクト名>.xcodeproj/project.pbxproj`
  - `<プロジェクト名>_main.m`
  - `Info.plist`

**オプション:**
- `-platform macos`: macOS用プロジェクト (デフォルト)
- `-platform ios`: iOS用プロジェクト

**例:**
```bash
gen.exe -xcode myapp
gen.exe -xcode -platform ios myiosapp
```

### iOS + macOS 両対応プロジェクト
```bash
gen.exe -objc <プロジェクト名>
```
- `<プロジェクト名>/`フォルダを作成
- 以下のサブプロジェクトを生成:
  - `<プロジェクト名>.xcodeproj/` (親プロジェクト)
  - `ios_<プロジェクト名>/` (iOSプロジェクト)
  - `macos_<プロジェクト名>/` (macOSプロジェクト)

**例:**
```bash
gen.exe -objc crossplatform
```

### 従来の使い方 (デフォルト)
```bash
gen.exe [-src] [-path <出力先>] <プロジェクト名>
```
- DSP/DSWファイルをカレントディレクトリに生成

**オプション:**
- `-src`: main.cを含むプロジェクトを生成
- `-path <出力先>`: 出力先フォルダを指定

**例:**
```bash
gen.exe myproject
gen.exe -src myproject
gen.exe -src -path output myproject
```

## 出力ファイル構造

### -vs6 の場合
```
myproject/
├── win_myproject.dsp
├── con_myproject.dsp
├── myproject.dsw
├── build_myproject.bat
└── myproject_main.c
```

### -vcxproj の場合
```
myproject/
├── myproject.sln
├── win_myproject.vcxproj
├── win_myproject.vcxproj.user
├── win_myproject.vcxproj.filters
├── con_myproject.vcxproj
├── con_myproject.vcxproj.user
├── con_myproject.vcxproj.filters
└── myproject_main.c
```

### -winobjc の場合
```
myproject/
├── myproject.sln
├── myproject.vcxproj
├── myproject.vcxproj.filters
└── main.m
```

### -xcode の場合
```
myproject/
├── myproject.xcodeproj/
│   └── project.pbxproj
├── myproject_main.m
└── Info.plist
```

### -objc の場合
```
myproject/
├── myproject.xcodeproj/
│   └── project.pbxproj
├── ios_myproject/
│   ├── ios_myproject.xcodeproj/
│   ├── ios_myproject_main.m
│   └── Info.plist
└── macos_myproject/
    ├── macos_myproject.xcodeproj/
    ├── macos_myproject_main.m
    └── Info.plist
```

## 使用例

### Visual Studio 2022プロジェクトを作成
```bash
gen.exe -vcxproj MyApp
cd MyApp
start MyApp.sln
```

### Visual C++ 6.0プロジェクトを作成
```bash
gen.exe -vs6 LegacyApp
cd LegacyApp
msdev LegacyApp.dsw
```

### Windows Objective-Cアプリを作成
```bash
gen.exe -winobjc -gnustep ../gnustep WinObjCApp
cd WinObjCApp
start WinObjCApp.sln
```

### iOS/macOS クロスプラットフォームアプリを作成
```bash
gen.exe -objc CrossPlatformApp
cd CrossPlatformApp
open CrossPlatformApp.xcodeproj
```

## 注意事項

- `-vcxproj`と`-vs6`は、プロジェクト名のフォルダを自動作成します
- `-winobjc`は、GNUstepランタイムがインストールされている必要があります
- `-xcode`と`-objc`は、macOS環境でのみ動作します
- カレントディレクトリにファイルを直接生成する場合は、オプションなしで実行してください

## コンパイル方法

```bash
cl gen.c /Fegen.exe
```

## バージョン情報
- Visual C++ 6.0 対応
- Visual Studio 2022 対応
- Xcode (macOS/iOS) 対応
- GNUstep (Windows Objective-C) 対応
