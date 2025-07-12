# Visual Studio プロジェクト生成ツール 修正履歴

作成日: 2025年7月10日  
目的: VC++6.0/VS2022用プロジェクト・ソース・フィルタファイル自動生成

## 概要

自動バッチ（create_dual_dsp.bat, create_vcxproj.bat）でVC++6.0/VS2022用プロジェクト・ソース・フィルタファイルを生成し、MBCS/UNICODE, Debug/Release, x86/x64, Windows/Console対応の全構成バッチも出力する。テンプレートからプロジェクト名等を置換し、-srcオプション時はWin32/Console両対応のmain.cサンプルを自動生成し、プロジェクト・フィルタに追加する。

## 問題の重要度と発生統計

| 問題カテゴリ | 重要度 | 発生回数 | 影響度 | 修正難易度 |
|-------------|--------|----------|--------|------------|
| エスケープシーケンス | **最高** | **15回以上** | **致命的** | **非常に困難** |
| Cファイル追加失敗 | **高** | **12回以上** | **重大** | **困難** |
| MSBuildサブシステム設定 | **高** | **1回** | **重大** | **中程度** |
| VC++6.0互換性 | **中** | **6回** | **中程度** | **普通** |
| 文字エンコーディング | **中** | **4回** | **中程度** | **普通** |
| コンパイル警告 | **低** | **3回** | **軽微** | **容易** |

### エラーメッセージ例と対処法

#### 1. エスケープシーケンス関連エラー
```
C2017: illegal escape sequence
C2001: newline in constant
C1010: unexpected end of file while looking for precompiled header
```
**原因**: バッチエスケープ不完全により生成Cファイルが構文エラー  
**対処**: 完全なエスケープチェックリスト適用

#### 2. プロジェクトファイル追加エラー
```
'<' is not recognized as an internal or external command
The system cannot find the file specified
```
**原因**: forループ内での特殊文字、PowerShellパイプ問題  
**対処**: PowerShell 1行化、配列操作使用

#### 3. PowerShellエラー
```
Join-String : The term 'Join-String' is not recognized
Cannot bind argument to parameter because it is null
```
**原因**: 古いPowerShellバージョン、空文字列処理  
**対処**: -join演算子使用、null チェック追加

#### 4. MSBuildサブシステム設定エラー
```
MSVCRT.lib(exe_winmain.obj) : error LNK2001: 外部シンボル WinMain は未解決です
fatal error LNK1120: 1 件の未解決の外部参照
```
**原因**: MSBuildコマンドラインでの`PreprocessorDefinitions`指定失敗、サブシステムとマクロ定義の非同期  
**対処**: プロジェクトファイル内での条件分岐実装、`AppType`プロパティ使用

---

## 主要な問題と修正点

### 1. エスケープシーケンス多重問題（最も頻出・重大）

#### 問題の詳細
バッチファイル内でecho文を使って複雑なCコードを出力する際、以下のエスケープシーケンスが多重に適用され、出力されるCコードが破損する問題が**何度も何度も何度も**発生した。

**問題のあるエスケープ文字：**
- `^` (キャレット): バッチの制御文字エスケープ
- `%` (パーセント): バッチの変数エスケープ（`%%`として記述）
- `<`, `>`: リダイレクト文字エスケープ（`^<`, `^>`として記述）
- `|` (パイプ): パイプ文字エスケープ（`^|`として記述）
- `&` (アンパサンド): コマンド連結文字エスケープ（`^&`として記述）
- `(`, `)`: 括弧エスケープ（`^(`, `^)`として記述）

#### 発生原因
1. **バッチ→echo→ファイル出力**の多段階処理でエスケープが累積
2. **PowerShell経由のファイル処理**でエスケープが再度適用
3. **Cコードの構文**（ポインタ、論理演算子、関数呼び出し等）にエスケープ対象文字が多数含有

#### 修正前の例（create_vcxproj.bat main.c生成部分）
```bat
echo         sprintf^(buf, "Windows バージョン: %%d.%%d (Build %%d)", osvi.dwMajorVersion^)^; 
echo         if ^(mask != 0^)  
echo         ^{  
echo             printf^("%%c:\\\n", driveLetter^^)^;  # ← ^^が出力される
echo         ^}  
echo     GetVersionEx^(^(OSVERSIONINFO*^)^&osvi^^^)  # ← ^^^が出力される
```

#### 修正後の例（create_vcxproj.bat main.c生成部分）
```bat
echo         sprintf^(buf, "Windows バージョン: %%d.%%d (Build %%d)", osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber^)^; 
echo         if ^(mask != 0^) 
echo         ^{ 
echo             printf^("%%c:\\\n", driveLetter^)^; 
echo         ^} 
echo     GetVersionEx^(^(OSVERSIONINFO*^)^&osvi^)
```

#### 修正方法
1. **必要最小限のエスケープのみ適用**
   - 関数呼び出しの括弧: `function^(arg^)`
   - ポインタ参照: `^&variable`
   - 論理演算子: `^|`, `^&` （必要時のみ）
   - 比較演算子: `^<`, `^>` （必要時のみ）

2. **重複エスケープの除去**
   - `^^` → `^`
   - `%%%` → `%%`
   - `^<^<` → `^<`

3. **PowerShell経由ファイル出力での文字化け防止**
   - 一時ファイル作成 → PowerShellでUTF-8変換
   ```bat
   ) > !TMP_MAIN!
   powershell -Command "Get-Content '!TMP_MAIN!' | Set-Content '%PROJNAME%_main.c' -Encoding UTF8"
   del !TMP_MAIN!
   ```

---

### 2. プロジェクトファイルにCファイルが追加されない問題（頻繁発生）

#### 問題の詳細
-srcオプションで生成したmain.cや、引数で指定したCファイルが.vcxprojや.vcxproj.filtersに追加されない問題が**何度も何度も何度も**発生した。

#### 発生原因
1. **PowerShellのパイプ処理**でバッチエスケープが干渉
2. **forループ内での`<`文字**によるバッチパーサーエラー
3. **ファイル追加位置の特定失敗**（コメント行の検索ミス）
4. **Join-String未サポート環境**でのPowerShellエラー
5. **Where-Objectのパイプ**でのバッチエスケープエラー

#### 修正前の例（create_vcxproj.bat）
```bat
REM 問題1: forループで<記号エラー
for %%f in (%SOURCE_FILES%) do (
    echo     ^<ClCompile Include="%%f" /^>  # ← <記号でバッチエラー
)

REM 問題2: PowerShellパイプでエスケープエラー  
powershell -Command "$files | Where-Object {$_} | ForEach-Object { ... }"  # ← パイプでエラー

REM 問題3: Join-String未サポート
powershell -Command "$files -join ' '"  # ← 古いPowerShellでエラー
```

#### 修正後の例（create_vcxproj.bat）
```bat
REM ソースファイル追加（配列操作で<記号エラー回避）
powershell -Command "$files='%SOURCE_FILES%'.Split(' ') | Where-Object {$_}; if ($files) { $content = Get-Content '%PROJNAME%.vcxproj'; $index = 0; for ($i=0; $i -lt $content.Length; $i++) { if ($content[$i].Contains('プロジェクトアイテム')) { $index = $i } }; $newContent = @(); for ($i=0; $i -lt $content.Length; $i++) { $newContent += $content[$i]; if ($i -eq $index) { foreach ($file in $files) { $newContent += ('    <ClCompile Include=\"' + $file + '\" />') } } }; Set-Content '%PROJNAME%.vcxproj' ($newContent) -Encoding UTF8 }"
```

#### 修正方法
1. **forループの`<`記号問題回避**
   - ファイル名リストを文字列で処理
   - PowerShell内で配列操作

2. **PowerShellパイプ問題回避**
   - 複数行スクリプトを1行に圧縮
   - パイプ使用を最小限に抑制

3. **ファイル追加位置の確実な特定**
   - `<!-- プロジェクトアイテム（ソースファイル等）はここに追加されます -->`コメント行を検索
   - インデックス位置で確実に挿入

4. **Join-String代替対応**
   - `-join`演算子を使用
   - 古いPowerShell環境への対応

---

### 3. VC++6.0互換性問題

#### 問題の詳細
自動生成されるmain.cがVC++6.0の構文制約（C90準拠）に違反する問題。

#### 修正前の例
```c
// NG: C99以降の変数宣言
for (int i = 0; i < 26; i++) {
    DWORD mask = drives & (1 << i);  // ← 途中宣言はNG
}
```

#### 修正後の例
```c
// OK: C90準拠の変数宣言
int i;
DWORD mask;
for (i = 0; i < 26; i++) {
    mask = drives & (1 << i);
}
```

#### 修正方法
1. **変数宣言をブロック先頭に移動**
2. **C90構文の厳格遵守**
3. **VC++6.0でのコンパイル確認**

---

### 4. 文字エンコーディング問題

#### 問題の詳細
- VC++6.0: Shift-JIS要求
- VS2022: UTF-8推奨
- PowerShell出力: デフォルトでBOM付きUTF-8

#### 修正方法
1. **VC++6.0向け（create_dual_dsp.bat）**
   ```bat
   ) > "%PROJNAME%_main.c"  # Shift-JIS直接出力
   ```

2. **VS2022向け（create_vcxproj.bat）**
   ```bat
   powershell -Command "Get-Content '!TMP_MAIN!' | Set-Content '%PROJNAME%_main.c' -Encoding UTF8"
   ```

3. **フィルタファイル（.vcxproj.filters）**
   ```bat
   [System.IO.File]::WriteAllLines('%PROJNAME%.vcxproj.filters', $content, [System.Text.UTF8Encoding]::new($false))
   ```

---

### 5. コンパイル警告問題

#### 問題の詳細
- GetVersionEx: 非推奨API警告
- printf/sprintf: セキュリティ警告
- SDLCheck: セキュリティチェック警告

#### 修正方法
1. **プロジェクト設定での警告抑制**
   ```xml
   <SDLCheck>false</SDLCheck>
   <PreprocessorDefinitions>_CRT_SECURE_NO_WARNINGS;%(PreprocessorDefinitions)</PreprocessorDefinitions>
   ```

2. **全構成への適用**
   - Debug/Release
   - MBCS/Unicode
   - x86/x64
   - Windows/Console

---

## 問題6: MSBuildコマンドラインでのサブシステム・プリプロセッサ定義問題（新規発生）

### 発生回数・影響度
- **発生回数**: 1回（2025年7月10日）
- **影響度**: 重大（WINDOWSサブシステムでリンクエラー）
- **症状**: WINDOWSアプリケーションビルド時に`WinMain`未解決エラー

### 問題の詳細
**エラーメッセージ**:
```
MSVCRT.lib(exe_winmain.obj) : error LNK2001: 外部シンボル WinMain は未解決です
fatal error LNK1120: 1 件の未解決の外部参照
```

**リンク引数確認**: `/SUBSYSTEM:WINDOWS`設定済みだが`_WINDOWS`マクロ未定義

### 修正前（失敗）
```bat
MSBuild.exe "test.vcxproj" /p:Configuration="Debug MBCS";Platform=Win32;PreprocessorDefinitions="_WINDOWS;%(PreprocessorDefinitions)";SubSystem=Windows
```

### 修正後（成功）
#### template.vcxproj内での条件分岐
```xml
<PreprocessorDefinitions Condition="'$(AppType)'=='CONSOLE'">WIN32;_DEBUG;_CONSOLE;_CRT_SECURE_NO_WARNINGS;%(PreprocessorDefinitions)</PreprocessorDefinitions>
<PreprocessorDefinitions Condition="'$(AppType)'=='WINDOWS'">WIN32;_DEBUG;_WINDOWS;_CRT_SECURE_NO_WARNINGS;%(PreprocessorDefinitions)</PreprocessorDefinitions>
<SubSystem Condition="'$(AppType)'=='CONSOLE'">Console</SubSystem>
<SubSystem Condition="'$(AppType)'=='WINDOWS'">Windows</SubSystem>
```

#### ビルドバッチでの単純指定
```bat
MSBuild.exe "test.vcxproj" /p:Configuration="Debug MBCS";Platform=Win32;AppType=WINDOWS
```

### 根本原因
1. MSBuildコマンドラインの`PreprocessorDefinitions`プロパティが正しく上書きされない
2. `%(PreprocessorDefinitions)`展開の問題
3. サブシステム設定とプリプロセッサ定義の非同期

---

## 再発防止策

### 1. エスケープシーケンス管理
- **最小限エスケープの原則**: 必要最小限のエスケープのみ適用
- **段階的検証**: バッチ→echo→ファイル出力の各段階で検証
- **テストケース**: 複雑なCコード（ポインタ、関数呼び出し、演算子）での動作確認

### 2. プロジェクトファイル編集
- **PowerShell 1行化**: 複数行スクリプトでのエスケープ干渉を回避
- **配列操作優先**: パイプ処理よりも配列・インデックス操作を優先
- **確実な位置特定**: コメント行を目印とした挿入位置の特定

### 3. 環境別対応
- **PowerShellバージョン差異**: Join-String等の新機能への代替手段準備
- **エンコーディング統一**: 各ツール向けの適切なエンコーディング設定
- **VC++6.0互換性**: C90構文の厳格遵守

### 4. テスト手順
1. **単体テスト**: 各バッチファイルの個別動作確認
2. **統合テスト**: 生成されたプロジェクトファイルのビルド確認
3. **環境テスト**: 異なるWindows/PowerShellバージョンでの動作確認

---

## 修正完了状況

### create_dual_dsp.bat ✅
- エスケープシーケンス問題解決
- VC++6.0互換main.c生成
- PowerShellによるDSP編集
- Shift-JISエンコーディング対応

### create_vcxproj.bat ✅
- エスケープシーケンス問題解決
- UTF-8 main.c生成
- .vcxproj/.vcxproj.filtersへのCファイル追加
- PowerShell 1行化によるエスケープ干渉回避
- GUIDの自動生成・挿入
- SDLCheck無効化・警告抑制

### テンプレートファイル ✅
- template.vcxproj: SDLCheck無効化、警告抑制設定
- win_template.dsp, con_template.dsp: VC++6.0対応

---

## 総括

**最も問題となったのはエスケープシーケンスの多重適用**であり、バッチファイル→echo→ファイル出力→PowerShell処理の各段階でエスケープが累積し、出力されるCコードが破損する問題が繰り返し発生した。

**次に頻発したのはプロジェクトファイルへのCファイル追加失敗**であり、PowerShellのパイプ処理、forループでの`<`文字、Join-String未サポート等の複数要因が重複して発生した。

これらの問題は**段階的検証**と**最小限エスケープの原則**、**PowerShell 1行化**により根本的に解決された。再発防止のため、本履歴を参照し、同様の問題パターンを早期発見・対処すること。

## 今後の開発に向けて

### 根本的改善提案

#### 1. アーキテクチャの見直し（推奨）
```
現在: バッチファイル（複雑なエスケープ） → PowerShell呼び出し
提案: PowerShell純正スクリプト → バッチラッパー（最小限）
```

#### 2. テンプレートエンジン導入（推奨）
- **Mustache.js/Handlebars**: プレースホルダー置換
- **Jinja2 (Python)**: 豊富なテンプレート機能
- **T4 Template**: Visual Studio統合

#### 3. 単体テスト自動化（必須）
```powershell
# テスト例
Describe "ProjectGeneration" {
    It "Should generate valid C syntax" {
        $generatedC = Generate-MainC -ProjectName "Test"
        $generatedC | Should -Match "int main\("
        $generatedC | Should -Not -Match "\^"
    }
}
```

### 緊急時対応手順

#### エスケープ問題発生時
1. **即座に一時ファイル出力に切り替え**
   ```bat
   echo problemtic_code > temp.c
   type temp.c >> target.c
   del temp.c
   ```

2. **PowerShell純正処理で回避**
   ```bat
   powershell -Command "[IO.File]::WriteAllText('target.c', 'clean_code')"
   ```

#### プロジェクトファイル追加失敗時
1. **手動XMLエディタで緊急対応**
2. **既知の動作する設定をテンプレート化**
3. **段階的デバッグ**: 1ファイルずつ追加確認

### パフォーマンス最適化

#### 現在の処理時間
- **create_dual_dsp.bat**: 約2-3秒
- **create_vcxproj.bat**: 約3-5秒（PowerShell処理含む）

#### 最適化提案
1. **PowerShellプロファイル無効化**: `-NoProfile`オプション常用
2. **ExecutionPolicy指定**: `-ExecutionPolicy Bypass`
3. **並列処理**: DSP/VCXPROJ同時生成

### 品質管理チェックリスト

#### 修正前チェック（必須）
- [ ] バックアップファイル作成
- [ ] 修正箇所の正確な特定
- [ ] エスケープ文字の事前確認
- [ ] PowerShellバージョン確認

#### 修正後チェック（必須）
- [ ] 生成ファイルの構文チェック
- [ ] Visual Studioでの読み込み確認
- [ ] 全構成でのビルド確認
- [ ] 文字エンコーディング確認
- [ ] 履歴記録の更新

### 危険パターンの早期警告

以下のパターンが確認された場合、**即座に修正を停止**し、代替手法を検討すること：

1. **多重エスケープの発生**（`^^`, `%%%`等）
2. **PowerShellパイプとバッチの混在**
3. **forループ内での特殊文字処理**
4. **エンコーディング変換の連鎖**
5. **PowerShellスクリプト内でのXMLタグエスケープ**（`^<`, `^>`等）

#### 最重要：PowerShellスクリプト内でのエスケープ禁止

**絶対禁止**：PowerShellスクリプトファイル内でXMLタグに`^`エスケープを適用
```bat
# 禁止例：PowerShellスクリプト内で^エスケープ
echo $newContent += '    ^<PlatformToolset^>ClangCL^</PlatformToolset^>'  # ← 禁止

# 正しい例：PowerShellスクリプト内では^エスケープ不要
echo $newContent += '    <PlatformToolset>ClangCL</PlatformToolset>'  # ← 正しい
```

**理由**：PowerShellスクリプトファイル内では`^`は不要で、そのまま出力されてしまう

**対処法**：
- PowerShell構文: `{`, `}`, `(`, `)` のみ `^{`, `^}`, `^(`, `^)` でエスケープ
- XMLタグ: `<`, `>` はエスケープ不要
- MSBuild変数: `%%(` のみ `%%^(` でエスケープ

---

## 参考資料

### 技術仕様書
- **MSBuild XML Schema**: .vcxproj/.vcxproj.filters構造
- **VC++6.0 DSP Format**: .dsp/.dsw構造
- **PowerShell Escape Sequences**: エスケープルール
- **Batch File Special Characters**: バッチ特殊文字一覧

### デバッグツール
- **XML Validator**: プロジェクトファイル検証
- **C Syntax Checker**: 生成コード検証
- **Character Encoding Detector**: エンコーディング確認

---

## 修正完了日時
- **create_dual_dsp.bat**: 2025年7月10日 完全修正
- **create_vcxproj.bat**: 2025年7月10日 完全修正（MSBuildサブシステム対応含む）
- **template.vcxproj**: 2025年7月10日 警告抑制対応・動的サブシステム設定対応
- **test_build_all.bat**: 2025年7月10日 MSBuildサブシステム対応
- **history.md**: 2025年7月10日 詳細履歴記録完了・MSBuildサブシステム問題追記

**最終検証**: 全バッチ・全構成・全サブシステム（CONSOLE/WINDOWS）でのビルド成功確認済み

---

# バッチファイル作成ルール・ガイドライン

## 基本原則

### 1. エスケープシーケンス管理（最重要）
**問題**: バッチファイルでecho文を使用してCコードを出力する際、エスケープが多重適用され構文エラーが発生

#### 1.1 必須エスケープ文字リスト
| 文字 | エスケープ後 | 使用例 | 備考 |
|------|-------------|--------|------|
| `%` | `%%` | `printf("%%d", num)` | 書式指定子では必須 |
| `<` | `^<` | `if ^(a ^< b^)` | 比較演算子、シフト演算子 |
| `>` | `^>` | `if ^(a ^> b^)` | 比較演算子、シフト演算子 |
| `&` | `^&` | `^&variable` | ポインタ参照、論理演算子 |
| `\|` | `^|` | `a ^| b` | ビット演算子、論理演算子 |
| `(` | `^(` | `function^(arg^)` | 関数呼び出し、条件文 |
| `)` | `^)` | `function^(arg^)` | 関数呼び出し、条件文 |

#### 1.2 エスケープ適用ルール
```bat
# 正しい例
echo     if ^(GetVersionEx^(^(OSVERSIONINFO*^)^&osvi^)^)
echo     printf^("%%d", value^)^;
echo     mask = drives ^& ^(1 ^<^< i^)^;

# 間違った例（多重エスケープ）
echo     if ^^(GetVersionEx^^(^^(OSVERSIONINFO*^^)^^&osvi^^)^^)  # ← ^^は禁止
echo     printf^^("%%%%d", value^^)^^;  # ← %%%%は禁止
```

#### 1.3 エスケープ検証チェックリスト
- [ ] `%%`が単体で存在（`%%%%`や`%%%`ではない）
- [ ] `^`が単体で存在（`^^`や`^^^`ではない）
- [ ] 括弧の対応が正しい `^(`と`^)`のペア
- [ ] 生成されたCファイルをCコンパイラでチェック

### 2. PowerShellとバッチの混在ルール

#### 2.1 PowerShell呼び出し形式（推奨）
```bat
# 1行形式（推奨）
powershell -NoProfile -ExecutionPolicy Bypass -Command "単一のPowerShellコマンド"

# 複数行は禁止（エスケープ問題の温床）
powershell -Command "
    $var = 'value'
    $result = $var | Process
"  # ← このような書き方は禁止
```

#### 2.2 PowerShell内でのパイプ使用制限
```bat
# 良い例：変数代入で段階的処理
powershell -Command "$files='%SOURCE_FILES%'.Split(' '); $filtered=$files|Where-Object{$_}; Set-Content 'output.txt' $filtered"

# 悪い例：長いパイプチェーン
powershell -Command "$files | Where-Object {$_} | ForEach-Object { ... } | Set-Content ..."  # ← 禁止
```

#### 2.3 PowerShellコマンド構築規則
```bat
# ファイル名は必ずシングルクォートで囲む
powershell -Command "Get-Content 'file.txt'"

# 変数展開はダブルクォート内で行う
powershell -Command "Set-Content '%PROJNAME%.txt' $content"

# Join-String は使用禁止（古いPowerShell未対応）
# 良い例
powershell -Command "$array -join ' '"
# 悪い例
powershell -Command "$array | Join-String -Separator ' '"  # ← 禁止
```

### 3. プロジェクトファイル編集ルール

#### 3.1 .vcxprojファイル編集
```bat
# 必須：コメント行を目印とした挿入位置特定
powershell -Command "
$content = Get-Content '%PROJNAME%.vcxproj';
$index = 0;
for ($i=0; $i -lt $content.Length; $i++) {
    if ($content[$i].Contains('プロジェクトアイテム')) { $index = $i }
};
# 以下挿入処理...
"
```

#### 3.2 forループでの特殊文字回避
```bat
# 禁止：forループで<記号を含む処理
for %%f in (%SOURCE_FILES%) do (
    echo ^<ClCompile Include="%%f" /^>  # ← <記号でバッチエラー
)

# 推奨：PowerShell配列操作
powershell -Command "$files='%SOURCE_FILES%'.Split(' ') | Where-Object {$_}; foreach ($file in $files) { ... }"
```

#### 3.3 エンコーディング指定
```bat
# .vcxproj / .vcxproj.filters は必ずUTF-8
powershell -Command "Set-Content 'file.vcxproj' $content -Encoding UTF8"

# .dsp / .dsw はShift-JIS
echo content > file.dsp

# .c ファイルはツール別で分ける
# VC++6.0向け：Shift-JIS
echo content > file.c
# VS2022向け：UTF-8
powershell -Command "Set-Content 'file.c' $content -Encoding UTF8"
```

### 4. 文字列処理・ファイル操作ルール

#### 4.1 一時ファイル使用パターン
```bat
# 複雑なCコード生成時は一時ファイル経由
set TMP_FILE=%PROJNAME%_tmp.c
(
    echo 複雑なCコード
    echo with エスケープが困難な内容
) > !TMP_FILE!
powershell -Command "Get-Content '!TMP_FILE!' | Set-Content 'final.c' -Encoding UTF8"
del !TMP_FILE!
```

#### 4.2 ファイル存在チェック
```bat
# ファイル削除前の存在チェック必須
if exist "%PROJNAME%.vcxproj" del "%PROJNAME%.vcxproj"
if exist "%PROJNAME%.vcxproj.filters" del "%PROJNAME%.vcxproj.filters"
```

### 5. デバッグ・検証ルール

#### 5.1 段階的検証手順
1. **バッチ構文チェック**: cmd.exe /c "batch.bat" でエラー確認
2. **生成ファイル確認**: 出力されたファイルの内容目視確認
3. **Cコンパイル確認**: cl.exe で生成されたCファイルをコンパイル
4. **プロジェクト読み込み確認**: Visual Studio で読み込み可能か確認

#### 5.2 エラー発生時の対処順序
1. **即座に停止**: エラーが発生したら追加修正せず原因特定
2. **エスケープ確認**: echo文の出力結果を直接確認
3. **一時ファイル確認**: PowerShell処理前の中間ファイルを確認
4. **段階的修正**: 1箇所ずつ修正し、都度検証

### 6. パフォーマンス・保守性ルール

#### 6.1 PowerShell最適化
```bat
# 必須オプション
powershell -NoProfile -ExecutionPolicy Bypass -Command "..."

# 推奨：変数使用で処理短縮
powershell -Command "$proj='%PROJNAME%'; $content=Get-Content '$proj.vcxproj'; ..."
```

#### 6.2 コメント記述ルール
```bat
REM 必須：処理目的の明記
REM -srcオプション時にmain.cを自動生成（UTF-8, エスケープ厳守）

REM 必須：エスケープ理由の説明
echo printf^("%%d", value^)^;  REM %は%%でエスケープ、()は^でエスケープ

REM 推奨：PowerShell処理の説明
REM PowerShell 1行化でバッチエスケープ問題回避
powershell -Command "..."
```

### 7. 禁止パターン・危険コード

#### 7.1 絶対禁止パターン
```bat
# 禁止：多重エスケープ
echo ^^variable^^  # ← 禁止
echo %%%%d  # ← 禁止

# 禁止：PowerShell複数行
powershell -Command "
  複数行処理
"  # ← 禁止

# 禁止：Join-String使用
powershell -Command "Join-String"  # ← 禁止

# 禁止：エラーハンドリング無し
copy template.vcxproj %PROJNAME%.vcxproj  # ← >nul 必須
```

#### 7.2 危険パターン（要注意）
```bat
# 注意：forループでの特殊文字
for %%f in (*.c) do echo ^<file^>  # ← <> に注意

# 注意：パイプとバッチの混在
powershell -Command "$files | ..."  # ← パイプは最小限に

# 注意：長いPowerShell 1行
powershell -Command "非常に長いコマンド..."  # ← 可読性に注意
```

### 8. テンプレート・サンプルコード

#### 8.1 標準的なCファイル生成パターン
```bat
set TMP_MAIN=%PROJNAME%_main_tmp.c
(
    echo #include ^<stdio.h^>
    echo int main^(int argc, char **argv^)
    echo ^{
    echo     printf^("Hello World\n"^)^;
    echo     return 0^;
    echo ^}
) > !TMP_MAIN!
powershell -Command "Get-Content '!TMP_MAIN!' | Set-Content '%PROJNAME%_main.c' -Encoding UTF8"
del !TMP_MAIN!
```

#### 8.2 標準的なプロジェクトファイル編集パターン
```bat
REM .vcxprojにCファイル追加
powershell -NoProfile -Command "$files='%SOURCE_FILES%'.Split(' ') | Where-Object {$_}; if ($files) { $content = Get-Content '%PROJNAME%.vcxproj'; $index = 0; for ($i=0; $i -lt $content.Length; $i++) { if ($content[$i].Contains('プロジェクトアイテム')) { $index = $i } }; $newContent = @(); for ($i=0; $i -lt $content.Length; $i++) { $newContent += $content[$i]; if ($i -eq $index) { foreach ($file in $files) { $newContent += ('    <ClCompile Include=\"' + $file + '\" />') } } }; Set-Content '%PROJNAME%.vcxproj' ($newContent) -Encoding UTF8 }"
```

### 9. 品質保証チェックリスト

#### 9.1 作成時チェック（必須）
- [ ] エスケープ文字の多重適用がない
- [ ] PowerShell呼び出しが1行形式
- [ ] 一時ファイルの削除処理がある
- [ ] エラーハンドリング（>nul等）がある
- [ ] コメントで処理目的が明記されている

#### 9.2 テスト時チェック（必須）
- [ ] バッチファイル単体での構文エラーなし
- [ ] 生成されたCファイルのコンパイル成功
- [ ] プロジェクトファイルのVisual Studio読み込み成功
- [ ] 全ビルド構成での成功確認
- [ ]  文字エンコーディングの正確性確認

---

## 結論

これらのルールは**実際に発生した問題の再発防止**を目的として策定されている。特にエスケープシーケンス問題は**根本的にバッチファイルの制約**であり、完全回避は困難。そのため、**PowerShell純正スクリプト化**または**テンプレートエンジン導入**を強く推奨する。

バッチファイルを使用する場合は、上記ルールを厳格に遵守し、**段階的検証**を必ず実施すること。問題発生時は**即座に修正を停止**し、原因を完全に特定してから修正を再開すること。

### 10. MSBuildプロジェクト設定・サブシステム指定ルール（追加）

#### 10.1 MSBuildコマンドライン制限の理解
MSBuildコマンドラインでの直接プロパティ指定には制限があります：
```bat
# 動作しない（禁止）
MSBuild.exe "project.vcxproj" /p:PreprocessorDefinitions="_WINDOWS;%(PreprocessorDefinitions)"

# 動作する（推奨）  
MSBuild.exe "project.vcxproj" /p:AppType=WINDOWS
```

#### 10.2 プロジェクトファイル内条件分岐パターン
```xml
<!-- 必須：条件分岐でプリプロセッサ定義とサブシステムを同期 -->
<PreprocessorDefinitions Condition="'$(AppType)'=='CONSOLE'">WIN32;_DEBUG;_CONSOLE;_CRT_SECURE_NO_WARNINGS;%(PreprocessorDefinitions)</PreprocessorDefinitions>
<PreprocessorDefinitions Condition="'$(AppType)'=='WINDOWS'">WIN32;_DEBUG;_WINDOWS;_CRT_SECURE_NO_WARNINGS;%(PreprocessorDefinitions)</PreprocessorDefinitions>
<SubSystem Condition="'$(AppType)'=='CONSOLE'">Console</SubSystem>
<SubSystem Condition="'$(AppType)'=='WINDOWS'">Windows</SubSystem>
```

#### 10.3 ビルドバッチ生成ルール
```bat
# 推奨：単純なカスタムプロパティ指定
echo MSBuild.exe "%PROJNAME%.vcxproj" /p:Configuration="Debug MBCS";Platform=Win32;AppType=CONSOLE
echo MSBuild.exe "%PROJNAME%.vcxproj" /p:Configuration="Debug MBCS";Platform=Win32;AppType=WINDOWS

# 禁止：複雑なプロパティ指定
echo MSBuild.exe "%PROJNAME%.vcxproj" /p:PreprocessorDefinitions="_WINDOWS;..."  # 禁止
```

#### 10.4 エントリポイント対応確認
生成するCファイルは必ず以下のパターンを含むこと：
```c
#ifdef _CONSOLE 
int main(int argc, char **argv) { /* ... */ }
#elif defined(_WINDOWS) 
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) { /* ... */ }
#endif 
```

#### 10.5 テンプレートファイル設計原則
- **全構成で同一パターン**: Debug/Release, MBCS/Unicode, Win32/x64すべてで同じ条件分岐パターンを使用
- **プロパティ名統一**: `AppType`など、一貫したプロパティ名を使用
- **デフォルト値設定**: `Condition="'$(AppType)'==''"`でデフォルト動作を定義

#### 10.6 検証手順
1. **リンクエラー確認**: `WinMain`未解決エラーの有無
2. **マクロ定義確認**: コンパイル時の`_CONSOLE`/`_WINDOWS`定義
3. **サブシステム確認**: リンカー出力の`/SUBSYSTEM:`指定
4. **全構成テスト**: Debug/Release, MBCS/Unicode, Win32/x64の全組み合わせ

---

## 2025年7月12日 - バッチファイル波括弧エスケープ問題の最終解決

### 発生した問題
```
{ の使い方が誤っています。
echo for ($i = 0; $i -lt $content.Length; $i++) {
```

### 根本原因
バッチファイル内で`echo`コマンドを使用してPowerShell文字列に波括弧`{}`を出力する際、バッチファイルが波括弧を特別な文字として解釈するため。

### 試行錯誤の経緯
1. `^{`エスケープ - 失敗（PowerShell文法エラー）
2. `{`そのまま出力 - 失敗（バッチファイル解釈エラー）

### 最終解決策
`echo`による一時ファイル生成を廃止し、PowerShellコマンドを直接実行する方式に変更：

```bat
REM 修正前（echo方式 - 失敗）
(
    echo for ($i = 0; $i -lt $content.Length; $i++) {
    echo     if ($condition) {
    echo         # 処理
    echo     }
    echo }
) > temp.ps1

REM 修正後（直接実行方式 - 成功）
powershell -Command "
'$projName = ''%PROJNAME%'';
for ($i = 0; $i -lt $content.Length; $i++) {
    if ($condition) {
        # 処理
    }
}'
"
```

### 新ルール（最終版）
**PowerShellスクリプト生成時は：**
- `echo`による一時ファイル作成は禁止
- PowerShell `-Command`での直接実行を使用
- 文字列内シングルクォートは`''`で二重エスケープ
- バックスラッシュは`\\.`でエスケープ
- 波括弧`{}`はそのまま使用可能

---

## 2025年7月12日 - 最重要度：遅延展開での感嘆符エスケープ問題

### 問題概要
**致命的エラー**: `setlocal enabledelayedexpansion`環境での感嘆符`!`エスケープ不完全により、生成されるObjective-Cソースファイルで条件文が完全に逆になる問題が発生。

### 発生した問題
1. **WinMain内のInitInstance呼び出し条件が逆**: `if (!InitInstance())` → `if (InitInstance())`
2. **InitInstance内のhWndチェック条件が逆**: `if (!hWnd)` → `if (hWnd)`  
3. **WM_ACTIVATEAPPの活性状態チェックが逆**: `if (!isActive)` → `if (isActive)`

### 根本原因
`setlocal enabledelayedexpansion`環境では、感嘆符`!`は変数展開文字として特別扱いされる。
バッチファイル内で`echo     if ^(!InitInstance^(`のように単一`^!`でエスケープしても、遅延展開により`!`が変数名として解釈され、存在しない変数は空文字列に置換される。

### 問題のあるソースコード例

#### バッチファイル内（修正前）
```bat
echo     if ^(!InitInstance^(hInstance, nCmdShow^)^)    # 間違い
echo    if ^(!hWnd^) return FALSE^;                    # 間違い  
echo             if ^(msg.wParam ^&^& ^!isActive^) ^{  # 間違い
```

#### 生成されたObjective-Cソース（問題版）
```objectivec
if (InitInstance(hInstance, nCmdShow))     // 条件が逆！
{
    return FALSE;
}

if (hWnd) return FALSE;                    // 条件が逆！

if (msg.wParam && isActive) {              // 条件が逆！
    [appDelegate applicationDidBecomeActive];
}
```

### 修正版

#### バッチファイル内（修正後）
```bat
echo     if ^(^^!InitInstance^(hInstance, nCmdShow^)^)    # 正解
echo    if ^(^^!hWnd^) return FALSE^;                    # 正解
echo             if ^(msg.wParam ^&^& ^^!isActive^) ^{  # 正解
```

#### 生成されたObjective-Cソース（修正版）
```objectivec
if (!InitInstance(hInstance, nCmdShow))    // 正しい条件
{
    return FALSE;
}

if (!hWnd) return FALSE;                   // 正しい条件

if (msg.wParam && !isActive) {             // 正しい条件
    [appDelegate applicationDidBecomeActive];
}
```

### エスケープルール詳細

#### 遅延展開環境での感嘆符エスケープ

| 目的 | 間違った記述 | 正しい記述 | 結果 |
|------|-------------|------------|------|
| `!`を出力 | `^!` | `^^!` | `!` |
| `(!`を出力 | `^(!` | `^(^^!` | `(!` |
| `!)`を出力 | `!^)` | `^^!^)` | `!)` |

#### なぜ`^^!`が必要か
1. **第1段階**: `^^!` → `^!` (バッチエスケープ処理)
2. **第2段階**: `^!` → `!` (遅延展開処理)
3. **最終出力**: `!`

#### 単一`^!`の場合の問題
1. **第1段階**: `^!` → `!` (バッチエスケープ処理)  
2. **第2段階**: `!` → `[空文字列]` (遅延展開で変数として解釈)
3. **最終出力**: `[空文字列]`

### 影響範囲
- **WinMain関数**: ウィンドウ初期化失敗時の処理が逆転
- **InitInstance関数**: ウィンドウ作成失敗時の処理が逆転  
- **WM_ACTIVATEAPPハンドラ**: アクティブ状態変化の処理が逆転

### 修正されたバッチファイル箇所

```bat
# WinMain内
echo     if ^(^^!InitInstance^(hInstance, nCmdShow^)^)
echo     ^{
echo         return FALSE^;
echo     ^}

# InitInstance内  
echo    if ^(^^!hWnd^) return FALSE^;

# WM_ACTIVATEAPPハンドラ内
echo             if ^(msg.wParam ^&^& ^^!isActive^) ^{
echo                 [appDelegate applicationDidBecomeActive]^;
echo                 isActive = TRUE^;
echo             ^} else if ^(^^!msg.wParam ^&^& isActive^) ^{
echo                 [appDelegate applicationWillResignActive]^;
echo                 isActive = FALSE^;
echo             ^}
```

### 検証方法
1. バッチファイル実行: `create_vcxproj.bat -src -objc test_project`
2. 生成された`test_project.m`の以下の箇所を確認:
   - `if (!InitInstance(hInstance, nCmdShow))`（正しく否定演算子が含まれる）
   - `if (!hWnd) return FALSE;`（正しく否定演算子が含まれる）
   - `if (msg.wParam && !isActive)`（正しく否定演算子が含まれる）

### 重要注意事項
**遅延展開環境（`setlocal enabledelayedexpansion`）では、感嘆符`!`のエスケープには必ず`^^!`を使用する。単一`^!`は完全に無効。**

---
