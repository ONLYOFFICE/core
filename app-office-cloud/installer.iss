#ifndef AppSource
#error AppSource is required
#endif
#ifndef OutputDirectory
#define OutputDirectory "."
#endif
[Setup]
AppId={{8EF10DC1-0DB5-4794-8624-5DD74A8195CD}
AppName=ONLYOFFICE WPS
AppVersion=9.4.0.3
AppPublisher=MIDIA · Cloud MERP y compatibilidad WPS
AppPublisherURL=https://gestion.midiashops.com/diseno-v2/apps.html#onlyoffice-wps
DefaultDirName={localappdata}\Programs\ONLYOFFICE WPS
DefaultGroupName=ONLYOFFICE WPS
PrivilegesRequired=lowest
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
MinVersion=10.0
OutputDir={#OutputDirectory}
OutputBaseFilename=ONLYOFFICE-WPS-Cloud-9.4.0.3-Windows-x64-Setup
Compression=lzma2/fast
SolidCompression=yes
ChangesAssociations=yes
CloseApplications=yes
DisableProgramGroupPage=yes
SetupIconFile={#AppSource}\MIDIA-Office.ico
UninstallDisplayIcon={app}\MIDIA-Office.ico
[Languages]
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "english"; MessagesFile: "compiler:Default.isl"
[Files]
Source: "{#AppSource}\*"; DestDir: "{app}"; Excludes: "unins*"; Flags: ignoreversion recursesubdirs createallsubdirs
[Icons]
Name: "{group}\ONLYOFFICE WPS"; Filename: "{app}\MIDIACloud.exe"; Parameters: "--office"; IconFilename: "{app}\MIDIA-Office.ico"
Name: "{group}\MIDIA Cloud"; Filename: "https://gestion.midiashops.com/diseno-v2/office-cloud.html"
[Registry]
Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; ValueName: "MIDIAOfficeCloud"; ValueType: string; ValueData: """{app}\MIDIACloud.exe"" --serve"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Classes\MIDIA.ONLYOFFICEWPS.xlsx"; ValueType: string; ValueData: "Excel con fotos WPS y MIDIA Cloud"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Classes\MIDIA.ONLYOFFICEWPS.xlsx\DefaultIcon"; ValueType: string; ValueData: """{app}\MIDIA-Office.ico"",0"
Root: HKCU; Subkey: "Software\Classes\MIDIA.ONLYOFFICEWPS.xlsx\shell\open\command"; ValueType: string; ValueData: """{app}\MIDIACloud.exe"" --office ""%1"""
Root: HKCU; Subkey: "Software\Classes\.xlsx\OpenWithProgids"; ValueName: "MIDIA.ONLYOFFICEWPS.xlsx"; ValueType: none; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\ONLYOFFICEWPS\Capabilities"; ValueName: "ApplicationName"; ValueType: string; ValueData: "ONLYOFFICE WPS"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\ONLYOFFICEWPS\Capabilities"; ValueName: "ApplicationDescription"; ValueType: string; ValueData: "Fotos WPS y guardado automático en MIDIA Cloud"
Root: HKCU; Subkey: "Software\ONLYOFFICEWPS\Capabilities\FileAssociations"; ValueName: ".xlsx"; ValueType: string; ValueData: "MIDIA.ONLYOFFICEWPS.xlsx"
Root: HKCU; Subkey: "Software\RegisteredApplications"; ValueName: "ONLYOFFICEWPS"; ValueType: string; ValueData: "Software\ONLYOFFICEWPS\Capabilities"; Flags: uninsdeletevalue
[Run]
Filename: "{app}\MIDIACloud.exe"; Parameters: "--serve"; Flags: nowait
Filename: "ms-settings:defaultapps?registeredAppUser=ONLYOFFICEWPS"; Description: "Elegir ONLYOFFICE WPS para los archivos Excel"; Flags: shellexec postinstall skipifsilent
