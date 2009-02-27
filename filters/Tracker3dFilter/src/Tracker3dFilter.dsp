# Microsoft Developer Studio Project File - Name="Tracker3dFilter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Tracker3dFilter - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Tracker3dFilter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Tracker3dFilter.mak" CFG="Tracker3dFilter - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Tracker3dFilter - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Tracker3dFilter - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Tracker3dFilter - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\_temp\Tracker3dFilter_Rls"
# PROP Intermediate_Dir "..\..\..\_temp\Tracker3dFilter_Rls"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /Zi /O2 /Ob2 /I "..\..\..\CV\include" /I "..\..\..\cvaux\include" /I "..\include" /D DBG=0 /D "NDEBUG" /D "STRICT" /D "WIN32" /D "_WIN32" /D _X86_=1 /D WINVER=0x400 /D _WIN32_WINNT=0x0400 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "WIN32"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 strmbase.lib cv.lib cvaux.lib winmm.lib kernel32.lib advapi32.lib user32.lib gdi32.lib shell32.lib comdlg32.lib ole32.lib oleaut32.lib uuid.lib /nologo /entry:"DllEntryPoint@12" /dll /pdb:none /debug /machine:I386 /out:"..\..\..\bin\Tracker3dFilter.ax" /libpath:"..\..\..\lib" /subsystem:windows,4.0 /opt:ref /release /debug:none
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "Tracker3dFilter - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\_temp\Tracker3dFilter_Dbg"
# PROP Intermediate_Dir "..\..\..\_temp\Tracker3dFilter_Dbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /MDd /W3 /GX /Zi /Od /Ob1 /Gy /I "..\..\..\CV\include" /I "..\..\..\cvaux\include" /I "..\include" /D DBG=1 /D "DEBUG" /D "_DEBUG" /D "STRICT" /D "WIN32" /D "_WIN32" /D _X86_=1 /D WINVER=0x0400 /D _WIN32_WINNT=0x0400 /YX"streams.h" /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "WIN32"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib /subsystem:windows /dll /debug /machine:I386 /pdbtype:none
# ADD LINK32 strmbasd.lib cvd.lib cvauxd.lib winmm.lib kernel32.lib advapi32.lib user32.lib gdi32.lib shell32.lib comdlg32.lib ole32.lib oleaut32.lib uuid.lib /nologo /entry:"DllEntryPoint@12" /dll /pdb:none /debug /machine:I386 /out:"..\..\..\bin\Tracker3dFilterd.ax" /libpath:"..\..\..\lib" /subsystem:windows,4.0
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "Tracker3dFilter - Win32 Release"
# Name "Tracker3dFilter - Win32 Debug"
# Begin Source File

SOURCE=.\FilenamesDialog.cpp
    
# End Source File
# Begin Source File

SOURCE=.\FilenamesDialog.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Tracker3dFilter.cpp
    
# End Source File
# Begin Source File

SOURCE=.\Tracker3dFilter.def
# End Source File
# Begin Source File

SOURCE=.\Tracker3dFilter.h
# End Source File
# Begin Source File

SOURCE=.\Tracker3dPropertyPage.cpp
   
# End Source File
# Begin Source File

SOURCE=.\Tracker3dPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\Tracker3dPropertyPage.rc
# End Source File
# End Target
# End Project
