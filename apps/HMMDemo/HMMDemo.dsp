# Microsoft Developer Studio Project File - Name="HMMDemo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=HMMDemo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HMMDemo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HMMDemo.mak" CFG="HMMDemo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HMMDemo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "HMMDemo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HMMDemo - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\_temp\HMMDemo_Rls"
# PROP Intermediate_Dir "..\..\_temp\HMMDemo_Rls"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\cxcore\include" /I "..\..\cvaux\include" /I "..\..\CV\Include" /I "..\Common" /I "..\..\OtherLibs\highgui" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 cvaux.lib cxcore.lib cv.lib vfw32.lib highgui.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\bin\HMMDemo.exe" /libpath:"..\..\lib" /libpath:"..\..\OtherLibs\vlgrfmts"

!ELSEIF  "$(CFG)" == "HMMDemo - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\_temp\HMMDemo_Dbg"
# PROP Intermediate_Dir "..\..\_temp\HMMDemo_Dbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\cxcore\include" /I "..\..\cvaux\include" /I "..\..\CV\Include" /I "..\Common" /I "..\..\OtherLibs\highgui" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 cxcored.lib cvauxd.lib cvd.lib vfw32.lib highguid.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin\HMMDemod.exe" /pdbtype:sept /libpath:"..\..\lib"

!ENDIF 

# Begin Target

# Name "HMMDemo - Win32 Release"
# Name "HMMDemo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Common\Camera.cpp
# End Source File
# Begin Source File

SOURCE=.\ContEHMM.cpp
# End Source File
# Begin Source File

SOURCE=.\FaceBase.cpp
# End Source File
# Begin Source File

SOURCE=.\HMMDemo.cpp
# End Source File
# Begin Source File

SOURCE=.\HMMDemo.rc
# End Source File
# Begin Source File

SOURCE=.\HMMDemoDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\HMMDemoView.cpp
# End Source File
# Begin Source File

SOURCE=.\HMMParams.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageBaseView.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoDialogs.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MiscParams.cpp
# End Source File
# Begin Source File

SOURCE=.\SamplingParams.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\HMMDemo.ico
# End Source File
# Begin Source File

SOURCE=.\Res\HMMDemo.rc2
# End Source File
# Begin Source File

SOURCE=.\Res\HMMDemoDoc.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Common\Camera.h
# End Source File
# Begin Source File

SOURCE=.\ContEHMM.h
# End Source File
# Begin Source File

SOURCE=.\FaceBase.h
# End Source File
# Begin Source File

SOURCE=.\HMMDemo.h
# End Source File
# Begin Source File

SOURCE=.\HMMDemoDoc.h
# End Source File
# Begin Source File

SOURCE=.\HMMDemoView.h
# End Source File
# Begin Source File

SOURCE=.\HMMParams.h
# End Source File
# Begin Source File

SOURCE=.\ImageBaseView.h
# End Source File
# Begin Source File

SOURCE=.\InfoDialogs.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MiscParams.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SamplingParams.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# End Target
# End Project
