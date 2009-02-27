# Microsoft Developer Studio Project File - Name="Tracker3dDemo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Tracker3dDemo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Tracker3dDemo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Tracker3dDemo.mak" CFG="Tracker3dDemo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Tracker3dDemo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Tracker3dDemo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Tracker3dDemo - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\_temp\Tracker3dDemo_Rls"
# PROP Intermediate_Dir "..\..\_temp\Tracker3dDemo_Rls"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /Gz /MD /W3 /GX /Zi /O2 /I "..\..\cxcore\include" /I "..\..\CV\include" /I "..\..\cvaux\include" /I "..\..\filters\Tracker3dFilter\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX"objbase.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 cxcore.lib cv.lib cvaux.lib winmm.lib strmbase.lib /nologo /subsystem:windows /pdb:none /debug /machine:I386 /out:"..\..\bin\Tracker3dDemo.exe" /libpath:"..\..\lib"

!ELSEIF  "$(CFG)" == "Tracker3dDemo - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\_temp\Tracker3dDemo_Dbg"
# PROP Intermediate_Dir "..\..\_temp\Tracker3dDemo_Dbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /Gz /MDd /W3 /GX /Zi /Od /I "..\..\cxcore\include" /I "..\..\CV\include" /I "..\..\cvaux\include" /I "..\..\filters\Tracker3dFilter\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX"objbase.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 cxcored.lib cvd.lib cvauxd.lib winmm.lib strmbasd.lib /nologo /subsystem:windows /pdb:none /debug /machine:I386 /out:"..\..\bin\Tracker3dDemod.exe" /libpath:"..\..\lib"

!ENDIF 

# Begin Target

# Name "Tracker3dDemo - Win32 Release"
# Name "Tracker3dDemo - Win32 Debug"
# Begin Source File

SOURCE=.\FilenamesDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\FilenamesDialog.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Tracker3dDemo.cpp
# End Source File
# Begin Source File

SOURCE=.\Tracker3dDemo.rc
# End Source File
# End Target
# End Project
