@echo off

rem clean up previously created setup files
del "..\release\Qudoku_setup.exe"
rmdir /s /q "..\release\WindowsInstaller"

for /f %%A in ('powershell -ExecutionPolicy Bypass -File getExePath.ps1') do set "exePath=%%A"
rem set /p exePath=<exePath.txt

rem if not "%exePath%"=="" (
rem     echo No file selected! Aborting...
rem     cmd /k
rem )

rem copy required files to Installer directory
mkdir "..\release\WindowsInstaller"
mkdir "..\release\WindowsInstaller\data"
mkdir "..\release\WindowsInstaller\ico"
copy "%exePath%" "..\release\WindowsInstaller"
rem copy "%exePath%\version.txt" "..\release\WindowsInstaller"
copy ".\icons\Qudoku.ico" "..\release\WindowsInstaller\ico"
copy ".\icons\uninstall.ico" "..\release\WindowsInstaller\ico"
copy "..\Code\data\example.txt" "..\release\WindowsInstaller\data"
copy "..\license.txt" "..\release\WindowsInstaller"

rem Get required libraries that the Qudoku.exe needs
C:\Qt\6.5.2\mingw_64\bin\windeployqt6.exe "..\release\WindowsInstaller\Qudoku.exe"

rem remove unnecessary stuff
rmdir /s /q "..\release\WindowsInstaller\generic"
rmdir /s /q "..\release\WindowsInstaller\iconengines"
rmdir /s /q "..\release\WindowsInstaller\imageformats"
rmdir /s /q "..\release\WindowsInstaller\networkinformation"
rmdir /s /q "..\release\WindowsInstaller\tls"
rmdir /s /q "..\release\WindowsInstaller\translations"
del "..\release\WindowsInstaller\D3Dcompiler_47.dll"
del "..\release\WindowsInstaller\opengl32sw.dll"
del "..\release\WindowsInstaller\Qt6Network.dll"

rem Create Installer executable
"C:\Program Files (x86)\Inno Setup 6\ISCC.exe" createSetup.iss

cmd /k
