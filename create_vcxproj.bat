setlocal enabledelayedexpansion

REM -cleanオプション対応: 指定したプロジェクトの全ファイル・フォルダを削除
set CLEAN_MODE=
if /i "%~1"=="-clean" (
    set CLEAN_MODE=1
    shift
    goto clean_project
)

REM オプション解析ループ - 全ての引数を順次処理
set GEN_MAIN_SRC=
set OBJC_MODE=
set GNUSTEP_PATH=./gnustep
set PROJNAME=

REM 全ての引数を最初から順番に処理
:parse_args
if "%~1"=="" goto check_projname

REM オプション処理
if /i "%~1"=="-src" (
    set GEN_MAIN_SRC=1
    shift
    goto parse_args
)
if /i "%~1"=="-objc" (
    set OBJC_MODE=1
    shift
    goto parse_args
)
if /i "%~1"=="-path" (
    if not "%~2"=="" (
        set GNUSTEP_PATH=%~2
        shift
        shift
        goto parse_args
    ) else (
        echo エラー: -pathオプションに値が指定されていません
        exit /b 1
    )
)

REM オプション以外の最初の引数をプロジェクト名とする
if not defined PROJNAME (
    set PROJNAME=%~1
    shift
    goto parse_args
)

REM プロジェクト名設定後の引数は追加ソースファイル
set ADDITIONAL_SOURCES=!ADDITIONAL_SOURCES! %~1
shift
goto parse_args

:check_projname
if not defined PROJNAME (
    echo エラー: プロジェクト名を指定してください
    echo 使用方法: create_vcxproj.bat [-src] [-objc] [-path gnustep_path] project_name [source_files...]
    exit /b 1
)

goto generate_main_files

:generate_main_files
if defined GEN_MAIN_SRC (
    if defined OBJC_MODE (
        echo Objective-C main.mファイルを生成中...
        REM Objective-C用main.mファイル生成（C言語形式）
        set TMP_MAIN=!PROJNAME!_tmp.m
        (
            echo // ObjCWin32.m : Objective-C デリゲート/ビュー/ビューコントローラ構造でのWin32アプリ
            echo //
            echo.
            echo #include ^<Foundation/Foundation.h^>
            echo #define WIN32_LEAN_AND_MEAN
            echo #include ^<windows.h^>
            echo.
            echo // --- AppDelegate ---
            echo @interface AppDelegate : NSObject
            echo @property ^(nonatomic, assign^) HINSTANCE hInstance^;
            echo @property ^(nonatomic, assign^) int nCmdShow^;
            echo - ^(void^)applicationWillFinishLaunching^;
            echo - ^(void^)applicationDidFinishLaunching^;
            echo - ^(void^)applicationDidBecomeActive^;
            echo - ^(void^)applicationWillResignActive^;
            echo - ^(void^)applicationWillTerminate^;
            echo @end
            echo.
            echo // --- View ---
            echo @interface MyView : NSObject
            echo @property ^(nonatomic, assign^) HWND hWnd^;
            echo - ^(void^)drawRect^;
            echo - ^(void^)handleMouseDownAt:^(POINT^)pt^;
            echo @end
            echo.
            echo // --- ViewController ---
            echo @interface MyViewController : NSObject
            echo @property ^(nonatomic, strong^) MyView *view^;
            echo - ^(void^)loadViewWithParent:^(HWND^)parent^;
            echo - ^(void^)handleMouseDownAt:^(POINT^)pt^;
            echo @end
            echo.
            echo @implementation AppDelegate
            echo - ^(void^)applicationWillFinishLaunching ^{
            echo     NSLog^(@"[AppDelegate] アプリケーションの起動準備中"^)^;
            echo ^}
            echo - ^(void^)applicationDidFinishLaunching ^{
            echo     NSLog^(@"[AppDelegate] アプリケーションの起動が完了しました"^)^;
            echo ^}
            echo - ^(void^)applicationDidBecomeActive ^{
            echo     NSLog^(@"[AppDelegate] アクティブになりました"^)^;
            echo ^}
            echo - ^(void^)applicationWillResignActive ^{
            echo     NSLog^(@"[AppDelegate] 非アクティブになりました"^)^;
            echo ^}
            echo - ^(void^)applicationWillTerminate ^{
            echo     NSLog^(@"[AppDelegate] アプリケーションを終了します"^)^;
            echo ^}
            echo @end
            echo.
            echo @implementation MyView
            echo - ^(void^)drawRect ^{
            echo     PAINTSTRUCT ps^;
            echo     HDC hdc = BeginPaint^(self.hWnd, ^&ps^)^;
            echo     TextOutW^(hdc, 20, 20, L"Hello from MyView!", 18^)^;
            echo     EndPaint^(self.hWnd, ^&ps^)^;
            echo ^}
            echo - ^(void^)handleMouseDownAt:^(POINT^)pt ^{
            echo     NSLog^(@"[MyView] マウスがクリックされました: x=%%ld, y=%%ld", pt.x, pt.y^)^;
            echo     // 必要ならここで再描画や他の処理
            echo ^}
            echo @end
            echo.
            echo @implementation MyViewController
            echo - ^(void^)loadViewWithParent:^(HWND^)parent ^{
            echo     self.view = [[MyView alloc] init]^;
            echo     self.view.hWnd = parent^;
            echo     NSLog^(@"[MyViewController] ビューが親ウィンドウにアタッチされました"^)^;
            echo ^}
            echo - ^(void^)handleMouseDownAt:^(POINT^)pt ^{
            echo     if ^(self.view^) ^{
            echo         [self.view handleMouseDownAt:pt]^;
            echo     ^}
            echo ^}
            echo @end
            echo.
            echo // グローバル変数
            echo HINSTANCE hInst^;
            echo WCHAR szWindowClass[] = L"ObjCWin32WindowClass"^;
            echo AppDelegate *appDelegate^;
            echo MyViewController *viewController^;
            echo.
            echo // プロトタイプ宣言
            echo ATOM                MyRegisterClass^(HINSTANCE hInstance^)^;
            echo BOOL                InitInstance^(HINSTANCE, int^)^;
            echo LRESULT CALLBACK    WndProc^(HWND, UINT, WPARAM, LPARAM^)^;
            echo.
            echo // WinMain
            echo int APIENTRY wWinMain^(_In_ HINSTANCE hInstance,
            echo                      _In_opt_ HINSTANCE hPrevInstance,
            echo                      _In_ LPWSTR    lpCmdLine,
            echo                      _In_ int       nCmdShow^)
            echo ^{
            echo     UNREFERENCED_PARAMETER^(hPrevInstance^)^;
            echo     UNREFERENCED_PARAMETER^(lpCmdLine^)^;
            echo.
            echo     appDelegate = [[AppDelegate alloc] init]^;
            echo     appDelegate.hInstance = hInstance^;
            echo     appDelegate.nCmdShow = nCmdShow^;
            echo     [appDelegate applicationWillFinishLaunching]^;
            echo     [appDelegate applicationDidFinishLaunching]^;
            echo.
            echo     MyRegisterClass^(hInstance^)^;
            echo.
            echo     if ^(^^!InitInstance^(hInstance, nCmdShow^)^)
            echo     ^{
            echo         return FALSE^;
            echo     ^}
            echo.
            echo     [appDelegate applicationDidBecomeActive]^;
            echo.
            echo     MSG msg^;
            echo     BOOL isActive = TRUE^;
            echo     while ^(GetMessage^(^&msg, 0, 0, 0^)^)
            echo     ^{
            echo         if ^(msg.message == WM_ACTIVATEAPP^) ^{
            echo             if ^(msg.wParam ^&^& ^^!isActive^) ^{
            echo                 [appDelegate applicationDidBecomeActive]^;
            echo                 isActive = TRUE^;
            echo             ^} else if ^(^^!msg.wParam ^&^& isActive^) ^{
            echo                 [appDelegate applicationWillResignActive]^;
            echo                 isActive = FALSE^;
            echo             ^}
            echo         ^}
            echo         TranslateMessage^(^&msg^)^;
            echo         DispatchMessage^(^&msg^)^;
            echo     ^}
            echo     [appDelegate applicationWillTerminate]^;
            echo     return ^(int^) msg.wParam^;
            echo ^}
            echo.
            echo // ウィンドウクラス登録
            echo ATOM MyRegisterClass^(HINSTANCE hInstance^)
            echo ^{
            echo     WNDCLASSEXW wcex^;
            echo     wcex.cbSize = sizeof^(WNDCLASSEX^)^;
            echo     wcex.style          = CS_HREDRAW ^| CS_VREDRAW^;
            echo     wcex.lpfnWndProc    = WndProc^;
            echo     wcex.cbClsExtra     = 0^;
            echo     wcex.cbWndExtra     = 0^;
            echo     wcex.hInstance      = hInstance^;
            echo     wcex.hIcon          = LoadIcon^(0, IDI_APPLICATION^)^; // デフォルトアイコン
            echo     wcex.hCursor        = LoadCursor^(0, IDC_ARROW^)^;
            echo     wcex.hbrBackground  = ^(HBRUSH^)^(COLOR_WINDOW+1^)^;
            echo     wcex.lpszMenuName   = 0^; // メニューなし
            echo     wcex.lpszClassName  = szWindowClass^;
            echo     wcex.hIconSm        = LoadIcon^(0, IDI_APPLICATION^)^; // デフォルトアイコン
            echo     return RegisterClassExW^(^&wcex^)^;
            echo ^}
            echo.
            echo // ウィンドウ生成
            echo BOOL InitInstance^(HINSTANCE hInstance, int nCmdShow^)
            echo ^{
            echo    hInst = hInstance^;
            echo    LPCWSTR windowTitle = L"Hello Objective-C Window"^;
            echo    HWND hWnd = CreateWindowW^(szWindowClass, windowTitle, WS_OVERLAPPEDWINDOW,
            echo       CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 0, 0, hInstance, 0^)^;
            echo    if ^(^^!hWnd^) return FALSE^;
            echo    ShowWindow^(hWnd, nCmdShow^)^;
            echo    UpdateWindow^(hWnd^)^;
            echo    // ViewController生成・Viewロード
            echo    viewController = [[MyViewController alloc] init]^;
            echo    [viewController loadViewWithParent:hWnd]^;
            echo    return TRUE^;
            echo ^}
            echo.
            echo // ウィンドウプロシージャ
            echo LRESULT CALLBACK WndProc^(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam^)
            echo ^{
            echo     switch ^(message^)
            echo     ^{
            echo     case WM_PAINT:
            echo         if ^(viewController ^&^& viewController.view^) ^{
            echo             [viewController.view drawRect]^;
            echo         ^}
            echo         break^;
            echo     case WM_LBUTTONDOWN: ^{
            echo         if ^(viewController^) ^{
            echo             POINT pt = ^{ LOWORD^(lParam^), HIWORD^(lParam^) ^}^;
            echo             [viewController handleMouseDownAt:pt]^;
            echo         ^}
            echo         break^;
            echo     ^}
            echo     case WM_DESTROY:
            echo         PostQuitMessage^(0^)^;
            echo         break^;
            echo     default:
            echo         return DefWindowProc^(hWnd, message, wParam, lParam^)^;
            echo     ^}
            echo     return 0^;
            echo ^}
        ) > "!TMP_MAIN!"
        if exist "!TMP_MAIN!" (
            powershell -Command "Get-Content '!TMP_MAIN!' | Set-Content '!PROJNAME!.m' -Encoding UTF8"
            del "!TMP_MAIN!"
            if exist "!PROJNAME!.m" (
                echo !PROJNAME!.m を作成しました
            ) else (
                echo エラー: !PROJNAME!.m の作成に失敗しました
            )
        ) else (
            echo エラー: 一時ファイルの作成に失敗しました
        )
    ) else (
        echo C言語 main.cファイルを生成中...
        REM 通常のC言語用main.cファイル生成
        set TMP_MAIN=!PROJNAME!_main_tmp.c
    (
        echo #include ^<windows.h^>
        echo #include ^<stdio.h^>
        echo #include ^<versionhelpers.h^>
        echo.
        echo /*
        echo  * Win32 コンソール/Windowsアプリケーション両対応サンプル
        echo  * SDLチェック無効、CRT警告抑制対応版
        echo  */
        echo void sample_main^(void^); 
        echo.
        echo void sample_main^(void^) 
        echo { 
        echo     OSVERSIONINFOEX osvi^; 
        echo     char buf[512]^; 
        echo     HANDLE hConsole^; 
        echo     SYSTEMTIME st^; 
        echo     DWORD drives^; 
        echo     int i^; 
        echo     char driveLetter^; 
        echo #ifdef _WINDOWS 
        echo     char dt[64]^; 
        echo #endif 
        echo.
        echo     ZeroMemory^(^&osvi, sizeof^(OSVERSIONINFOEX^)^)^; 
        echo     osvi.dwOSVersionInfoSize = sizeof^(OSVERSIONINFOEX^)^; 
        echo.
        echo     if ^(GetVersionEx^(^(OSVERSIONINFO*^)^&osvi^)^) 
        echo     ^{ 
        echo         sprintf^(buf, "Windows バージョン: %%d.%%d (Build %%d)", osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber^)^; 
        echo     ^} 
        echo     else 
        echo     ^{ 
        echo         sprintf^(buf, "Windows バージョン情報を取得できませんでした"^)^; 
        echo     ^} 
        echo.
        echo #ifdef _CONSOLE 
        echo     printf^("%%s\n", buf^)^; 
        echo     SetConsoleTitle^("Win32 サンプル（_CONSOLE）"^)^; 
        echo     hConsole = GetStdHandle^(STD_OUTPUT_HANDLE^)^; 
        echo     SetConsoleTextAttribute^(hConsole, FOREGROUND_RED ^| FOREGROUND_GREEN ^| FOREGROUND_INTENSITY^)^; 
        echo     printf^("\n現在の日時:\n"^)^; 
        echo #else 
        echo     MessageBox^(NULL, buf, "Win32 サンプル（_WINDOWS）", MB_OK ^| MB_ICONINFORMATION^)^; 
        echo #endif 
        echo.
        echo     GetLocalTime^(^&st^)^; 
        echo.
        echo #ifdef _CONSOLE 
        echo     printf^("%%04d/%%02d/%%02d %%02d:%%02d:%%02d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond^)^; 
        echo.
        echo     drives = GetLogicalDrives^(^)^; 
        echo     printf^("\n利用可能なドライブ:\n"^)^; 
        echo.
        echo     for ^(i = 0^; i ^< 26^; i++^) 
        echo     ^{ 
        echo         DWORD mask = drives ^& ^(1 ^<^< i^)^; 
        echo         if ^(mask != 0^) 
        echo         ^{ 
        echo             driveLetter = 'A' + i^; 
        echo             printf^("%%c:\\\n", driveLetter^)^; 
        echo         ^} 
        echo     ^} 
        echo.
        echo     printf^("\nキーを押して終了してください..."^)^; 
        echo     getchar^(^)^; 
        echo #else 
        echo     sprintf^(dt, "%%04d/%%02d/%%02d %%02d:%%02d:%%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond^)^; 
        echo     MessageBox^(NULL, dt, "現在の日時", MB_OK^)^; 
        echo #endif 
        echo ^} 
        echo.
        echo #ifdef _CONSOLE 
        echo int main^(int argc, char **argv^) 
        echo ^{ 
        echo     sample_main^(^)^; 
        echo     return 0^; 
        echo ^} 
        echo #elif defined^(_WINDOWS^) 
        echo int WINAPI WinMain^(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow^) 
        echo ^{ 
        echo     sample_main^(^)^; 
        echo     return 0^; 
        echo ^} 
        echo #endif 
    ) > !TMP_MAIN!
    if exist "!TMP_MAIN!" (
        powershell -Command "Get-Content '!TMP_MAIN!' | Set-Content '!PROJNAME!_main.c' -Encoding UTF8"
        del "!TMP_MAIN!"
        if exist "!PROJNAME!_main.c" (
            echo !PROJNAME!_main.c を作成しました
        ) else (
            echo エラー: !PROJNAME!_main.c の作成に失敗しました
        )
    ) else (
        echo エラー: 一時ファイルの作成に失敗しました
    )
    )
)

REM GUIDを生成
for /f "delims=" %%i in ('powershell -Command "[guid]::NewGuid().ToString()"') do set GUID=%%i

REM テンプレートをコピー
if defined OBJC_MODE (
    REM Objective-C用テンプレートを使用（まだ作成していないので、通常テンプレートをベースに動的生成）
    copy template.vcxproj %PROJNAME%.vcxproj > nul
) else (
    copy template.vcxproj %PROJNAME%.vcxproj > nul
)

REM プロジェクト名とGUIDを置換
powershell -Command "(Get-Content '%PROJNAME%.vcxproj') -replace 'TEMPLATE', '%PROJNAME%' -replace '{TEMPLATE_GUID}', '{%GUID%}' | Set-Content '%PROJNAME%.vcxproj' -Encoding UTF8"

REM Objective-C用の設定を追加
if defined OBJC_MODE (
    echo GNUStep設定を適用中...
    echo 使用するGNUStepパス: %GNUSTEP_PATH%
    
    REM PowerShellスクリプトを一時ファイルとして作成
    set PS_SCRIPT=%PROJNAME%_objc_setup.ps1
    (
        echo $projName = '%PROJNAME%'
        echo $gnustepPath = '%GNUSTEP_PATH%'
        echo.
        echo # 全ての構成でLLVM^(clang-cl^)ツールセットに変更し、x64構成にObjective-C設定を追加
        echo $content = Get-Content "$projName.vcxproj"
        echo $newContent = @^(^)
        echo $inX64Group = $false
        echo.
        echo foreach ^($line in $content^) ^{
        echo     if ^($line -match '^<PlatformToolset^>v143^</PlatformToolset^>'^) ^{
        echo         $newContent += '    ^<PlatformToolset^>ClangCL^</PlatformToolset^>'
        echo     ^} elseif ^($line -match 'ItemDefinitionGroup.*x64'^) ^{
        echo         $inX64Group = $true
        echo         $newContent += $line
        echo     ^} elseif ^($line -match '^</ItemDefinitionGroup^>' -and $inX64Group^) ^{
        echo         $inX64Group = $false
        echo         $newContent += $line
        echo     ^} elseif ^($inX64Group -and $line -match '^<PreprocessorDefinitions^>.*^</PreprocessorDefinitions^>' -and $line -notmatch 'GNUSTEP'^) ^{
        echo         $newContent += $line -replace '^<PreprocessorDefinitions^>', '^<PreprocessorDefinitions^>GNUSTEP;GNUSTEP_WITH_DLL;GNUSTEP_RUNTIME=1;_NONFRAGILE_ABI=1;_NATIVE_OBJC_EXCEPTIONS;' -replace '^</PreprocessorDefinitions^>', ';%%^(PreprocessorDefinitions^)^</PreprocessorDefinitions^>'
        echo     ^} elseif ^($inX64Group -and $line -match '^</ClCompile^>'^) ^{
        echo         $newContent += '      ^<AdditionalIncludeDirectories^>' + $gnustepPath + '/include;%%^(AdditionalIncludeDirectories^)^</AdditionalIncludeDirectories^>'
        echo         $newContent += '      ^<AdditionalOptions^>-fobjc-runtime=gnustep-2.0 -Xclang -fexceptions -Xclang -fobjc-exceptions -fblocks -Xclang -fobjc-arc %%^(AdditionalOptions^)^</AdditionalOptions^>'
        echo         $newContent += '      ^<RuntimeLibrary^>MultiThreadedDebug^</RuntimeLibrary^>'
        echo         $newContent += $line
        echo     ^} elseif ^($inX64Group -and $line -match '^</Link^>'^) ^{
        echo         $newContent += '      ^<AdditionalLibraryDirectories^>' + $gnustepPath + '/lib;%%^(AdditionalLibraryDirectories^)^</AdditionalLibraryDirectories^>'
        echo         $newContent += '      ^<AdditionalDependencies^>gnustep-base.lib;objc.lib;dispatch.lib;%%^(AdditionalDependencies^)^</AdditionalDependencies^>'
        echo         $newContent += $line
        echo     ^} else ^{
        echo         $newContent += $line
        echo     ^}
        echo ^}
        echo.
        echo Set-Content "$projName.vcxproj" $newContent -Encoding UTF8
        echo.
        echo # DLLコピー設定を追加
        echo $content = Get-Content "$projName.vcxproj"
        echo $newContent = @^(^)
        echo.
        echo foreach ^($line in $content^) ^{
        echo     if ^($line -match 'Import.*Microsoft\.Cpp\.targets'^) ^{
        echo         $newContent += '  ^<ItemGroup^>'
        echo         $newContent += '    ^<Content Include="' + $gnustepPath + '/$(LibrariesArchitecture)/$(Configuration)/bin/*.dll"^>'
        echo         $newContent += '      ^<CopyToOutputDirectory^>PreserveNewest^</CopyToOutputDirectory^>'
        echo         $newContent += '      ^<TargetPath^>%%^(Filename^)%%^(Extension^)^</TargetPath^>'
        echo         $newContent += '    ^</Content^>'
        echo         $newContent += '  ^</ItemGroup^>'
        echo     ^}
        echo     $newContent += $line
        echo ^}
        echo.
        echo Set-Content "$projName.vcxproj" $newContent -Encoding UTF8
    ) > "!PS_SCRIPT!"
    
    REM PowerShellスクリプトを実行
    powershell -NoProfile -ExecutionPolicy Bypass -File "!PS_SCRIPT!"
    
    REM 一時ファイルを削除
    if exist "!PS_SCRIPT!" del "!PS_SCRIPT!"
)

REM ソースファイルのリストを作成（スペースで区切る）
set SOURCE_FILES=
if defined GEN_MAIN_SRC (
    if defined OBJC_MODE (
        set SOURCE_FILES=%PROJNAME%.m
    ) else (
        set SOURCE_FILES=%PROJNAME%_main.c
    )
)

REM 追加のソースファイルがあれば追加
if defined ADDITIONAL_SOURCES (
    set SOURCE_FILES=!SOURCE_FILES!!ADDITIONAL_SOURCES!
)

:create_filters
REM ソースファイルがある場合は追加（-src時のmain.cも含む）
if defined OBJC_MODE (
    REM Objective-C用ClCompile（CompileAsタグ付き）- PowerShellスクリプトファイル使用
    set PS_ADDFILES=!PROJNAME!_addfiles.ps1
    (
        echo $files = '%SOURCE_FILES%'.Split^(' '^) ^| Where-Object {$_}
        echo if ^($files^) {
        echo     $content = Get-Content '%PROJNAME%.vcxproj'
        echo     $index = 0
        echo     for ^($i=0; $i -lt $content.Length; $i++^) {
        echo         if ^($content[$i].Contains^('プロジェクトアイテム'^)^) { $index = $i }
        echo     }
        echo     $newContent = @^(^)
        echo     for ^($i=0; $i -lt $content.Length; $i++^) {
        echo         $newContent += $content[$i]
        echo         if ^($i -eq $index^) {
        echo             foreach ^($file in $files^) {
        echo                 $newContent += '    ^<ClCompile Include="' + $file + '"^>'
        echo                 $newContent += '      ^<CompileAs Condition="''$(Configuration)|$(Platform)''==''Debug MBCS|x64''"^>'
        echo                 $newContent += '      ^</CompileAs^>'
        echo                 $newContent += '      ^<CompileAs Condition="''$(Configuration)|$(Platform)''==''Release MBCS|x64''"^>'
        echo                 $newContent += '      ^</CompileAs^>'
        echo                 $newContent += '      ^<CompileAs Condition="''$(Configuration)|$(Platform)''==''Debug Unicode|x64''"^>'
        echo                 $newContent += '      ^</CompileAs^>'
        echo                 $newContent += '      ^<CompileAs Condition="''$(Configuration)|$(Platform)''==''Release Unicode|x64''"^>'
        echo                 $newContent += '      ^</CompileAs^>'
        echo                 $newContent += '    ^</ClCompile^>'
        echo             }
        echo         }
        echo     }
        echo     Set-Content '%PROJNAME%.vcxproj' $newContent -Encoding UTF8
        echo }
    ) > "!PS_ADDFILES!"
    powershell -NoProfile -ExecutionPolicy Bypass -File "!PS_ADDFILES!"
    if exist "!PS_ADDFILES!" del "!PS_ADDFILES!"
) else (
    REM 通常のC/C++用ClCompile - PowerShellスクリプトファイル使用
    set PS_ADDFILES=!PROJNAME!_addfiles.ps1
    (
        echo $files = '%SOURCE_FILES%'.Split^(' '^) ^| Where-Object {$_}
        echo if ^($files^) {
        echo     $content = Get-Content '%PROJNAME%.vcxproj'
        echo     $index = 0
        echo     for ^($i=0; $i -lt $content.Length; $i++^) {
        echo         if ^($content[$i].Contains^('プロジェクトアイテム'^)^) { $index = $i }
        echo     }
        echo     $newContent = @^(^)
        echo     for ^($i=0; $i -lt $content.Length; $i++^) {
        echo         $newContent += $content[$i]
        echo         if ^($i -eq $index^) {
        echo             foreach ^($file in $files^) {
        echo                 $newContent += '    ^<ClCompile Include="' + $file + '" /^>'
        echo             }
        echo         }
        echo     }
        echo     Set-Content '%PROJNAME%.vcxproj' $newContent -Encoding UTF8
        echo }
    ) > "!PS_ADDFILES!"
    powershell -NoProfile -ExecutionPolicy Bypass -File "!PS_ADDFILES!"
    if exist "!PS_ADDFILES!" del "!PS_ADDFILES!"
)

REM フィルタファイルをPowerShellで直接UTF-8で作成
if defined OBJC_MODE (
    REM Objective-C用フィルタ（.m拡張子を追加）
    powershell -NoProfile -ExecutionPolicy Bypass -Command "$filters = @(@{Name='ソース ファイル';Ext='cpp;c;cc;cxx;c++;cppm;ixx;def;odl;idl;hpj;bat;asm;asmx;mm;m'},@{Name='ヘッダー ファイル';Ext='h;hh;hpp;hxx;h++;hm;inl;inc;ipp;xsd'},@{Name='リソース ファイル';Ext='rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx;tiff;tif;png;wav;mfcribbon-ms'}); $content = @(); $content += '<?xml version=\"1.0\" encoding=\"utf-8\"?>'; $content += '<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">'; $content += '  <ItemGroup>'; foreach ($filter in $filters) { $content += '    <Filter Include=\"' + $filter.Name + '\">'; $content += '      <UniqueIdentifier>{' + [guid]::NewGuid() + '}</UniqueIdentifier>'; $content += '      <Extensions>' + $filter.Ext + '</Extensions>'; $content += '    </Filter>' }; $content += '  </ItemGroup>'; $sourceFiles = '%SOURCE_FILES%' -split ' ' | Where-Object { $_ }; if ($sourceFiles) { $content += '  <ItemGroup>'; foreach ($file in $sourceFiles) { $content += '    <ClCompile Include=\"' + $file + '\">'; $content += '      <Filter>ソース ファイル</Filter>'; $content += '    </ClCompile>' }; $content += '  </ItemGroup>' }; $content += '</Project>'; [System.IO.File]::WriteAllLines('%PROJNAME%.vcxproj.filters', $content, [System.Text.UTF8Encoding]::new($false))"
) else (
    REM 通常のC/C++用フィルタ
    powershell -NoProfile -ExecutionPolicy Bypass -Command "$filters = @(@{Name='ソース ファイル';Ext='cpp;c;cc;cxx;c++;cppm;ixx;def;odl;idl;hpj;bat;asm;asmx'},@{Name='ヘッダー ファイル';Ext='h;hh;hpp;hxx;h++;hm;inl;inc;ipp;xsd'},@{Name='リソース ファイル';Ext='rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx;tiff;tif;png;wav;mfcribbon-ms'}); $content = @(); $content += '<?xml version=\"1.0\" encoding=\"utf-8\"?>'; $content += '<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">'; $content += '  <ItemGroup>'; foreach ($filter in $filters) { $content += '    <Filter Include=\"' + $filter.Name + '\">'; $content += '      <UniqueIdentifier>{' + [guid]::NewGuid() + '}</UniqueIdentifier>'; $content += '      <Extensions>' + $filter.Ext + '</Extensions>'; $content += '    </Filter>' }; $content += '  </ItemGroup>'; $sourceFiles = '%SOURCE_FILES%' -split ' ' | Where-Object { $_ }; if ($sourceFiles) { $content += '  <ItemGroup>'; foreach ($file in $sourceFiles) { $content += '    <ClCompile Include=\"' + $file + '\">'; $content += '      <Filter>ソース ファイル</Filter>'; $content += '    </ClCompile>' }; $content += '  </ItemGroup>' }; $content += '</Project>'; [System.IO.File]::WriteAllLines('%PROJNAME%.vcxproj.filters', $content, [System.Text.UTF8Encoding]::new($false))"
)


REM 全構成ビルドバッチも生成（MBCS/UNICODE・x86/x64・Debug/Release・WINDOWS/CONSOLE）
(
    echo @echo off
    echo REM MBCS/CONSOLE/x86
    echo MSBuild.exe "%PROJNAME%.vcxproj" /p:Configuration="Debug MBCS";Platform=Win32;AppType=CONSOLE
    echo MSBuild.exe "%PROJNAME%.vcxproj" /p:Configuration="Release MBCS";Platform=Win32;AppType=CONSOLE
    echo REM MBCS/CONSOLE/x64
    echo MSBuild.exe "%PROJNAME%.vcxproj" /p:Configuration="Debug MBCS";Platform=x64;AppType=CONSOLE
    echo MSBuild.exe "%PROJNAME%.vcxproj" /p:Configuration="Release MBCS";Platform=x64;AppType=CONSOLE
    echo REM MBCS/WINDOWS/x86
    echo MSBuild.exe "%PROJNAME%.vcxproj" /p:Configuration="Debug MBCS";Platform=Win32;AppType=WINDOWS
    echo MSBuild.exe "%PROJNAME%.vcxproj" /p:Configuration="Release MBCS";Platform=Win32;AppType=WINDOWS
    echo REM MBCS/WINDOWS/x64
    echo MSBuild.exe "%PROJNAME%.vcxproj" /p:Configuration="Debug MBCS";Platform=x64;AppType=WINDOWS
    echo MSBuild.exe "%PROJNAME%.vcxproj" /p:Configuration="Release MBCS";Platform=x64;AppType=WINDOWS
    echo REM UNICODE/CONSOLE/x86
    echo MSBuild.exe "%PROJNAME%.vcxproj" /p:Configuration="Debug Unicode";Platform=Win32;AppType=CONSOLE
    echo MSBuild.exe "%PROJNAME%.vcxproj" /p:Configuration="Release Unicode";Platform=Win32;AppType=CONSOLE
    echo REM UNICODE/CONSOLE/x64
    echo MSBuild.exe "%PROJNAME%.vcxproj" /p:Configuration="Debug Unicode";Platform=x64;AppType=CONSOLE
    echo MSBuild.exe "%PROJNAME%.vcxproj" /p:Configuration="Release Unicode";Platform=x64;AppType=CONSOLE
    echo REM UNICODE/WINDOWS/x86
    echo MSBuild.exe "%PROJNAME%.vcxproj" /p:Configuration="Debug Unicode";Platform=Win32;AppType=WINDOWS
    echo MSBuild.exe "%PROJNAME%.vcxproj" /p:Configuration="Release Unicode";Platform=Win32;AppType=WINDOWS
    echo REM UNICODE/WINDOWS/x64
    echo MSBuild.exe "%PROJNAME%.vcxproj" /p:Configuration="Debug Unicode";Platform=x64;AppType=WINDOWS
    echo MSBuild.exe "%PROJNAME%.vcxproj" /p:Configuration="Release Unicode";Platform=x64;AppType=WINDOWS
    echo.
    echo echo === すべての構成のビルドが完了しました ===
) > %PROJNAME%_build_all.bat

echo.
if defined OBJC_MODE (
    echo Objective-C プロジェクト %PROJNAME% を作成しました
    echo GNUStepパス: %GNUSTEP_PATH%
) else (
    echo プロジェクト %PROJNAME% を作成しました
)
goto :eof

:clean_project
REM プロジェクト名を引数で受け取る
if "%~1"=="" (
    echo プロジェクト名を指定してください
    echo 使用法: create_vcxproj.bat -clean プロジェクト名
    exit /b 1
)
set PROJNAME=%~1

echo %PROJNAME% プロジェクトの全ファイル・フォルダを削除しています...

REM プロジェクトファイル削除
if exist "%PROJNAME%.vcxproj" (
    del "%PROJNAME%.vcxproj" >nul 2>&1
    echo 削除: %PROJNAME%.vcxproj
)
if exist "%PROJNAME%.vcxproj.filters" (
    del "%PROJNAME%.vcxproj.filters" >nul 2>&1
    echo 削除: %PROJNAME%.vcxproj.filters
)
if exist "%PROJNAME%.vcxproj.user" (
    del "%PROJNAME%.vcxproj.user" >nul 2>&1
    echo 削除: %PROJNAME%.vcxproj.user
)

REM ビルドバッチファイル削除
if exist "%PROJNAME%_build_all.bat" (
    del "%PROJNAME%_build_all.bat" >nul 2>&1
    echo 削除: %PROJNAME%_build_all.bat
)

REM main.cファイル削除
if exist "%PROJNAME%_main.c" (
    del "%PROJNAME%_main.c" >nul 2>&1
    echo 削除: %PROJNAME%_main.c
)
if exist "%PROJNAME%.m" (
    del "%PROJNAME%.m" >nul 2>&1
    echo 削除: %PROJNAME%.m
)
if exist "%PROJNAME%_main.m" (
    del "%PROJNAME%_main.m" >nul 2>&1
    echo 削除: %PROJNAME%_main.m
)

REM ビルド出力フォルダ削除（Win32構成）
if exist "Debug MBCS" (
    rmdir /s /q "Debug MBCS" >nul 2>&1
    echo 削除: Debug MBCS フォルダ
)
if exist "Release MBCS" (
    rmdir /s /q "Release MBCS" >nul 2>&1
    echo 削除: Release MBCS フォルダ
)
if exist "Debug Unicode" (
    rmdir /s /q "Debug Unicode" >nul 2>&1
    echo 削除: Debug Unicode フォルダ
)
if exist "Release Unicode" (
    rmdir /s /q "Release Unicode" >nul 2>&1
    echo 削除: Release Unicode フォルダ
)

REM x64ビルド出力フォルダ削除
if exist "x64" (
    rmdir /s /q "x64" >nul 2>&1
    echo 削除: x64 フォルダ
)

REM プロジェクト名付きビルド出力フォルダ削除（Win32構成）
if exist "%PROJNAME%\Debug MBCS" (
    rmdir /s /q "%PROJNAME%\Debug MBCS" >nul 2>&1
    echo 削除: %PROJNAME%\Debug MBCS フォルダ
)
if exist "%PROJNAME%\Release MBCS" (
    rmdir /s /q "%PROJNAME%\Release MBCS" >nul 2>&1
    echo 削除: %PROJNAME%\Release MBCS フォルダ
)
if exist "%PROJNAME%\Debug Unicode" (
    rmdir /s /q "%PROJNAME%\Debug Unicode" >nul 2>&1
    echo 削除: %PROJNAME%\Debug Unicode フォルダ
)
if exist "%PROJNAME%\Release Unicode" (
    rmdir /s /q "%PROJNAME%\Release Unicode" >nul 2>&1
    echo 削除: %PROJNAME%\Release Unicode フォルダ
)

REM プロジェクト名付きx64ビルド出力フォルダ削除
if exist "%PROJNAME%\x64" (
    rmdir /s /q "%PROJNAME%\x64" >nul 2>&1
    echo 削除: %PROJNAME%\x64 フォルダ
)

REM プロジェクト名フォルダが空の場合は削除
if exist "%PROJNAME%" (
    rmdir "%PROJNAME%" >nul 2>&1
    if not exist "%PROJNAME%" (
        echo 削除: %PROJNAME% フォルダ
    )
)

REM Visual Studioの一時ファイル削除
if exist "*.pdb" (
    del "*.pdb" >nul 2>&1
    echo 削除: PDBファイル
)
if exist "*.ilk" (
    del "*.ilk" >nul 2>&1
    echo 削除: ILKファイル
)
if exist "*.obj" (
    del "*.obj" >nul 2>&1
    echo 削除: OBJファイル
)

echo.
echo === %PROJNAME% プロジェクトのクリーンアップが完了しました ===

goto :eof
