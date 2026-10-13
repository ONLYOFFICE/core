#ifndef AppSource
  #error AppSource is required
#endif
#ifndef OutputDirectory
  #define OutputDirectory "."
#endif
[Setup]
AppId={{8EF10DC1-0DB5-4794-8624-5DD74A8195CD}
AppName=ONLYOFFICE WPS
AppVersion=9.4.0.1
AppPublisher=Compilacion local de ONLYOFFICE con compatibilidad WPS
AppPublisherURL=https://github.com/guozz1984/core
DefaultDirName={localappdata}\Programs\ONLYOFFICE WPS
DefaultGroupName=ONLYOFFICE WPS
PrivilegesRequired=lowest
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
MinVersion=10.0
OutputDir={#OutputDirectory}
OutputBaseFilename=ONLYOFFICE-WPS-9.4-Windows-x64-Setup
Compression=lzma2/fast
SolidCompression=yes
ChangesAssociations=yes
CloseApplications=yes
DisableProgramGroupPage=yes
UninstallDisplayIcon={app}\DesktopEditors.exe
InfoAfterFile=LEEME.txt
[Languages]
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "english"; MessagesFile: "compiler:Default.isl"
[Files]
Source: "{#AppSource}\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
[Icons]
Name: "{group}\ONLYOFFICE WPS"; Filename: "{app}\DesktopEditors.exe"
Name: "{group}\Elegir como editor predeterminado de Excel"; Filename: "ms-settings:defaultapps?registeredAppUser=ONLYOFFICEWPS"
[Registry]
Root: HKCU; Subkey: "Software\Classes\MIDIA.ONLYOFFICEWPS.xlsx"; ValueType: string; ValueData: "Excel compatible con fotos WPS"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Classes\MIDIA.ONLYOFFICEWPS.xlsx\DefaultIcon"; ValueType: string; ValueData: """{app}\DesktopEditors.exe"",0"
Root: HKCU; Subkey: "Software\Classes\MIDIA.ONLYOFFICEWPS.xlsx\shell\open\command"; ValueType: string; ValueData: """{app}\DesktopEditors.exe"" ""%1"""
Root: HKCU; Subkey: "Software\Classes\.xlsx\OpenWithProgids"; ValueName: "MIDIA.ONLYOFFICEWPS.xlsx"; ValueType: none; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\ONLYOFFICEWPS\Capabilities"; ValueName: "ApplicationName"; ValueType: string; ValueData: "ONLYOFFICE WPS"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\ONLYOFFICEWPS\Capabilities"; ValueName: "ApplicationDescription"; ValueType: string; ValueData: "ONLYOFFICE con importacion automatica de fotos WPS en Excel"
Root: HKCU; Subkey: "Software\ONLYOFFICEWPS\Capabilities\FileAssociations"; ValueName: ".xlsx"; ValueType: string; ValueData: "MIDIA.ONLYOFFICEWPS.xlsx"
Root: HKCU; Subkey: "Software\RegisteredApplications"; ValueName: "ONLYOFFICEWPS"; ValueType: string; ValueData: "Software\ONLYOFFICEWPS\Capabilities"; Flags: uninsdeletevalue
[Run]
Filename: "ms-settings:defaultapps?registeredAppUser=ONLYOFFICEWPS"; Description: "Elegir ONLYOFFICE WPS para abrir los archivos Excel"; Flags: shellexec postinstall skipifsilent
