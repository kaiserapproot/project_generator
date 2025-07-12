setlocal enabledelayedexpansion

REM -clean�I�v�V�����Ή�: �w�肵���v���W�F�N�g�̑S�t�@�C���E�t�H���_���폜
set CLEAN_MODE=
if /i "%~1"=="-clean" (
    set CLEAN_MODE=1
    shift
    goto clean_project
)

REM �I�v�V������̓��[�v - �S�Ă̈�������������
set GEN_MAIN_SRC=
set OBJC_MODE=
set GNUSTEP_PATH=./gnustep
set PROJNAME=

REM �S�Ă̈������ŏ����珇�Ԃɏ���
:parse_args
if "%~1"=="" goto check_projname

REM �I�v�V��������
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
        echo �G���[: -path�I�v�V�����ɒl���w�肳��Ă��܂���
        exit /b 1
    )
)

REM �I�v�V�����ȊO�̍ŏ��̈������v���W�F�N�g���Ƃ���
if not defined PROJNAME (
    set PROJNAME=%~1
    shift
    goto parse_args
)

REM �v���W�F�N�g���ݒ��̈����͒ǉ��\�[�X�t�@�C��
set ADDITIONAL_SOURCES=!ADDITIONAL_SOURCES! %~1
shift
goto parse_args

:check_projname
if not defined PROJNAME (
    echo �G���[: �v���W�F�N�g�����w�肵�Ă�������
    echo �g�p���@: create_vcxproj.bat [-src] [-objc] [-path gnustep_path] project_name [source_files...]
    exit /b 1
)

goto generate_main_files

:generate_main_files
if defined GEN_MAIN_SRC (
    if defined OBJC_MODE (
        echo Objective-C main.m�t�@�C���𐶐���...
        REM Objective-C�pmain.m�t�@�C�������iC����`���j
        set TMP_MAIN=!PROJNAME!_tmp.m
        (
            echo // ObjCWin32.m : Objective-C �f���Q�[�g/�r���[/�r���[�R���g���[���\���ł�Win32�A�v��
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
            echo     NSLog^(@"[AppDelegate] �A�v���P�[�V�����̋N��������"^)^;
            echo ^}
            echo - ^(void^)applicationDidFinishLaunching ^{
            echo     NSLog^(@"[AppDelegate] �A�v���P�[�V�����̋N�����������܂���"^)^;
            echo ^}
            echo - ^(void^)applicationDidBecomeActive ^{
            echo     NSLog^(@"[AppDelegate] �A�N�e�B�u�ɂȂ�܂���"^)^;
            echo ^}
            echo - ^(void^)applicationWillResignActive ^{
            echo     NSLog^(@"[AppDelegate] ��A�N�e�B�u�ɂȂ�܂���"^)^;
            echo ^}
            echo - ^(void^)applicationWillTerminate ^{
            echo     NSLog^(@"[AppDelegate] �A�v���P�[�V�������I�����܂�"^)^;
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
            echo     NSLog^(@"[MyView] �}�E�X���N���b�N����܂���: x=%%ld, y=%%ld", pt.x, pt.y^)^;
            echo     // �K�v�Ȃ炱���ōĕ`��⑼�̏���
            echo ^}
            echo @end
            echo.
            echo @implementation MyViewController
            echo - ^(void^)loadViewWithParent:^(HWND^)parent ^{
            echo     self.view = [[MyView alloc] init]^;
            echo     self.view.hWnd = parent^;
            echo     NSLog^(@"[MyViewController] �r���[���e�E�B���h�E�ɃA�^�b�`����܂���"^)^;
            echo ^}
            echo - ^(void^)handleMouseDownAt:^(POINT^)pt ^{
            echo     if ^(self.view^) ^{
            echo         [self.view handleMouseDownAt:pt]^;
            echo     ^}
            echo ^}
            echo @end
            echo.
            echo // �O���[�o���ϐ�
            echo HINSTANCE hInst^;
            echo WCHAR szWindowClass[] = L"ObjCWin32WindowClass"^;
            echo AppDelegate *appDelegate^;
            echo MyViewController *viewController^;
            echo.
            echo // �v���g�^�C�v�錾
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
            echo // �E�B���h�E�N���X�o�^
            echo ATOM MyRegisterClass^(HINSTANCE hInstance^)
            echo ^{
            echo     WNDCLASSEXW wcex^;
            echo     wcex.cbSize = sizeof^(WNDCLASSEX^)^;
            echo     wcex.style          = CS_HREDRAW ^| CS_VREDRAW^;
            echo     wcex.lpfnWndProc    = WndProc^;
            echo     wcex.cbClsExtra     = 0^;
            echo     wcex.cbWndExtra     = 0^;
            echo     wcex.hInstance      = hInstance^;
            echo     wcex.hIcon          = LoadIcon^(0, IDI_APPLICATION^)^; // �f�t�H���g�A�C�R��
            echo     wcex.hCursor        = LoadCursor^(0, IDC_ARROW^)^;
            echo     wcex.hbrBackground  = ^(HBRUSH^)^(COLOR_WINDOW+1^)^;
            echo     wcex.lpszMenuName   = 0^; // ���j���[�Ȃ�
            echo     wcex.lpszClassName  = szWindowClass^;
            echo     wcex.hIconSm        = LoadIcon^(0, IDI_APPLICATION^)^; // �f�t�H���g�A�C�R��
            echo     return RegisterClassExW^(^&wcex^)^;
            echo ^}
            echo.
            echo // �E�B���h�E����
            echo BOOL InitInstance^(HINSTANCE hInstance, int nCmdShow^)
            echo ^{
            echo    hInst = hInstance^;
            echo    LPCWSTR windowTitle = L"Hello Objective-C Window"^;
            echo    HWND hWnd = CreateWindowW^(szWindowClass, windowTitle, WS_OVERLAPPEDWINDOW,
            echo       CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 0, 0, hInstance, 0^)^;
            echo    if ^(^^!hWnd^) return FALSE^;
            echo    ShowWindow^(hWnd, nCmdShow^)^;
            echo    UpdateWindow^(hWnd^)^;
            echo    // ViewController�����EView���[�h
            echo    viewController = [[MyViewController alloc] init]^;
            echo    [viewController loadViewWithParent:hWnd]^;
            echo    return TRUE^;
            echo ^}
            echo.
            echo // �E�B���h�E�v���V�[�W��
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
                echo !PROJNAME!.m ���쐬���܂���
            ) else (
                echo �G���[: !PROJNAME!.m �̍쐬�Ɏ��s���܂���
            )
        ) else (
            echo �G���[: �ꎞ�t�@�C���̍쐬�Ɏ��s���܂���
        )
    ) else (
        echo C���� main.c�t�@�C���𐶐���...
        REM �ʏ��C����pmain.c�t�@�C������
        set TMP_MAIN=!PROJNAME!_main_tmp.c
    (
        echo #include ^<windows.h^>
        echo #include ^<stdio.h^>
        echo #include ^<versionhelpers.h^>
        echo.
        echo /*
        echo  * Win32 �R���\�[��/Windows�A�v���P�[�V�������Ή��T���v��
        echo  * SDL�`�F�b�N�����ACRT�x���}���Ή���
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
        echo         sprintf^(buf, "Windows �o�[�W����: %%d.%%d (Build %%d)", osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber^)^; 
        echo     ^} 
        echo     else 
        echo     ^{ 
        echo         sprintf^(buf, "Windows �o�[�W���������擾�ł��܂���ł���"^)^; 
        echo     ^} 
        echo.
        echo #ifdef _CONSOLE 
        echo     printf^("%%s\n", buf^)^; 
        echo     SetConsoleTitle^("Win32 �T���v���i_CONSOLE�j"^)^; 
        echo     hConsole = GetStdHandle^(STD_OUTPUT_HANDLE^)^; 
        echo     SetConsoleTextAttribute^(hConsole, FOREGROUND_RED ^| FOREGROUND_GREEN ^| FOREGROUND_INTENSITY^)^; 
        echo     printf^("\n���݂̓���:\n"^)^; 
        echo #else 
        echo     MessageBox^(NULL, buf, "Win32 �T���v���i_WINDOWS�j", MB_OK ^| MB_ICONINFORMATION^)^; 
        echo #endif 
        echo.
        echo     GetLocalTime^(^&st^)^; 
        echo.
        echo #ifdef _CONSOLE 
        echo     printf^("%%04d/%%02d/%%02d %%02d:%%02d:%%02d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond^)^; 
        echo.
        echo     drives = GetLogicalDrives^(^)^; 
        echo     printf^("\n���p�\�ȃh���C�u:\n"^)^; 
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
        echo     printf^("\n�L�[�������ďI�����Ă�������..."^)^; 
        echo     getchar^(^)^; 
        echo #else 
        echo     sprintf^(dt, "%%04d/%%02d/%%02d %%02d:%%02d:%%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond^)^; 
        echo     MessageBox^(NULL, dt, "���݂̓���", MB_OK^)^; 
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
            echo !PROJNAME!_main.c ���쐬���܂���
        ) else (
            echo �G���[: !PROJNAME!_main.c �̍쐬�Ɏ��s���܂���
        )
    ) else (
        echo �G���[: �ꎞ�t�@�C���̍쐬�Ɏ��s���܂���
    )
    )
)

REM GUID�𐶐�
for /f "delims=" %%i in ('powershell -Command "[guid]::NewGuid().ToString()"') do set GUID=%%i

REM �e���v���[�g���R�s�[
if defined OBJC_MODE (
    REM Objective-C�p�e���v���[�g���g�p�i�܂��쐬���Ă��Ȃ��̂ŁA�ʏ�e���v���[�g���x�[�X�ɓ��I�����j
    copy template.vcxproj %PROJNAME%.vcxproj > nul
) else (
    copy template.vcxproj %PROJNAME%.vcxproj > nul
)

REM �v���W�F�N�g����GUID��u��
powershell -Command "(Get-Content '%PROJNAME%.vcxproj') -replace 'TEMPLATE', '%PROJNAME%' -replace '{TEMPLATE_GUID}', '{%GUID%}' | Set-Content '%PROJNAME%.vcxproj' -Encoding UTF8"

REM Objective-C�p�̐ݒ��ǉ�
if defined OBJC_MODE (
    echo GNUStep�ݒ��K�p��...
    echo �g�p����GNUStep�p�X: %GNUSTEP_PATH%
    
    REM PowerShell�X�N���v�g���ꎞ�t�@�C���Ƃ��č쐬
    set PS_SCRIPT=%PROJNAME%_objc_setup.ps1
    (
        echo $projName = '%PROJNAME%'
        echo $gnustepPath = '%GNUSTEP_PATH%'
        echo.
        echo # �S�Ă̍\����LLVM^(clang-cl^)�c�[���Z�b�g�ɕύX���Ax64�\����Objective-C�ݒ��ǉ�
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
        echo # DLL�R�s�[�ݒ��ǉ�
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
    
    REM PowerShell�X�N���v�g�����s
    powershell -NoProfile -ExecutionPolicy Bypass -File "!PS_SCRIPT!"
    
    REM �ꎞ�t�@�C�����폜
    if exist "!PS_SCRIPT!" del "!PS_SCRIPT!"
)

REM �\�[�X�t�@�C���̃��X�g���쐬�i�X�y�[�X�ŋ�؂�j
set SOURCE_FILES=
if defined GEN_MAIN_SRC (
    if defined OBJC_MODE (
        set SOURCE_FILES=%PROJNAME%.m
    ) else (
        set SOURCE_FILES=%PROJNAME%_main.c
    )
)

REM �ǉ��̃\�[�X�t�@�C��������Βǉ�
if defined ADDITIONAL_SOURCES (
    set SOURCE_FILES=!SOURCE_FILES!!ADDITIONAL_SOURCES!
)

:create_filters
REM �\�[�X�t�@�C��������ꍇ�͒ǉ��i-src����main.c���܂ށj
if defined OBJC_MODE (
    REM Objective-C�pClCompile�iCompileAs�^�O�t���j- PowerShell�X�N���v�g�t�@�C���g�p
    set PS_ADDFILES=!PROJNAME!_addfiles.ps1
    (
        echo $files = '%SOURCE_FILES%'.Split^(' '^) ^| Where-Object {$_}
        echo if ^($files^) {
        echo     $content = Get-Content '%PROJNAME%.vcxproj'
        echo     $index = 0
        echo     for ^($i=0; $i -lt $content.Length; $i++^) {
        echo         if ^($content[$i].Contains^('�v���W�F�N�g�A�C�e��'^)^) { $index = $i }
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
    REM �ʏ��C/C++�pClCompile - PowerShell�X�N���v�g�t�@�C���g�p
    set PS_ADDFILES=!PROJNAME!_addfiles.ps1
    (
        echo $files = '%SOURCE_FILES%'.Split^(' '^) ^| Where-Object {$_}
        echo if ^($files^) {
        echo     $content = Get-Content '%PROJNAME%.vcxproj'
        echo     $index = 0
        echo     for ^($i=0; $i -lt $content.Length; $i++^) {
        echo         if ^($content[$i].Contains^('�v���W�F�N�g�A�C�e��'^)^) { $index = $i }
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

REM �t�B���^�t�@�C����PowerShell�Œ���UTF-8�ō쐬
if defined OBJC_MODE (
    REM Objective-C�p�t�B���^�i.m�g���q��ǉ��j
    powershell -NoProfile -ExecutionPolicy Bypass -Command "$filters = @(@{Name='�\�[�X �t�@�C��';Ext='cpp;c;cc;cxx;c++;cppm;ixx;def;odl;idl;hpj;bat;asm;asmx;mm;m'},@{Name='�w�b�_�[ �t�@�C��';Ext='h;hh;hpp;hxx;h++;hm;inl;inc;ipp;xsd'},@{Name='���\�[�X �t�@�C��';Ext='rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx;tiff;tif;png;wav;mfcribbon-ms'}); $content = @(); $content += '<?xml version=\"1.0\" encoding=\"utf-8\"?>'; $content += '<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">'; $content += '  <ItemGroup>'; foreach ($filter in $filters) { $content += '    <Filter Include=\"' + $filter.Name + '\">'; $content += '      <UniqueIdentifier>{' + [guid]::NewGuid() + '}</UniqueIdentifier>'; $content += '      <Extensions>' + $filter.Ext + '</Extensions>'; $content += '    </Filter>' }; $content += '  </ItemGroup>'; $sourceFiles = '%SOURCE_FILES%' -split ' ' | Where-Object { $_ }; if ($sourceFiles) { $content += '  <ItemGroup>'; foreach ($file in $sourceFiles) { $content += '    <ClCompile Include=\"' + $file + '\">'; $content += '      <Filter>�\�[�X �t�@�C��</Filter>'; $content += '    </ClCompile>' }; $content += '  </ItemGroup>' }; $content += '</Project>'; [System.IO.File]::WriteAllLines('%PROJNAME%.vcxproj.filters', $content, [System.Text.UTF8Encoding]::new($false))"
) else (
    REM �ʏ��C/C++�p�t�B���^
    powershell -NoProfile -ExecutionPolicy Bypass -Command "$filters = @(@{Name='�\�[�X �t�@�C��';Ext='cpp;c;cc;cxx;c++;cppm;ixx;def;odl;idl;hpj;bat;asm;asmx'},@{Name='�w�b�_�[ �t�@�C��';Ext='h;hh;hpp;hxx;h++;hm;inl;inc;ipp;xsd'},@{Name='���\�[�X �t�@�C��';Ext='rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx;tiff;tif;png;wav;mfcribbon-ms'}); $content = @(); $content += '<?xml version=\"1.0\" encoding=\"utf-8\"?>'; $content += '<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">'; $content += '  <ItemGroup>'; foreach ($filter in $filters) { $content += '    <Filter Include=\"' + $filter.Name + '\">'; $content += '      <UniqueIdentifier>{' + [guid]::NewGuid() + '}</UniqueIdentifier>'; $content += '      <Extensions>' + $filter.Ext + '</Extensions>'; $content += '    </Filter>' }; $content += '  </ItemGroup>'; $sourceFiles = '%SOURCE_FILES%' -split ' ' | Where-Object { $_ }; if ($sourceFiles) { $content += '  <ItemGroup>'; foreach ($file in $sourceFiles) { $content += '    <ClCompile Include=\"' + $file + '\">'; $content += '      <Filter>�\�[�X �t�@�C��</Filter>'; $content += '    </ClCompile>' }; $content += '  </ItemGroup>' }; $content += '</Project>'; [System.IO.File]::WriteAllLines('%PROJNAME%.vcxproj.filters', $content, [System.Text.UTF8Encoding]::new($false))"
)


REM �S�\���r���h�o�b�`�������iMBCS/UNICODE�Ex86/x64�EDebug/Release�EWINDOWS/CONSOLE�j
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
    echo echo === ���ׂĂ̍\���̃r���h���������܂��� ===
) > %PROJNAME%_build_all.bat

echo.
if defined OBJC_MODE (
    echo Objective-C �v���W�F�N�g %PROJNAME% ���쐬���܂���
    echo GNUStep�p�X: %GNUSTEP_PATH%
) else (
    echo �v���W�F�N�g %PROJNAME% ���쐬���܂���
)
goto :eof

:clean_project
REM �v���W�F�N�g���������Ŏ󂯎��
if "%~1"=="" (
    echo �v���W�F�N�g�����w�肵�Ă�������
    echo �g�p�@: create_vcxproj.bat -clean �v���W�F�N�g��
    exit /b 1
)
set PROJNAME=%~1

echo %PROJNAME% �v���W�F�N�g�̑S�t�@�C���E�t�H���_���폜���Ă��܂�...

REM �v���W�F�N�g�t�@�C���폜
if exist "%PROJNAME%.vcxproj" (
    del "%PROJNAME%.vcxproj" >nul 2>&1
    echo �폜: %PROJNAME%.vcxproj
)
if exist "%PROJNAME%.vcxproj.filters" (
    del "%PROJNAME%.vcxproj.filters" >nul 2>&1
    echo �폜: %PROJNAME%.vcxproj.filters
)
if exist "%PROJNAME%.vcxproj.user" (
    del "%PROJNAME%.vcxproj.user" >nul 2>&1
    echo �폜: %PROJNAME%.vcxproj.user
)

REM �r���h�o�b�`�t�@�C���폜
if exist "%PROJNAME%_build_all.bat" (
    del "%PROJNAME%_build_all.bat" >nul 2>&1
    echo �폜: %PROJNAME%_build_all.bat
)

REM main.c�t�@�C���폜
if exist "%PROJNAME%_main.c" (
    del "%PROJNAME%_main.c" >nul 2>&1
    echo �폜: %PROJNAME%_main.c
)
if exist "%PROJNAME%.m" (
    del "%PROJNAME%.m" >nul 2>&1
    echo �폜: %PROJNAME%.m
)
if exist "%PROJNAME%_main.m" (
    del "%PROJNAME%_main.m" >nul 2>&1
    echo �폜: %PROJNAME%_main.m
)

REM �r���h�o�̓t�H���_�폜�iWin32�\���j
if exist "Debug MBCS" (
    rmdir /s /q "Debug MBCS" >nul 2>&1
    echo �폜: Debug MBCS �t�H���_
)
if exist "Release MBCS" (
    rmdir /s /q "Release MBCS" >nul 2>&1
    echo �폜: Release MBCS �t�H���_
)
if exist "Debug Unicode" (
    rmdir /s /q "Debug Unicode" >nul 2>&1
    echo �폜: Debug Unicode �t�H���_
)
if exist "Release Unicode" (
    rmdir /s /q "Release Unicode" >nul 2>&1
    echo �폜: Release Unicode �t�H���_
)

REM x64�r���h�o�̓t�H���_�폜
if exist "x64" (
    rmdir /s /q "x64" >nul 2>&1
    echo �폜: x64 �t�H���_
)

REM �v���W�F�N�g���t���r���h�o�̓t�H���_�폜�iWin32�\���j
if exist "%PROJNAME%\Debug MBCS" (
    rmdir /s /q "%PROJNAME%\Debug MBCS" >nul 2>&1
    echo �폜: %PROJNAME%\Debug MBCS �t�H���_
)
if exist "%PROJNAME%\Release MBCS" (
    rmdir /s /q "%PROJNAME%\Release MBCS" >nul 2>&1
    echo �폜: %PROJNAME%\Release MBCS �t�H���_
)
if exist "%PROJNAME%\Debug Unicode" (
    rmdir /s /q "%PROJNAME%\Debug Unicode" >nul 2>&1
    echo �폜: %PROJNAME%\Debug Unicode �t�H���_
)
if exist "%PROJNAME%\Release Unicode" (
    rmdir /s /q "%PROJNAME%\Release Unicode" >nul 2>&1
    echo �폜: %PROJNAME%\Release Unicode �t�H���_
)

REM �v���W�F�N�g���t��x64�r���h�o�̓t�H���_�폜
if exist "%PROJNAME%\x64" (
    rmdir /s /q "%PROJNAME%\x64" >nul 2>&1
    echo �폜: %PROJNAME%\x64 �t�H���_
)

REM �v���W�F�N�g���t�H���_����̏ꍇ�͍폜
if exist "%PROJNAME%" (
    rmdir "%PROJNAME%" >nul 2>&1
    if not exist "%PROJNAME%" (
        echo �폜: %PROJNAME% �t�H���_
    )
)

REM Visual Studio�̈ꎞ�t�@�C���폜
if exist "*.pdb" (
    del "*.pdb" >nul 2>&1
    echo �폜: PDB�t�@�C��
)
if exist "*.ilk" (
    del "*.ilk" >nul 2>&1
    echo �폜: ILK�t�@�C��
)
if exist "*.obj" (
    del "*.obj" >nul 2>&1
    echo �폜: OBJ�t�@�C��
)

echo.
echo === %PROJNAME% �v���W�F�N�g�̃N���[���A�b�v���������܂��� ===

goto :eof
