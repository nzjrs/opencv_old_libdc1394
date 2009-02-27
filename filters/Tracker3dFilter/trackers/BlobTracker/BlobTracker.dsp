# Microsoft Developer Studio Project File - Name="BlobTracker" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BlobTracker - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BlobTracker.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BlobTracker.mak" CFG="BlobTracker - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BlobTracker - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BlobTracker - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BlobTracker - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\_temp\BlobTracker_Rls"
# PROP Intermediate_Dir "..\..\..\..\_temp\BlobTracker_Rls"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /Zi /O2 /Ob2 /I "..\..\..\..\CV\include" /I "..\..\..\..\cvaux\include" /I "..\..\include" /D DBG=0 /D "NDEBUG" /D "STRICT" /D "WIN32" /D "_WIN32" /D "_WINDOWS" /D "_MBCS" /D _X86_=1 /D WINVER=0x400 /D _WIN32_WINNT=0x0400 /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib advapi32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib advapi32.lib ole32.lib oleaut32.lib uuid.lib strmbase.lib winmm.lib cv.lib /nologo /entry:"DllEntryPoint@12" /dll /pdb:none /debug /machine:I386 /def:".\BlobTracker.def" /out:"..\..\..\..\bin\BlobTracker.dll" /libpath:"..\..\..\..\lib"

!ELSEIF  "$(CFG)" == "BlobTracker - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\_temp\BlobTracker_Dbg"
# PROP Intermediate_Dir "..\..\..\..\_temp\BlobTracker_Dbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /GX /Zi /Od /I "..\..\..\..\CV\include" /I "..\..\..\..\cvaux\include" /I "..\..\include" /D DBG=1 /D "DEBUG" /D "_DEBUG" /D "STRICT" /D "WIN32" /D "_WIN32" /D "_WINDOWS" /D "_MBCS" /D _X86_=1 /D WINVER=0x400 /D _WIN32_WINNT=0x0400 /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib advapi32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /pdb:none /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib advapi32.lib ole32.lib oleaut32.lib uuid.lib strmbasd.lib winmm.lib cvd.lib /nologo /entry:"DllEntryPoint@12" /dll /pdb:none /debug /machine:I386 /def:".\BlobTracker.def" /out:"..\..\..\..\bin\BlobTrackerd.dll" /libpath:"..\..\..\..\lib"

!ENDIF 

# Begin Target

# Name "BlobTracker - Win32 Release"
# Name "BlobTracker - Win32 Debug"
# Begin Source File

SOURCE=.\BlobTracker.cpp
# End Source File
# Begin Source File

SOURCE=.\BlobTracker.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\BlobTracker.h
# End Source File
# Begin Source File

SOURCE=.\BlobTrackerPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\BlobTrackerPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\BlobTrackerPropertyPage.rc
# End Source File
# Begin Source File

SOURCE=.\BlobTrkObject.h
# End Source File
# Begin Source File

SOURCE=.\BlobTrkOut.cpp
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Target
# End Project
