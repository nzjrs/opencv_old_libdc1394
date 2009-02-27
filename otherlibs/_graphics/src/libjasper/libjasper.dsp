# Microsoft Developer Studio Project File - Name="libjasper" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libjasper - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libjasper.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libjasper.mak" CFG="libjasper - Win32 Release"
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
# PROP Output_Dir "..\..\..\..\_temp\libjasper_Release"
# PROP Intermediate_Dir "..\..\..\..\_temp\libjasper_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "JAS_WIN_MSVC_BUILD" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\libjasper.lib"

!ELSEIF  "$(CFG)" == "libjasper - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\_temp\libjasper_Debug"
# PROP Intermediate_Dir "..\..\..\..\_temp\libjasper_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "JAS_WIN_MSVC_BUILD" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\libjasperd.lib"

!ENDIF 

# Begin Target

# Name "libjasper - Win32 Release"
# Name "libjasper - Win32 Debug"
# Begin Source File

SOURCE=.\jas_cm.c
# End Source File
# Begin Source File

SOURCE=..\..\include\jasper\jas_cm.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jasper\jas_config.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jasper\jas_config2.h
# End Source File
# Begin Source File

SOURCE=.\jas_debug.c
# End Source File
# Begin Source File

SOURCE=..\..\include\jasper\jas_debug.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jasper\jas_fix.h
# End Source File
# Begin Source File

SOURCE=.\jas_getopt.c
# End Source File
# Begin Source File

SOURCE=..\..\include\jasper\jas_getopt.h
# End Source File
# Begin Source File

SOURCE=.\jas_icc.c
# End Source File
# Begin Source File

SOURCE=..\..\include\jasper\jas_icc.h
# End Source File
# Begin Source File

SOURCE=.\jas_iccdata.c
# End Source File
# Begin Source File

SOURCE=.\jas_image.c
# End Source File
# Begin Source File

SOURCE=..\..\include\jasper\jas_image.h
# End Source File
# Begin Source File

SOURCE=.\jas_init.c
# End Source File
# Begin Source File

SOURCE=..\..\include\jasper\jas_init.h
# End Source File
# Begin Source File

SOURCE=.\jas_malloc.c
# End Source File
# Begin Source File

SOURCE=..\..\include\jasper\jas_malloc.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jasper\jas_math.h
# End Source File
# Begin Source File

SOURCE=.\jas_seq.c
# End Source File
# Begin Source File

SOURCE=..\..\include\jasper\jas_seq.h
# End Source File
# Begin Source File

SOURCE=.\jas_stream.c
# End Source File
# Begin Source File

SOURCE=..\..\include\jasper\jas_stream.h
# End Source File
# Begin Source File

SOURCE=.\jas_string.c
# End Source File
# Begin Source File

SOURCE=..\..\include\jasper\jas_string.h
# End Source File
# Begin Source File

SOURCE=.\jas_tmr.c
# End Source File
# Begin Source File

SOURCE=..\..\include\jasper\jas_tmr.h
# End Source File
# Begin Source File

SOURCE=.\jas_tvp.c
# End Source File
# Begin Source File

SOURCE=..\..\include\jasper\jas_tvp.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jasper\jas_types.h
# End Source File
# Begin Source File

SOURCE=.\jas_version.c
# End Source File
# Begin Source File

SOURCE=..\..\include\jasper\jas_version.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jasper\jasper.h
# End Source File
# Begin Source File

SOURCE=.\jp2_cod.c
# End Source File
# Begin Source File

SOURCE=.\jp2_cod.h
# End Source File
# Begin Source File

SOURCE=.\jp2_dec.c
# End Source File
# Begin Source File

SOURCE=.\jp2_dec.h
# End Source File
# Begin Source File

SOURCE=.\jp2_enc.c
# End Source File
# Begin Source File

SOURCE=.\jpc_bs.c
# End Source File
# Begin Source File

SOURCE=.\jpc_bs.h
# End Source File
# Begin Source File

SOURCE=.\jpc_cod.h
# End Source File
# Begin Source File

SOURCE=.\jpc_cs.c
# End Source File
# Begin Source File

SOURCE=.\jpc_cs.h
# End Source File
# Begin Source File

SOURCE=.\jpc_dec.c
# End Source File
# Begin Source File

SOURCE=.\jpc_dec.h
# End Source File
# Begin Source File

SOURCE=.\jpc_enc.c
# End Source File
# Begin Source File

SOURCE=.\jpc_enc.h
# End Source File
# Begin Source File

SOURCE=.\jpc_fix.h
# End Source File
# Begin Source File

SOURCE=.\jpc_flt.h
# End Source File
# Begin Source File

SOURCE=.\jpc_math.c
# End Source File
# Begin Source File

SOURCE=.\jpc_math.h
# End Source File
# Begin Source File

SOURCE=.\jpc_mct.c
# End Source File
# Begin Source File

SOURCE=.\jpc_mct.h
# End Source File
# Begin Source File

SOURCE=.\jpc_mqcod.c
# End Source File
# Begin Source File

SOURCE=.\jpc_mqcod.h
# End Source File
# Begin Source File

SOURCE=.\jpc_mqdec.c
# End Source File
# Begin Source File

SOURCE=.\jpc_mqdec.h
# End Source File
# Begin Source File

SOURCE=.\jpc_mqenc.c
# End Source File
# Begin Source File

SOURCE=.\jpc_mqenc.h
# End Source File
# Begin Source File

SOURCE=.\jpc_qmfb.c
# End Source File
# Begin Source File

SOURCE=.\jpc_qmfb.h
# End Source File
# Begin Source File

SOURCE=.\jpc_t1cod.c
# End Source File
# Begin Source File

SOURCE=.\jpc_t1cod.h
# End Source File
# Begin Source File

SOURCE=.\jpc_t1dec.c
# End Source File
# Begin Source File

SOURCE=.\jpc_t1dec.h
# End Source File
# Begin Source File

SOURCE=.\jpc_t1enc.c
# End Source File
# Begin Source File

SOURCE=.\jpc_t1enc.h
# End Source File
# Begin Source File

SOURCE=.\jpc_t2cod.c
# End Source File
# Begin Source File

SOURCE=.\jpc_t2cod.h
# End Source File
# Begin Source File

SOURCE=.\jpc_t2dec.c
# End Source File
# Begin Source File

SOURCE=.\jpc_t2dec.h
# End Source File
# Begin Source File

SOURCE=.\jpc_t2enc.c
# End Source File
# Begin Source File

SOURCE=.\jpc_t2enc.h
# End Source File
# Begin Source File

SOURCE=.\jpc_tagtree.c
# End Source File
# Begin Source File

SOURCE=.\jpc_tagtree.h
# End Source File
# Begin Source File

SOURCE=.\jpc_tsfb.c
# End Source File
# Begin Source File

SOURCE=.\jpc_tsfb.h
# End Source File
# Begin Source File

SOURCE=.\jpc_util.c
# End Source File
# Begin Source File

SOURCE=.\jpc_util.h
# End Source File
# End Target
# End Project
