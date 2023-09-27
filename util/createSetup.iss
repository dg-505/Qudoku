#define Name "Qudoku"
#define Version "1.0"
#define ExeName "Qudoku.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{62B2B268-0C2C-469D-A5A7-0029787FC318}
AppName={#Name}
AppVersion={#Version}
DefaultDirName={localappdata}\{#Name}
DefaultGroupName={#Name}
AllowNoIcons=yes
LicenseFile=../release/WindowsInstaller/license.txt
OutputDir=../release
OutputBaseFilename=Qudoku_setup
SetupIconFile=../release/WindowsInstaller/ico/Qudoku.ico
Compression=lzma
SolidCompression=yes
ShowTasksTreeLines=True
AlwaysShowGroupOnReadyPage=True
AlwaysShowDirOnReadyPage=True
UninstallDisplayIcon={app}/ico/uninstall.ico
WizardStyle=modern
AllowCancelDuringInstall=False
DisableDirPage=no
AlwaysShowComponentsList=False
WizardImageFile=icons/SetupWizardImg.bmp
WizardSmallImageFile=icons/SetupWizardImgSmall.bmp
AppCopyright=GPL v3
UsePreviousPrivileges=False
PrivilegesRequired=lowest

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"


; remove entire program directory when installing or uninstalling
[UninstallDelete]
Type: filesandordirs; Name: "{app}"

[InstallDelete]
Type: filesandordirs; Name: "{app}"

[Code]
var
    DataDirPage: TInputDirWizardPage;
    DataDirLabel: TLabel;
procedure UpdateDataDirLabel; // update the entry at the "Ready to install memo" page
begin
    if DataDirPage.Values[0] <> '' then
        DataDirLabel.Caption := 'Data Location:' + #13#10 + #9 + DataDirPage.Values[0]
    else
        DataDirLabel.Caption := '';
end;

// New Setup Page: Set data directory
procedure InitializeWizard();
begin
    DataDirPage := CreateInputDirPage(wpSelectDir, 'Select Data Location', 'Where should Qudoku''s data files be stored?', 'Setup will specify the following folder as Qudoku''s default data directory.', True, 'Qudoku');
    DataDirPage.Add('To continue, click Next. If you would like to select a different folder, click Browse:');
    DataDirPage.Values[0] := ExpandConstant('{userdocs}/Qudoku');
    // Create a label on the ReadyForInstallationPage
    DataDirLabel := TLabel.Create(WizardForm);
    DataDirLabel.Parent := WizardForm.ReadyMemo;
    DataDirLabel.Left := WizardForm.ReadyMemo.Left;
    DataDirLabel.Top := WizardForm.ReadyMemo.Top + WizardForm.ReadyMemo.Height - ScaleY(10);
    DataDirLabel.Caption := 'Data Location:' + #13#10 + #9 + DataDirPage.Values[0];
    // Update the label text
    UpdateDataDirLabel;
end;

// procedure CurPageChanged(CurPageID: Integer);
// begin
//   // Update the label text whenever the page changes
//   if CurPageID = wpReady then
//     UpdateDataDirLabel;
// end;

// Return the specified data directory from the additional wizard page
function GetDataDir(Param: String): String;
var
    DataDir: String;
begin
    DataDir := DataDirPage.Values[0];
    StringChangeEx(DataDir, '\', '/', True); // Exchange backslashes '\' with forward slashes '/'
    Result := DataDir;
end;

// Check if the specified data directory aleady exists on the file system
function DataDirExists(): Boolean;
begin
    { Find out if data dir already exists }
    Result := DirExists(GetDataDir(''));
end;

// Show the data directory in Windows Explorer
procedure OpenDataDirInExplorer(Sender: TObject);
var
    ErrorCode: Integer;
begin
    ShellExec('', GetIniString('DIRS', 'DataDir', '', ExpandConstant('{app}/Qudoku.ini')), '', '', SW_SHOW, ewNoWait, ErrorCode);
end;

// Add page to Uninstaller to optionally remove the data directory
var
    RemoveDataDirUninstallPage: TNewNotebookPage;
    UninstallNextButton: TNewButton;
    ShowDataDirInExplorerButton: TNewButton;
    RemoveDataDirCheckbox: TNewCheckBox;
    RemoveDataDirLabel: TNewStaticText;
procedure InitializeUninstallProgressForm();
begin
    RemoveDataDirUninstallPage := TNewNotebookPage.Create(UninstallProgressForm);
    RemoveDataDirUninstallPage.Notebook := UninstallProgressForm.InnerNotebook;
    RemoveDataDirUninstallPage.Parent := UninstallProgressForm.InnerNotebook;
    RemoveDataDirUninstallPage.Align := alClient

    RemoveDataDirCheckbox := TNewCheckBox.Create(UninstallProgressForm);
    RemoveDataDirCheckbox.Parent := RemoveDataDirUninstallPage;
    RemoveDataDirCheckbox.Caption := '&Remove data directory';
    RemoveDataDirCheckbox.Left := ScaleX(10);
    RemoveDataDirCheckbox.Top := ScaleY(10);
    RemoveDataDirCheckbox.Checked := True;
    RemoveDataDirCheckbox.Width := UninstallProgressForm.InnerNotebook.Width / 3;
    RemoveDataDirCheckbox.Height := ScaleY(20);
 
    ShowDataDirInExplorerButton := TNewButton.Create(UninstallProgressForm);
    ShowDataDirInExplorerButton.Parent := RemoveDataDirUninstallPage;
    ShowDataDirInExplorerButton.Caption := 'Show data directory in Windows Explorer';
    ShowDataDirInExplorerButton.Top := RemoveDataDirCheckbox.Top;
    ShowDataDirInExplorerButton.Left := RemoveDataDirCheckbox.Left + RemoveDataDirCheckbox.Width;
    ShowDataDirInExplorerButton.Width := UninstallProgressForm.InnerNotebook.Width - RemoveDataDirCheckbox.Width - ScaleX(50);
    ShowDataDirInExplorerButton.Height := RemoveDataDirCheckbox.Height;
    ShowDataDirInExplorerButton.OnClick := @OpenDataDirInExplorer;
 
    RemoveDataDirLabel := TNewStaticText.Create(UninstallProgressForm);
    RemoveDataDirLabel.Parent := RemoveDataDirUninstallPage;
    RemoveDataDirLabel.Top := RemoveDataDirCheckbox.Top + RemoveDataDirCheckbox.Height + ScaleY(10);
    RemoveDataDirLabel.Left := RemoveDataDirCheckbox.Left;
    RemoveDataDirLabel.Width := UninstallProgressForm.InnerNotebook.Width;
    RemoveDataDirLabel.Height := RemoveDataDirLabel.AdjustHeight();
    RemoveDataDirLabel.WordWrap := True;
    RemoveDataDirLabel.Caption := 'This will remove the data directory (Current location is ' + GetIniString('DIRS', 'DataDir', '', ExpandConstant('{app}') + '/Qudoku.ini') + ')' + #13#10 + #13#10 + 'CAUTION: This will permanently delete the entire folder ' + GetIniString('DIRS', 'DataDir', '', ExpandConstant('{app}') + '/Qudoku.ini') + ' WITHOUT asking for confirmation!' + #13#10 + 'This cannot be undone, so make sure that you know what you''re doing!!!';

    UninstallProgressForm.InnerNotebook.ActivePage := RemoveDataDirUninstallPage;

    UninstallNextButton := TNewButton.Create(UninstallProgressForm);
    UninstallNextButton.Caption := 'Uninstall';
    UninstallNextButton.Parent := UninstallProgressForm;
    UninstallNextButton.Left := UninstallProgressForm.CancelButton.Left - UninstallProgressForm.CancelButton.Width - ScaleX(10);
    UninstallNextButton.Top := UninstallProgressForm.CancelButton.Top;
    UninstallNextButton.Width := UninstallProgressForm.CancelButton.Width;
    UninstallNextButton.Height := UninstallProgressForm.CancelButton.Height;
    UninstallNextButton.ModalResult := mrOk;
    UninstallNextButton.Default := True;

    UninstallProgressForm.CancelButton.Enabled := True;
    UninstallProgressForm.CancelButton.ModalResult := mrCancel;

    if UninstallProgressForm.ShowModal = mrCancel then Abort;

    UninstallProgressForm.InnerNotebook.ActivePage := UninstallProgressForm.InstallingPage;
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
var
    IniDataDir: String;
begin
    if CurUninstallStep = usUninstall then
    begin
         if RemoveDataDirCheckbox.Checked = True then
         begin
                 IniDataDir := GetIniString('DIRS', 'DataDir', '', ExpandConstant('{app}/Qudoku.ini'));
                 DelTree(IniDataDir, True, True, True);
         end;
    end;
end;

[Files]
Source: "../release/WindowsInstaller/*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Tasks: desktopicon
Source: "../release/WindowsInstaller/data/example.txt"; DestDir: "{code:GetDataDir}"; Flags: confirmoverwrite recursesubdirs uninsneveruninstall
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#Name}"; Filename: "{app}\{#ExeName}";
Name: "{group}\{cm:UninstallProgram,{#Name}}"; Filename: "{uninstallexe}"; IconFilename: "{app}/ico/uninstall.ico"
Name: "{userdesktop}\{#Name}"; Filename: "{app}\{#ExeName}"; IconFilename: "{app}/ico/Qudoku.ico"; Tasks: desktopicon

[Run]
Filename: "{app}\{#ExeName}"; Description: "{cm:LaunchProgram,{#StringChange(Name, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Dirs]
Name: {code:GetDataDir}; Check: not DataDirExists; Flags: uninsneveruninstall; Permissions: users-modify

[INI]
Filename: "{app}/Qudoku.ini"; Section: "DIRS"; Key: "DataDir"; String: "{code:getDataDir}"; Flags: createkeyifdoesntexist uninsdeletesection
