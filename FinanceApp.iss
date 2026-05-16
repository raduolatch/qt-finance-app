[Setup]
AppName=FinanceApp
AppVersion=1.0
DefaultDirName={pf}\FinanceApp
DefaultGroupName=FinanceApp
OutputDir=C:\apkproject\FinanceApp\installer
OutputBaseFilename=FinanceApp_Installer
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "C:\apkproject\FinanceApp\build\Desktop_Qt_6_11_0_MinGW_64_bit-Debug\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{commondesktop}\FinanceApp"; Filename: "{app}\FinanceApp.exe"
Name: "{group}\FinanceApp"; Filename: "{app}\FinanceApp.exe"

[Run]
Filename: "{app}\FinanceApp.exe"; Description: "Jalankan FinanceApp sekarang"; Flags: nowait postinstall skipifsilent