/* create_dual_dsp.bat の main.c 出力例と同じ内容 */
static const char main_c_literal[] =
    "#include <windows.h>\r\n"
    "#include <stdio.h>\r\n"
    "\r\n"
    "/*\r\n"
    " * Win32 コンソール/Windowsアプリケーション両対応サンプル\r\n"
    " */\r\n"
    "void sample_main(void);\r\n"
    "\r\n"
    "void sample_main(void)\r\n"
    "{\r\n"
    "    DWORD dwVersion;\r\n"
    "    DWORD dwWindowsMajorVersion;\r\n"
    "    DWORD dwWindowsMinorVersion;\r\n"
    "    DWORD dwBuild;\r\n"
    "    char buf[256];\r\n"
    "    HANDLE hConsole;\r\n"
    "    SYSTEMTIME st;\r\n"
    "    DWORD drives;\r\n"
    "    int i;\r\n"
    "    char driveLetter;\r\n"
    "#ifdef _WINDOWS\r\n"
    "    char dt[64];\r\n"
    "#endif\r\n"
    "\r\n"
    "    dwVersion = GetVersion();\r\n"
    "    dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));\r\n"
    "    dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));\r\n"
    "    dwBuild = 0;\r\n"
    "\r\n"
    "    if (dwVersion < 0x80000000)\r\n"
    "    {\r\n"
    "        dwBuild = (DWORD)(HIWORD(dwVersion));\r\n"
    "    }\r\n"
    "\r\n"
    "    sprintf(buf, \"Windows バージョン: %d.%d (Build %d)\\n\",\r\n"
    "            dwWindowsMajorVersion, dwWindowsMinorVersion, dwBuild);\r\n"
    "\r\n"
    "#ifdef _CONSOLE\r\n"
    "    printf(\"%s\", buf);\r\n"
    "    SetConsoleTitle(\"Win32 サンプル（_CONSOLE）\");\r\n"
    "    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);\r\n"
    "    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);\r\n"
    "    printf(\"\\n現在の日時:\\n\");\r\n"
    "#else\r\n"
    "    MessageBox(NULL, buf, \"Win32 サンプル（_WINDOWS）\", MB_OK | MB_ICONINFORMATION);\r\n"
    "#endif\r\n"
    "\r\n"
    "    GetLocalTime(&st);\r\n"
    "\r\n"
    "#ifdef _CONSOLE\r\n"
    "    printf(\"%04d/%02d/%02d %02d:%02d:%02d\\n\",\r\n"
    "            st.wYear, st.wMonth, st.wDay,\r\n"
    "            st.wHour, st.wMinute, st.wSecond);\r\n"
    "\r\n"
    "    drives = GetLogicalDrives();\r\n"
    "    printf(\"\\n利用可能なドライブ:\\n\");\r\n"
    "\r\n"
    "    for (i = 0; i < 26; i++)\r\n"
    "    {\r\n"
    "        int mask = drives & (1 << i);\r\n"
    "        if (mask != 0)\r\n"
    "        {\r\n"
    "            driveLetter = 'A' + i;\r\n"
    "            printf(\"%c:\\\\n\", driveLetter);\r\n"
    "        }\r\n"
    "    }\r\n"
    "\r\n"
    "    printf(\"\\nキーを押して終了してください...\");\r\n"
    "    getchar();\r\n"
    "#else\r\n"
    "    sprintf(dt, \"%04d/%02d/%02d %02d:%02d:%02d\",\r\n"
    "            st.wYear, st.wMonth, st.wDay,\r\n"
    "            st.wHour, st.wMinute, st.wSecond);\r\n"
    "    MessageBox(NULL, dt, \"現在の日時\", MB_OK);\r\n"
    "#endif\r\n"
    "}\r\n"
    "\r\n"
    "#ifdef _CONSOLE\r\n"
    "int main(int argc, char **argv)\r\n"
    "{\r\n"
    "    sample_main();\r\n"
    "    return 0;\r\n"
    "}\r\n"
    "#elif defined(_WINDOWS)\r\n"
    "int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow)\r\n"
    "{\r\n"
    "    sample_main();\r\n"
    "    return 0;\r\n"
    "}\r\n"
    "#endif\r\n";
#include <stdio.h>
/*
 * gen.c - VC++6 プロジェクト生成の最小実装 (MVP)
 * 機能: gen [-src] <プロジェクト名>
 * 出力: win_<proj>.dsp, con_<proj>.dsp, <proj>.dsw, build_<proj>.bat
 * -src 指定時は <proj>_main.c を生成
 *
 * TCC でコンパイルできるよう依存を最小限にしています。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include <ctype.h>

/* 埋め込みテンプレート: win_template.dsp の完全な内容 */
static const char win_template_literal[] =
    "# Microsoft Developer Studio Project File - Name=\"win_TEMPLATE\" - Package Owner=<4>\r\n"
    "# Microsoft Developer Studio Generated Build File, Format Version 6.00\r\n"
    "# ** 編集しないでください **\r\n"
    "\r\n"
    "# TARGTYPE \"Win32 (x86) Application\" 0x0101\r\n"
    "\r\n"
    "CFG=win_TEMPLATE - Win32 Debug\r\n"
    "!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。\r\n"
    "!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください\r\n"
    "!MESSAGE \r\n"
    "!MESSAGE NMAKE /f \"win_TEMPLATE.mak\".\r\n"
    "!MESSAGE \r\n"
    "!MESSAGE NMAKE の実行時に構成を指定できます\r\n"
    "!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:\r\n"
    "!MESSAGE \r\n"
    "!MESSAGE NMAKE /f \"win_TEMPLATE.mak\" CFG=\"win_TEMPLATE - Win32 Debug\"\r\n"
    "!MESSAGE \r\n"
    "!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:\r\n"
    "!MESSAGE \r\n"
    "!MESSAGE \"win_TEMPLATE - Win32 Release\" (\"Win32 (x86) Application\" 用)\r\n"
    "!MESSAGE \"win_TEMPLATE - Win32 Debug\" (\"Win32 (x86) Application\" 用)\r\n"
    "!MESSAGE \"win_TEMPLATE - Win32 Unicode Release\" (\"Win32 (x86) Application\" 用)\r\n"
    "!MESSAGE \"win_TEMPLATE - Win32 Unicode Debug\" (\"Win32 (x86) Application\" 用)\r\n"
    "!MESSAGE \r\n"
    "\r\n"
    "# Begin Project\r\n"
    "# PROP AllowPerConfigDependencies 0\r\n"
    "# PROP Scc_ProjName \"\"\r\n"
    "# PROP Scc_LocalPath \"\"\r\n"
    "CPP=cl.exe\r\n"
    "MTL=midl.exe\r\n"
    "RSC=rc.exe\r\n"
    "\r\n"
    "!IF  \"$(CFG)\" == \"win_TEMPLATE - Win32 Release\"\r\n"
    "\r\n"
    "# PROP BASE Use_MFC 0\r\n"
    "# PROP BASE Use_Debug_Libraries 0\r\n"
    "# PROP BASE Output_Dir \"Release\"\r\n"
    "# PROP BASE Intermediate_Dir \"Release\"\r\n"
    "# PROP BASE Target_Dir \"\"\r\n"
    "# PROP Use_MFC 0\r\n"
    "# PROP Use_Debug_Libraries 0\r\n"
    "# PROP Output_Dir \"Release\"\r\n"
    "# PROP Intermediate_Dir \"Release\"\r\n"
    "# PROP Target_Dir \"\"\r\n"
    "# ADD BASE CPP /nologo /W3 /GX /O2 /D \"WIN32\" /D \"NDEBUG\" /D \"_WINDOWS\" /D \"_MBCS\" /YX /FD /c\r\n"
    "# ADD CPP /nologo /W3 /GX /O2 /D \"WIN32\" /D \"NDEBUG\" /D \"_WINDOWS\" /D \"_MBCS\" /YX /FD /c\r\n"
    "# ADD BASE MTL /nologo /D \"NDEBUG\" /mktyplib203 /win32\r\n"
    "# ADD MTL /nologo /D \"NDEBUG\" /mktyplib203 /win32\r\n"
    "# ADD BASE RSC /l 0x411 /d \"NDEBUG\"\r\n"
    "# ADD RSC /l 0x411 /d \"NDEBUG\"\r\n"
    "BSC32=bscmake.exe\r\n"
    "# ADD BASE BSC32 /nologo\r\n"
    "# ADD BSC32 /nologo\r\n"
    "LINK32=link.exe\r\n"
    "# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386\r\n"
    "# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386\r\n"
    "\r\n"
    "!ELSEIF  \"$(CFG)\" == \"win_TEMPLATE - Win32 Debug\"\r\n"
    "\r\n"
    "# PROP BASE Use_MFC 0\r\n"
    "# PROP BASE Use_Debug_Libraries 1\r\n"
    "# PROP BASE Output_Dir \"Debug\"\r\n"
    "# PROP BASE Intermediate_Dir \"Debug\"\r\n"
    "# PROP BASE Target_Dir \"\"\r\n"
    "# PROP Use_MFC 0\r\n"
    "# PROP Use_Debug_Libraries 1\r\n"
    "# PROP Output_Dir \"Debug\"\r\n"
    "# PROP Intermediate_Dir \"Debug\"\r\n"
    "# PROP Target_Dir \"\"\r\n"
    "# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_WINDOWS\" /D \"_MBCS\" /YX /FD /GZ /c\r\n"
    "# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_WINDOWS\" /D \"_MBCS\" /YX /FD /GZ /c\r\n"
    "# ADD BASE MTL /nologo /D \"_DEBUG\" /mktyplib203 /win32\r\n"
    "# ADD MTL /nologo /D \"_DEBUG\" /mktyplib203 /win32\r\n"
    "# ADD BASE RSC /l 0x411 /d \"_DEBUG\"\r\n"
    "# ADD RSC /l 0x411 /d \"_DEBUG\"\r\n"
    "BSC32=bscmake.exe\r\n"
    "# ADD BASE BSC32 /nologo\r\n"
    "# ADD BSC32 /nologo\r\n"
    "LINK32=link.exe\r\n"
    "# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept\r\n"
    "# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept\r\n"
    "\r\n"
    "!ELSEIF  \"$(CFG)\" == \"win_TEMPLATE - Win32 Unicode Release\"\r\n"
    "\r\n"
    "# PROP BASE Use_MFC 0\r\n"
    "# PROP BASE Use_Debug_Libraries 0\r\n"
    "# PROP BASE Output_Dir \"URelease\"\r\n"
    "# PROP BASE Intermediate_Dir \"URelease\"\r\n"
    "# PROP BASE Target_Dir \"\"\r\n"
    "# PROP Use_MFC 0\r\n"
    "# PROP Use_Debug_Libraries 0\r\n"
    "# PROP Output_Dir \"URelease\"\r\n"
    "# PROP Intermediate_Dir \"URelease\"\r\n"
    "# PROP Target_Dir \"\"\r\n"
    "# ADD BASE CPP /nologo /W3 /GX /O2 /D \"WIN32\" /D \"NDEBUG\" /D \"_WINDOWS\" /D \"UNICODE\" /D \"_UNICODE\" /YX /FD /c\r\n"
    "# ADD CPP /nologo /W3 /GX /O2 /D \"WIN32\" /D \"NDEBUG\" /D \"_WINDOWS\" /D \"UNICODE\" /D \"_UNICODE\" /YX /FD /c\r\n"
    "# ADD BASE MTL /nologo /D \"NDEBUG\" /mktyplib203 /win32\r\n"
    "# ADD MTL /nologo /D \"NDEBUG\" /mktyplib203 /win32\r\n"
    "# ADD BASE RSC /l 0x411 /d \"NDEBUG\"\r\n"
    "# ADD RSC /l 0x411 /d \"NDEBUG\"\r\n"
    "BSC32=bscmake.exe\r\n"
    "# ADD BASE BSC32 /nologo\r\n"
    "# ADD BSC32 /nologo\r\n"
    "LINK32=link.exe\r\n"
    "# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386\r\n"
    "# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386\r\n"
    "\r\n"
    "!ELSEIF  \"$(CFG)\" == \"win_TEMPLATE - Win32 Unicode Debug\"\r\n"
    "\r\n"
    "# PROP BASE Use_MFC 0\r\n"
    "# PROP BASE Use_Debug_Libraries 1\r\n"
    "# PROP BASE Output_Dir \"UDebug\"\r\n"
    "# PROP BASE Intermediate_Dir \"UDebug\"\r\n"
    "# PROP BASE Target_Dir \"\"\r\n"
    "# PROP Use_MFC 0\r\n"
    "# PROP Use_Debug_Libraries 1\r\n"
    "# PROP Output_Dir \"UDebug\"\r\n"
    "# PROP Intermediate_Dir \"UDebug\"\r\n"
    "# PROP Target_Dir \"\"\r\n"
    "# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_WINDOWS\" /D \"UNICODE\" /D \"_UNICODE\" /YX /FD /GZ /c\r\n"
    "# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_WINDOWS\" /D \"UNICODE\" /D \"_UNICODE\" /YX /FD /GZ /c\r\n"
    "# ADD BASE MTL /nologo /D \"_DEBUG\" /mktyplib203 /win32\r\n"
    "# ADD MTL /nologo /D \"_DEBUG\" /mktyplib203 /win32\r\n"
    "# ADD BASE RSC /l 0x411 /d \"_DEBUG\"\r\n"
    "# ADD RSC /l 0x411 /d \"_DEBUG\"\r\n"
    "BSC32=bscmake.exe\r\n"
    "# ADD BASE BSC32 /nologo\r\n"
    "# ADD BSC32 /nologo\r\n"
    "LINK32=link.exe\r\n"
    "# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept\r\n"
    "# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept\r\n"
    "\r\n"
    "!ENDIF \r\n"
    "\r\n"
    "# Begin Target\r\n"
    "# Name \"win_TEMPLATE - Win32 Release\"\r\n"
    "# Name \"win_TEMPLATE - Win32 Debug\"\r\n"
    "# Name \"win_TEMPLATE - Win32 Unicode Release\"\r\n"
    "# Name \"win_TEMPLATE - Win32 Unicode Debug\"\r\n"
    "# Begin Group \"Source Files\"\r\n"
    "# PROP Default_Filter \"cpp;c;cxx;rc;def;r;odl;idl;hpj;bat\"\r\n"
    "# End Group\r\n"
    "# Begin Group \"Header Files\"\r\n"
    "# PROP Default_Filter \"h;hpp;hxx;hm;inl\"\r\n"
    "# End Group\r\n"
    "# Begin Group \"Resource Files\"\r\n"
    "# PROP Default_Filter \"ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe\"\r\n"
    "# End Group\r\n"
    "# End Target\r\n"
    "# End Project\r\n";

/* 埋め込みテンプレート: con_template.dsp の完全な内容 */
static const char con_template_literal[] =
    "# Microsoft Developer Studio Project File - Name=\"con_TEMPLATE\" - Package Owner=<4>\r\n"
    "# Microsoft Developer Studio Generated Build File, Format Version 6.00\r\n"
    "# ** 編集しないでください **\r\n"
    "\r\n"
    "# TARGTYPE \"Win32 (x86) Console Application\" 0x0103\r\n"
    "\r\n"
    "CFG=con_TEMPLATE - Win32 Debug\r\n"
    "!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。\r\n"
    "!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください\r\n"
    "!MESSAGE \r\n"
    "!MESSAGE NMAKE /f \"con_TEMPLATE.mak\".\r\n"
    "!MESSAGE \r\n"
    "!MESSAGE NMAKE の実行時に構成を指定できます\r\n"
    "!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:\r\n"
    "!MESSAGE \r\n"
    "!MESSAGE NMAKE /f \"con_TEMPLATE.mak\" CFG=\"con_TEMPLATE - Win32 Debug\"\r\n"
    "!MESSAGE \r\n"
    "!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:\r\n"
    "!MESSAGE \r\n"
    "!MESSAGE \"con_TEMPLATE - Win32 Release\" (\"Win32 (x86) Console Application\" 用)\r\n"
    "!MESSAGE \"con_TEMPLATE - Win32 Debug\" (\"Win32 (x86) Console Application\" 用)\r\n"
    "!MESSAGE \"con_TEMPLATE - Win32 Unicode Release\" (\"Win32 (x86) Console Application\" 用)\r\n"
    "!MESSAGE \"con_TEMPLATE - Win32 Unicode Debug\" (\"Win32 (x86) Console Application\" 用)\r\n"
    "!MESSAGE \r\n"
    "\r\n"
    "# Begin Project\r\n"
    "# PROP AllowPerConfigDependencies 0\r\n"
    "# PROP Scc_ProjName \"\"\r\n"
    "# PROP Scc_LocalPath \"\"\r\n"
    "CPP=cl.exe\r\n"
    "MTL=midl.exe\r\n"
    "RSC=rc.exe\r\n"
    "\r\n"
    "!IF  \"$(CFG)\" == \"con_TEMPLATE - Win32 Release\"\r\n"
    "\r\n"
    "# PROP BASE Use_MFC 0\r\n"
    "# PROP BASE Use_Debug_Libraries 0\r\n"
    "# PROP BASE Output_Dir \"Release\"\r\n"
    "# PROP BASE Intermediate_Dir \"Release\"\r\n"
    "# PROP BASE Target_Dir \"\"\r\n"
    "# PROP Use_MFC 0\r\n"
    "# PROP Use_Debug_Libraries 0\r\n"
    "# PROP Output_Dir \"Release\"\r\n"
    "# PROP Intermediate_Dir \"Release\"\r\n"
    "# PROP Target_Dir \"\"\r\n"
    "# ADD BASE CPP /nologo /W3 /GX /O2 /D \"WIN32\" /D \"NDEBUG\" /D \"_CONSOLE\" /D \"_MBCS\" /YX /FD /c\r\n"
    "# ADD CPP /nologo /W3 /GX /O2 /D \"WIN32\" /D \"NDEBUG\" /D \"_CONSOLE\" /D \"_MBCS\" /YX /FD /c\r\n"
    "# ADD BASE MTL /nologo /D \"NDEBUG\" /mktyplib203 /win32\r\n"
    "# ADD MTL /nologo /D \"NDEBUG\" /mktyplib203 /win32\r\n"
    "# ADD BASE RSC /l 0x411 /d \"NDEBUG\"\r\n"
    "# ADD RSC /l 0x411 /d \"NDEBUG\"\r\n"
    "BSC32=bscmake.exe\r\n"
    "# ADD BASE BSC32 /nologo\r\n"
    "# ADD BSC32 /nologo\r\n"
    "LINK32=link.exe\r\n"
    "# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386\r\n"
    "# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386\r\n"
    "\r\n"
    "!ELSEIF  \"$(CFG)\" == \"con_TEMPLATE - Win32 Debug\"\r\n"
    "\r\n"
    "# PROP BASE Use_MFC 0\r\n"
    "# PROP BASE Use_Debug_Libraries 1\r\n"
    "# PROP BASE Output_Dir \"Debug\"\r\n"
    "# PROP BASE Intermediate_Dir \"Debug\"\r\n"
    "# PROP BASE Target_Dir \"\"\r\n"
    "# PROP Use_MFC 0\r\n"
    "# PROP Use_Debug_Libraries 1\r\n"
    "# PROP Output_Dir \"Debug\"\r\n"
    "# PROP Intermediate_Dir \"Debug\"\r\n"
    "# PROP Target_Dir \"\"\r\n"
    "# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_CONSOLE\" /D \"_MBCS\" /YX /FD /GZ /c\r\n"
    "# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_CONSOLE\" /D \"_MBCS\" /YX /FD /GZ /c\r\n"
    "# ADD BASE MTL /nologo /D \"_DEBUG\" /mktyplib203 /win32\r\n"
    "# ADD MTL /nologo /D \"_DEBUG\" /mktyplib203 /win32\r\n"
    "# ADD BASE RSC /l 0x411 /d \"_DEBUG\"\r\n"
    "# ADD RSC /l 0x411 /d \"_DEBUG\"\r\n"
    "BSC32=bscmake.exe\r\n"
    "# ADD BASE BSC32 /nologo\r\n"
    "# ADD BSC32 /nologo\r\n"
    "LINK32=link.exe\r\n"
    "# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept\r\n"
    "# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept\r\n"
    "\r\n"
    "!ELSEIF  \"$(CFG)\" == \"con_TEMPLATE - Win32 Unicode Release\"\r\n"
    "\r\n"
    "# PROP BASE Use_MFC 0\r\n"
    "# PROP BASE Use_Debug_Libraries 0\r\n"
    "# PROP BASE Output_Dir \"URelease\"\r\n"
    "# PROP BASE Intermediate_Dir \"URelease\"\r\n"
    "# PROP BASE Target_Dir \"\"\r\n"
    "# PROP Use_MFC 0\r\n"
    "# PROP Use_Debug_Libraries 0\r\n"
    "# PROP Output_Dir \"URelease\"\r\n"
    "# PROP Intermediate_Dir \"URelease\"\r\n"
    "# PROP Target_Dir \"\"\r\n"
    "# ADD BASE CPP /nologo /W3 /GX /O2 /D \"WIN32\" /D \"NDEBUG\" /D \"_CONSOLE\" /D \"UNICODE\" /D \"_UNICODE\" /YX /FD /c\r\n"
    "# ADD CPP /nologo /W3 /GX /O2 /D \"WIN32\" /D \"NDEBUG\" /D \"_CONSOLE\" /D \"UNICODE\" /D \"_UNICODE\" /YX /FD /c\r\n"
    "# ADD BASE MTL /nologo /D \"NDEBUG\" /mktyplib203 /win32\r\n"
    "# ADD MTL /nologo /D \"NDEBUG\" /mktyplib203 /win32\r\n"
    "# ADD BASE RSC /l 0x411 /d \"NDEBUG\"\r\n"
    "# ADD RSC /l 0x411 /d \"NDEBUG\"\r\n"
    "BSC32=bscmake.exe\r\n"
    "# ADD BASE BSC32 /nologo\r\n"
    "# ADD BSC32 /nologo\r\n"
    "LINK32=link.exe\r\n"
    "# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386\r\n"
    "# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386\r\n"
    "\r\n"
    "!ELSEIF  \"$(CFG)\" == \"con_TEMPLATE - Win32 Unicode Debug\"\r\n"
    "\r\n"
    "# PROP BASE Use_MFC 0\r\n"
    "# PROP BASE Use_Debug_Libraries 1\r\n"
    "# PROP BASE Output_Dir \"UDebug\"\r\n"
    "# PROP BASE Intermediate_Dir \"UDebug\"\r\n"
    "# PROP BASE Target_Dir \"\"\r\n"
    "# PROP Use_MFC 0\r\n"
    "# PROP Use_Debug_Libraries 1\r\n"
    "# PROP Output_Dir \"UDebug\"\r\n"
    "# PROP Intermediate_Dir \"UDebug\"\r\n"
    "# PROP Target_Dir \"\"\r\n"
    "# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_CONSOLE\" /D \"UNICODE\" /D \"_UNICODE\" /YX /FD /GZ /c\r\n"
    "# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_CONSOLE\" /D \"UNICODE\" /D \"_UNICODE\" /YX /FD /GZ /c\r\n"
    "# ADD BASE MTL /nologo /D \"_DEBUG\" /mktyplib203 /win32\r\n"
    "# ADD MTL /nologo /D \"_DEBUG\" /mktyplib203 /win32\r\n"
    "# ADD BASE RSC /l 0x411 /d \"_DEBUG\"\r\n"
    "# ADD RSC /l 0x411 /d \"_DEBUG\"\r\n"
    "BSC32=bscmake.exe\r\n"
    "# ADD BASE BSC32 /nologo\r\n"
    "# ADD BSC32 /nologo\r\n"
    "LINK32=link.exe\r\n"
    "# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept\r\n"
    "# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept\r\n"
    "\r\n"
    "!ENDIF \r\n"
    "\r\n"
    "# Begin Target\r\n"
    "# Name \"con_TEMPLATE - Win32 Release\"\r\n"
    "# Name \"con_TEMPLATE - Win32 Debug\"\r\n"
    "# Name \"con_TEMPLATE - Win32 Unicode Release\"\r\n"
    "# Name \"con_TEMPLATE - Win32 Unicode Debug\"\r\n"
    "# Begin Group \"Source Files\"\r\n"
    "# PROP Default_Filter \"cpp;c;cxx;rc;def;r;odl;idl;hpj;bat\"\r\n"
    "# End Group\r\n"
    "# Begin Group \"Header Files\"\r\n"
    "# PROP Default_Filter \"h;hpp;hxx;hm;inl\"\r\n"
    "# End Group\r\n"
    "# Begin Group \"Resource Files\"\r\n"
    "# PROP Default_Filter \"ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe\"\r\n"
    "# End Group\r\n"
    "# End Target\r\n"
    "# End Project\r\n";

static char *read_whole_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    if (fseek(f, 0, SEEK_END) != 0) { fclose(f); return NULL; }
    long sz = ftell(f);
    if (sz < 0) { fclose(f); return NULL; }
    rewind(f);
    char *buf = (char*)malloc((size_t)sz + 1);
    if (!buf) { fclose(f); return NULL; }
    if (fread(buf, 1, (size_t)sz, f) != (size_t)sz) { free(buf); fclose(f); return NULL; }
    buf[sz] = '\0';
    fclose(f);
    return buf;
}

static int write_file(const char *path, const char *data, size_t len) {
    /* ソースファイルが Shift-JIS (CP932) で保存されている前提で、
     * 文字列リテラルをそのまま書き出す（変換なし）。
     * これにより .dsp/.dsw も自動的に Shift-JIS で出力される。
     */
    FILE *f = fopen(path, "wb");
    if (!f) return 0;
    if (fwrite(data, 1, len, f) != len) { fclose(f); return 0; }
    fclose(f);
    return 1;
}

/* 文字列中の 'from' をすべて 'to' に置換する（新しい文字列を割り当てて返す） */
static char *replace_all(const char *str, const char *from, const char *to) {
    if (!str || !from || !to) return NULL;
    size_t slen = strlen(str);
    size_t flen = strlen(from);
    size_t tlen = strlen(to);
    if (flen == 0) return NULL;
    /* count occurrences */
    size_t count = 0;
    const char *p = str;
    while ((p = strstr(p, from)) != NULL) { count++; p += flen; }
    size_t newlen = slen + count * (tlen > flen ? (tlen - flen) : (flen - tlen));
    char *out = (char*)malloc(newlen + 1);
    if (!out) return NULL;
    char *dst = out;
    const char *cur = str;
    while ((p = strstr(cur, from)) != NULL) {
        size_t chunk = (size_t)(p - cur);
        memcpy(dst, cur, chunk); dst += chunk;
        memcpy(dst, to, tlen); dst += tlen;
        cur = p + flen;
    }
    strcpy(dst, cur);
    return out;
}

/* '# Begin Group "Source Files"' の後、最初の '# End Group' の直前に SOURCE 行を挿入する */
static char *insert_main_into_dsp(const char *content, const char *projname) {
    const char *group_marker = "# Begin Group \"Source Files\"";
    const char *end_marker = "# End Group";
    const char *start = strstr(content, group_marker);
    if (!start) return NULL; /* 見つからない */
    const char *end = strstr(start, end_marker);
    if (!end) return NULL;
    /* 既に同じ SOURCE が存在しないか確認（重複挿入防止） */
    {
        char checkbuf[256];
        snprintf(checkbuf, sizeof(checkbuf), "SOURCE=.\\%s_main.c", projname);
        if (strstr(start, checkbuf) != NULL) {
            return NULL; /* 既に存在するため挿入不要 */
        }
    }
    /* build insertion */
    char insertbuf[1024];
    snprintf(insertbuf, sizeof(insertbuf), "# Begin Source File\r\n\r\nSOURCE=.\\%s_main.c\r\n\r\n# End Source File\r\n", projname);
    /* create new content */
    size_t before_len = (size_t)(end - content);
    size_t insert_len = strlen(insertbuf);
    size_t after_len = strlen(end);
    char *out = (char*)malloc(before_len + insert_len + after_len + 1);
    if (!out) return NULL;
    memcpy(out, content, before_len);
    memcpy(out + before_len, insertbuf, insert_len);
    strcpy(out + before_len + insert_len, end);
    return out;
}

static int generate_main_c(const char *projname, const char *outdir) {
    char path[1024];
    if (outdir && outdir[0] != '\0')
        snprintf(path, sizeof(path), "%s\\%s_main.c", outdir, projname);
    else
        snprintf(path, sizeof(path), "%s_main.c", projname);
    return write_file(path, main_c_literal, sizeof(main_c_literal) - 1);
}

/* Objective-C main.m ファイルを生成 */
static int generate_main_m_macos(const char *projname, const char *outdir) {
    const char *main_m_content = 
        "#import <Cocoa/Cocoa.h>\r\n"
        "\r\n"
        "int main(int argc, const char * argv[]) {\r\n"
        "    @autoreleasepool {\r\n"
        "        return NSApplicationMain(argc, argv);\r\n"
        "    }\r\n"
        "}\r\n";
    
    char path[1024];
    if (outdir && outdir[0] != '\0')
        snprintf(path, sizeof(path), "%s/%s_main.m", outdir, projname);
    else
        snprintf(path, sizeof(path), "%s_main.m", projname);
    
    return write_file(path, main_m_content, strlen(main_m_content));
}

static int generate_main_m_ios(const char *projname, const char *outdir) {
    const char *main_m_content = 
        "#import <UIKit/UIKit.h>\r\n"
        "\r\n"
        "int main(int argc, char * argv[]) {\r\n"
        "    @autoreleasepool {\r\n"
        "        return UIApplicationMain(argc, argv, nil, nil);\r\n"
        "    }\r\n"
        "}\r\n";
    
    char path[1024];
    if (outdir && outdir[0] != '\0')
        snprintf(path, sizeof(path), "%s/%s_main.m", outdir, projname);
    else
        snprintf(path, sizeof(path), "%s_main.m", projname);
    
    return write_file(path, main_m_content, strlen(main_m_content));
}

/* Info.plist ファイルを生成 */
static int generate_info_plist_macos(const char *projname, const char *outdir) {
    const char *plist_content = 
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
        "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\r\n"
        "<plist version=\"1.0\">\r\n"
        "<dict>\r\n"
        "    <key>CFBundleIdentifier</key>\r\n"
        "    <string>com.example.PROJNAME</string>\r\n"
        "    <key>CFBundleName</key>\r\n"
        "    <string>PROJNAME</string>\r\n"
        "    <key>CFBundleVersion</key>\r\n"
        "    <string>1.0</string>\r\n"
        "    <key>CFBundleShortVersionString</key>\r\n"
        "    <string>1.0</string>\r\n"
        "    <key>CFBundleExecutable</key>\r\n"
        "    <string>${PROJECT_NAME}</string>\r\n"
        "    <key>CFBundlePackageType</key>\r\n"
        "    <string>APPL</string>\r\n"
        "    <key>CFBundleInfoDictionaryVersion</key>\r\n"
        "    <string>6.0</string>\r\n"
        "    <key>LSMinimumSystemVersion</key>\r\n"
        "    <string>10.14</string>\r\n"
        "    <key>NSHumanReadableCopyright</key>\r\n"
        "    <string>Copyright (c) 2024. All rights reserved.</string>\r\n"
        "</dict>\r\n"
        "</plist>\r\n";
    
    char path[1024];
    char *plist_replaced = replace_all(plist_content, "PROJNAME", projname);
    if (!plist_replaced) return 0;
    
    if (outdir && outdir[0] != '\0')
        snprintf(path, sizeof(path), "%s/Info.plist", outdir);
    else
        snprintf(path, sizeof(path), "Info.plist");
    
    int result = write_file(path, plist_replaced, strlen(plist_replaced));
    free(plist_replaced);
    return result;
}

/* Parent Xcode プロジェクト（iOS と macOS サブプロジェクトを含む）を生成 */
static int generate_parent_xcode_project(const char *projname, const char *root_dir) {
    char path[1024];
    char xcodeproj_dir[1024];
    char pbxproj_content[20480];
    int pos = 0;
    
    /* Parent プロジェクトディレクトリを構成 */
    snprintf(xcodeproj_dir, sizeof(xcodeproj_dir), "%s/%s.xcodeproj", root_dir, projname);
    
    /* ディレクトリ作成 */
    if (!make_dir_recursive(xcodeproj_dir)) return 0;
    
    /* pbxproj コンテンツを構築 */
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "// !$*UTF8*$!\n{\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\tarchiveVersion = 1;\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\tclasses = {};\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\tobjectVersion = 50;\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\tobjects = {\n");
    
    /* PBXFileReference section */
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t/* Begin PBXFileReference section */\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\tFILE_IOS = {isa = PBXFileReference; lastKnownFileType = \"wrapper.pb-project\"; name = \"ios_%s.xcodeproj\"; path = \"ios_%s/ios_%s.xcodeproj\"; sourceTree = \"<group>\"; };\n", projname, projname, projname);
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\tFILE_MACOS = {isa = PBXFileReference; lastKnownFileType = \"wrapper.pb-project\"; name = \"macos_%s.xcodeproj\"; path = \"macos_%s/macos_%s.xcodeproj\"; sourceTree = \"<group>\"; };\n", projname, projname, projname);
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t/* End PBXFileReference section */\n");
    
    /* PBXGroup section */
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t/* Begin PBXGroup section */\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\tGROUP_ROOT = {isa = PBXGroup; children = (FILE_IOS, FILE_MACOS); name = %s; sourceTree = \"<group>\"; };\n", projname);
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t/* End PBXGroup section */\n");
    
    /* PBXProject section */
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t/* Begin PBXProject section */\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\tPROJ_OBJ = {\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t\tisa = PBXProject;\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t\tattributes = {};\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t\tbuildConfigurationList = CONFIG_LIST;\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t\tcompatibilityVersion = \"Xcode 9.3\";\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t\tdevelopmentRegion = en;\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t\thasScannedForEncodings = 0;\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t\tknownRegions = (en, Base);\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t\tmainGroup = GROUP_ROOT;\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t\tprojectDirPath = \"\";\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t\tprojectReferences = ({ProductGroup = GROUP_ROOT; ProjectRef = FILE_IOS;}, {ProductGroup = GROUP_ROOT; ProjectRef = FILE_MACOS;});\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t\tprojectRoot = \"\";\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t\ttargets = ();\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t};\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t/* End PBXProject section */\n");
    
    /* XCBuildConfiguration section */
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t/* Begin XCBuildConfiguration section */\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\tCONFIG_DEBUG = {isa = XCBuildConfiguration; buildSettings = {}; name = Debug; };\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\tCONFIG_RELEASE = {isa = XCBuildConfiguration; buildSettings = {}; name = Release; };\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t/* End XCBuildConfiguration section */\n");
    
    /* XCConfigurationList section */
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t/* Begin XCConfigurationList section */\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\tCONFIG_LIST = {isa = XCConfigurationList; buildConfigurations = (CONFIG_DEBUG, CONFIG_RELEASE); defaultConfigurationIsVisible = 0; defaultConfigurationName = Release; };\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t\t/* End XCConfigurationList section */\n");
    
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\t};\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "\trootObject = PROJ_OBJ;\n");
    pos += snprintf(pbxproj_content + pos, sizeof(pbxproj_content) - pos, "}\n");
    
    snprintf(path, sizeof(path), "%s/project.pbxproj", xcodeproj_dir);
    int ret = write_file(path, pbxproj_content, strlen(pbxproj_content));
    if (!ret) fprintf(stderr, "警告: Parent pbxproj 生成失敗: %s\n", path);
    return ret;
}

/* Xcode プロジェクト (.xcodeproj/project.pbxproj) を生成 */
static int generate_xcode_project(const char *projname, const char *outdir, const char *platform) {
    char path[1024];
    char xcodeproj_dir[1024];
    char project_dir[1024];
    
    /* プロジェクトディレクトリパスを構成 */
    if (outdir && outdir[0] != '\0') {
        snprintf(project_dir, sizeof(project_dir), "%s", outdir);
        snprintf(xcodeproj_dir, sizeof(xcodeproj_dir), "%s/%s.xcodeproj", outdir, projname);
    } else {
        snprintf(project_dir, sizeof(project_dir), "%s", projname);
        snprintf(xcodeproj_dir, sizeof(xcodeproj_dir), "%s.xcodeproj", projname);
    }
    
    /* ディレクトリ作成 */
    if (!make_dir_recursive(project_dir)) return 0;
    if (!make_dir_recursive(xcodeproj_dir)) return 0;
    
    /* プラットフォーム固有の設定 */
    const char *build_settings_debug;
    const char *build_settings_release;
    const char *framework_ref;     /* フレームワークの PBXFileReference */
    const char *framework_build;   /* フレームワークの PBXBuildFile */
    const char *framework_link;    /* FrameworksBuildPhase の files */
    
    if (strcmp(platform, "ios") == 0) {
        build_settings_debug = "SDKROOT = iphoneos; TARGETED_DEVICE_FAMILY = \"1,2\"; IPHONEOS_DEPLOYMENT_TARGET = 14.0; SUPPORTS_MACCATALYST = NO;";
        build_settings_release = "SDKROOT = iphoneos; TARGETED_DEVICE_FAMILY = \"1,2\"; IPHONEOS_DEPLOYMENT_TARGET = 14.0; SUPPORTS_MACCATALYST = NO;";
        framework_ref = "        20 /* UIKit.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = UIKit.framework; path = System/Library/Frameworks/UIKit.framework; sourceTree = SDKROOT; };\n"
                        "        21 /* Foundation.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = Foundation.framework; path = System/Library/Frameworks/Foundation.framework; sourceTree = SDKROOT; };\n";
        framework_build = "        22 /* UIKit.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 20; };\n"
                          "        23 /* Foundation.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 21; };\n";
        framework_link = "                22,\n                23,\n";
    } else {
        build_settings_debug = "SDKROOT = macosx; MACOSX_DEPLOYMENT_TARGET = 10.14; SUPPORTS_MACCATALYST = NO;";
        build_settings_release = "SDKROOT = macosx; MACOSX_DEPLOYMENT_TARGET = 10.14; SUPPORTS_MACCATALYST = NO;";
        framework_ref = "        20 /* Cocoa.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = Cocoa.framework; path = System/Library/Frameworks/Cocoa.framework; sourceTree = SDKROOT; };\n"
                        "        21 /* Foundation.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = Foundation.framework; path = System/Library/Frameworks/Foundation.framework; sourceTree = SDKROOT; };\n";
        framework_build = "        22 /* Cocoa.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 20; };\n"
                          "        23 /* Foundation.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 21; };\n";
        framework_link = "                22,\n                23,\n";
    }
    
    /* project.pbxproj 完全版テンプレート（フレームワークリンク対応） */
    char pbxproj_content[20480];
    snprintf(pbxproj_content, sizeof(pbxproj_content),
        "// !$*UTF8*$!\n"
        "{\n"
        "    archiveVersion = 1;\n"
        "    classes = {\n"
        "    };\n"
        "    objectVersion = 50;\n"
        "    objects = {\n"
        "\n"
        "        /* Begin PBXFileReference section */\n"
        "        1 /* %s_main.m */ = {isa = PBXFileReference; lastKnownFileType = sourcecode.c.objc; path = %s_main.m; sourceTree = \"<group>\"; };\n"
        "        2 /* Info.plist */ = {isa = PBXFileReference; lastKnownFileType = text.plist.xml; path = Info.plist; sourceTree = \"<group>\"; };\n"
        "        3 /* %s.app */ = {isa = PBXFileReference; explicitFileType = wrapper.application; path = %s.app; sourceTree = BUILT_PRODUCTS_DIR; };\n"
        "%s"
        "        /* End PBXFileReference section */\n"
        "\n"
        "        /* Begin PBXGroup section */\n"
        "        4 = {\n"
        "            isa = PBXGroup;\n"
        "            children = (\n"
        "                1, /* %s_main.m */\n"
        "                2, /* Info.plist */\n"
        "            );\n"
        "            name = %s;\n"
        "            sourceTree = \"<group>\";\n"
        "        };\n"
        "        5 = {\n"
        "            isa = PBXGroup;\n"
        "            children = (\n"
        "                3, /* %s.app */\n"
        "            );\n"
        "            name = Products;\n"
        "            sourceTree = \"<group>\";\n"
        "        };\n"
        "        /* End PBXGroup section */\n"
        "\n"
        "        /* Begin PBXProject section */\n"
        "        6 = {\n"
        "            isa = PBXProject;\n"
        "            attributes = {\n"
        "                LastUpgradeCheck = 1210;\n"
        "                TargetAttributes = {\n"
        "                    8 = {\n"
        "                        CreatedOnToolsVersion = 12.1;\n"
        "                    };\n"
        "                };\n"
        "            };\n"
        "            buildConfigurationList = 7;\n"
        "            compatibilityVersion = \"Xcode 9.3\";\n"
        "            developmentRegion = en;\n"
        "            knownRegions = (\n"
        "                en,\n"
        "                Base,\n"
        "            );\n"
        "            mainGroup = 4;\n"
        "            productRefGroup = 5;\n"
        "            projectDirPath = \"\";\n"
        "            projectRoot = \"\";\n"
        "            targets = (\n"
        "                8,\n"
        "            );\n"
        "        };\n"
        "        /* End PBXProject section */\n"
        "\n"
        "        /* Begin PBXNativeTarget section */\n"
        "        8 = {\n"
        "            isa = PBXNativeTarget;\n"
        "            buildConfigurationList = 9;\n"
        "            buildPhases = (\n"
        "                10,\n"
        "                16,\n"
        "                17,\n"
        "            );\n"
        "            buildRules = (\n"
        "            );\n"
        "            dependencies = (\n"
        "            );\n"
        "            name = %s;\n"
        "            productName = %s;\n"
        "            productReference = 3;\n"
        "            productType = \"com.apple.product-type.application\";\n"
        "        };\n"
        "        /* End PBXNativeTarget section */\n"
        "\n"
        "        /* Begin PBXSourcesBuildPhase section */\n"
        "        10 = {\n"
        "            isa = PBXSourcesBuildPhase;\n"
        "            buildActionMask = 2147483647;\n"
        "            files = (\n"
        "                11,\n"
        "            );\n"
        "            runOnlyForDeploymentPostprocessing = 0;\n"
        "        };\n"
        "        /* End PBXSourcesBuildPhase section */\n"
        "\n"
        "        /* Begin PBXFrameworksBuildPhase section */\n"
        "        16 = {\n"
        "            isa = PBXFrameworksBuildPhase;\n"
        "            buildActionMask = 2147483647;\n"
        "            files = (\n"
        "%s"
        "            );\n"
        "            runOnlyForDeploymentPostprocessing = 0;\n"
        "        };\n"
        "        /* End PBXFrameworksBuildPhase section */\n"
        "\n"
        "        /* Begin PBXResourcesBuildPhase section */\n"
        "        17 = {\n"
        "            isa = PBXResourcesBuildPhase;\n"
        "            buildActionMask = 2147483647;\n"
        "            files = (\n"
        "            );\n"
        "            runOnlyForDeploymentPostprocessing = 0;\n"
        "        };\n"
        "        /* End PBXResourcesBuildPhase section */\n"
        "\n"
        "        /* Begin PBXBuildFile section */\n"
        "        11 = {isa = PBXBuildFile; fileRef = 1; };\n"
        "%s"
        "        /* End PBXBuildFile section */\n"
        "\n"
        "        /* Begin XCBuildConfiguration section */\n"
        "        12 = {\n"
        "            isa = XCBuildConfiguration;\n"
        "            buildSettings = {\n"
        "                ALWAYS_SEARCH_USER_PATHS = NO;\n"
        "                CLANG_ENABLE_OBJC_ARC = YES;\n"
        "                CLANG_ENABLE_OBJC_WEAK = YES;\n"
        "                CLANG_WARN_BLOCK_CAPTURE_AUTORELEASING = YES;\n"
        "                CLANG_WARN_BOOL_CONVERSION = YES;\n"
        "                CLANG_WARN_COMMA = YES;\n"
        "                CLANG_WARN_CONSTANT_CONVERSION = YES;\n"
        "                CLANG_WARN_DIRECT_OBJC_ISA_USAGE = YES_ERROR;\n"
        "                CLANG_WARN_EMPTY_BODY = YES;\n"
        "                CLANG_WARN_ENUM_CONVERSION = YES;\n"
        "                CLANG_WARN_INT_CONVERSION = YES;\n"
        "                CLANG_WARN_NON_LITERAL_NULL_CONVERSION = YES;\n"
        "                CLANG_WARN_OBJC_IMPLICIT_RETAIN_SELF = YES;\n"
        "                CLANG_WARN_OBJC_LITERAL_CONVERSION = YES;\n"
        "                CLANG_WARN_OBJC_ROOT_CLASS = YES_ERROR;\n"
        "                CLANG_WARN_RANGE_LOOP_ANALYSIS = YES;\n"
        "                CLANG_WARN_STRICT_PROTOTYPES = YES;\n"
        "                CLANG_WARN_SUSPICIOUS_MOVE = YES;\n"
        "                CLANG_WARN_UNREACHABLE_CODE = YES;\n"
        "                COPY_PHASE_STRIP = NO;\n"
        "                DEBUG_INFORMATION_FORMAT = dwarf;\n"
        "                ENABLE_STRICT_OBJC_MSGSEND = YES;\n"
        "                ENABLE_TESTABILITY = YES;\n"
        "                GCC_C_LANGUAGE_STANDARD = gnu11;\n"
        "                GCC_DYNAMIC_NO_PIC = NO;\n"
        "                GCC_NO_COMMON_BLOCKS = YES;\n"
        "                GCC_OPTIMIZATION_LEVEL = 0;\n"
        "                GCC_PREPROCESSOR_DEFINITIONS = (\"DEBUG=1\", \"$(inherited)\");\n"
        "                GCC_WARN_64_TO_32_BIT_CONVERSION = YES;\n"
        "                GCC_WARN_ABOUT_RETURN_TYPE = YES_ERROR;\n"
        "                GCC_WARN_UNDECLARED_SELECTOR = YES;\n"
        "                GCC_WARN_UNINITIALIZED_AUTOS = YES_AGGRESSIVE;\n"
        "                GCC_WARN_UNUSED_FUNCTION = YES;\n"
        "                GCC_WARN_UNUSED_VARIABLE = YES;\n"
        "                MTL_ENABLE_DEBUG_INFO = INCLUDE_SOURCE;\n"
        "                MTL_FAST_MATH = YES;\n"
        "                ONLY_ACTIVE_ARCH = YES;\n"
        "                %s\n"
        "            };\n"
        "            name = Debug;\n"
        "        };\n"
        "        13 = {\n"
        "            isa = XCBuildConfiguration;\n"
        "            buildSettings = {\n"
        "                ALWAYS_SEARCH_USER_PATHS = NO;\n"
        "                CLANG_ENABLE_OBJC_ARC = YES;\n"
        "                CLANG_ENABLE_OBJC_WEAK = YES;\n"
        "                CLANG_WARN_BLOCK_CAPTURE_AUTORELEASING = YES;\n"
        "                CLANG_WARN_BOOL_CONVERSION = YES;\n"
        "                CLANG_WARN_COMMA = YES;\n"
        "                CLANG_WARN_CONSTANT_CONVERSION = YES;\n"
        "                CLANG_WARN_DIRECT_OBJC_ISA_USAGE = YES_ERROR;\n"
        "                CLANG_WARN_EMPTY_BODY = YES;\n"
        "                CLANG_WARN_ENUM_CONVERSION = YES;\n"
        "                CLANG_WARN_INT_CONVERSION = YES;\n"
        "                CLANG_WARN_NON_LITERAL_NULL_CONVERSION = YES;\n"
        "                CLANG_WARN_OBJC_IMPLICIT_RETAIN_SELF = YES;\n"
        "                CLANG_WARN_OBJC_LITERAL_CONVERSION = YES;\n"
        "                CLANG_WARN_OBJC_ROOT_CLASS = YES_ERROR;\n"
        "                CLANG_WARN_RANGE_LOOP_ANALYSIS = YES;\n"
        "                CLANG_WARN_STRICT_PROTOTYPES = YES;\n"
        "                CLANG_WARN_SUSPICIOUS_MOVE = YES;\n"
        "                CLANG_WARN_UNREACHABLE_CODE = YES;\n"
        "                COPY_PHASE_STRIP = YES;\n"
        "                DEBUG_INFORMATION_FORMAT = \"dwarf-with-dsym\";\n"
        "                ENABLE_NS_ASSERTIONS = NO;\n"
        "                ENABLE_STRICT_OBJC_MSGSEND = YES;\n"
        "                GCC_C_LANGUAGE_STANDARD = gnu11;\n"
        "                GCC_NO_COMMON_BLOCKS = YES;\n"
        "                GCC_WARN_64_TO_32_BIT_CONVERSION = YES;\n"
        "                GCC_WARN_ABOUT_RETURN_TYPE = YES_ERROR;\n"
        "                GCC_WARN_UNDECLARED_SELECTOR = YES;\n"
        "                GCC_WARN_UNINITIALIZED_AUTOS = YES_AGGRESSIVE;\n"
        "                GCC_WARN_UNUSED_FUNCTION = YES;\n"
        "                GCC_WARN_UNUSED_VARIABLE = YES;\n"
        "                MTL_ENABLE_DEBUG_INFO = NO;\n"
        "                MTL_FAST_MATH = YES;\n"
        "                VALIDATE_PRODUCT = YES;\n"
        "                %s\n"
        "            };\n"
        "            name = Release;\n"
        "        };\n"
        "        14 = {\n"
        "            isa = XCBuildConfiguration;\n"
        "            buildSettings = {\n"
        "                CODE_SIGN_STYLE = Automatic;\n"
        "                INFOPLIST_FILE = Info.plist;\n"
        "                LD_RUNPATH_SEARCH_PATHS = (\"$(inherited)\", \"@executable_path/Frameworks\");\n"
        "                PRODUCT_BUNDLE_IDENTIFIER = com.example.%s;\n"
        "                PRODUCT_NAME = \"$(TARGET_NAME)\";\n"
        "                DEVELOPMENT_TEAM = \"\";\n"
        "            };\n"
        "            name = Debug;\n"
        "        };\n"
        "        15 = {\n"
        "            isa = XCBuildConfiguration;\n"
        "            buildSettings = {\n"
        "                CODE_SIGN_STYLE = Automatic;\n"
        "                INFOPLIST_FILE = Info.plist;\n"
        "                LD_RUNPATH_SEARCH_PATHS = (\"$(inherited)\", \"@executable_path/Frameworks\");\n"
        "                PRODUCT_BUNDLE_IDENTIFIER = com.example.%s;\n"
        "                PRODUCT_NAME = \"$(TARGET_NAME)\";\n"
        "                DEVELOPMENT_TEAM = \"\";\n"
        "            };\n"
        "            name = Release;\n"
        "        };\n"
        "        /* End XCBuildConfiguration section */\n"
        "\n"
        "        /* Begin XCConfigurationList section */\n"
        "        7 = {\n"
        "            isa = XCConfigurationList;\n"
        "            buildConfigurations = (\n"
        "                12,\n"
        "                13,\n"
        "            );\n"
        "            defaultConfigurationIsVisible = 0;\n"
        "            defaultConfigurationName = Release;\n"
        "        };\n"
        "        9 = {\n"
        "            isa = XCConfigurationList;\n"
        "            buildConfigurations = (\n"
        "                14,\n"
        "                15,\n"
        "            );\n"
        "            defaultConfigurationIsVisible = 0;\n"
        "            defaultConfigurationName = Release;\n"
        "        };\n"
        "        /* End XCConfigurationList section */\n"
        "\n"
        "    };\n"
        "    rootObject = 6;\n"
        "}\n",
        projname, projname, projname, projname, framework_ref,
        projname, projname, projname,
        projname, projname,
        framework_link, framework_build,
        build_settings_debug, build_settings_release, projname, projname);
    
    snprintf(path, sizeof(path), "%s/project.pbxproj", xcodeproj_dir);
    return write_file(path, pbxproj_content, strlen(pbxproj_content));
}

static int generate_dsw(const char *projname, const char *outdir) {
    char path[1024];
    if (outdir && outdir[0] != '\0')
        snprintf(path, sizeof(path), "%s\\%s.dsw", outdir, projname);
    else
        snprintf(path, sizeof(path), "%s.dsw", projname);
    FILE *f = fopen(path, "wb");
    if (!f) return 0;
    /* create_dual_dsp.bat と同等の構造を出力する */
    fprintf(f, "Microsoft Developer Studio Workspace File, Format Version 6.00\r\n");
    fprintf(f, "# WARNING: DO NOT EDIT OR DELETE THIS WORKSPACE FILE!\r\n\r\n");
    fprintf(f, "\r\n");
    fprintf(f, "###############################################################################\r\n\r\n");
    /* Project エントリ: .dsw と .dsp は同ディレクトリに置かれる想定で相対パスを参照する */
    fprintf(f, "Project: \"win_%s\"=\".\\win_%s.dsp\" - Package Owner=<4>\r\n\r\n", projname, projname);
    fprintf(f, "Package=<5>\r\n");
    fprintf(f, "{{{\r\n");
    fprintf(f, "}}}\r\n\r\n");
    fprintf(f, "Package=<4>\r\n");
    fprintf(f, "{{{\r\n");
    fprintf(f, "}}}\r\n\r\n");
    fprintf(f, "###############################################################################\r\n\r\n");
    fprintf(f, "Project: \"con_%s\"=\".\\con_%s.dsp\" - Package Owner=<4>\r\n\r\n", projname, projname);
    fprintf(f, "Package=<5>\r\n");
    fprintf(f, "{{{\r\n");
    fprintf(f, "}}}\r\n\r\n");
    fprintf(f, "Package=<4>\r\n");
    fprintf(f, "{{{\r\n");
    fprintf(f, "}}}\r\n\r\n");
    fprintf(f, "###############################################################################\r\n\r\n");
    fprintf(f, "Global:\r\n\r\n");
    fprintf(f, "Package=<5>\r\n");
    fprintf(f, "{{{\r\n");
    fprintf(f, "}}}\r\n\r\n");
    fprintf(f, "Package=<3>\r\n");
    fprintf(f, "{{{\r\n");
    fprintf(f, "}}}\r\n\r\n");
    fprintf(f, "###############################################################################\r\n");
    fclose(f);
    return 1;
}

static int generate_build_bat(const char *projname, const char *outdir) {
    char path[1024];
    if (outdir && outdir[0] != '\0')
        snprintf(path, sizeof(path), "%s\\build_%s.bat", outdir, projname);
    else
        snprintf(path, sizeof(path), "build_%s.bat", projname);
    FILE *f = fopen(path, "wb");
    if (!f) return 0;
    fprintf(f, "@echo off\r\n");
    fprintf(f, "set MSDEV=msdev.exe\r\n");
    fprintf(f, "echo --- ビルドを開始します ---\r\n");
    fprintf(f, "%%MSDEV%% \"%s.dsw\" /make \"win_%s - Win32 Release\" /rebuild\r\n", projname, projname);
    fprintf(f, "if errorlevel 1 goto error\r\n");
    fprintf(f, "%%MSDEV%% \"%s.dsw\" /make \"win_%s - Win32 Debug\" /rebuild\r\n", projname, projname);
    fprintf(f, "if errorlevel 1 goto error\r\n");
    fprintf(f, "%%MSDEV%% \"%s.dsw\" /make \"con_%s - Win32 Release\" /rebuild\r\n", projname, projname);
    fprintf(f, "if errorlevel 1 goto error\r\n");
    fprintf(f, "%%MSDEV%% \"%s.dsw\" /make \"con_%s - Win32 Debug\" /rebuild\r\n", projname, projname);
    fprintf(f, "if errorlevel 1 goto error\r\n");
    fprintf(f, "echo --- ビルドが正常に完了しました ---\r\n");
    fprintf(f, "exit /b 0\r\n");
    fprintf(f, ":error\r\n");
    fprintf(f, "echo --- ビルドエラーが発生しました ---\r\n");
    fprintf(f, "exit /b 1\r\n");
    fclose(f);
    return 1;
}

/* ディレクトリを再帰的に作成する (/, \ の両方の区切りを扱う) */
static int make_dir_recursive(const char *path) {
    if (!path || path[0] == '\0') return 1;
    char tmp[1024];
    size_t len = strlen(path);
    if (len >= sizeof(tmp)) return 0;
    strcpy(tmp, path);
    /* セパレータをバックスラッシュに正規化（Windows向け） */
    for (size_t i = 0; i < len; i++) if (tmp[i] == '/') tmp[i] = '\\';

    /* 各パス要素を順に作成する */
    for (size_t i = 0; i < len; i++) {
        if (tmp[i] == '\\') {
            /* Windows ではドライブルート (例: "E:\") の作成をスキップする (index 2 以下) */
            if (i <= 2) continue;
            char save = tmp[i];
            tmp[i] = '\0';
#ifdef _WIN32
            if (_mkdir(tmp) != 0) {
                if (errno != EEXIST) { tmp[i] = save; return 0; }
            }
#else
            if (mkdir(tmp, 0755) != 0) {
                if (errno != EEXIST) { tmp[i] = save; return 0; }
            }
#endif
            tmp[i] = save;
        }
    }
    /* 最後のコンポーネントを作成 */
#ifdef _WIN32
    if (_mkdir(tmp) != 0) {
        if (errno != EEXIST) return 0;
    }
#else
    if (mkdir(tmp, 0755) != 0) {
        if (errno != EEXIST) return 0;
    }
#endif
    return 1;
}

/* GUID を生成する（簡易版：ランダムな16進数） */
static void generate_guid(char *buf, size_t bufsize) {
    unsigned int seed = (unsigned int)time(NULL);
    srand(seed);
    snprintf(buf, bufsize, "%08X-%04X-%04X-%04X-%012llX",
        (unsigned int)rand() << 16 | (unsigned int)rand(),
        (unsigned int)rand() & 0xFFFF,
        (unsigned int)rand() & 0xFFFF,
        (unsigned int)rand() & 0xFFFF,
        ((unsigned long long)rand() << 32) | (unsigned int)rand()
    );
}

/* Visual Studio ソリューションファイル (.sln) を生成 */
static int generate_sln(const char *projname, const char *outdir) {
    char path[1024];
    char sln_content[8192];
    char guid_win[64], guid_con[64];
    
    /* GUID を生成 */
    generate_guid(guid_win, sizeof(guid_win));
    generate_guid(guid_con, sizeof(guid_con));
    
    /* .sln ファイルの内容を構成 */
    /* プロジェクトで定義されている構成: Debug MBCS|Win32, Debug Unicode|Win32, Release MBCS|Win32, Release Unicode|Win32
                                      Debug MBCS|x64,   Debug Unicode|x64,   Release MBCS|x64,   Release Unicode|x64 */
    snprintf(sln_content, sizeof(sln_content),
        "Microsoft Visual Studio Solution File, Format Version 12.00\r\n"
        "# Visual Studio Version 17\r\n"
        "VisualStudioVersion = 17.0.31919.166\r\n"
        "MinimumVisualStudioVersion = 10.0.40219.1\r\n"
        "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"win_%s\", \"win_%s.vcxproj\", \"{%s}\"\r\n"
        "EndProject\r\n"
        "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"con_%s\", \"con_%s.vcxproj\", \"{%s}\"\r\n"
        "EndProject\r\n"
        "Global\r\n"
        "\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\r\n"
        "\t\tDebug MBCS|Win32 = Debug MBCS|Win32\r\n"
        "\t\tDebug Unicode|Win32 = Debug Unicode|Win32\r\n"
        "\t\tRelease MBCS|Win32 = Release MBCS|Win32\r\n"
        "\t\tRelease Unicode|Win32 = Release Unicode|Win32\r\n"
        "\t\tDebug MBCS|x64 = Debug MBCS|x64\r\n"
        "\t\tDebug Unicode|x64 = Debug Unicode|x64\r\n"
        "\t\tRelease MBCS|x64 = Release MBCS|x64\r\n"
        "\t\tRelease Unicode|x64 = Release Unicode|x64\r\n"
        "\tEndGlobalSection\r\n"
        "\tGlobalSection(ProjectConfigurationPlatforms) = postSolution\r\n"
        "\t\t{%s}.Debug MBCS|Win32.ActiveCfg = Debug MBCS|Win32\r\n"
        "\t\t{%s}.Debug MBCS|Win32.Build.0 = Debug MBCS|Win32\r\n"
        "\t\t{%s}.Debug Unicode|Win32.ActiveCfg = Debug Unicode|Win32\r\n"
        "\t\t{%s}.Debug Unicode|Win32.Build.0 = Debug Unicode|Win32\r\n"
        "\t\t{%s}.Release MBCS|Win32.ActiveCfg = Release MBCS|Win32\r\n"
        "\t\t{%s}.Release MBCS|Win32.Build.0 = Release MBCS|Win32\r\n"
        "\t\t{%s}.Release Unicode|Win32.ActiveCfg = Release Unicode|Win32\r\n"
        "\t\t{%s}.Release Unicode|Win32.Build.0 = Release Unicode|Win32\r\n"
        "\t\t{%s}.Debug MBCS|x64.ActiveCfg = Debug MBCS|x64\r\n"
        "\t\t{%s}.Debug MBCS|x64.Build.0 = Debug MBCS|x64\r\n"
        "\t\t{%s}.Debug Unicode|x64.ActiveCfg = Debug Unicode|x64\r\n"
        "\t\t{%s}.Debug Unicode|x64.Build.0 = Debug Unicode|x64\r\n"
        "\t\t{%s}.Release MBCS|x64.ActiveCfg = Release MBCS|x64\r\n"
        "\t\t{%s}.Release MBCS|x64.Build.0 = Release MBCS|x64\r\n"
        "\t\t{%s}.Release Unicode|x64.ActiveCfg = Release Unicode|x64\r\n"
        "\t\t{%s}.Release Unicode|x64.Build.0 = Release Unicode|x64\r\n"
        "\t\t{%s}.Debug MBCS|Win32.ActiveCfg = Debug MBCS|Win32\r\n"
        "\t\t{%s}.Debug MBCS|Win32.Build.0 = Debug MBCS|Win32\r\n"
        "\t\t{%s}.Debug Unicode|Win32.ActiveCfg = Debug Unicode|Win32\r\n"
        "\t\t{%s}.Debug Unicode|Win32.Build.0 = Debug Unicode|Win32\r\n"
        "\t\t{%s}.Release MBCS|Win32.ActiveCfg = Release MBCS|Win32\r\n"
        "\t\t{%s}.Release MBCS|Win32.Build.0 = Release MBCS|Win32\r\n"
        "\t\t{%s}.Release Unicode|Win32.ActiveCfg = Release Unicode|Win32\r\n"
        "\t\t{%s}.Release Unicode|Win32.Build.0 = Release Unicode|Win32\r\n"
        "\t\t{%s}.Debug MBCS|x64.ActiveCfg = Debug MBCS|x64\r\n"
        "\t\t{%s}.Debug MBCS|x64.Build.0 = Debug MBCS|x64\r\n"
        "\t\t{%s}.Debug Unicode|x64.ActiveCfg = Debug Unicode|x64\r\n"
        "\t\t{%s}.Debug Unicode|x64.Build.0 = Debug Unicode|x64\r\n"
        "\t\t{%s}.Release MBCS|x64.ActiveCfg = Release MBCS|x64\r\n"
        "\t\t{%s}.Release MBCS|x64.Build.0 = Release MBCS|x64\r\n"
        "\t\t{%s}.Release Unicode|x64.ActiveCfg = Release Unicode|x64\r\n"
        "\t\t{%s}.Release Unicode|x64.Build.0 = Release Unicode|x64\r\n"
        "\tEndGlobalSection\r\n"
        "\tGlobalSection(SolutionProperties) = preSolution\r\n"
        "\t\tHideSolutionNode = FALSE\r\n"
        "\tEndGlobalSection\r\n"
        "EndGlobal\r\n",
        projname, projname, guid_win,
        projname, projname, guid_con,
        /* win_* プロジェクト用 (8x2 = 16個) */
        guid_win, guid_win,
        guid_win, guid_win,
        guid_win, guid_win,
        guid_win, guid_win,
        guid_win, guid_win,
        guid_win, guid_win,
        guid_win, guid_win,
        guid_win, guid_win,
        /* con_* プロジェクト用 (8x2 = 16個) */
        guid_con, guid_con,
        guid_con, guid_con,
        guid_con, guid_con,
        guid_con, guid_con,
        guid_con, guid_con,
        guid_con, guid_con,
        guid_con, guid_con,
        guid_con, guid_con);
    
    /* 出力パスを構築 */
    if (outdir && outdir[0] != '\0')
        snprintf(path, sizeof(path), "%s\\%s.sln", outdir, projname);
    else
        snprintf(path, sizeof(path), "%s.sln", projname);
    
    /* ファイルに書き出す */
    if (!write_file(path, sln_content, strlen(sln_content))) {
        fprintf(stderr, "エラー: %s の書き込みに失敗しました\n", path);
        return 0;
    }
    
    return 1;
}

static int generate_vcxproj(const char *projname, const char *outdir, int gen_src) {
    char path[1024];
    char guid_win[64], guid_con[64];
    
    /* GUID を生成 (Windows アプリ用と コンソールアプリ用) */
    generate_guid(guid_win, sizeof(guid_win));
    generate_guid(guid_con, sizeof(guid_con));
    
    /* テンプレートファイルを読む */
    if (access("template.vcxproj", 0) != 0) {
        fprintf(stderr, "警告: template.vcxproj が見つかりません\n");
        return 0;
    }
    
    char *template_base = read_whole_file("template.vcxproj");
    if (!template_base) {
        fprintf(stderr, "エラー: template.vcxproj の読み込みに失敗しました\n");
        return 0;
    }
    
    /* ========== Windows アプリプロジェクト ========== */
    char *template_win = (char*)malloc(strlen(template_base) + 1);
    if (!template_win) {
        fprintf(stderr, "エラー: Windows アプリテンプレートのメモリ割り当てに失敗しました\n");
        free(template_base);
        return 0;
    }
    strcpy(template_win, template_base);
    
    /* TEMPLATE をプロジェクト名に置換 (win_* 用) */
    char win_proj_name[256];
    snprintf(win_proj_name, sizeof(win_proj_name), "win_%s", projname);
    char *content_win = replace_all(template_win, "TEMPLATE", win_proj_name);
    free(template_win);
    if (!content_win) {
        fprintf(stderr, "エラー: Windows プロジェクト名置換に失敗しました\n");
        free(template_base);
        return 0;
    }
    
    /* {TEMPLATE_GUID} を生成した GUID に置換 */
    char guid_placeholder[64];
    snprintf(guid_placeholder, sizeof(guid_placeholder), "{%s}", guid_win);
    char *content_win2 = replace_all(content_win, "{TEMPLATE_GUID}", guid_placeholder);
    free(content_win);
    if (!content_win2) {
        fprintf(stderr, "エラー: Windows GUID置換に失敗しました\n");
        free(template_base);
        return 0;
    }
    
    /* -src 指定時は Windows プロジェクトに main.c を追加 */
    char *content_win_final = content_win2;
    if (gen_src) {
        char main_c_entry[512];
        snprintf(main_c_entry, sizeof(main_c_entry),
            "  <ItemGroup>\r\n"
            "    <ClCompile Include=\"%s_main.c\" />\r\n"
            "  </ItemGroup>\r\n"
            "</Project>", projname);
        
        content_win_final = replace_all(content_win2, "</Project>", main_c_entry);
        free(content_win2);
        if (!content_win_final) {
            fprintf(stderr, "エラー: Windows プロジェクトへの main.c 追加に失敗しました\n");
            free(template_base);
            return 0;
        }
    }
    
    /* Windows アプリ用 vcxproj ファイルに書き出す */
    if (outdir && outdir[0] != '\0')
        snprintf(path, sizeof(path), "%s\\win_%s.vcxproj", outdir, projname);
    else
        snprintf(path, sizeof(path), "win_%s.vcxproj", projname);
    
    if (!write_file(path, content_win_final, strlen(content_win_final))) {
        fprintf(stderr, "エラー: %s の書き込みに失敗しました\n", path);
        free(content_win_final);
        free(template_base);
        return 0;
    }
    free(content_win_final);
    
    /* ========== コンソールアプリプロジェクト ========== */
    char *template_con = (char*)malloc(strlen(template_base) + 1);
    if (!template_con) {
        fprintf(stderr, "エラー: コンソールアプリテンプレートのメモリ割り当てに失敗しました\n");
        free(template_base);
        return 0;
    }
    strcpy(template_con, template_base);
    free(template_base); /* これ以上不要 */
    
    /* TEMPLATE をプロジェクト名に置換 (con_* 用) */
    char con_proj_name[256];
    snprintf(con_proj_name, sizeof(con_proj_name), "con_%s", projname);
    char *content_con = replace_all(template_con, "TEMPLATE", con_proj_name);
    free(template_con);
    if (!content_con) {
        fprintf(stderr, "エラー: コンソールプロジェクト名置換に失敗しました\n");
        return 0;
    }
    
    /* コンソール用にサブシステム設定を Console に統一 */
    /* AppType が空の場合の Windows 設定を Console に置換 */
    char *content_con2 = replace_all(content_con, "<SubSystem Condition=\" '$(SubSystem)' == '' \">Windows</SubSystem>",
                                     "<SubSystem Condition=\" '$(SubSystem)' == '' \">Console</SubSystem>");
    free(content_con);
    if (!content_con2) {
        fprintf(stderr, "エラー: コンソール SubSystem置換に失敗しました\n");
        return 0;
    }
    
    /* AppType が WINDOWS の場合の SubSystem を CONSOLE に置換 */
    char *content_con3 = replace_all(content_con2, "<SubSystem Condition=\"'$(AppType)'=='WINDOWS'\">Windows</SubSystem>",
                                     "<SubSystem Condition=\"'$(AppType)'=='WINDOWS'\">Console</SubSystem>");
    free(content_con2);
    if (!content_con3) {
        fprintf(stderr, "エラー: コンソール WINDOWS SubSystem置換に失敗しました\n");
        return 0;
    }
    
    /* {TEMPLATE_GUID} を生成した GUID に置換 */
    snprintf(guid_placeholder, sizeof(guid_placeholder), "{%s}", guid_con);
    char *content_con4 = replace_all(content_con3, "{TEMPLATE_GUID}", guid_placeholder);
    free(content_con3);
    if (!content_con4) {
        fprintf(stderr, "エラー: コンソール GUID置換に失敗しました\n");
        return 0;
    }
    
    /* コンソール用プロジェクトに AppType=CONSOLE プロパティを追加 */
    /* <WindowsTargetPlatformVersion> の直後に追加 */
    char *content_con5 = replace_all(content_con4, 
        "<WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>\r\n  </PropertyGroup>\r\n  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />",
        "<WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>\r\n    <AppType>CONSOLE</AppType>\r\n  </PropertyGroup>\r\n  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />");
    free(content_con4);
    if (!content_con5) {
        fprintf(stderr, "エラー: コンソール AppType プロパティ追加に失敗しました\n");
        return 0;
    }
    
    /* -src 指定時はコンソールプロジェクトに main.c を追加 */
    char *content_con_final = content_con5;
    if (gen_src) {
        char main_c_entry[512];
        snprintf(main_c_entry, sizeof(main_c_entry),
            "  <ItemGroup>\r\n"
            "    <ClCompile Include=\"%s_main.c\" />\r\n"
            "  </ItemGroup>\r\n"
            "</Project>", projname);
        
        content_con_final = replace_all(content_con5, "</Project>", main_c_entry);
        free(content_con5);
        if (!content_con_final) {
            fprintf(stderr, "エラー: コンソールプロジェクトへの main.c 追加に失敗しました\n");
            return 0;
        }
    }
    
    /* コンソールアプリ用 vcxproj ファイルに書き出す */
    if (outdir && outdir[0] != '\0')
        snprintf(path, sizeof(path), "%s\\con_%s.vcxproj", outdir, projname);
    else
        snprintf(path, sizeof(path), "con_%s.vcxproj", projname);
    
    if (!write_file(path, content_con_final, strlen(content_con_final))) {
        fprintf(stderr, "エラー: %s の書き込みに失敗しました\n", path);
        free(content_con_final);
        return 0;
    }
    free(content_con_final);
    
    /* .vcxproj.user ファイルを生成（Windows 用） */
    const char *user_content = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
                               "<Project ToolsVersion=\"Current\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\r\n"
                               "  <PropertyGroup />\r\n"
                               "</Project>\r\n";
    
    if (outdir && outdir[0] != '\0')
        snprintf(path, sizeof(path), "%s\\win_%s.vcxproj.user", outdir, projname);
    else
        snprintf(path, sizeof(path), "win_%s.vcxproj.user", projname);
    
    if (!write_file(path, user_content, strlen(user_content))) {
        fprintf(stderr, "警告: %s の書き込みに失敗しました\n", path);
    }
    
    /* .vcxproj.user ファイルを生成（コンソール用） */
    if (outdir && outdir[0] != '\0')
        snprintf(path, sizeof(path), "%s\\con_%s.vcxproj.user", outdir, projname);
    else
        snprintf(path, sizeof(path), "con_%s.vcxproj.user", projname);
    
    if (!write_file(path, user_content, strlen(user_content))) {
        fprintf(stderr, "警告: %s の書き込みに失敗しました\n", path);
    }
    
    /* -src 指定時は .vcxproj.filters も生成 */
    if (gen_src) {
        /* Windows アプリ用フィルタ */
        char filters_content[2048];
        snprintf(filters_content, sizeof(filters_content),
            "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
            "<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\r\n"
            "  <ItemGroup>\r\n"
            "    <Filter Include=\"Source Files\">\r\n"
            "      <UniqueIdentifier>{4FC737F1-C7A5-4376-A066-2A32D752A2FF}</UniqueIdentifier>\r\n"
            "      <Extensions>cpp;c;cc;cxx;c++;cppm;ixx;def;odl;idl;hpj;bat;asm;asmx</Extensions>\r\n"
            "    </Filter>\r\n"
            "    <Filter Include=\"Header Files\">\r\n"
            "      <UniqueIdentifier>{93995380-89BD-4b04-88EB-625FBE52EBFB}</UniqueIdentifier>\r\n"
            "      <Extensions>h;hh;hpp;hxx;h++;hm;inl;inc;ipp;xsd</Extensions>\r\n"
            "    </Filter>\r\n"
            "    <Filter Include=\"Resource Files\">\r\n"
            "      <UniqueIdentifier>{67DA6AB6-F800-4c08-8B7A-83BB121AAD01}</UniqueIdentifier>\r\n"
            "      <Extensions>rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx;tiff;tif;png;wav;mfcribbon-ms</Extensions>\r\n"
            "    </Filter>\r\n"
            "  </ItemGroup>\r\n"
            "  <ItemGroup>\r\n"
            "    <ClCompile Include=\"%s_main.c\">\r\n"
            "      <Filter>Source Files</Filter>\r\n"
            "    </ClCompile>\r\n"
            "  </ItemGroup>\r\n"
            "</Project>\r\n", projname);
        
        if (outdir && outdir[0] != '\0')
            snprintf(path, sizeof(path), "%s\\win_%s.vcxproj.filters", outdir, projname);
        else
            snprintf(path, sizeof(path), "win_%s.vcxproj.filters", projname);
        
        if (!write_file(path, filters_content, strlen(filters_content))) {
            fprintf(stderr, "警告: %s の書き込みに失敗しました\n", path);
        }
        
        /* コンソールアプリ用フィルタ */
        if (outdir && outdir[0] != '\0')
            snprintf(path, sizeof(path), "%s\\con_%s.vcxproj.filters", outdir, projname);
        else
            snprintf(path, sizeof(path), "con_%s.vcxproj.filters", projname);
        
        if (!write_file(path, filters_content, strlen(filters_content))) {
            fprintf(stderr, "警告: %s の書き込みに失敗しました\n", path);
        }
    }
    
    return 1;
}

 int main(int argc, char **argv) {
    int gen_src = 0;
    int gen_vcxproj = 0;
    int gen_xcode = 0;
    int gen_objc = 0;  /* iOS と macOS の両方を生成 */
    const char *proj = NULL;
    const char *platform = NULL;  /* "macos" または "ios" */
    char outdir[512] = "";

    if (argc < 2) {
        fprintf(stderr, "使い方: %s [-src] [-vcxproj] [-xcode [-platform macos|ios]] [-objc] [-path <outdir>] <プロジェクト名>\n", argv[0]);
        return 1;
    }

    /* 汎用引数解析: -src フラグ、-vcxproj フラグ、-xcode フラグ、-objc フラグ、-platform <os>、-path <outdir>、最後の非オプションを proj 名とする */
    for (int ai = 1; ai < argc; ai++) {
        if (strcmp(argv[ai], "-src") == 0) {
            gen_src = 1;
            continue;
        }
        if (strcmp(argv[ai], "-vcxproj") == 0) {
            gen_vcxproj = 1;
            continue;
        }
        if (strcmp(argv[ai], "-xcode") == 0) {
            gen_xcode = 1;
            if (platform == NULL) platform = "macos";  /* default platform */
            continue;
        }
        if (strcmp(argv[ai], "-objc") == 0) {
            gen_objc = 1;
            continue;
        }
        if (strcmp(argv[ai], "-platform") == 0) {
            if (ai + 1 < argc) {
                platform = argv[ai + 1];
                if (strcmp(platform, "macos") != 0 && strcmp(platform, "ios") != 0) {
                    fprintf(stderr, "エラー: -platform は 'macos' または 'ios' のいずれかを指定してください\n");
                    return 1;
                }
                ai++; /* skip value */
                continue;
            } else {
                fprintf(stderr, "エラー: -platform に続けてプラットフォーム (macos/ios) を指定してください\n");
                return 1;
            }
        }
        if (strcmp(argv[ai], "-path") == 0 || strcmp(argv[ai], "-outdir") == 0) {
            if (ai + 1 < argc) {
                snprintf(outdir, sizeof(outdir), "%s", argv[ai + 1]);
                ai++; /* skip value */
                continue;
            } else {
                fprintf(stderr, "エラー: -path に続けて出力先フォルダを指定してください\n");
                return 1;
            }
        }
        /* otherwise treat as project name if not set yet */
        if (!proj) proj = argv[ai];
    }

    if (!proj) {
        fprintf(stderr, "エラー: プロジェクト名を指定してください\n");
        return 1;
    }

    /* If -src is set and no explicit outdir given, default outdir to project name */
    if (gen_src && outdir[0] == '\0') {
        snprintf(outdir, sizeof(outdir), "%s", proj);
    }

    /* 埋め込みテンプレートはソース上部のグローバル変数 `win_template_literal` と
     * `con_template_literal` を使用します（外部ファイルは不要）。 */

    /* コピーして free できる形にする（既存ロジックを流用） */
    char *win_tpl = (char*)malloc(strlen(win_template_literal) + 1);
    char *con_tpl = (char*)malloc(strlen(con_template_literal) + 1);
    if (!win_tpl || !con_tpl) {
        fprintf(stderr, "エラー: テンプレート文字列の割り当てに失敗しました\n");
        free(win_tpl); free(con_tpl);
        return 1;
    }
    strcpy(win_tpl, win_template_literal);
    strcpy(con_tpl, con_template_literal);

    /* 単純置換: TEMPLATE -> proj */
    char *win_out = replace_all(win_tpl, "TEMPLATE", proj);
    char *con_out = replace_all(con_tpl, "TEMPLATE", proj);
    free(win_tpl); free(con_tpl);
    if (!win_out || !con_out) {
        fprintf(stderr, "エラー: 置換処理に失敗しました\n");
        free(win_out); free(con_out);
        return 1;
    }

    /* con_template は既にコンソールアプリケーション用テンプレートなので補正不要 */

    /* -src 指定時は Source Files グループに SOURCE を挿入 */
    if (gen_src) {
        char *tmp;
        tmp = insert_main_into_dsp(win_out, proj);
        if (tmp) { free(win_out); win_out = tmp; }
        tmp = insert_main_into_dsp(con_out, proj);
        if (tmp) { free(con_out); con_out = tmp; }
    }

    /* 出力ディレクトリ作成 (必要なら) */
    if (outdir[0] != '\0') {
        if (!make_dir_recursive(outdir)) {
            fprintf(stderr, "エラー: 出力フォルダ '%s' の作成に失敗しました (errno=%d)\n", outdir, errno);
            free(win_out); free(con_out);
            return 1;
        }
    }

    /* ファイルを書き出す (outdir が指定されていればそこへ) */
    char path[1024];
    
    /* -objc 指定時は DSP/DSW ファイルを出力しない */
    if (!gen_objc) {
        if (gen_src && outdir[0] != '\0') snprintf(path, sizeof(path), "%s\\win_%s.dsp", outdir, proj);
        else snprintf(path, sizeof(path), "win_%s.dsp", proj);
        if (!write_file(path, win_out, strlen(win_out))) { fprintf(stderr, "エラー: %s の書き込みに失敗しました\n", path); }
        if (gen_src && outdir[0] != '\0') snprintf(path, sizeof(path), "%s\\con_%s.dsp", outdir, proj);
        else snprintf(path, sizeof(path), "con_%s.dsp", proj);
        if (!write_file(path, con_out, strlen(con_out))) { fprintf(stderr, "エラー: %s の書き込みに失敗しました\n", path); }

        /* .dsw と build バッチを生成 (デフォルト、またはvcxproj指定時も) */
        if (!gen_vcxproj) {
            if (!generate_dsw(proj, outdir[0] != '\0' ? outdir : "")) fprintf(stderr, "警告: .dsw の生成に失敗しました\n");
            if (!generate_build_bat(proj, outdir[0] != '\0' ? outdir : "")) fprintf(stderr, "警告: build バッチの生成に失敗しました\n");
        }
    }

    /* -src 指定時に main.c を生成 */
    if (gen_src) {
        if (!generate_main_c(proj, outdir[0] != '\0' ? outdir : "")) fprintf(stderr, "警告: main.c の生成に失敗しました\n");
    }

    /* -vcxproj 指定時は .dsp/.dsw も生成する */
    if (gen_vcxproj) {
        /* con_*.dsp と win_*.dsp を生成 */
        if (!generate_dsw(proj, outdir[0] != '\0' ? outdir : "")) fprintf(stderr, "警告: .dsw の生成に失敗しました\n");
        if (!generate_build_bat(proj, outdir[0] != '\0' ? outdir : "")) fprintf(stderr, "警告: build バッチの生成に失敗しました\n");
        
        /* Visual Studio プロジェクトを生成 */
        if (!generate_vcxproj(proj, outdir[0] != '\0' ? outdir : "", gen_src)) fprintf(stderr, "警告: Visual Studio プロジェクトの生成に失敗しました\n");
        
        /* Visual Studio ソリューションファイルを生成 */
        if (!generate_sln(proj, outdir[0] != '\0' ? outdir : "")) fprintf(stderr, "警告: Visual Studio ソリューションファイルの生成に失敗しました\n");
    }

    /* -xcode 指定時は Xcode プロジェクトを生成 */
    if (gen_xcode) {
        char xcode_proj_dir[512];
        if (platform == NULL) platform = "macos";  /* default */
        
        /* Xcode プロジェクト専用ディレクトリを設定: projname フォルダ */
        if (outdir[0] != '\0') {
            snprintf(xcode_proj_dir, sizeof(xcode_proj_dir), "%s/%s", outdir, proj);
        } else {
            snprintf(xcode_proj_dir, sizeof(xcode_proj_dir), "%s", proj);
        }
        
        /* プロジェクトディレクトリ作成 */
        if (!make_dir_recursive(xcode_proj_dir)) {
            fprintf(stderr, "警告: Xcode プロジェクトフォルダ '%s' の作成に失敗しました\n", xcode_proj_dir);
        }
        
        /* プラットフォームに応じた main.m を生成 (xcode_proj_dir に出力) */
        if (strcmp(platform, "ios") == 0) {
            if (!generate_main_m_ios(proj, xcode_proj_dir)) fprintf(stderr, "警告: iOS main.m の生成に失敗しました\n");
        } else {
            if (!generate_main_m_macos(proj, xcode_proj_dir)) fprintf(stderr, "警告: macOS main.m の生成に失敗しました\n");
        }
        
        /* Info.plist を生成 (xcode_proj_dir に出力) */
        if (!generate_info_plist_macos(proj, xcode_proj_dir)) fprintf(stderr, "警告: Info.plist の生成に失敗しました\n");
        
        /* Xcode プロジェクトを生成 (xcode_proj_dir に出力) */
        if (!generate_xcode_project(proj, xcode_proj_dir, platform)) fprintf(stderr, "警告: Xcode プロジェクトの生成に失敗しました\n");
    }

    /* -objc 指定時は iOS と macOS の両方の Xcode プロジェクトを生成 */
    if (gen_objc) {
        char proj_root_dir[512];
        char ios_proj_name[256];
        char macos_proj_name[256];
        char ios_proj_dir[512];
        char macos_proj_dir[512];
        
        snprintf(ios_proj_name, sizeof(ios_proj_name), "ios_%s", proj);
        snprintf(macos_proj_name, sizeof(macos_proj_name), "macos_%s", proj);
        snprintf(proj_root_dir, sizeof(proj_root_dir), "%s", proj);
        snprintf(ios_proj_dir, sizeof(ios_proj_dir), "%s/ios_%s", proj, proj);
        snprintf(macos_proj_dir, sizeof(macos_proj_dir), "%s/macos_%s", proj, proj);
        
        /* 親ディレクトリ作成 */
        if (!make_dir_recursive(proj_root_dir)) {
            fprintf(stderr, "警告: プロジェクトフォルダ '%s' の作成に失敗しました\n", proj_root_dir);
        }
        
        /* iOS プロジェクトディレクトリ作成 */
        if (!make_dir_recursive(ios_proj_dir)) {
            fprintf(stderr, "警告: iOS プロジェクトフォルダ '%s' の作成に失敗しました\n", ios_proj_dir);
        }
        
        /* iOS プロジェクトを生成（プロジェクト名に ios_ プレフィックス） */
        if (!generate_main_m_ios(ios_proj_name, ios_proj_dir)) fprintf(stderr, "警告: iOS main.m の生成に失敗しました\n");
        if (!generate_info_plist_macos(ios_proj_name, ios_proj_dir)) fprintf(stderr, "警告: iOS Info.plist の生成に失敗しました\n");
        if (!generate_xcode_project(ios_proj_name, ios_proj_dir, "ios")) fprintf(stderr, "警告: iOS Xcode プロジェクトの生成に失敗しました\n");
        
        /* macOS プロジェクトディレクトリ作成 */
        if (!make_dir_recursive(macos_proj_dir)) {
            fprintf(stderr, "警告: macOS プロジェクトフォルダ '%s' の作成に失敗しました\n", macos_proj_dir);
        }
        
        /* macOS プロジェクトを生成（プロジェクト名に macos_ プレフィックス） */
        if (!generate_main_m_macos(macos_proj_name, macos_proj_dir)) fprintf(stderr, "警告: macOS main.m の生成に失敗しました\n");
        if (!generate_info_plist_macos(macos_proj_name, macos_proj_dir)) fprintf(stderr, "警告: macOS Info.plist の生成に失敗しました\n");
        if (!generate_xcode_project(macos_proj_name, macos_proj_dir, "macos")) fprintf(stderr, "警告: macOS Xcode プロジェクトの生成に失敗しました\n");
        
        /* Parent Xcode プロジェクトを生成 */
        if (!generate_parent_xcode_project(proj, proj_root_dir)) fprintf(stderr, "警告: Parent Xcode プロジェクトの生成に失敗しました\n");
    }

    /* 出力メッセージ */
    if (!gen_objc) {
        printf("生成しました: win_%s.dsp, con_%s.dsp, %s.dsw, build_%s.bat\n", proj, proj, proj, proj);
    }
    if (gen_src) printf("生成しました: %s_main.c\n", proj);
    if (gen_vcxproj) {
        printf("生成しました: %s.sln\n", proj);
        printf("生成しました: win_%s.vcxproj, con_%s.vcxproj\n", proj, proj);
        printf("生成しました: win_%s.vcxproj.user, con_%s.vcxproj.user\n", proj, proj);
        if (gen_src) printf("生成しました: win_%s.vcxproj.filters, con_%s.vcxproj.filters\n", proj, proj);
    }
    if (gen_xcode) {
        printf("生成しました: %s/%s.xcodeproj/project.pbxproj\n", proj, proj);
        printf("生成しました: %s/%s_main.m\n", proj, proj);
        printf("生成しました: %s/Info.plist\n", proj);
    }
    if (gen_objc) {
        printf("生成しました: %s/%s.xcodeproj/project.pbxproj\n", proj, proj);
        printf("生成しました: %s/ios_%s/ios_%s.xcodeproj/project.pbxproj\n", proj, proj, proj);
        printf("生成しました: %s/ios_%s/ios_%s_main.m\n", proj, proj, proj);
        printf("生成しました: %s/ios_%s/Info.plist\n", proj, proj);
        printf("生成しました: %s/macos_%s/macos_%s.xcodeproj/project.pbxproj\n", proj, proj, proj);
        printf("生成しました: %s/macos_%s/macos_%s_main.m\n", proj, proj, proj);
        printf("生成しました: %s/macos_%s/Info.plist\n", proj, proj);
    }

    free(win_out); free(con_out);
    return 0;
}

