# Microsoft Developer Studio Project File - Name="libpsd" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libpsd - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LibPSD.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LibPSD.mak" CFG="libpsd - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libpsd - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "libpsd - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libpsd - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "libpsd - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libpsd - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_CRT_SECURE_NO_DEPRECATE" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x410 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libpsd - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_CRT_SECURE_NO_DEPRECATE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x410 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libpsd - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libpsd___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "libpsd___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode_Debug"
# PROP Intermediate_Dir "Unicode_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_AFXDLL" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_UNICODE" /D "UNICODE" /D "_CRT_SECURE_NO_DEPRECATE" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libpsd - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "libpsd___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "libpsd___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode_Release"
# PROP Intermediate_Dir "Unicode_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_UNICODE" /D "UNICODE" /D "_CRT_SECURE_NO_DEPRECATE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libpsd - Win32 Debug"
# Name "libpsd - Win32 Release"
# Name "libpsd - Win32 Unicode Debug"
# Name "libpsd - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\adjustment.c
# End Source File
# Begin Source File

SOURCE=.\bevel_emboss.c
# End Source File
# Begin Source File

SOURCE=.\bitmap.c
# End Source File
# Begin Source File

SOURCE=.\blend.c
# End Source File
# Begin Source File

SOURCE=.\boundary.c
# End Source File
# Begin Source File

SOURCE=.\brightness_contrast.c
# End Source File
# Begin Source File

SOURCE=.\channel_image.c
# End Source File
# Begin Source File

SOURCE=.\channel_mixer.c
# End Source File
# Begin Source File

SOURCE=.\color.c
# End Source File
# Begin Source File

SOURCE=.\color_balance.c
# End Source File
# Begin Source File

SOURCE=.\color_mode.c
# End Source File
# Begin Source File

SOURCE=.\color_overlay.c
# End Source File
# Begin Source File

SOURCE=.\curves.c
# End Source File
# Begin Source File

SOURCE=.\descriptor.c
# End Source File
# Begin Source File

SOURCE=.\drop_shadow.c
# End Source File
# Begin Source File

SOURCE=.\effects.c
# End Source File
# Begin Source File

SOURCE=.\file_header.c
# End Source File
# Begin Source File

SOURCE=.\fixed.c
# End Source File
# Begin Source File

SOURCE=.\gaussian_blur.c
# End Source File
# Begin Source File

SOURCE=.\gradient_blend.c
# End Source File
# Begin Source File

SOURCE=.\gradient_fill.c
# End Source File
# Begin Source File

SOURCE=.\gradient_map.c
# End Source File
# Begin Source File

SOURCE=.\gradient_overlay.c
# End Source File
# Begin Source File

SOURCE=.\hue_saturation.c
# End Source File
# Begin Source File

SOURCE=.\image_data.c
# End Source File
# Begin Source File

SOURCE=.\image_resource.c
# End Source File
# Begin Source File

SOURCE=.\inner_glow.c
# End Source File
# Begin Source File

SOURCE=.\inner_shadow.c
# End Source File
# Begin Source File

SOURCE=.\invert.c
# End Source File
# Begin Source File

SOURCE=.\layer_mask.c
# End Source File
# Begin Source File

SOURCE=.\levels.c
# End Source File
# Begin Source File

SOURCE=.\outer_glow.c
# End Source File
# Begin Source File

SOURCE=.\path.c
# End Source File
# Begin Source File

SOURCE=.\pattern.c
# End Source File
# Begin Source File

SOURCE=.\pattern_fill.c
# End Source File
# Begin Source File

SOURCE=.\pattern_overlay.c
# End Source File
# Begin Source File

SOURCE=.\photo_filter.c
# End Source File
# Begin Source File

SOURCE=.\posterize.c
# End Source File
# Begin Source File

SOURCE=.\psd.c
# End Source File
# Begin Source File

SOURCE=.\psd_system.c
# End Source File
# Begin Source File

SOURCE=.\psd_zip.c
# End Source File
# Begin Source File

SOURCE=.\rect.c
# End Source File
# Begin Source File

SOURCE=.\satin.c
# End Source File
# Begin Source File

SOURCE=.\selective_color.c
# End Source File
# Begin Source File

SOURCE=.\solid_color.c
# End Source File
# Begin Source File

SOURCE=.\stream.c
# End Source File
# Begin Source File

SOURCE=.\stroke.c
# End Source File
# Begin Source File

SOURCE=.\threshold.c
# End Source File
# Begin Source File

SOURCE=.\thumbnail.c
# End Source File
# Begin Source File

SOURCE=.\type_tool.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\libpsd.h
# End Source File
# Begin Source File

SOURCE=.\psd_bitmap.h
# End Source File
# Begin Source File

SOURCE=.\psd_blend.h
# End Source File
# Begin Source File

SOURCE=.\psd_color.h
# End Source File
# Begin Source File

SOURCE=.\psd_config.h
# End Source File
# Begin Source File

SOURCE=.\psd_descriptor.h
# End Source File
# Begin Source File

SOURCE=.\psd_fixed.h
# End Source File
# Begin Source File

SOURCE=.\psd_gradient.h
# End Source File
# Begin Source File

SOURCE=.\psd_math.h
# End Source File
# Begin Source File

SOURCE=.\psd_rect.h
# End Source File
# Begin Source File

SOURCE=.\psd_stream.h
# End Source File
# Begin Source File

SOURCE=.\psd_system.h
# End Source File
# Begin Source File

SOURCE=.\psd_types.h
# End Source File
# End Group
# End Target
# End Project
