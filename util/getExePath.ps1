Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.IO

#$FolderBrowser = New-Object System.Windows.Forms.FolderBrowserDialog
#$FolderBrowser.ShowNewFolderButton = $false
#$FolderBrowser.Description = "Select build directory"
#$FolderBrowser.ShowDialog()
#Write-Output $FolderBrowser.SelectedPath

$cwd = [System.IO.Path]::GetDirectoryName($MyInvocation.MyCommand.Definition)
$relativeBuildPath = "..\build\QtCreator\Release\"
$buildDir = Join-Path -Path $cwd -ChildPath $relativeBuildPath
$exeName = "Qudoku.exe"
$exePath = Join-Path -Path $buildDir -ChildPath $exeName
$OpenFileDialog = New-Object System.Windows.Forms.OpenFileDialog
$OpenFileDialog.InitialDirectory = $buildDir
$OpenFileDialog.FileName = $exePath
$OpenFileDialog.Filter = "Applications (*.exe)|*.exe"
$OpenFileDialog.ShowDialog() | Out-Null

Write-Output $OpenFileDialog.FileName
