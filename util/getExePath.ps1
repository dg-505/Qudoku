Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.IO

#$FolderBrowser = New-Object System.Windows.Forms.FolderBrowserDialog
#$FolderBrowser.ShowNewFolderButton = $false
#$FolderBrowser.Description = "Select build directory"
#$FolderBrowser.ShowDialog()
#Write-Output $FolderBrowser.SelectedPath

$dir = [System.IO.Path]::GetDirectoryName($MyInvocation.MyCommand.Definition)
$OpenFileDialog = New-Object System.Windows.Forms.OpenFileDialog
$OpenFileDialog.InitialDirectory = $dir
$OpenFileDialog.Filter = "Applications (*.exe)|*.exe"
$OpenFileDialog.ShowDialog() | Out-Null

Write-Output $OpenFileDialog.FileName
