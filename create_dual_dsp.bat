setlocal enabledelayedexpansion

REM -src�I�v�V�����Ή�: �v���W�F�N�g��_main.c����������
set GEN_MAIN_SRC=
if /i "%~1"=="-src" (
    set GEN_MAIN_SRC=1
    shift
)

REM �v���W�F�N�g���������Ŏ󂯎��
if "%~1"=="" (
    echo �v���W�F�N�g�����w�肵�Ă�������
    exit /b 1
)
set PROJNAME=%~1

if defined GEN_MAIN_SRC (
    echo #include ^<windows.h^> > "!PROJNAME!_main.c"
    echo #include ^<stdio.h^> >> "!PROJNAME!_main.c"
    echo. >> "!PROJNAME!_main.c"
    echo /* >> "!PROJNAME!_main.c"
    echo  * Win32 �R���\�[��/Windows�A�v���P�[�V�������Ή��T���v�� >> "!PROJNAME!_main.c"
    echo  */ >> "!PROJNAME!_main.c"
    echo void sample_main^(void^); >> "!PROJNAME!_main.c"
    echo. >> "!PROJNAME!_main.c"
    echo void sample_main^(void^) >> "!PROJNAME!_main.c"
    echo { >> "!PROJNAME!_main.c"
    echo     DWORD dwVersion; >> "!PROJNAME!_main.c"
    echo     DWORD dwWindowsMajorVersion; >> "!PROJNAME!_main.c"
    echo     DWORD dwWindowsMinorVersion; >> "!PROJNAME!_main.c"
    echo     DWORD dwBuild; >> "!PROJNAME!_main.c"
    echo     char buf[256]; >> "!PROJNAME!_main.c"
    echo     HANDLE hConsole; >> "!PROJNAME!_main.c"
    echo     SYSTEMTIME st; >> "!PROJNAME!_main.c"
    echo     DWORD drives; >> "!PROJNAME!_main.c"
    echo     int i; >> "!PROJNAME!_main.c"
    echo     char driveLetter; >> "!PROJNAME!_main.c"
    echo #ifdef _WINDOWS >> "!PROJNAME!_main.c"
    echo     char dt[64]; >> "!PROJNAME!_main.c"
    echo #endif >> "!PROJNAME!_main.c"
    echo. >> "!PROJNAME!_main.c"
    echo     dwVersion = GetVersion^(^); >> "!PROJNAME!_main.c"
    echo     dwWindowsMajorVersion = ^(DWORD^)^(LOBYTE^(LOWORD^(dwVersion^)^)^); >> "!PROJNAME!_main.c"
    echo     dwWindowsMinorVersion = ^(DWORD^)^(HIBYTE^(LOWORD^(dwVersion^)^)^); >> "!PROJNAME!_main.c"
    echo     dwBuild = 0; >> "!PROJNAME!_main.c"
    echo. >> "!PROJNAME!_main.c"
    echo     if ^(dwVersion ^< 0x80000000^) >> "!PROJNAME!_main.c"
    echo     { >> "!PROJNAME!_main.c"
    echo         dwBuild = ^(DWORD^)^(HIWORD^(dwVersion^)^); >> "!PROJNAME!_main.c"
    echo     } >> "!PROJNAME!_main.c"
    echo. >> "!PROJNAME!_main.c"
    echo     sprintf^(buf, "Windows �o�[�W����: %%d.%%d ^(Build %%d^)\n", >> "!PROJNAME!_main.c"
    echo             dwWindowsMajorVersion, dwWindowsMinorVersion, dwBuild^); >> "!PROJNAME!_main.c"
    echo. >> "!PROJNAME!_main.c"
    echo #ifdef _CONSOLE >> "!PROJNAME!_main.c"
    echo     printf^("%%s", buf^); >> "!PROJNAME!_main.c"
    echo     SetConsoleTitle^("Win32 �T���v��^�i_CONSOLE^�j"^); >> "!PROJNAME!_main.c"
    echo     hConsole = GetStdHandle^(STD_OUTPUT_HANDLE^); >> "!PROJNAME!_main.c"
    echo     SetConsoleTextAttribute^(hConsole, FOREGROUND_RED ^| FOREGROUND_GREEN ^| FOREGROUND_INTENSITY^); >> "!PROJNAME!_main.c"
    echo     printf^("\n���݂̓���:\n"^); >> "!PROJNAME!_main.c"
    echo #else >> "!PROJNAME!_main.c"
    echo     MessageBox^(NULL, buf, "Win32 �T���v��^�i_WINDOWS^�j", MB_OK ^| MB_ICONINFORMATION^); >> "!PROJNAME!_main.c"
    echo #endif >> "!PROJNAME!_main.c"
    echo. >> "!PROJNAME!_main.c"
    echo     GetLocalTime^(^&st^); >> "!PROJNAME!_main.c"
    echo. >> "!PROJNAME!_main.c"
    echo #ifdef _CONSOLE >> "!PROJNAME!_main.c"
    echo     printf^("%%04d/%%02d/%%02d %%02d:%%02d:%%02d\n", >> "!PROJNAME!_main.c"
    echo             st.wYear, st.wMonth, st.wDay, >> "!PROJNAME!_main.c"
    echo             st.wHour, st.wMinute, st.wSecond^); >> "!PROJNAME!_main.c"
    echo. >> "!PROJNAME!_main.c"
    echo     drives = GetLogicalDrives^(^); >> "!PROJNAME!_main.c"
    echo     printf^("\n���p�\�ȃh���C�u:\n"^); >> "!PROJNAME!_main.c"
    echo. >> "!PROJNAME!_main.c"
    echo     for ^(i = 0; i ^< 26; i++^) >> "!PROJNAME!_main.c"
    echo     { >> "!PROJNAME!_main.c"
    echo         int mask = drives ^& ^(1 ^<^< i^); >> "!PROJNAME!_main.c"
    echo         if ^(mask != 0^) >> "!PROJNAME!_main.c"
    echo         { >> "!PROJNAME!_main.c"
    echo             driveLetter = 'A' + i; >> "!PROJNAME!_main.c"
    echo             printf^("%%c:\\\n", driveLetter^); >> "!PROJNAME!_main.c"
    echo         } >> "!PROJNAME!_main.c"
    echo     } >> "!PROJNAME!_main.c"
    echo. >> "!PROJNAME!_main.c"
    echo     printf^("\n�L�[�������ďI�����Ă�������..."^); >> "!PROJNAME!_main.c"
    echo     getchar^(^); >> "!PROJNAME!_main.c"
    echo #else >> "!PROJNAME!_main.c"
    echo     sprintf^(dt, "%%04d/%%02d/%%02d %%02d:%%02d:%%02d", >> "!PROJNAME!_main.c"
    echo             st.wYear, st.wMonth, st.wDay, >> "!PROJNAME!_main.c"
    echo             st.wHour, st.wMinute, st.wSecond^); >> "!PROJNAME!_main.c"
    echo     MessageBox^(NULL, dt, "���݂̓���", MB_OK^); >> "!PROJNAME!_main.c"
    echo #endif >> "!PROJNAME!_main.c"
    echo } >> "!PROJNAME!_main.c"
    echo. >> "!PROJNAME!_main.c"
    echo #ifdef _CONSOLE >> "!PROJNAME!_main.c"
    echo int main^(int argc, char **argv^) >> "!PROJNAME!_main.c"
    echo { >> "!PROJNAME!_main.c"
    echo     sample_main^(^); >> "!PROJNAME!_main.c"
    echo     return 0; >> "!PROJNAME!_main.c"
    echo } >> "!PROJNAME!_main.c"
    echo #elif defined^(_WINDOWS^) >> "!PROJNAME!_main.c"
    echo int WINAPI WinMain^(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow^) >> "!PROJNAME!_main.c"
    echo { >> "!PROJNAME!_main.c"
    echo     sample_main^(^); >> "!PROJNAME!_main.c"
    echo     return 0; >> "!PROJNAME!_main.c"
    echo } >> "!PROJNAME!_main.c"
    echo #endif >> "!PROJNAME!_main.c"
)

REM Windows�pDSP�t�@�C���쐬
copy win_template.dsp win_%PROJNAME%.dsp > nul
powershell -Command "(Get-Content win_%PROJNAME%.dsp) -replace 'TEMPLATE', '%PROJNAME%' | Set-Content win_%PROJNAME%.dsp -Encoding Default"

REM Windows�pDSP�t�@�C���Ƀ��C���\�[�X�t�@�C����ǉ�
if defined GEN_MAIN_SRC (
    powershell -NoProfile -ExecutionPolicy Bypass -Command " $content = Get-Content -Path 'win_%PROJNAME%.dsp'; $sourceGroupStart = -1; $sourceGroupEnd = -1; for ($i = 0; $i -lt $content.Length; $i++) { if ($content[$i] -match '# Begin Group \"Source Files\"') { $sourceGroupStart = $i; } if ($sourceGroupStart -ne -1 -and $content[$i] -match '# End Group' -and $sourceGroupEnd -eq -1) { $sourceGroupEnd = $i; break; } }; if ($sourceGroupStart -ne -1 -and $sourceGroupEnd -ne -1) { $before = @($content[0..($sourceGroupEnd - 1)]); if (-not ($before -match 'SOURCE=.\\%PROJNAME%_main.c')) { $after = @($content[$sourceGroupEnd..($content.Length - 1)]); $insert = @('# Begin Source File', '', 'SOURCE=.\%PROJNAME%_main.c', '', '# End Source File'); ($before + $insert + $after) | Set-Content -Path 'win_%PROJNAME%.dsp' -Encoding Default } } "
)
echo win_%PROJNAME%.dsp ���쐬���܂���

REM Console�pDSP�t�@�C���쐬
copy con_template.dsp con_%PROJNAME%.dsp > nul
powershell -Command "(Get-Content con_%PROJNAME%.dsp) -replace 'TEMPLATE', '%PROJNAME%' | Set-Content con_%PROJNAME%.dsp -Encoding Default"

REM Console�pDSP�t�@�C���Ƀ��C���\�[�X�t�@�C����ǉ�
if defined GEN_MAIN_SRC (
    powershell -NoProfile -ExecutionPolicy Bypass -Command " $content = Get-Content -Path 'con_%PROJNAME%.dsp'; $sourceGroupStart = -1; $sourceGroupEnd = -1; for ($i = 0; $i -lt $content.Length; $i++) { if ($content[$i] -match '# Begin Group \"Source Files\"') { $sourceGroupStart = $i; } if ($sourceGroupStart -ne -1 -and $content[$i] -match '# End Group' -and $sourceGroupEnd -eq -1) { $sourceGroupEnd = $i; break; } }; if ($sourceGroupStart -ne -1 -and $sourceGroupEnd -ne -1) { $before = @($content[0..($sourceGroupEnd - 1)]); if (-not ($before -match 'SOURCE=.\\%PROJNAME%_main.c')) { $after = @($content[$sourceGroupEnd..($content.Length - 1)]); $insert = @('# Begin Source File', '', 'SOURCE=.\%PROJNAME%_main.c', '', '# End Source File'); ($before + $insert + $after) | Set-Content -Path 'con_%PROJNAME%.dsp' -Encoding Default } } "
)
echo con_%PROJNAME%.dsp ���쐬���܂���

REM DSW�t�@�C�������������i���������̂܂܁A2��DSP���܂߂�j
(
    echo Microsoft Developer Studio Workspace File, Format Version 6.00
    echo # WARNING: DO NOT EDIT OR DELETE THIS WORKSPACE FILE!
    echo.
    echo ###############################################################################
    echo.
    echo Project: "win_%PROJNAME%"=".\win_%PROJNAME%.dsp" - Package Owner=^<4^>
    echo.
    echo Package=^<5^>
    echo {{{
    echo }}}
    echo.
    echo Package=^<4^>
    echo {{{
    echo }}}
    echo.
    echo ###############################################################################
    echo.
    echo Project: "con_%PROJNAME%"=".\con_%PROJNAME%.dsp" - Package Owner=^<4^>
    echo.
    echo Package=^<5^>
    echo {{{
    echo }}}
    echo.
    echo Package=^<4^>
    echo {{{
    echo }}}
    echo.
    echo ###############################################################################
    echo.
    echo Global:
    echo.
    echo Package=^<5^>
    echo {{{
    echo }}}
    echo.
    echo Package=^<3^>
    echo {{{
    echo }}}
    echo.
    echo ###############################################################################
) > %PROJNAME%.dsw
echo %PROJNAME%.dsw ���쐬���܂���

REM �r���h�o�b�`�t�@�C���𐶐�
(
    echo @echo off
    echo set MSDEV=msdev.exe
    echo echo --- �r���h���J�n���܂� ---
    echo %%MSDEV%% "%PROJNAME%.dsw" /make "win_%PROJNAME% - Win32 Release" /rebuild
    echo if errorlevel 1 goto error
    echo %%MSDEV%% "%PROJNAME%.dsw" /make "win_%PROJNAME% - Win32 Debug" /rebuild
    echo if errorlevel 1 goto error
    echo %%MSDEV%% "%PROJNAME%.dsw" /make "con_%PROJNAME% - Win32 Release" /rebuild
    echo if errorlevel 1 goto error
    echo %%MSDEV%% "%PROJNAME%.dsw" /make "con_%PROJNAME% - Win32 Debug" /rebuild
    echo if errorlevel 1 goto error
    echo echo --- �r���h������Ɋ������܂��� ---
    echo exit /b 0
    echo :error
    echo echo --- �r���h�G���[���������܂��� ---
    echo exit /b 1
) > build_%PROJNAME%.bat
echo build_%PROJNAME%.bat ���쐬���܂���

echo �������܂����B
