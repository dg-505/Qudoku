@echo off

rem clean up previously created setup files
del "..\release\Qudoku-*.exe"
rmdir /s /q "..\release\WindowsInstaller"

for /f %%A in ('powershell -ExecutionPolicy Bypass -File getExePath.ps1') do set "exePath=%%A"

rem copy required files to Installer directory
mkdir "..\release\WindowsInstaller"
mkdir "..\release\WindowsInstaller\data"
mkdir "..\release\WindowsInstaller\ico"
mkdir "..\release\WindowsInstaller\i18n"
copy "%exePath%" "..\release\WindowsInstaller"
copy "..\Code\i18n\*.qm" "..\release\WindowsInstaller\i18n"
copy ".\icons\Qudoku.ico" "..\release\WindowsInstaller\ico"
copy ".\icons\uninstall.ico" "..\release\WindowsInstaller\ico"
copy "..\Code\data\example.txt" "..\release\WindowsInstaller\data"
copy "..\license.txt" "..\release\WindowsInstaller"

rem Get required libraries that the Qudoku.exe needs
C:\Qt\5.15.16\mingw81_64\bin\windeployqt.exe "..\release\WindowsInstaller\Qudoku.exe"

copy "C:\Qt\5.15.16\mingw81_64\bin\libgcc_s_seh-1.dll" ..\release\WindowsInstaller
copy "C:\Qt\5.15.16\mingw81_64\bin\libstdc++-6.dll" ..\release\WindowsInstaller
copy "C:\Qt\5.15.16\mingw81_64\bin\libwinpthread-1.dll" ..\release\WindowsInstaller

rem remove unnecessary stuff
rmdir /s /q "..\release\WindowsInstaller\iconengines"
rmdir /s /q "..\release\WindowsInstaller\imageformats"
rmdir /s /q "..\release\WindowsInstaller\translations"
del "..\release\WindowsInstaller\D3Dcompiler_47.dll"
del "..\release\WindowsInstaller\libEGL.dll"
del "..\release\WindowsInstaller\libGLESv2.dll"
del "..\release\WindowsInstaller\opengl32sw.dll"
del "..\release\WindowsInstaller\Qt5Svg.dll"

rem Create Installer executable
"C:\Program Files (x86)\Inno Setup 6\ISCC.exe" createSetup.iss

cmd /k
