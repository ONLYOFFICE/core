@ECHO OFF
REM # MS-DOS batch file to create an project file for MS Visual Studio
REM Usage: msvcxml prefix source > solution
REM   prefix is used to prefix the names of the individual project files
REM   source is the source of the file list (eg ld.tr)
REM   solution is hte name of the solution file.
REM
REM Before using this batch file, you must :
REM 1) Build Ghostscript from the makefiles at least once
REM 2) Copy the batch file to the GS directory (one level above toolbin)
REM
REM The makefile build of GS will create a file called ld.tr, which is required
REM to give us the list of source files we need.
REM
REM Example usage
REM msvcxml.bat gs obj\ld.tr > ghostscript.sln
REM
REM # $Id: msvcxml.bat 9395 2009-01-26 17:16:47Z ken $

if "%1" == "" goto usage
if "%1" == "DLL" goto DLL
if "%1" == "echogs" goto echogs
if "%1" == "genarch" goto genarch
if "%1" == "mkromfs" goto mkromfs
if "%1" == "GUI" goto GUI
if "%1" == "Command" goto Command
if "%1" == "props" goto props
if "%1" == "gconfig" goto gconfig

echo Microsoft Visual Studio Solution File, Format Version 9.00
echo # Visual Studio 2005
echo Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "genarch", "%1-genarch.vcproj", "{A1C8B0BC-2B54-4FDF-8EFF-BA84F5214B92}"
echo EndProject
echo Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "echogs", "%1-echogs.vcproj", "{A1C8B0BC-2B54-4FDF-8EFF-BA84F5214B91}"
echo EndProject
echo Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "mkromfs", "%1-mkromfs.vcproj", "{45156C0D-CC16-4155-9D1F-063300029610}"
echo 	ProjectSection(ProjectDependencies) = postProject
echo 		{A1C8B0BC-2B54-4FDF-8EFF-BA84F5214B91} = {A1C8B0BC-2B54-4FDF-8EFF-BA84F5214B91}
echo 		{A1C8B0BC-2B54-4FDF-8EFF-BA84F5214B92} = {A1C8B0BC-2B54-4FDF-8EFF-BA84F5214B92}
echo 	EndProjectSection
echo EndProject
echo Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "gsdll32", "%1-dll.vcproj", "{C40391B4-5B95-45C1-8364-F7EC3B323C3D}"
echo 	ProjectSection(ProjectDependencies) = postProject
echo 		{A1C8B0BC-2B54-4FDF-8EFF-BA84F5214B91} = {A1C8B0BC-2B54-4FDF-8EFF-BA84F5214B91}
echo 		{A1C8B0BC-2B54-4FDF-8EFF-BA84F5214B92} = {A1C8B0BC-2B54-4FDF-8EFF-BA84F5214B92}
echo 		{45156C0D-CC16-4155-9D1F-063300029610} = {45156C0D-CC16-4155-9D1F-063300029610}
echo 	EndProjectSection
echo EndProject
echo Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "gswin32", "%1-exe.vcproj", "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}"
echo 	ProjectSection(ProjectDependencies) = postProject
echo 		{C40391B4-5B95-45C1-8364-F7EC3B323C3D} = {C40391B4-5B95-45C1-8364-F7EC3B323C3D}
echo 		{A1C8B0BC-2B54-4FDF-8EFF-BA84F5214B91} = {A1C8B0BC-2B54-4FDF-8EFF-BA84F5214B91}
echo 	EndProjectSection
echo EndProject
echo Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}")  = "gswin32c", "%1-exe-c.vcproj", "{5321B1FE-11FF-4FC3-923C-0A213501F23A}"
echo 	ProjectSection(ProjectDependencies) = postProject
echo 		{C40391B4-5B95-45C1-8364-F7EC3B323C3D} = {C40391B4-5B95-45C1-8364-F7EC3B323C3D}
echo 		{A1C8B0BC-2B54-4FDF-8EFF-BA84F5214B91} = {A1C8B0BC-2B54-4FDF-8EFF-BA84F5214B91}
echo 	EndProjectSection
echo EndProject
echo Global
echo   GlobalSection(SolutionConfigurationPlatforms) = preSolution
echo     Debug^|Win32 = Debug^|Win32
echo   EndGlobalSection
echo   GlobalSection(ProjectConfigurationPlatforms) = postSolution
echo     {A1C8B0BC-2B54-4FDF-8EFF-BA84F5214B91}.Debug^|Win32.ActiveCfg = Debug^|Win32
echo     {A1C8B0BC-2B54-4FDF-8EFF-BA84F5214B91}.Debug^|Win32.Build.0 = Debug^|Win32
echo     {C40391B4-5B95-45C1-8364-F7EC3B323C3D}.Debug^|Win32.ActiveCfg = Debug^|Win32
echo     {C40391B4-5B95-45C1-8364-F7EC3B323C3D}.Debug^|Win32.Build.0 = Debug^|Win32
echo     {8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}.Debug^|Win32.ActiveCfg = Debug^|Win32
echo     {8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}.Debug^|Win32.Build.0 = Debug^|Win32
echo     {5321B1FE-11FF-4FC3-923C-0A213501F23A}.Debug^|Win32.ActiveCfg = Debug^|Win32
echo     {5321B1FE-11FF-4FC3-923C-0A213501F23A}.Debug^|Win32.Build.0 = Debug^|Win32
echo   EndGlobalSection
echo   GlobalSection(SolutionProperties) = preSolution
echo     HideSolutionNode = FALSE
echo   EndGlobalSection
echo EndGlobal

call msvcxml.bat DLL %2 > %1-dll.vcproj
call msvcxml.bat GUI > %1-exe.vcproj
call msvcxml.bat Command > %1-exe-c.vcproj
call msvcxml.bat echogs > %1-echogs.vcproj
call msvcxml.bat genarch > %1-genarch.vcproj
call msvcxml.bat mkromfs > %1-mkromfs.vcproj
call msvcxml.bat props > macros.vsprops
call msvcxml.bat gconfig > gconfig.h
goto end

:usage
@ECHO OFF
echo # MS-DOS batch file to create an project file for MS Visual Studio 
echo.
echo   Usage: msvcxml prefix source ^> solution
echo     prefix is used to prefix the names of the individual project files
echo     source is the source of the file list (eg ld.tr)
echo     solution is the name of the solution file.
echo.
echo Before using this batch file, you must :
echo   1) Build Ghostscript from the makefiles at least once
echo   2) Copy the batch file to the GS directory (one level above toolbin)
echo.
echo The makefile build of GS will create a file called ld.tr, which is required
echo to give us the list of source files we need.
echo.
echo Example usage
echo   msvcxml.bat gs obj\ld.tr ^> ghostscript.sln
echo.
goto end

:DLL
echo ^<?xml version="1.0" encoding="utf-8" standalone="no"?^>
echo ^<VisualStudioProject
echo  ProjectType="Visual C++"
echo  Version="8.00"
echo  Name="gsdll32"
echo  ProjectGUID="{C40391B4-5B95-45C1-8364-F7EC3B323C3D}"
echo  RootNamespace="gs"
echo  Keyword="Win32Proj"
echo ^>
echo ^<Platforms^>
echo  ^<Platform Name="Win32"/^>
echo ^</Platforms^>
echo ^<ToolFiles^>
echo ^</ToolFiles^>
echo ^<Configurations^>
echo  ^<Configuration 
echo    Name="Debug|Win32"
echo    IntermediateDirectory="$(SolutionDir)\$(ConfigurationName)debugobj"
echo    OutputDirectory="$(SolutionDir)\$(ConfigurationName)debugbin"
echo    InheritedPropertySheets="$(SolutionDir)macros.vsprops"
echo    ConfigurationType="2"
echo    CharacterSet="0"
echo  ^>
echo  ^<Tool Name="VCCLCompilerTool"
echo    Optimization="0"
echo    PreprocessorDefinitions="__WIN32__;_DEBUG;_CRT_SECURE_NO_DEPRECATE;EXCLUDE_BMP_SUPPORT;EXCLUDE_JPG_SUPPORT;EXCLUDE_MIF_SUPPORT;EXCLUDE_PGX_SUPPORT;EXCLUDE_PNM_SUPPORT;EXCLUDE_RAS_SUPPORT;EXCLUDE_PNG_SUPPORTS;CHECK_INTERRUPTS;_Windows;_USRDLL;GX_COLOR_INDEX_TYPE=unsigned __int64"
echo    AdditionalIncludeDirectories="$(SolutionDir)base;$(SolutionDir)psi;$(SolutionDir)debugobj;$(SolutionDir)jpeg;$(SolutionDir)zlib;$(SolutionDir)icclib;$(SolutionDir)imdi;$(SolutionDir)jbig2dec;$(SolutionDir)ijs;$(SolutionDir)libpng;$(SolutionDir)jasper\src\libjasper\include"
echo    MinimalRebuild="true"
echo    DisableLanguageExtensions="false"
echo    BasicRuntimeChecks="3"
echo    UsePrecompiledHeader="0"
echo    WarningLevel="2"
echo    Detect64BitPortabilityProblems="true"
echo    DebugInformationFormat="4"
echo    CompileAs="1"
echo    DisableSpecificWarnings="4996;4224"
echo  /^>
echo  ^<Tool
echo    Name="VCResourceCompilerTool"
echo    PreprocessorDefinitions="_DEBUG"
echo    Culture="1033"
echo  /^>
echo  ^<Tool Name="VCLinkerTool"
echo    LinkIncremental="2"
echo    ModuleDefinitionFile="$(SolutionDir)psi\gsdll32.def"
echo    GenerateDebugInformation="true"
echo    SubSystem="1"
echo    TargetMachine="1"
echo   /^>
echo  ^</Configuration^>
echo  ^<Configuration 
echo    Name="Release|Win32"
echo    IntermediateDirectory="$(SolutionDir)\$(ConfigurationName)obj"
echo    OutputDirectory="$(SolutionDir)\$(ConfigurationName)bin"
echo    InheritedPropertySheets="$(SolutionDir)macros.vsprops"
echo    ConfigurationType="2"
echo    CharacterSet="0"
echo  ^>
echo  ^<Tool Name="VCCLCompilerTool"
echo    Optimization="2"
echo    PreprocessorDefinitions="__WIN32__;_CRT_SECURE_NO_DEPRECATE;EXCLUDE_BMP_SUPPORT;EXCLUDE_JPG_SUPPORT;EXCLUDE_MIF_SUPPORT;EXCLUDE_PGX_SUPPORT;EXCLUDE_PNM_SUPPORT;EXCLUDE_RAS_SUPPORT;EXCLUDE_PNG_SUPPORTS;CHECK_INTERRUPTS;_Windows;_USRDLL;GX_COLOR_INDEX_TYPE=unsigned __int64"
echo    AdditionalIncludeDirectories="$(SolutionDir)base;$(SolutionDir)psi;$(SolutionDir)debugobj;$(SolutionDir)jpeg;$(SolutionDir)zlib;$(SolutionDir)icclib;$(SolutionDir)imdi;$(SolutionDir)jbig2dec;$(SolutionDir)ijs;$(SolutionDir)libpng;$(SolutionDir)jasper\src\libjasper\include"
echo    MinimalRebuild="true"
echo    DisableLanguageExtensions="false"
echo    BasicRuntimeChecks="3"
echo    UsePrecompiledHeader="0"
echo    WarningLevel="2"
echo    Detect64BitPortabilityProblems="true"
echo    DebugInformationFormat="0"
echo    CompileAs="1"
echo    DisableSpecificWarnings="4996;4224"
echo  /^>
echo  ^<Tool
echo    Name="VCResourceCompilerTool"
echo    PreprocessorDefinitions=""
echo    Culture="1033"
echo  /^>
echo  ^<Tool Name="VCLinkerTool"
echo    LinkIncremental="2"
echo    ModuleDefinitionFile="$(SolutionDir)psi\gsdll32.def"
echo    GenerateDebugInformation="true"
echo    SubSystem="1"
echo    TargetMachine="1"
echo   /^>
echo  ^</Configuration^>
echo ^</Configurations^>
echo ^<References^>
echo ^</References^>
echo ^<Files^>
echo ^<Filter Name="DLL Source Files"
echo         Filter="c"^>
setlocal

for /F %%o in (%2) do (
  if exist .\base\%%~no.c (
    if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmsxf" (
      echo ^<File RelativePath=".\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwdib" (
      echo ^<File RelativePath=".\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwpr2" (
      echo ^<File RelativePath=".\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_msprn" (
      echo ^<File RelativePath=".\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_mswin" (
      echo ^<File RelativePath=".\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_ntfs" (
      echo ^<File RelativePath=".\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_wgetv" (
      echo ^<File RelativePath=".\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "ijs_exec_win" (
      echo ^<File RelativePath=".\ijs\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_win32" (
      echo ^<File RelativePath=".\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else echo ^<File RelativePath=".\base\%%~no.c"/^>
  ) else if exist .\psi\%%~no.c (
    if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\psi\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\psi\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmsxf" (
      echo ^<File RelativePath=".\psi\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwdib" (
      echo ^<File RelativePath=".\psi\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwpr2" (
      echo ^<File RelativePath=".\psi\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_msprn" (
      echo ^<File RelativePath=".\psi\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_mswin" (
      echo ^<File RelativePath=".\psi\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_ntfs" (
      echo ^<File RelativePath=".\psi\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_wgetv" (
      echo ^<File RelativePath=".\psi\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "ijs_exec_win" (
      echo ^<File RelativePath=".\ijs\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_win32" (
      echo ^<File RelativePath=".\psi\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else echo ^<File RelativePath=".\psi\%%~no.c"/^>
  ) else if exist .\zlib\%%~no.c (
    if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\zlib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\zlib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmsxf" (
      echo ^<File RelativePath=".\zlib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwdib" (
      echo ^<File RelativePath=".\zlib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwpr2" (
      echo ^<File RelativePath=".\zlib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_msprn" (
      echo ^<File RelativePath=".\zlib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_mswin" (
      echo ^<File RelativePath=".\zlib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_ntfs" (
      echo ^<File RelativePath=".\zlib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_wgetv" (
      echo ^<File RelativePath=".\zlib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "ijs_exec_win" (
      echo ^<File RelativePath=".\zllib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_win32" (
      echo ^<File RelativePath=".\zlib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else echo ^<File RelativePath=".\zlib\%%~no.c"/^>
  ) else if exist .\jpeg\%%~no.c (
    if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\jpeg\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\jpeg\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmsxf" (
      echo ^<File RelativePath=".\jpeg\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwdib" (
      echo ^<File RelativePath=".\jpeg\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwpr2" (
      echo ^<File RelativePath=".\jpeg\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_msprn" (
      echo ^<File RelativePath=".\jpeg\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_mswin" (
      echo ^<File RelativePath=".\jpeg\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_ntfs" (
      echo ^<File RelativePath=".\jpeg\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_wgetv" (
      echo ^<File RelativePath=".\jpeg\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "ijs_exec_win" (
      echo ^<File RelativePath=".\jpeg\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_win32" (
      echo ^<File RelativePath=".\jpeg\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else echo ^<File RelativePath=".\jpeg\%%~no.c"/^>
  ) else if exist .\libpng\%%~no.c (
    if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\libpng\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\libpng\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmsxf" (
      echo ^<File RelativePath=".\libpng\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwdib" (
      echo ^<File RelativePath=".\libpng\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwpr2" (
      echo ^<File RelativePath=".\libpng\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_msprn" (
      echo ^<File RelativePath=".\libpng\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_mswin" (
      echo ^<File RelativePath=".\libpng\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_ntfs" (
      echo ^<File RelativePath=".\libpng\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_wgetv" (
      echo ^<File RelativePath=".\libpng\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "ijs_exec_win" (
      echo ^<File RelativePath=".\libpng\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_win32" (
      echo ^<File RelativePath=".\libpng\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else echo ^<File RelativePath=".\libpng\%%~no.c"/^>
  ) else if exist .\jbig2dec\%%~no.c (
    if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\jbig2dec\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\jbig2dec\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmsxf" (
      echo ^<File RelativePath=".\jbig2dec\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwdib" (
      echo ^<File RelativePath=".\jbig2dec\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwpr2" (
      echo ^<File RelativePath=".\jbig2dec\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_msprn" (
      echo ^<File RelativePath=".\jbig2dec\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_mswin" (
      echo ^<File RelativePath=".\jbig2dec\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_ntfs" (
      echo ^<File RelativePath=".\jbig2dec\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_wgetv" (
      echo ^<File RelativePath=".\jbig2dec\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "ijs_exec_win" (
      echo ^<File RelativePath=".\jbig2dec\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_win32" (
      echo ^<File RelativePath=".\jbig2dec\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else echo ^<File RelativePath=".\jbig2dec\%%~no.c"/^>
  ) else if exist .\jasper\src\libjasper\base\%%~no.c (
    if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\jasper\src\libjasper\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\jasper\src\libjasper\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmsxf" (
      echo ^<File RelativePath=".\jasper\src\libjasper\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwdib" (
      echo ^<File RelativePath=".\jasper\src\libjasper\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwpr2" (
      echo ^<File RelativePath=".\jasper\src\libjasper\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_msprn" (
      echo ^<File RelativePath=".\jasper\src\libjasper\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_mswin" (
      echo ^<File RelativePath=".\jasper\src\libjasper\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_ntfs" (
      echo ^<File RelativePath=".\jasper\src\libjasper\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_wgetv" (
      echo ^<File RelativePath=".\jasper\src\libjasper\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "ijs_exec_win" (
      echo ^<File RelativePath=".\.\jasper\src\libjasper\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_win32" (
      echo ^<File RelativePath=".\jasper\src\libjasper\base\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else echo ^<File RelativePath=".\jasper\src\libjasper\base\%%~no.c"/^>
  ) else if exist .\jasper\src\libjasper\jpc\%%~no.c (
    if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jpc\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jpc\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmsxf" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jpc\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwdib" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jpc\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwpr2" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jpc\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_msprn" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jpc\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_mswin" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jpc\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_ntfs" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jpc\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_wgetv" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jpc\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "ijs_exec_win" (
      echo ^<File RelativePath=".\.\jasper\src\libjasper\jpc\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_win32" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jpc\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else echo ^<File RelativePath=".\jasper\src\libjasper\jpc\%%~no.c"/^>
  ) else if exist .\jasper\src\libjasper\jp2\%%~no.c (
    if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jp2\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jp2\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmsxf" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jp2\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwdib" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jp2\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwpr2" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jp2\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_msprn" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jp2\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_mswin" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jp2\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_ntfs" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jp2\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_wgetv" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jp2\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "ijs_exec_win" (
      echo ^<File RelativePath=".\.\jasper\src\libjasper\jp2\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_win32" (
      echo ^<File RelativePath=".\jasper\src\libjasper\jp2\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else echo ^<File RelativePath=".\jasper\src\libjasper\jp2\%%~no.c"/^>
  ) else if exist .\icclib\%%~no.c (
    if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\icclib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\icclib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmsxf" (
      echo ^<File RelativePath=".\icclib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwdib" (
      echo ^<File RelativePath=".\icclib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwpr2" (
      echo ^<File RelativePath=".\icclib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_msprn" (
      echo ^<File RelativePath=".\icclib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_mswin" (
      echo ^<File RelativePath=".\icclib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_ntfs" (
      echo ^<File RelativePath=".\icclib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_wgetv" (
      echo ^<File RelativePath=".\icclib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "ijs_exec_win" (
      echo ^<File RelativePath=".\icclib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_win32" (
      echo ^<File RelativePath=".\icclib\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else echo ^<File RelativePath=".\icclib\%%~no.c"/^>
  ) else if exist .\ijs\%%~no.c (
    if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\ijs\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmswn" (
      echo ^<File RelativePath=".\ijs\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevmsxf" (
      echo ^<File RelativePath=".\ijs\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwdib" (
      echo ^<File RelativePath=".\ijs\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gdevwpr2" (
      echo ^<File RelativePath=".\ijs\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_msprn" (
      echo ^<File RelativePath=".\ijs\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_mswin" (
      echo ^<File RelativePath=".\ijs\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_ntfs" (
      echo ^<File RelativePath=".\ijs\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_wgetv" (
      echo ^<File RelativePath=".\ijs\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "ijs_exec_win" (
      echo ^<File RelativePath=".\ijs\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else if /i "%%~no" EQU "gp_win32" (
      echo ^<File RelativePath=".\ijs\%%~no.c"^>
      echo   ^<FileConfiguration Name="Debug|Win32"^>
      echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
      echo   ^</FileConfiguration^>
      echo ^</File^>
    ) else echo ^<File RelativePath=".\ijs\%%~no.c"/^>
  ) else echo ^<!-- %%~no.c not found in search path --^>
)
endlocal

rem HACK
echo ^<File RelativePath=".\psi\zfjbig2.c"/^>
echo ^<File RelativePath=".\debugobj\gscdefs.c"/^>
echo ^<File RelativePath=".\debugobj\iconfig.c"/^>
echo ^<File RelativePath=".\debugobj\gconfig.c"/^>
echo ^<File RelativePath=".\jpeg\jfdctfst.c"/^>
echo ^<File RelativePath=".\jpeg\jidctflt.c"/^>
echo ^<File RelativePath=".\jpeg\jfdctflt.c"/^>
echo ^<File RelativePath=".\jpeg\jcphuff.c"/^>
echo ^<File RelativePath=".\jpeg\jidctfst.c"/^>
echo ^<File RelativePath=".\jpeg\jidctred.c"/^>
echo ^<File RelativePath=".\jpeg\jdmerge.c"/^>
echo ^<File RelativePath=".\jpeg\jquant1.c"/^>
echo ^<File RelativePath=".\jpeg\jquant2.c"/^>
echo ^<File RelativePath=".\debugobj\gsromfs1.c"/^>

REM Add the DLL source files
echo ^<File RelativePath=".\psi\gsdll.c"^>
echo   ^<FileConfiguration Name="Debug|Win32"^>
echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
echo   ^</FileConfiguration^>
echo ^</File^>
echo ^<File RelativePath=".\base\gp_msdll.c"^>
echo   ^<FileConfiguration Name="Debug|Win32"^>
echo     ^<Tool Name="VCCLCompilerTool" DisableLanguageExtensions="false" /^>
echo   ^</FileConfiguration^>
echo ^</File^>
echo ^<File 
echo   RelativePath=".\psi\gsdll32.rc"
echo   ^>
echo ^<FileConfiguration
echo Name="Debug|Win32"
echo ^>
echo ^<Tool
echo Name="VCResourceCompilerTool"
echo AdditionalIncludeDirectories=".\debugobj;&quot;$(SolutionDir)base&quot;"
echo /^>
echo ^</FileConfiguration^>
echo ^</File^>

rem remaining boilerplate
echo  ^</Filter^>
echo ^</Files^>
echo ^</VisualStudioProject^>
goto end

:echogs
echo ^<?xml version="1.0" encoding="utf-8" standalone="no"?^>
echo ^<VisualStudioProject
echo  ProjectType="Visual C++"
echo  Version="8.00"
echo  Name="echogs"
echo  ProjectGUID="{A1C8B0BC-2B54-4FDF-8EFF-BA84F5214B91}"
echo  RootNamespace="gs"
echo  Keyword="Win32Proj"
echo ^>
echo ^<Platforms^>
echo  ^<Platform Name="Win32"/^>
echo ^</Platforms^>
echo ^<ToolFiles^>
echo ^</ToolFiles^>
echo ^<Configurations^>
echo  ^<Configuration 
echo    Name="Debug|Win32"
echo    OutputDirectory="$(SolutionDir)debugbin"
echo    IntermediateDirectory="$(SolutionDir)debugobj"
echo    InheritedPropertySheets="$(SolutionDir)macros.vsprops"
echo    ConfigurationType="1"
echo    CharacterSet="0"
echo  ^>
echo  ^<Tool Name="VCCLCompilerTool"
echo    Optimization="0"
echo    PreprocessorDefinitions="__WIN32__;_DEBUG;_CRT_SECURE_NO_DEPRECATE;_CONSOLE"
echo    AdditionalIncludeDirectories="$(SolutionDir)base;$(SolutionDir)psi;$(SolutionDir)debugobj"
echo    MinimalRebuild="true"
echo    BasicRuntimeChecks="3"
echo    UsePrecompiledHeader="0"
echo    WarningLevel="2"
echo    Detect64BitPortabilityProblems="true"
echo    DebugInformationFormat="4"
echo    CompileAs="1"
echo  /^>
echo  ^<Tool
echo    Name="VCPreBuildEventTool"
echo    CommandLine="copy $(SolutionDir)psi\iconf.c $(IntDir)\iconfig.c&#x0D;&#x0A;copy $(SolutionDir)base\gconf.c $(IntDir)\gconfig.c&#x0D;&#x0A;copy $(SolutionDir)gconfig.h $(IntDir)\gconfig.h&#x0D;&#x0A;"
echo  /^>
echo  ^<Tool
echo    Name="VCResourceCompilerTool"
echo    PreprocessorDefinitions="_DEBUG"
echo    Culture="1033"
echo  /^>
echo  ^<Tool Name="VCLinkerTool"
echo    LinkIncremental="2"
echo    GenerateDebugInformation="true"
echo    SubSystem="1"
echo    TargetMachine="1"
echo   /^>
echo  ^<Tool
echo    Name="VCPostBuildEventTool"
echo    CommandLine="$(OutDir)\echogs -w $(IntDir)\gconfigd.h -x 23 define -s -u GS_LIB_DEFAULT -x 2022 $(GS_LIB_DEFAULT) -x 22&#x0D;&#x0A;$(OutDir)\echogs -a $(IntDir)\gconfigd.h -x 23 define -s -u GS_DEV_DEFAULT -x 2022 $(GS_DEV_DEFAULT) -x 22&#x0D;&#x0A;$(OutDir)\echogs -a $(IntDir)\gconfigd.h -x 23 define -s -u GS_CACHE_DIR -x 2022 $(GS_CACHE_DIR) -x 22&#x0D;&#x0A;$(OutDir)\echogs -a $(IntDir)\gconfigd.h -x 23 define -s -u SEARCH_HERE_FIRST -s $(SEARCH_HERE_FIRST)&#x0D;&#x0A;$(OutDir)\echogs -a $(IntDir)\gconfigd.h -x 23 define -s -u GS_DOCDIR -x 2022 $(GS_DOCDIR) -x 22&#x0D;&#x0A;$(OutDir)\echogs -a $(IntDir)\gconfigd.h -x 23 define -s -u GS_INIT -x 2022 $(GS_INIT) -x 22&#x0D;&#x0A;$(OutDir)\echogs -a $(IntDir)\gconfigd.h -x 23 define -s -u GS_REVISION -s $(GS_REVISION)&#x0D;&#x0A;$(OutDir)\echogs -a $(IntDir)\gconfigd.h -x 23 define -s -u GS_REVISIONDATE -s $(GS_REVISIONDATE)&#x0D;&#x0A;"
echo  /^>
echo  ^</Configuration^>
echo ^</Configurations^>
echo ^<References^>
echo ^</References^>
echo ^<Files^>
echo ^<Filter Name="echogs utility Source Files"
echo         Filter="c"^>
echo ^<File RelativePath=".\base\echogs.c"/^>
echo  ^</Filter^>
echo ^</Files^>
echo ^</VisualStudioProject^>
goto end

:genarch
echo ^<?xml version="1.0" encoding="utf-8" standalone="no"?^>
echo ^<VisualStudioProject
echo  ProjectType="Visual C++"
echo  Version="8.00"
echo  Name="genarch"
echo  ProjectGUID="{A1C8B0BC-2B54-4FDF-8EFF-BA84F5214B92}"
echo  RootNamespace="gs"
echo  Keyword="Win32Proj"
echo ^>
echo ^<Platforms^>
echo  ^<Platform Name="Win32"/^>
echo ^</Platforms^>
echo ^<ToolFiles^>
echo ^</ToolFiles^>
echo ^<Configurations^>
echo  ^<Configuration 
echo    Name="Debug|Win32"
echo    OutputDirectory="$(SolutionDir)debugobj"
echo    IntermediateDirectory="$(SolutionDir)debugobj"
echo    InheritedPropertySheets="$(SolutionDir)macros.vsprops"
echo    ConfigurationType="1"
echo    CharacterSet="0"
echo  ^>
echo  ^<Tool Name="VCCLCompilerTool"
echo    Optimization="0"
echo    PreprocessorDefinitions="__WIN32__;_DEBUG;_CRT_SECURE_NO_DEPRECATE;_CONSOLE;GX_COLOR_INDEX_TYPE=unsigned __int64"
echo    AdditionalIncludeDirectories="$(SolutionDir)base;$(SolutionDir)psi;$(SolutionDir)debugobj"
echo    MinimalRebuild="true"
echo    BasicRuntimeChecks="3"
echo    UsePrecompiledHeader="0"
echo    WarningLevel="2"
echo    Detect64BitPortabilityProblems="true"
echo    DebugInformationFormat="4"
echo    CompileAs="1"
echo  /^>
echo  ^<Tool
echo    Name="VCResourceCompilerTool"
echo    PreprocessorDefinitions="_DEBUG"
echo    Culture="1033"
echo  /^>
echo  ^<Tool Name="VCLinkerTool"
echo    LinkIncremental="2"
echo    GenerateDebugInformation="true"
echo    SubSystem="1"
echo    TargetMachine="1"
echo   /^>
echo   ^<Tool
echo     Name="VCPostBuildEventTool"
echo     CommandLine="$(OutDir)\genarch.exe $(IntDir)\arch.h"
echo   /^>
echo  ^</Configuration^>
echo ^</Configurations^>
echo ^<References^>
echo ^</References^>
echo ^<Files^>
echo ^<Filter Name="genarch utility Source Files"
echo         Filter="c"^>
echo ^<File RelativePath=".\base\genarch.c"/^>
echo  ^</Filter^>
echo ^</Files^>
echo ^</VisualStudioProject^>
goto end

:mkromfs
echo ^<?xml version="1.0" encoding="utf-8" standalone="no"?^>
echo ^<VisualStudioProject
echo  ProjectType="Visual C++"
echo  Version="8.00"
echo  Name="mkromfs"
echo  ProjectGUID="{45156C0D-CC16-4155-9D1F-063300029610}"
echo  RootNamespace="mkromfs"
echo  Keyword="Win32Proj"
echo ^>
echo ^<Platforms^>
echo  ^<Platform Name="Win32"/^>
echo ^</Platforms^>
echo ^<ToolFiles^>
echo ^</ToolFiles^>
echo ^<Configurations^>
echo  ^<Configuration 
echo    Name="Debug|Win32"
echo    OutputDirectory="$(SolutionDir)debugbin"
echo    IntermediateDirectory="$(SolutionDir)debugobj"
echo    InheritedPropertySheets="$(SolutionDir)macros.vsprops"
echo    ConfigurationType="1"
echo    CharacterSet="0"
echo  ^>
echo  ^<Tool Name="VCCLCompilerTool"
echo    Optimization="0"
echo    PreprocessorDefinitions="__WIN32__;_DEBUG;_CRT_SECURE_NO_DEPRECATE;_CONSOLE;_Windows"
echo   AdditionalIncludeDirectories="$(SolutionDir)zlib;$(SolutionDir)base;$(SolutionDir)psi;$(SolutionDir)debugobj"
echo    MinimalRebuild="true"
echo    BasicRuntimeChecks="3"
echo    UsePrecompiledHeader="0"
echo    WarningLevel="2"
echo    Detect64BitPortabilityProblems="true"
echo    DebugInformationFormat="4"
echo    CompileAs="1"
echo  /^>
echo  ^<Tool
echo    Name="VCResourceCompilerTool"
echo    PreprocessorDefinitions="_DEBUG"
echo    Culture="1033"
echo  /^>
echo  ^<Tool Name="VCLinkerTool"
echo    LinkIncremental="2"
echo    GenerateDebugInformation="true"
echo    SubSystem="1"
echo    TargetMachine="1"
echo   /^>
echo  ^<Tool
echo    Name="VCPreBuildEventTool"
echo    CommandLine="$(OutDir)\echogs -wb $(IntDir)\gswin.ico -n -X -r $(SolutionDir)base\gswin.icx&#x0D;&#x0A;$(OutDir)\echogs -w $(IntDir)\gconfig_.h -x 2f2a20 This file deliberately left blank. -x 2a2f&#x0D;&#x0A;$(OutDir)\echogs -w $(IntDir)\jconfig.h -+R $(SolutionDir)base\stdpn.h -+R $(SolutionDir)base\stdpre.h -+R $(SolutionDir)base\gsjconf.h&#x0D;&#x0A;copy $(SolutionDir)base\gsjmorec.h $(IntDir)\jmorecfg.h&#x0D;&#x0A;copy $(SolutionDir)jpeg\jmorecfg.h $(IntDir)\jmcorig.h&#x0D;&#x0A;copy $(SolutionDir)jpeg\jpeglib.h $(IntDir)\jpeglib_.h&#x0D;&#x0A;copy $(SolutionDir)base\gscdef.c $(IntDir)\gscdefs.c&#x0D;&#x0A;"
echo  /^>
echo   ^<Tool
echo     Name="VCPostBuildEventTool"
echo     CommandLine="$(OutDir)\mkromfs.exe -o $(SolutionDir)debugobj\gsromfs1.c -X .svn -X CVS -c -P .\Resource\ -d Resource/ CIDFont\ CMap\ ColorSpace\ Decoding\ Encoding\ Font\ IdiomSet\ Init\ ProcSet\ SubstCID\ -d lib/ -P .\lib\"
echo   /^>
echo  ^</Configuration^>
echo ^</Configurations^>
echo ^<References^>
echo ^</References^>
echo ^<Files^>
echo ^<Filter Name="mkromfs utility Source Files"
echo         Filter="c"^>
echo ^<File RelativePath=".\base\mkromfs.c"/^>
echo ^<File RelativePath=".\zlib\adler32.c"/^>
echo ^<File RelativePath=".\zlib\compress.c"/^>
echo ^<File RelativePath=".\zlib\crc32.c"/^>
echo ^<File RelativePath=".\zlib\deflate.c"/^>
echo ^<File RelativePath=".\zlib\trees.c"/^>
echo ^<File RelativePath=".\zlib\zutil.c"/^>
echo ^<File RelativePath=".\base\gp_getnv.c"/^>
echo ^<File RelativePath=".\base\gp_ntfs.c"/^>
echo ^<File RelativePath=".\base\gp_win32.c"/^>
echo ^<File RelativePath=".\base\gpmisc.c"/^>
echo  ^</Filter^>
echo ^</Files^>
echo ^</VisualStudioProject^>
goto end

:GUI
echo ^<?xml version="1.0" encoding="utf-8" standalone="no"?^>
echo ^<VisualStudioProject
echo  ProjectType="Visual C++"
echo  Version="8.00"
echo  Name="gswin32"
echo  ProjectGUID="{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}"
echo  RootNamespace="gs"
echo  Keyword="Win32Proj"
echo ^>
echo ^<Platforms^>
echo  ^<Platform Name="Win32"/^>
echo ^</Platforms^>
echo ^<ToolFiles^>
echo ^</ToolFiles^>
echo ^<Configurations^>
echo  ^<Configuration 
echo    Name="Debug|Win32"
echo    IntermediateDirectory="$(SolutionDir)\$(ConfigurationName)debugobj"
echo    OutputDirectory="$(SolutionDir)\$(ConfigurationName)debugbin"
echo    InheritedPropertySheets="$(SolutionDir)macros.vsprops"
echo    ConfigurationType="1"
echo    CharacterSet="0"
echo  ^>
echo  ^<Tool Name="VCCLCompilerTool"
echo    Optimization="0"
echo    PreprocessorDefinitions="__WIN32__;_DEBUG;_CRT_SECURE_NO_DEPRECATE;_WINDOWS;WIN32"
echo    AdditionalIncludeDirectories="$(SolutionDir)base;$(SolutionDir)psi;$(SolutionDir)debugobj"
echo    MinimalRebuild="true"
echo    BasicRuntimeChecks="3"
echo    UsePrecompiledHeader="0"
echo    WarningLevel="2"
echo    Detect64BitPortabilityProblems="true"
echo    DebugInformationFormat="4"
echo    CompileAs="1"
echo  /^>
echo  ^<Tool
echo    Name="VCResourceCompilerTool"
echo    PreprocessorDefinitions="_DEBUG"
echo    Culture="1033"
echo  /^>
echo  ^<Tool Name="VCLinkerTool"
echo    LinkIncremental="2"
echo    GenerateDebugInformation="true"
echo    SubSystem="2"
echo    TargetMachine="1"
echo   /^>
echo  ^</Configuration^>
echo  ^<Configuration 
echo    Name="Release|Win32"
echo    IntermediateDirectory="$(SolutionDir)\$(ConfigurationName)obj"
echo    OutputDirectory="$(SolutionDir)\$(ConfigurationName)bin"
echo    InheritedPropertySheets="$(SolutionDir)macros.vsprops"
echo    ConfigurationType="1"
echo    CharacterSet="0"
echo  ^>
echo  ^<Tool Name="VCCLCompilerTool"
echo    Optimization="2"
echo    PreprocessorDefinitions="__WIN32__;_CRT_SECURE_NO_DEPRECATE;_WINDOWS;WIN32"
echo    AdditionalIncludeDirectories="$(SolutionDir)base;$(SolutionDir)psi;$(SolutionDir)debugobj"
echo    MinimalRebuild="true"
echo    BasicRuntimeChecks="3"
echo    UsePrecompiledHeader="0"
echo    WarningLevel="2"
echo    Detect64BitPortabilityProblems="true"
echo    DebugInformationFormat="0"
echo    CompileAs="1"
echo  /^>
echo  ^<Tool
echo    Name="VCResourceCompilerTool"
echo    PreprocessorDefinitions="_DEBUG"
echo    Culture="1033"
echo  /^>
echo  ^<Tool Name="VCLinkerTool"
echo    LinkIncremental="2"
echo    GenerateDebugInformation="true"
echo    SubSystem="2"
echo    TargetMachine="1"
echo   /^>
echo  ^</Configuration^>
echo ^</Configurations^>
echo ^<References^>
echo ^</References^>
echo ^<Files^>
echo ^<Filter Name="GUI executable Source Files"
echo         Filter="c"^>
echo ^<File RelativePath=".\psi\dwmain.c"/^>
echo ^<File RelativePath=".\psi\dwimg.c"/^>
echo ^<File RelativePath=".\psi\dwdll.c"/^>
echo ^<File RelativePath=".\psi\dwreg.c"/^>
echo ^<File RelativePath=".\psi\dwtext.c"/^>
echo ^<File RelativePath=".\base\gp_getnv.c"/^>
echo ^<File RelativePath=".\base\gscdef.c"/^>
echo ^<File RelativePath=".\debugbin\gsdll32.lib"/^>
echo  ^</Filter^>
echo ^</Files^>
echo ^</VisualStudioProject^>
goto end

:Command
echo ^<?xml version="1.0" encoding="utf-8" standalone="no"?^>
echo ^<VisualStudioProject
echo  ProjectType="Visual C++"
echo  Version="8.00"
echo  Name="gswin32c"
echo  ProjectGUID="{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}" 
echo  RootNamespace="gs"
echo  Keyword="Win32Proj"
echo ^>
echo ^<Platforms^>
echo  ^<Platform Name="Win32"/^>
echo ^</Platforms^>
echo ^<ToolFiles^>
echo ^</ToolFiles^>
echo ^<Configurations^>
echo  ^<Configuration
echo    Name="Debug|Win32"
echo    InheritedPropertySheets="$(SolutionDir)macros.vsprops"
echo    IntermediateDirectory="$(SolutionDir)\$(ConfigurationName)debugobj"
echo    OutputDirectory="$(SolutionDir)\$(ConfigurationName)debugbin"
echo    CharacterSet="0"
echo  ^>
echo  ^<Tool Name="VCCLCompilerTool"
echo    Optimization="0"
echo    PreprocessorDefinitions="__WIN32__;_DEBUG;_CRT_SECURE_NO_DEPRECATE;_CONSOLE"
echo    AdditionalIncludeDirectories="$(SolutionDir)base;$(SolutionDir)psi;$(SolutionDir)debugobj"
echo    MinimalRebuild="true"
echo    BasicRuntimeChecks="3"
echo    UsePrecompiledHeader="0"
echo    WarningLevel="2"
echo    Detect64BitPortabilityProblems="true"
echo    DebugInformationFormat="4"
echo    CompileAs="1"
echo  /^>
echo  ^<Tool
echo    Name="VCResourceCompilerTool"
echo    PreprocessorDefinitions="_DEBUG"
echo    Culture="1033"
echo  /^>
echo  ^<Tool Name="VCLinkerTool"
echo    LinkIncremental="2"
echo    GenerateDebugInformation="true"
echo    SubSystem="1"
echo    TargetMachine="1"
echo   /^>
echo  ^</Configuration^>
echo  ^<Configuration
echo    Name="Release|Win32"
echo    InheritedPropertySheets="$(SolutionDir)macros.vsprops"
echo    IntermediateDirectory="$(SolutionDir)\$(ConfigurationName)obj"
echo    OutputDirectory="$(SolutionDir)\$(ConfigurationName)bin"
echo    CharacterSet="0"
echo  ^>
echo  ^<Tool Name="VCCLCompilerTool"
echo    Optimization="2"
echo    PreprocessorDefinitions="__WIN32__;_CRT_SECURE_NO_DEPRECATE;_CONSOLE"
echo    AdditionalIncludeDirectories="$(SolutionDir)base;$(SolutionDir)psi;$(SolutionDir)debugobj"
echo    MinimalRebuild="true"
echo    BasicRuntimeChecks="3"
echo    UsePrecompiledHeader="0"
echo    WarningLevel="2"
echo    Detect64BitPortabilityProblems="true"
echo    DebugInformationFormat="0"
echo    CompileAs="1"
echo  /^>
echo  ^<Tool
echo    Name="VCResourceCompilerTool"
echo    PreprocessorDefinitions="_DEBUG"
echo    Culture="1033"
echo  /^>
echo  ^<Tool Name="VCLinkerTool"
echo    LinkIncremental="2"
echo    GenerateDebugInformation="true"
echo    SubSystem="1"
echo    TargetMachine="1"
echo   /^>
echo  ^</Configuration^>
echo ^</Configurations^>
echo ^<References^>
echo ^</References^>
echo ^<Files^>
echo ^<Filter Name="command line executable Source Files"
echo         Filter="c"^>
echo ^<File RelativePath=".\psi\dwmainc.c"/^>
echo ^<File RelativePath=".\psi\dwimg.c"/^>
echo ^<File RelativePath=".\psi\dwdll.c"/^>
echo ^<File RelativePath=".\psi\dwreg.c"/^>
echo ^<File RelativePath=".\psi\dwtext.c"/^>
echo ^<File RelativePath=".\base\gp_getnv.c"/^>
echo ^<File RelativePath=".\base\gscdef.c"/^>
echo ^<File RelativePath=".\debugbin\gsdll32.lib"/^>
echo  ^</Filter^>
echo ^</Files^>
echo ^</VisualStudioProject^>
goto end

:props
echo ^<?xml version="1.0"?^>
echo ^<VisualStudioPropertySheet 
echo	ProjectType="Visual C++" 
echo	Version="8.00" 
echo	Name="macros"
echo	^>
echo   ^<UserMacro Name="GS_VERSION_MAJOR" Value="8" /^>
echo   ^<UserMacro Name="GS_VERSION_MINOR" Value="64" /^>
echo   ^<UserMacro Name="GS_VERSION_MINOR0" Value="64" /^>
echo   ^<UserMacro Name="GS_VERSION" Value="$(GS_VERSION_MAJOR)$(GS_VERSION_MINOR0)" /^>
echo   ^<UserMacro Name="GS_DOT_VERSION" Value="$(GS_VERSION_MAJOR).$(GS_VERSION_MINOR0)" /^>
echo   ^<UserMacro Name="AROOTDIR" Value="c:/gs" /^>
echo   ^<UserMacro Name="GSROOTDIR" Value="$(AROOTDIR)/gs$(GS_DOT_VERSION)" /^>
echo   ^<UserMacro Name="GS_REVISIONDATE" Value="20090202" /^>
echo   ^<UserMacro Name="GS_DEV_DEFAULT" Value="" /^>
echo   ^<UserMacro Name="GS_CACHE_DIR" Value="" /^>
echo   ^<UserMacro Name="GS_DOCDIR" Value="$(GSROOTDIR)/doc" /^>
echo   ^<UserMacro Name="GS_INIT" Value="gs_init.ps" /^>
echo   ^<UserMacro Name="GS_LIB_DEFAULT" Value="$(GSROOTDIR)/lib;$(GSROOTDIR)/Resource/Font;$(AROOTDIR)/fonts" /^>
echo   ^<UserMacro Name="GS_REVISION" Value="$(GS_VERSION)" /^>
echo   ^<UserMacro Name="SEARCH_HERE_FIRST" Value="1" /^>
echo ^</VisualStudioPropertySheet^>
goto end

:gconfig
echo /* This file was generated automatically by genconf.c. */
echo #ifdef compositor_
echo compositor_(gs_composite_alpha_type)
echo compositor_(gs_composite_overprint_type)
echo compositor_(gs_composite_pdf14trans_type)
echo #endif
echo #ifdef device_
echo device_(gs_display_device)
echo device_(gs_mswindll_device)
echo device_(gs_mswinpr2_device)
echo device_(gs_epson_device)
echo device_(gs_eps9high_device)
echo device_(gs_eps9mid_device)
echo device_(gs_epsonc_device)
echo device_(gs_ibmpro_device)
echo #endif
echo #ifdef device2_
echo device2_(gs_deskjet_device)
echo device2_(gs_djet500_device)
echo device2_(gs_laserjet_device)
echo device2_(gs_ljetplus_device)
echo device2_(gs_ljet2p_device)
echo #endif
echo #ifdef device_
echo device_(gs_cdeskjet_device)
echo device_(gs_cdjcolor_device)
echo device_(gs_cdjmono_device)
echo device_(gs_cdj550_device)
echo device_(gs_uniprint_device)
echo device_(gs_djet500c_device)
echo device_(gs_declj250_device)
echo device_(gs_lj250_device)
echo device_(gs_ijs_device)
echo device_(gs_st800_device)
echo device_(gs_stcolor_device)
echo device_(gs_bj10e_device)
echo device_(gs_bj200_device)
echo device_(gs_t4693d2_device)
echo device_(gs_t4693d4_device)
echo device_(gs_t4693d8_device)
echo device_(gs_tek4696_device)
echo #endif
echo #ifdef device2_
echo device2_(gs_pcxmono_device)
echo device2_(gs_pcxgray_device)
echo device2_(gs_pcx16_device)
echo device2_(gs_pcx256_device)
echo device2_(gs_pcx24b_device)
echo device2_(gs_pcxcmyk_device)
echo device2_(gs_pbm_device)
echo device2_(gs_pbmraw_device)
echo device2_(gs_pgm_device)
echo device2_(gs_pgmraw_device)
echo device2_(gs_pgnm_device)
echo device2_(gs_pgnmraw_device)
echo device2_(gs_pkmraw_device)
echo device2_(gs_tiffcrle_device)
echo device2_(gs_tiffg3_device)
echo device2_(gs_tiffg32d_device)
echo device2_(gs_tiffg4_device)
echo device2_(gs_tifflzw_device)
echo device2_(gs_tiffpack_device)
echo device2_(gs_bmpmono_device)
echo device2_(gs_bmpgray_device)
echo device2_(gs_bmp16_device)
echo device2_(gs_bmp256_device)
echo device2_(gs_bmp16m_device)
echo device2_(gs_tiff12nc_device)
echo device2_(gs_tiff24nc_device)
echo device2_(gs_tiffgray_device)
echo device2_(gs_tiff32nc_device)
echo device2_(gs_tiffsep_device)
echo device2_(gs_psmono_device)
echo device2_(gs_bit_device)
echo device2_(gs_bitrgb_device)
echo device2_(gs_bitcmyk_device)
echo device2_(gs_pngmono_device)
echo device2_(gs_pnggray_device)
echo device2_(gs_png16_device)
echo device2_(gs_png256_device)
echo device2_(gs_png16m_device)
echo device2_(gs_pngalpha_device)
echo device2_(gs_jpeg_device)
echo device2_(gs_jpeggray_device)
echo device2_(gs_jpegcmyk_device)
echo device2_(gs_pdfwrite_device)
echo device2_(gs_pswrite_device)
echo device2_(gs_ps2write_device)
echo device2_(gs_epswrite_device)
echo device2_(gs_txtwrite_device)
echo device2_(gs_pxlmono_device)
echo device2_(gs_pxlcolor_device)
echo device2_(gs_svgwrite_device)
echo device2_(gs_bbox_device)
echo device2_(gs_ljet3_device)
echo device2_(gs_ljet3d_device)
echo device2_(gs_ljet4_device)
echo device2_(gs_ljet4d_device)
echo #endif
echo #ifdef device_
echo device_(gs_pj_device)
echo device_(gs_pjxl_device)
echo device_(gs_pjxl300_device)
echo device_(gs_jetp3852_device)
echo device_(gs_r4081_device)
echo device_(gs_lbp8_device)
echo device_(gs_m8510_device)
echo device_(gs_necp6_device)
echo device_(gs_bjc600_device)
echo device_(gs_bjc800_device)
echo #endif
echo #ifdef device2_
echo device2_(gs_pnm_device)
echo device2_(gs_pnmraw_device)
echo device2_(gs_ppm_device)
echo device2_(gs_ppmraw_device)
echo device2_(gs_pamcmyk32_device)
echo #endif
echo #ifdef device_
echo device_(gs_spotcmyk_device)
echo device_(gs_devicen_device)
echo #endif
echo #ifdef device2_
echo device2_(gs_bmpsep1_device)
echo device2_(gs_bmpsep8_device)
echo device2_(gs_bmp32b_device)
echo #endif
echo #ifdef device_
echo device_(gs_psdcmyk_device)
echo device_(gs_psdrgb_device)
echo #endif
echo #ifdef device2_
echo device2_(gs_nullpage_device)
echo #endif
echo #ifdef oper_
echo oper_(zbseq_l2_op_defs)
echo #endif
echo #ifdef psfile_
echo psfile_("gs_btokn.ps",11)
echo #endif
echo #ifdef oper_
echo oper_(zcolor1_op_defs)
echo oper_(zht1_op_defs)
echo oper_(zupath_l2_op_defs)
echo oper_(zvmem2_op_defs)
echo oper_(zdps1_l2_op_defs)
echo #endif
echo #ifdef psfile_
echo psfile_("gs_dps1.ps",10)
echo #endif
echo #ifdef oper_
echo oper_(ireclaim_l2_op_defs)
echo oper_(zcharx_op_defs)
echo #endif
echo #ifdef psfile_
echo psfile_("gs_dps2.ps",10)
echo #endif
echo #ifdef oper_
echo oper_(zchar1_op_defs)
echo oper_(zfont1_op_defs)
echo oper_(zmisc1_op_defs)
echo #endif
echo #ifdef psfile_
echo psfile_("gs_type1.ps",11)
echo #endif
echo #ifdef oper_
echo oper_(zusparam_op_defs)
echo oper_(zmisc2_op_defs)
echo #endif
echo #ifdef psfile_
echo psfile_("gs_lev2.ps",10)
echo psfile_("gs_res.ps",9)
echo #endif
echo #ifdef oper_
echo oper_(zchar2_op_defs)
echo oper_(zfont2_op_defs)
echo oper_(zchar42_op_defs)
echo oper_(zfont42_op_defs)
echo #endif
echo #ifdef psfile_
echo psfile_("gs_typ42.ps",11)
echo #endif
echo #ifdef oper_
echo oper_(zfrsd_op_defs)
echo oper_(zfcid0_op_defs)
echo oper_(zfcid1_op_defs)
echo #endif
echo #ifdef psfile_
echo psfile_("gs_cidfn.ps",11)
echo psfile_("gs_cidcm.ps",11)
echo psfile_("gs_fntem.ps",11)
echo psfile_("gs_cidtt.ps",11)
echo psfile_("gs_cidfm.ps",11)
echo #endif
echo #ifdef oper_
echo oper_(zcrd_l2_op_defs)
echo oper_(zfcmap_op_defs)
echo #endif
echo #ifdef psfile_
echo psfile_("gs_cmap.ps",10)
echo #endif
echo #ifdef oper_
echo oper_(zcfont_op_defs)
echo oper_(zfont0_op_defs)
echo oper_(zfdcte_op_defs)
echo oper_(zfdctd_op_defs)
echo oper_(zfdecode_op_defs)
echo oper_(zfilter2_op_defs)
echo oper_(ziodev2_l2_op_defs)
echo #endif
echo #ifdef io_device_
echo io_device_(gs_iodev_null)
echo io_device_(gs_iodev_calendar)
echo #endif
echo #ifdef oper_
echo oper_(zdevice2_l2_op_defs)
echo oper_(zmedia2_l2_op_defs)
echo #endif
echo #ifdef psfile_
echo psfile_("gs_setpd.ps",11)
echo #endif
echo #ifdef image_type_
echo image_type_(3,gs_image_type_3)
echo image_type_(4,gs_image_type_4)
echo #endif
echo #ifdef oper_
echo oper_(zpcolor_l2_op_defs)
echo oper_(zarith_op_defs)
echo oper_(zarray_op_defs)
echo oper_(zcontrol1_op_defs)
echo oper_(zcontrol2_op_defs)
echo oper_(zcontrol3_op_defs)
echo oper_(zdict1_op_defs)
echo oper_(zdict2_op_defs)
echo oper_(zfile_op_defs)
echo oper_(zfile1_op_defs)
echo oper_(zfileio1_op_defs)
echo oper_(zfileio2_op_defs)
echo oper_(zfilter_op_defs)
echo oper_(zfproc_op_defs)
echo oper_(zgeneric_op_defs)
echo oper_(ziodev_op_defs)
echo oper_(zmath_op_defs)
echo oper_(zalg_op_defs)
echo oper_(zmisc_op_defs)
echo oper_(zpacked_op_defs)
echo oper_(zrelbit_op_defs)
echo oper_(zstack_op_defs)
echo oper_(zstring_op_defs)
echo oper_(zsysvm_op_defs)
echo oper_(ztoken_op_defs)
echo oper_(ztype_op_defs)
echo oper_(zvmem_op_defs)
echo oper_(zbfont_op_defs)
echo oper_(zchar_a_op_defs)
echo oper_(zchar_b_op_defs)
echo oper_(zcolor_op_defs)
echo oper_(zcolor_ext_op_defs)
echo oper_(zdevice_op_defs)
echo oper_(zfont_op_defs)
echo oper_(zfontenum_op_defs)
echo oper_(zgstate1_op_defs)
echo oper_(zgstate2_op_defs)
echo oper_(zgstate3_op_defs)
echo oper_(zdfilter_op_defs)
echo oper_(zht_op_defs)
echo oper_(zimage_op_defs)
echo oper_(zmatrix_op_defs)
echo oper_(zmatrix2_op_defs)
echo oper_(zpaint_op_defs)
echo oper_(zpath_op_defs)
echo oper_(pantone_op_defs)
echo #endif
echo #ifdef io_device_
echo io_device_(gs_iodev_stdin)
echo io_device_(gs_iodev_stdout)
echo io_device_(gs_iodev_stderr)
echo io_device_(gs_iodev_lineedit)
echo io_device_(gs_iodev_statementedit)
echo #endif
echo #ifdef oper_
echo oper_(zdscpars_op_defs)
echo oper_(zfbcp_op_defs)
echo oper_(zpath1_op_defs)
echo #endif
echo #ifdef emulator_
echo emulator_("PostScript",10)
echo emulator_("PostScriptLevel1",16)
echo #endif
echo #ifdef oper_
echo oper_(zht2_l2_op_defs)
echo oper_(zcssepr_l2_op_defs)
echo oper_(zfsample_op_defs)
echo #endif
echo #ifdef function_type_
echo function_type_(4,gs_build_function_4)
echo #endif
echo #ifdef oper_
echo oper_(zfunc_op_defs)
echo #endif
echo #ifdef function_type_
echo function_type_(0,gs_build_function_0)
echo #endif
echo #ifdef oper_
echo oper_(zchar32_op_defs)
echo oper_(zfont32_op_defs)
echo #endif
echo #ifdef psfile_
echo psfile_("gs_typ32.ps",11)
echo #endif
echo #ifdef oper_
echo oper_(zfimscale_op_defs)
echo #endif
echo #ifdef emulator_
echo emulator_("PostScriptLevel2",16)
echo #endif
echo #ifdef psfile_
echo psfile_("gs_frsd.ps",10)
echo #endif
echo #ifdef oper_
echo oper_(zimage3_op_defs)
echo oper_(zmisc3_op_defs)
echo oper_(zcolor3_l3_op_defs)
echo oper_(zshade_op_defs)
echo #endif
echo #ifdef function_type_
echo function_type_(2,gs_build_function_2)
echo function_type_(3,gs_build_function_3)
echo #endif
echo #ifdef psfile_
echo psfile_("gs_ll3.ps",9)
echo #endif
echo #ifdef oper_
echo oper_(zfzlib_op_defs)
echo #endif
echo #ifdef emulator_
echo emulator_("PostScriptLevel3",16)
echo #endif
echo #ifdef psfile_
echo psfile_("gs_mex_e.ps",11)
echo psfile_("gs_mro_e.ps",11)
echo psfile_("gs_pdf_e.ps",11)
echo psfile_("gs_wan_e.ps",11)
echo #endif
echo #ifdef oper_
echo oper_(ztrans1_op_defs)
echo oper_(ztrans2_op_defs)
echo #endif
echo #ifdef image_type_
echo image_type_(3x,gs_image_type_3x)
echo #endif
echo #ifdef psfile_
echo psfile_("pdf_ops.ps",10)
echo psfile_("gs_l2img.ps",11)
echo psfile_("pdf_rbld.ps",11)
echo psfile_("pdf_base.ps",11)
echo psfile_("pdf_draw.ps",11)
echo psfile_("pdf_font.ps",11)
echo psfile_("pdf_main.ps",11)
echo psfile_("pdf_sec.ps",10)
echo psfile_("gs_css_e.ps",11)
echo psfile_("gs_cff.ps",9)
echo #endif
echo #ifdef oper_
echo oper_(zfmd5_op_defs)
echo oper_(zfarc4_op_defs)
echo oper_(zfaes_op_defs)
echo #endif
echo #ifdef psfile_
echo psfile_("gs_mgl_e.ps",11)
echo psfile_("gs_agl.ps",9)
echo psfile_("gs_ttf.ps",9)
echo #endif
echo #ifdef oper_
echo oper_(zicc_ll3_op_defs)
echo #endif
echo #ifdef psfile_
echo psfile_("gs_icc.ps",9)
echo #endif
echo #ifdef emulator_
echo emulator_("PDF",3)
echo #endif
echo #ifdef image_type_
echo image_type_(2,gs_image_type_2)
echo #endif
echo #ifdef oper_
echo oper_(zcontext1_op_defs)
echo oper_(zcontext2_op_defs)
echo oper_(zdps_op_defs)
echo #endif
echo #ifdef psfile_
echo psfile_("gs_dps.ps",9)
echo #endif
echo #ifdef oper_
echo oper_(zdpnext_op_defs)
echo #endif
echo #ifdef psfile_
echo psfile_("gs_dpnxt.ps",11)
echo psfile_("gs_epsf.ps",10)
echo #endif
echo #ifdef io_device_
echo io_device_(gs_iodev_handle)
echo io_device_(gs_iodev_printer)
echo io_device_(gs_iodev_pipe)
echo #endif
echo #ifdef oper_
echo oper_(zfapi_op_defs)
echo #endif
echo #ifdef psfile_
echo psfile_("gs_fapi.ps",10)
echo #endif
echo #ifdef oper_
echo oper_(zfjbig2_op_defs)
echo oper_(zfjpx_op_defs)
echo #endif
echo #ifdef init_
echo init_(gs_gxclfile_init)
echo init_(gs_gxclmem_init)
echo init_(gs_cl_zlib_init)
echo #endif
echo #ifdef psfile_
echo psfile_("gs_pdfwr.ps",11)
echo #endif
echo #ifdef io_device_
echo io_device_(gs_iodev_rom)
echo #endif
echo #ifdef init_
echo init_(gs_gshtscr_init)
echo #endif
echo #ifdef image_type_
echo image_type_(1,gs_image_type_1)
echo image_type_(mask1,gs_image_type_mask1)
echo #endif
echo #ifdef image_class_
echo image_class_(gs_image_class_0_interpolate)
echo image_class_(gs_image_class_1_simple)
echo image_class_(gs_image_class_2_fracs)
echo image_class_(gs_image_class_3_mono)
echo image_class_(gs_image_class_4_color)
echo #endif


:end

