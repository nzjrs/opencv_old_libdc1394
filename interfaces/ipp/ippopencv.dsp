# Microsoft Developer Studio Project File - Name="ippopencv" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ippopencv - Win32 Debug IPP5_1
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ippopencv.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ippopencv.mak" CFG="ippopencv - Win32 Debug IPP5_1"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ippopencv - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ippopencv - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ippopencv - Win32 Debug IPP5" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ippopencv - Win32 Release IPP5" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ippopencv - Win32 Debug IPP5_1" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ippopencv - Win32 Release IPP5_1" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ippopencv - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\_temp\ippopencv_Rls"
# PROP Intermediate_Dir "..\..\_temp\ippopencv_Rls"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "A6" /D "W7" /D IPP=400 /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ippcvemerged.lib ippcvmerged.lib ippimerged.lib ippiemerged.lib ippsmerged.lib ippsemerged.lib ippvmmerged.lib ippvmemerged.lib ippcorel.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\bin\ippopencv100.dll"

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\_temp\ippopencv_Dbg"
# PROP Intermediate_Dir "..\..\_temp\ippopencv_Dbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "A6" /D "W7" /D IPP=400 /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ippcvmerged.lib ippcvemerged.lib ippimerged.lib ippiemerged.lib ippsmerged.lib ippsemerged.lib ippvmmerged.lib ippvmemerged.lib ippcorel.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\bin\ippopencv100d.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Debug IPP5"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ippopencv___Win32_Debug_IPP5"
# PROP BASE Intermediate_Dir "ippopencv___Win32_Debug_IPP5"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\_temp\ippopencv_Dbg.IPP5"
# PROP Intermediate_Dir "..\..\_temp\ippopencv_Dbg.IPP5"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "A6" /D "W7" /D "IPP5" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "A6" /D "W7" /D IPP=500 /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ippcvmerged.lib ippcvemerged.lib ippimerged.lib ippiemerged.lib ippsmerged.lib ippsemerged.lib ippvmmerged.lib ippvmemerged.lib ippcorel.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\bin\ippopencv096d.dll" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ippcvmerged.lib ippcvemerged.lib ippimerged.lib ippiemerged.lib ippsmerged.lib ippsemerged.lib ippvmmerged.lib ippvmemerged.lib ippccmerged.lib ippccemerged.lib ippcorel.lib bufferoverflowu.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\bin\ippopencv100d.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Release IPP5"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ippopencv___Win32_Release_IPP5"
# PROP BASE Intermediate_Dir "ippopencv___Win32_Release_IPP5"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\_temp\ippopencv_Rls.IPP5"
# PROP Intermediate_Dir "..\..\_temp\ippopencv_Rls.IPP5"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "A6" /D "W7" /D "IPP5" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "A6" /D "W7" /D IPP=500 /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ippcvemerged.lib ippcvmerged.lib ippimerged.lib ippiemerged.lib ippsmerged.lib ippsemerged.lib ippvmmerged.lib ippvmemerged.lib ippcorel.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\bin\ippopencv096.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ippcvemerged.lib ippcvmerged.lib ippimerged.lib ippiemerged.lib ippsmerged.lib ippsemerged.lib ippvmmerged.lib ippvmemerged.lib ippccmerged.lib ippccemerged.lib ippcorel.lib  bufferoverflowu.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\bin\ippopencv100.dll"

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Debug IPP5_1"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ippopencv___Win32_Debug_IPP5_1"
# PROP BASE Intermediate_Dir "ippopencv___Win32_Debug_IPP5_1"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\_temp\ippopencv_Dbg.IPP5.1"
# PROP Intermediate_Dir "..\..\_temp\ippopencv_Dbg.IPP5.1"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "A6" /D "W7" /D IPP=500 /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "A6" /D "W7" /D IPP=510 /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ippcvmerged.lib ippcvemerged.lib ippimerged.lib ippiemerged.lib ippsmerged.lib ippsemerged.lib ippvmmerged.lib ippvmemerged.lib ippccmerged.lib ippccemerged.lib ippcorel.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\bin\ippopencv097d.dll" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ippcvmerged.lib ippcvemerged.lib ippimerged.lib ippiemerged.lib ippsmerged.lib ippsemerged.lib ippvmmerged.lib ippvmemerged.lib ippccmerged.lib ippccemerged.lib ippcorel.lib bufferoverflowu.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\bin\ippopencv100d.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Release IPP5_1"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ippopencv___Win32_Release_IPP5_1"
# PROP BASE Intermediate_Dir "ippopencv___Win32_Release_IPP5_1"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\_temp\ippopencv_Rls.IPP5.1"
# PROP Intermediate_Dir "..\..\_temp\ippopencv_Rls.IPP5.1"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "A6" /D "W7" /D IPP=500 /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "A6" /D "W7" /D IPP=510 /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ippcvemerged.lib ippcvmerged.lib ippimerged.lib ippiemerged.lib ippsmerged.lib ippsemerged.lib ippvmmerged.lib ippvmemerged.lib ippccmerged.lib ippccemerged.lib ippcorel.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\bin\ippopencv097.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ippcvemerged.lib ippcvmerged.lib ippimerged.lib ippiemerged.lib ippsmerged.lib ippsemerged.lib ippvmmerged.lib ippvmemerged.lib ippccmerged.lib ippccemerged.lib ippcorel.lib bufferoverflowu.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\bin\ippopencv100.dll"

!ENDIF 

# Begin Target

# Name "ippopencv - Win32 Release"
# Name "ippopencv - Win32 Debug"
# Name "ippopencv - Win32 Debug IPP5"
# Name "ippopencv - Win32 Release IPP5"
# Name "ippopencv - Win32 Debug IPP5_1"
# Name "ippopencv - Win32 Release IPP5_1"
# Begin Source File

SOURCE=.\dllmain.c
# End Source File
# Begin Source File

SOURCE=.\export4.def

!IF  "$(CFG)" == "ippopencv - Win32 Release"

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Debug"

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Debug IPP5"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Release IPP5"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Debug IPP5_1"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Release IPP5_1"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\export5.def

!IF  "$(CFG)" == "ippopencv - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Debug IPP5"

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Release IPP5"

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Debug IPP5_1"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Release IPP5_1"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\export51.def

!IF  "$(CFG)" == "ippopencv - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Debug IPP5"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Release IPP5"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Debug IPP5_1"

!ELSEIF  "$(CFG)" == "ippopencv - Win32 Release IPP5_1"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\opencvipp_funclist.h
# End Source File
# End Target
# End Project
