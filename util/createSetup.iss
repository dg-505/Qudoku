#define Name "Qudoku"
#define Version "2.0.0"
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
OutputBaseFilename=Qudoku-{#Version}
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
    HomeDirPage: TInputDirWizardPage;
    HomeDirLabel: TLabel;
procedure UpdateHomeDirLabel; // update the entry at the "Ready to install memo" page
begin
    if HomeDirPage.Values[0] <> '' then
        HomeDirLabel.Caption := 'Home directory:' + #13#10 + #9 + HomeDirPage.Values[0]
    else
        HomeDirLabel.Caption := '';
end;

// New Setup Page: Set data directory
procedure InitializeWizard();
begin
    HomeDirPage := CreateInputDirPage(wpSelectDir, 'Select Home Location', 'Where should Qudoku''s home directory be?', 'Setup will specify the following folder as Qudoku''s home directory.', True, 'Qudoku');
    HomeDirPage.Add('To continue, click Next. If you would like to select a different folder, click Browse:');
    HomeDirPage.Values[0] := ExpandConstant('{userdocs}/Qudoku');
    // Create a label on the ReadyForInstallationPage
    HomeDirLabel := TLabel.Create(WizardForm);
    HomeDirLabel.Parent := WizardForm.ReadyMemo;
    HomeDirLabel.Left := WizardForm.ReadyMemo.Left + 5;
    HomeDirLabel.Top := WizardForm.ReadyMemo.Top + WizardForm.ReadyMemo.Height - ScaleY(10);
    HomeDirLabel.Caption := 'Home directory:' + #13#10 + #9 + HomeDirPage.Values[0];
    // Update the label text
    UpdateHomeDirLabel;
end;

procedure CurPageChanged(CurPageID: Integer);
begin
  // Update the label text whenever the page changes
  if CurPageID = wpReady then
    UpdateHomeDirLabel;
end;

// Return the specified data directory from the additional wizard page
function GetHomeDir(Param: String): String;
var
    HomeDir: String;
begin
    HomeDir := HomeDirPage.Values[0];
    StringChangeEx(HomeDir, '\', '/', True); // Exchange backslashes '\' with forward slashes '/'
    Result := HomeDir;
end;

// Check if the specified data directory aleady exists on the file system
function HomeDirExists(): Boolean;
begin
    { Find out if data dir already exists }
    Result := DirExists(GetHomeDir(''));
end;

// Show the home directory in Windows Explorer
procedure OpenHomeDirInExplorer(Sender: TObject);
var
    ErrorCode: Integer;
    IniHomeDir: String;
begin
    IniHomeDir := GetIniString('DIRS', 'HomeDir', '', ExpandConstant('{app}/Qudoku.ini'));
    ShellExec('', IniHomeDir, '', '', SW_SHOWNORMAL, ewNoWait, ErrorCode);
end;

// Add page to Uninstaller to optionally remove the data directory
var
    RemoveHomeDirUninstallPage: TNewNotebookPage;
    UninstallNextButton: TNewButton;
    ShowHomeDirInExplorerButton: TNewButton;
    RemoveHomeDirCheckbox: TNewCheckBox;
    RemoveHomeDirLabel: TNewStaticText;
procedure InitializeUninstallProgressForm();
begin
    RemoveHomeDirUninstallPage := TNewNotebookPage.Create(UninstallProgressForm);
    RemoveHomeDirUninstallPage.Notebook := UninstallProgressForm.InnerNotebook;
    RemoveHomeDirUninstallPage.Parent := UninstallProgressForm.InnerNotebook;
    RemoveHomeDirUninstallPage.Align := alClient

    RemoveHomeDirCheckbox := TNewCheckBox.Create(UninstallProgressForm);
    RemoveHomeDirCheckbox.Parent := RemoveHomeDirUninstallPage;
    RemoveHomeDirCheckbox.Caption := '&Remove home directory';
    RemoveHomeDirCheckbox.Left := ScaleX(10);
    RemoveHomeDirCheckbox.Top := ScaleY(10);
    RemoveHomeDirCheckbox.Checked := True;
    RemoveHomeDirCheckbox.Width := UninstallProgressForm.InnerNotebook.Width / 3;
    RemoveHomeDirCheckbox.Height := ScaleY(20);
 
    ShowHomeDirInExplorerButton := TNewButton.Create(UninstallProgressForm);
    ShowHomeDirInExplorerButton.Parent := RemoveHomeDirUninstallPage;
    ShowHomeDirInExplorerButton.Caption := 'Show home directory in Windows Explorer';
    ShowHomeDirInExplorerButton.Top := RemoveHomeDirCheckbox.Top;
    ShowHomeDirInExplorerButton.Left := RemoveHomeDirCheckbox.Left + RemoveHomeDirCheckbox.Width;
    ShowHomeDirInExplorerButton.Width := UninstallProgressForm.InnerNotebook.Width - RemoveHomeDirCheckbox.Width - ScaleX(50);
    ShowHomeDirInExplorerButton.Height := RemoveHomeDirCheckbox.Height;
    ShowHomeDirInExplorerButton.OnClick := @OpenHomeDirInExplorer;
 
    RemoveHomeDirLabel := TNewStaticText.Create(UninstallProgressForm);
    RemoveHomeDirLabel.Parent := RemoveHomeDirUninstallPage;
    RemoveHomeDirLabel.Top := RemoveHomeDirCheckbox.Top + RemoveHomeDirCheckbox.Height + ScaleY(10);
    RemoveHomeDirLabel.Left := RemoveHomeDirCheckbox.Left;
    RemoveHomeDirLabel.Width := UninstallProgressForm.InnerNotebook.Width;
    RemoveHomeDirLabel.Height := RemoveHomeDirLabel.AdjustHeight();
    RemoveHomeDirLabel.WordWrap := True;
    RemoveHomeDirLabel.Caption := 'This will remove the home directory (Current location is ''' + GetIniString('DIRS', 'HomeDir', '', ExpandConstant('{app}') + '/Qudoku.ini') + ''')' + #13#10 + #13#10 + 'CAUTION: This will permanently delete the entire folder ''' + GetIniString('DIRS', 'HomeDir', '', ExpandConstant('{app}') + '/Qudoku.ini') + '''' + #13#10 + 'WITHOUT asking for confirmation!' + #13#10 + 'This cannot be undone, so make sure that you know what you''re doing!!!';

    UninstallProgressForm.InnerNotebook.ActivePage := RemoveHomeDirUninstallPage;

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
    IniHomeDir: String;
begin
    if CurUninstallStep = usUninstall then
    begin
         if RemoveHomeDirCheckbox.Checked = True then
         begin
                 IniHomeDir := GetIniString('DIRS', 'HomeDir', '', ExpandConstant('{app}/Qudoku.ini'));
                 DelTree(IniHomeDir, True, True, True);
         end;
    end;
end;

[Files]
Source: "../release/WindowsInstaller/*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "../release/WindowsInstaller/data/example.txt"; DestDir: "{code:GetHomeDir}\data"; Flags: confirmoverwrite recursesubdirs uninsneveruninstall
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#Name}"; Filename: "{app}\{#ExeName}";
Name: "{group}\{cm:UninstallProgram,{#Name}}"; Filename: "{uninstallexe}"; IconFilename: "{app}/ico/uninstall.ico"
Name: "{userdesktop}\{#Name}"; Filename: "{app}\{#ExeName}"; IconFilename: "{app}/ico/Qudoku.ico"; Tasks: desktopicon

[Run]
Filename: "{app}\{#ExeName}"; Description: "{cm:LaunchProgram,{#StringChange(Name, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Dirs]
Name: {code:GetHomeDir}; Check: not HomeDirExists; Flags: uninsneveruninstall; Permissions: users-modify
Name: "{code:GetHomeDir}\data"; Flags: uninsneveruninstall; Permissions: users-modify
Name: "{code:GetHomeDir}\export"; Flags: uninsneveruninstall; Permissions: users-modify

[INI]
Filename: "{app}/Qudoku.ini"; Section: "DIRS"; Key: "HomeDir"; String: "{code:getHomeDir}"; Flags: createkeyifdoesntexist uninsdeletesection
Filename: "{app}/Qudoku.ini"; Section: "I18N"; Key: "lang"; String: "en_US"; Flags: createkeyifdoesntexist uninsdeletesection
