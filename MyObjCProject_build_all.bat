@echo off
REM MBCS/CONSOLE/x86
MSBuild.exe "MyObjCProject.vcxproj" /p:Configuration="Debug MBCS";Platform=Win32;AppType=CONSOLE
MSBuild.exe "MyObjCProject.vcxproj" /p:Configuration="Release MBCS";Platform=Win32;AppType=CONSOLE
REM MBCS/CONSOLE/x64
MSBuild.exe "MyObjCProject.vcxproj" /p:Configuration="Debug MBCS";Platform=x64;AppType=CONSOLE
MSBuild.exe "MyObjCProject.vcxproj" /p:Configuration="Release MBCS";Platform=x64;AppType=CONSOLE
REM MBCS/WINDOWS/x86
MSBuild.exe "MyObjCProject.vcxproj" /p:Configuration="Debug MBCS";Platform=Win32;AppType=WINDOWS
MSBuild.exe "MyObjCProject.vcxproj" /p:Configuration="Release MBCS";Platform=Win32;AppType=WINDOWS
REM MBCS/WINDOWS/x64
MSBuild.exe "MyObjCProject.vcxproj" /p:Configuration="Debug MBCS";Platform=x64;AppType=WINDOWS
MSBuild.exe "MyObjCProject.vcxproj" /p:Configuration="Release MBCS";Platform=x64;AppType=WINDOWS
REM UNICODE/CONSOLE/x86
MSBuild.exe "MyObjCProject.vcxproj" /p:Configuration="Debug Unicode";Platform=Win32;AppType=CONSOLE
MSBuild.exe "MyObjCProject.vcxproj" /p:Configuration="Release Unicode";Platform=Win32;AppType=CONSOLE
REM UNICODE/CONSOLE/x64
MSBuild.exe "MyObjCProject.vcxproj" /p:Configuration="Debug Unicode";Platform=x64;AppType=CONSOLE
MSBuild.exe "MyObjCProject.vcxproj" /p:Configuration="Release Unicode";Platform=x64;AppType=CONSOLE
REM UNICODE/WINDOWS/x86
MSBuild.exe "MyObjCProject.vcxproj" /p:Configuration="Debug Unicode";Platform=Win32;AppType=WINDOWS
MSBuild.exe "MyObjCProject.vcxproj" /p:Configuration="Release Unicode";Platform=Win32;AppType=WINDOWS
REM UNICODE/WINDOWS/x64
MSBuild.exe "MyObjCProject.vcxproj" /p:Configuration="Debug Unicode";Platform=x64;AppType=WINDOWS
MSBuild.exe "MyObjCProject.vcxproj" /p:Configuration="Release Unicode";Platform=x64;AppType=WINDOWS

echo === Ç∑Ç◊ÇƒÇÃç\ê¨ÇÃÉrÉãÉhÇ™äÆóπÇµÇ‹ÇµÇΩ ===
