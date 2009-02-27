# Microsoft Developer Studio Project File - Name="ffopencv" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=FFOPENCV - WIN32 DEBUG
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ffopencv.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ffopencv.mak" CFG="FFOPENCV - WIN32 DEBUG"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ffopencv - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ffopencv - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ffopencv - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\_temp\ffopencv_Rls"
# PROP Intermediate_Dir "..\..\_temp\ffopencv_Rls"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ffopencv_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /Zi /O2 /I "..\..\cxcore\include" /I "..\..\cv\include" /I "..\highgui" /I ".\\" /I "..\_graphics\include" /D "NDEBUG" /D "HAVE_JPEG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "CVAPI_EXPORTS" /D "HAVE_PNG" /D "HAVE_TIFF" /D "HAVE_JASPER" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 cxcore.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib comctl32.lib libjpeg.lib libjasper.lib libpng.lib libtiff.lib zlib.lib vfw32.lib /nologo /dll /pdb:"..\..\bin/ffopencv110.pdb" /debug /machine:I386 /nodefaultlib:"libmmd.lib" /out:"..\..\bin/ffopencv110.dll" /implib:"..\..\lib/ffopencv.lib" /libpath:"..\..\lib" /libpath:"..\_graphics\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ffopencv - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\_temp\ffopencv_Dbg"
# PROP Intermediate_Dir "..\..\_temp\ffopencv_Dbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ffopencv_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /Zi /Od /I "..\..\cxcore\include" /I "..\..\cv\include" /I "..\highgui" /I ".\\" /I "..\_graphics\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "CVAPI_EXPORTS" /D "HAVE_PNG" /D "HAVE_TIFF" /D "HAVE_JASPER" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 cxcored.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib comctl32.lib libjpegd.lib libjasperd.lib libpngd.lib libtiffd.lib zlibd.lib vfw32.lib /nologo /dll /pdb:"..\..\bin/ffopencv110d.pdb" /debug /machine:I386 /nodefaultlib:"libmmdd.lib" /out:"..\..\bin/ffopencv110d.dll" /implib:"..\..\lib/ffopencvd.lib" /pdbtype:sept /libpath:"..\..\lib" /libpath:"..\_graphics\lib"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ENDIF 

# Begin Target

# Name "ffopencv - Win32 Release"
# Name "ffopencv - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ffopencv.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ffopencv.h
# End Source File
# Begin Source File

SOURCE=..\highgui\highgui.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
