## gen.exe に関する問題報告書（要約と修正履歴）

このドキュメントは、`gen.c`/`gen.exe` の開発過程で発生した主要な問題を一覧化し、
各問題の原因、該当ソース（抜粋）、修正内容、再現手順、検証方法をまとめたものです。

目的:
- 今まで発生した繰り返しの問題を振り返り、将来の回帰を防ぐ。
- 修正の根拠（なぜその修正が必要か）を明確にする。

---

## 目次
1. 概要（経緯の短いタイムライン）
2. 主要な問題と詳細
   - 2.1 .vcxproj での CompileAs タグ誤設定（重大）
   - 2.2 main.m のインクルード順の不一致による型衝突（winsock2 / BOOL）
   - 2.3 Win32 / x64 の設定不一致：GNUstep 設定が誤った構成に適用されていた
   - 2.4 不適切な LanguageStandard / RuntimeLibrary の扱い
   - 2.5 Windows 用プロジェクト出力場所とファイル分離の問題（.dsp/.dsw の生成条件）
   - 2.6 gen.c 内の重複した wWinMain シグネチャとテンプレート挿入のバグ
3. 各問題の詳細（原因・該当箇所・修正内容）
4. 修正後の検証手順
5. 推奨される次の改善事項

---

## 1. 概要（経緯）
- 初期: `-winobjc` オプション実装（.vcxproj / .sln / main.m 生成）
- ビルドエラー頻発 -> 比較調査により bat 生成ファイルとの差分を精査
- 根本原因が複数見つかり、段階的に修正

---

## 2. 主要な問題と短い説明

2.1 `<CompileAs>` タグの誤設定（致命的）
- 症状: `#import <Foundation/Foundation.h>` を含む main.m をビルドすると "#import of type library is an unsupported Microsoft feature" のようなエラーが出る。
- 原因: `.vcxproj` の `ClCompile` 要素に `<CompileAs>CompileAsCpp</CompileAs>` が直接設定されていた。これにより `.m` ファイルが強制的に C++ として扱われ、`#import` が COM の `#import` として解釈される。Windows/ClangCLのプリプロセッサでエラーとなる。
- 該当箇所（修正前の抜粋）:

```xml
<ClCompile Include="main.m">
  <CompileAs>CompileAsCpp</CompileAs>
</ClCompile>
```

- 修正内容: `.m` 拡張子に従わせるため、空の条件付き `<CompileAs>` タグを使用して Visual Studio にファイル拡張子に基づく動作を任せる。具体的には create_vcxproj.bat が出力する形に合わせた。

修正後（抜粋）:

```xml
<ClCompile Include="main.m">
  <CompileAs Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
  </CompileAs>
  <CompileAs Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
  </CompileAs>
</ClCompile>
```

理由: 明示的に `CompileAsCpp` を指定すると `.m` を Objective-C としてコンパイルできない。空の条件付き要素は Visual Studio が拡張子を見て適切に処理する形に合わせる。

2.2 main.m のインクルード順に起因する winsock2 / BOOL 型衝突
- 症状: clangcl で `WinBOOL` 未定義のエラーが大量に出る（winsock2.h 側）。
- 問題の発生点: `#include <Foundation/Foundation.h>` と `#include <windows.h>` の順序がファイルによって異なる。どちらを先に含めるかで Windows SDK と GNUstep のマクロ定義（特に `BOOL`）の展開が異なり、型解決に影響を与えた。
- 観察: バッチで生成した `winobjc_bat.m` は次の順序であり問題が発生しない:

```c
#include <Foundation/Foundation.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
```

- 一方、gen.c で生成された最初の main.m では逆順になっていてエラーが発生した。
- 修正: `generate_winobjc_main_m()` で出力されるヘッダー順をバッチ版と同じ（`Foundation` を先）に変更した。

2.3 Win32 と x64 の構成で GNUstep 設定が不適切に全ての構成へ適用されていた
- 症状: Win32 構成でも GNUstep のインクルードやライブラリオプションが含まれており、リンク/コンパイルで問題を引き起こす。
- 原因: `gen.c` の template 生成コードが create_vcxproj.bat の動作（GNUstep は x64 のみ）を再現していなかった。
- 修正: Win32 (x86) 向けの Debug/Release 構成から GNUstep 関連の `AdditionalIncludeDirectories`, `AdditionalLibraryDirectories`, `AdditionalOptions` 等を取り除き、x64 構成にのみ追加するようにした。

2.4 `<LanguageStandard>` と `<RuntimeLibrary>` の不整合
- 症状: `LanguageStandard` に `stdcpp17` が直接入っていたり、`RuntimeLibrary` が無かったりしてビルド設定が create_vcxproj.bat と差異があった。
- 修正: `LanguageStandard` の不適切な出力を削除し、create_vcxproj.bat に合わせて `RuntimeLibrary` を `MultiThreadedDebug` 等に設定するよう修正（Debug / Release の適切な値に調整）。

2.5 出力場所と DSP/DSW の生成条件の不一致（繰り返し発生）
- 症状: `gen.exe -vcxproj projname` を実行しても、期待どおり `projname\\projname.sln` のようなフォルダ内配置にならず、ルートに .dsp / .dsw が生成される、または .dsp/.dsw を生成してしまう。
- 原因:
  - 初期実装ではテンプレート処理（DSP/DSW の生成）を `outdir` の設定前に行っていた。
  - `-vcxproj` 指定時に `outdir` をプロジェクト名に設定するタイミングが遅く、既に `.dsp` を書き出してしまっていた。
  - また、`-vcxproj` では DSP/DSW を生成しない（それは -vs6 用のみ）が初期コードでは条件が不十分だった。
- 修正: 
  1. `outdir` の設定ルールを変更（`-vcxproj` または `-vs6` が指定され、`-path` が未指定なら outdir をプロジェクト名に先に設定）。
  2. DSP/DSW のテンプレート準備と書き出しを `-vcxproj`/`-winobjc`/`-vs6` 等のモードに応じて分岐させ、`-vcxproj` 実行時は .dsp/.dsw を出力しない。
  3. `-vs6` では DSP/DSW を生成する専用処理を用意し、その場合のみテンプレートから dsp/out を作成する。

結果: `gen.exe -vcxproj vs2022` は `vs2022\\vs2022.sln` 等を生成し、ルートに dsp/dsw を残さない。

2.6 gen.c 内の重複した wWinMain シグネチャ/テンプレートのミス
- 症状: 生成される文字列テンプレートに wWinMain のプロトタイプが二重に埋め込まれていたり、関数シグネチャの重複があり、結果として生成コードが不正になっていた。
- 原因: テンプレート編集時に文字列ブロックの一部が重複して残っていた。
- 修正: 該当テンプレート文字列の余分な重複を削除し、正しい wWinMain シグネチャを出力するように修正。

---

## 3. 各問題の詳細（例：ソース抜粋 + 修正内容）

（以下では `gen.c` 内の該当抜粋と、変更前後の差分を示します。行番号は開発中のファイル位置に依存するため省略しています。）

### 問題 2.1: `<CompileAs>` が CompileAsCpp に固定されていた

修正前（`gen.c` で生成されていた .vcxproj の抜粋）:

```xml
<ItemGroup>
  <ClCompile Include="main.m">
    <CompileAs>CompileAsCpp</CompileAs>
  </ClCompile>
</ItemGroup>
```

影響: `.m` ファイルが C++ として扱われ、`#import` 行が COM 型ライブラリのインポート指令として解釈されてコンパイルエラーになる。

修正後（`gen.c` 側を create_vcxproj.bat の出力に合わせた）:

```xml
<ItemGroup>
  <ClCompile Include="main.m">
    <CompileAs Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
    </CompileAs>
    <CompileAs Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    </CompileAs>
  </ClCompile>
</ItemGroup>
```

対応理由と検証方法:
- Visual Studio 側で拡張子に基づく扱い（Objective-C via Clang）を行わせるため。修正後は `#import <Foundation/Foundation.h>` を含む main.m が x64 Debug/Release で正しくコンパイルできることを確認。

### 問題 2.2: main.m のインクルード順での winsock2 / BOOL 型衝突

該当（修正前の `generate_winobjc_main_m()` の最初の数行）:

```c
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Foundation/Foundation.h>
```

この順序だと GNUstep のヘッダーが Windows SDK の定義後に来て一部のマクロの影響で型不整合を引き起こした（コンパイラ出力: unknown type name 'WinBOOL' 等）。

修正: `Foundation` を最初に include するように戻した。最終的に `gen.c` 内は以下の順序で生成されるように修正。

```c
#include <Foundation/Foundation.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
```

理由: バッチ版の生成ファイル（動作するもの）と同じ順序に合わせることで、GNUstep 側のマクロが先に定義され、winsock2 側の依存が満たされるようになった。

### 問題 2.3: Win32 設定に GNUstep が適用されていた

修正前: `generate_winobjc_vcxproj()` のテンプレートがすべての Platform/Configuration に GNUstep 設定を追加していた。

修正後: Win32（x86）向けの Debug/Release 構成から GNUstep 関連の `AdditionalIncludeDirectories`, `AdditionalLibraryDirectories`, `AdditionalOptions` 等を取り除き、x64 構成にのみ追加するようにした。

（具体的な `gen.c` 内の出力テンプレート部分で、条件で出力するよう修正）

### 問題 2.4: LanguageStandard と RuntimeLibrary の不整合

修正前: `LanguageStandard` が不適切に `stdcpp17` などを直書きしていたため create_vcxproj.bat と差があった。

修正後: `LanguageStandard` 削除、必要な場合 `RuntimeLibrary` を `MultiThreadedDebug` などにして create_vcxproj.bat と同等にした。

理由: MSVC/ClangCLのオプション互換を考慮し、create_vcxproj.bat と同じ出力を目指す。

### 問題 2.5: -vcxproj で .dsp/.dsw を生成してしまう / フォルダが作られない

修正前の問題点: 
- DSPテンプレート準備と出力が `outdir` 設定より前に実行され、`-vcxproj` 指定時に `outdir` を後で設定しても既にルートに dsp/dsw が書かれていた。
- `-vcxproj` と `-vs6` の分岐が不十分で、`-vcxproj` 指定時に .dsp/.dsw を生成してしまっていた。

修正措置:
1. `outdir` の初期化を早期に行う: `if ((gen_vcxproj || gen_vs6) && outdir is empty) outdir = proj;`
2. DSPテンプレートの準備はモードごとに分け、`-vcxproj` モードの時は DSP/DSW を生成しない。
3. `-vs6` モードでのみ `.dsp`/`.dsw` を生成する専用ルートを実装した。

結果: `gen.exe -vcxproj vs2022` は `vs2022\\vs2022.sln` 等を生成し、ルートに dsp/dsw を残さない。

### 問題 2.6: wWinMain のテンプレート重複

修正前: 文字列テンプレートに wWinMain の引数ブロックが重複して埋め込まれていた（テンプレート文字列の結合ミス）。

修正後: テンプレートの文字列結合を整理し、重複が無いことを確認した。

---

## 4. 修正後の検証手順（短く）
1. 再コンパイル: `cl gen.c /Fegen.exe`
2. VS2022 プロジェクト生成: `gen.exe -vcxproj vs2022` → 期待: `vs2022\\vs2022.sln`, `vs2022\\win_vs2022.vcxproj`, `vs2022\\con_vs2022.vcxproj` が生成され、ルートに `.dsp` や `.dsw` が無い事を確認
3. Winobjc 生成: `gen.exe -winobjc -gnustep ../gnustep winobjc_gen_c` → 期待: `winobjc_gen_c\\main.m` が `#include <Foundation/Foundation.h>` を先頭に持ちコンパイル可能
4. VS6 生成: `gen.exe -vs6 legacyproj` → 期待: `legacyproj\\win_legacyproj.dsp` / `legacyproj\\legacyproj.dsw` が生成
5. Visual Studio で x64 Debug ビルドを実行し、`#import` によるエラーが出ない事を確認

検証時のポイント:
- `winobjc` は x64 の構成にのみ GNUstep 設定が付与されている点を必ず確認
- `.vcxproj` の `<ClCompile Include="main.m">` の中身が空の条件付き `<CompileAs>` になっていることを確認

---

## 5. 推奨される次の改善事項（将来の防止策）
1. 単体テスト: 主要テンプレート生成のユニットテストを追加し、出力 XML/ファイルが期待通りかを検証する（`CompileAs`、`RuntimeLibrary`、`AdditionalIncludeDirectories` 等）。
2. CI ビルド: gen.exe をビルド・実行してサンプルプロジェクトを自動生成し、最小のビルドを走らせる。これにより回帰を早期に検知できる。
3. モード別テンプレート分離: DSP/DSW 用テンプレート、vcxproj 用テンプレート等を別関数・別ファイルで管理してモードの混在を防止する。
4. ロギング改善: 生成時のパス決定ロジック（`outdir` の値）を詳細ログで出力し、ユーザーがどこに何が生成されたかを追跡しやすくする。

---

## 付録: 修正した `gen.c` の確認ポイント（diff の要点）
- `generate_winobjc_vcxproj()` の `<ClCompile Include="main.m">` ブロックを確認
- `generate_winobjc_main_m()` 内インクルード順（Foundation を先）を確認
- `outdir` の初期化位置（`-vcxproj`/`-vs6` 用）を確認
- DSP/DSW 出力の条件分岐を確認（`-vcxproj` では出力しない）

---

以上。必要なら、このドキュメントをさらに分割して各修正ごとのパッチやユニットテスト、CIステップを別ファイルで追加します。
