# Microsoft Developer Studio Project File - Name="libjasper" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libjasper - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libjasper.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libjasper.mak" CFG="libjasper - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libjasper - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libjasper - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libjasper - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32_Release"
# PROP Intermediate_Dir "libjasper___Win32_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\libjasper\include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "JAS_WIN_MSVC_BUILD" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libjasper - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_Debug"
# PROP Intermediate_Dir "libjasper___Win32_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\libjasper\include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "JAS_WIN_MSVC_BUILD" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libjasper - Win32 Release"
# Name "libjasper - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\libjasper\bmp\bmp_cod.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\bmp\bmp_dec.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\bmp\bmp_enc.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\base\jas_cm.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\base\jas_debug.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\base\jas_getopt.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\base\jas_icc.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\base\jas_iccdata.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\base\jas_image.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\base\jas_init.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\base\jas_malloc.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\base\jas_seq.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\base\jas_stream.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\base\jas_string.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\base\jas_tvp.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\base\jas_version.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jp2\jp2_cod.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jp2\jp2_dec.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jp2\jp2_enc.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_bs.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_cs.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_dec.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_enc.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_math.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_mct.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_mqcod.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_mqdec.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_mqenc.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_qmfb.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_t1cod.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_t1dec.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_t1enc.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_t2cod.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_t2dec.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_t2enc.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_tagtree.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_tsfb.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_util.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpg\jpg_dummy.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpg\jpg_val.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\mif\mif_cod.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\pgx\pgx_cod.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\pgx\pgx_dec.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\pgx\pgx_enc.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\pnm\pnm_cod.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\pnm\pnm_dec.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\pnm\pnm_enc.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\ras\ras_cod.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\ras\ras_dec.c
# End Source File
# Begin Source File

SOURCE=..\libjasper\ras\ras_enc.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\libjasper\bmp\bmp_cod.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\include\jasper\jas_cm.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\include\jasper\jas_icc.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jp2\jp2_cod.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jp2\jp2_dec.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_bs.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_cod.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_cs.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_dec.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_enc.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_fix.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_flt.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_math.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_mct.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_mqcod.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_mqdec.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_mqenc.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_qmfb.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_t1cod.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_t1dec.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_t1enc.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_t2cod.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_t2dec.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_t2enc.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_tagtree.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_tsfb.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpc\jpc_util.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\jpg\jpg_cod.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\mif\mif_cod.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\pgx\pgx_cod.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\pnm\pnm_cod.h
# End Source File
# Begin Source File

SOURCE=..\libjasper\ras\ras_cod.h
# End Source File
# End Group
# End Target
# End Project
