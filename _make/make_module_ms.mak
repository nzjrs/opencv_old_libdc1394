#
# This is helper makefile that can build DLL or EXE for i386, amd64 or ia64 platform.
# User of the makefile should specify the following symbols:
#    * PLATFORM (I386/AMD64/IA64) - the target platform (I386 by default)
#    * DEBUG (1/0) - iff a debug version of the binary should be build (0 by default)
#    * TARGET - base name of the binary (ex.: cxcore, cv, ml, cvtest etc.)
#    * BINTYPE (DLL/CONSOLE/APP) - the type of application (dll/console app/app with gui), dll by default
#    * ROOT - the relative path to the root opencv directory
#    * OBJS - the list of object files, each file must be preceded with $(OBJPATH)\
#    * INCS - the list of include files
#    * CXX - the compiler to be used (cl by default)
#    * CXXFLAGS_PROJ - the initial compiler options
#           (some project-specific defines and the list of include directories)
#    * LIBS_PROJ - the initial linker options
#           (the list of linked libraries + path to the libraries)
#    * RES_FILE - resource file, if any
#    * PDBNAME - temporary pdb filename (e.g. vc60.pdb for MSVC6)
#

!ifndef MY_CXX
MY_CXX = cl
!endif

CXX = "$(MY_CXX)"

!ifndef MY_LINK
MY_LINK = link
!endif

LINK = "$(MY_LINK)"

!ifndef PDBNAME
PDBNAME = vc60.pdb
!endif

!ifndef SRCPATH
SRCPATH = .
!endif

# Analyze binary type

!ifndef BINTYPE
BINTYPE = DLL
!endif

!if "$(BINTYPE)"=="DLL"

!ifndef VER
VER = 110
!endif

EXT = .dll
CXXFLAGS_DLL = /D "CVAPI_EXPORTS"
LINKFLAGS_DLL = /subsystem:windows /dll /implib:$(OUTLIB)

!else

EXT = .exe
CXXFLAGS_DLL =

!if "$(BINTYPE)"=="CONSOLE"
LINKFLAGS_DLL = /subsystem:console
!else if "$(BINTYPE)"=="APP"
LINKFLAGS_DLL = /subsystem:windows
!endif

!endif

# Check debug/release

!ifndef DEBUG
DEBUG = 0
!endif

!if $(DEBUG)

CXXFLAGS_DBG = /D "_DEBUG" /MDd /Gm $(DBG_CHECK) /Od
TEMP_DBG_SUFFIX = _Dbg
DBG = d

!else

CXXFLAGS_DBG = /D "NDEBUG" /MD /O2 /Ob2
TEMP_DBG_SUFFIX = _Rls
DBG = 

!endif


# Analyze platform

!ifndef PLATFORM
PLATFORM = I386
!endif

!if "$(PLATFORM)" == "I386"

CXXFLAGS_P =
TEMP_P_SUFFIX =
OUT_P_SUFFIX =
DBG_CHECK = /GZ

!else if "$(PLATFORM)" == "AMD64"

CXXFLAGS_P = /D "WIN64" /D "EM64T"
TEMP_P_SUFFIX = .64
OUT_P_SUFFIX = _64
DBG_CHECK = 

!else if "$(PLATFORM)" == "IA64"

CXXFLAGS_P = /D "WIN64"
TEMP_P_SUFFIX = .i7
OUT_P_SUFFIX = _i7
DBG_CHECK = 

!else
!error Unknown platform
!endif

!ifndef PCH
PCH_TARGET =
PCH_OBJ =
CXXFLAGS_PCH =
!else
!ifndef PCH_STARTER
PCH_STARTER = precomp
!endif
PCH_TARGET = "$(OBJPATH)\$(TARGET).pch"
PCH_OBJ = $(OBJPATH)\$(PCH_STARTER).obj
CXXFLAGS_PCH = /Fp$(PCH_TARGET) /Yu"$(PCH)"
!endif

OBJPATH = $(ROOT)\_temp\$(TARGET)$(TEMP_DBG_SUFFIX)$(TEMP_P_SUFFIX)
OUTBIN = $(ROOT)\bin\$(TARGET)$(VER)$(DBG)$(OUT_P_SUFFIX)$(EXT)
OUTPDB = $(ROOT)\bin\$(TARGET)$(VER)$(DBG)$(OUT_P_SUFFIX).pdb
OUTLIB = $(ROOT)\lib\$(TARGET)$(DBG)$(OUT_P_SUFFIX).lib
TEMPPDB = $(OBJPATH)\$(PDBNAME)
DEF_LIBS = kernel32.lib user32.lib gdi32.lib advapi32.lib 

I=/I
LIBPATH=/libpath:

CXXFLAGS = /nologo /W4 /D "WIN32" /D"_WINDOWS" $(CXXFLAGS_P) $(CXXFLAGS_DBG) $(CXXFLAGS_DLL) /EHsc /Zi /FD $(CXXFLAGS_PCH) $(CXXFLAGS_PROJ) /Fd"$(TEMPPDB)" $(MY_CXXFLAGS) /c /Fo
LINKFLAGS = /nologo /machine:$(PLATFORM) /pdb:$(OUTPDB) /debug /out:$(OUTBIN) $(LINKFLAGS_DLL) $(DEF_LIBS) $(MY_LINKFLAGS)

$(OUTBIN): $(PCH_TARGET) $(OBJS)
    @echo ********* Linking $(TARGET) ...
    @-mkdir $(ROOT)\bin 2> nul
    @-mkdir $(ROOT)\lib 2> nul
    @$(LINK) $(LINKFLAGS) $(LIBS_PROJ) $(PCH_OBJ) $(OBJS)

!ifdef PCH

$(PCH_TARGET): $(PCH_STARTER).cpp
    @-mkdir $(OBJPATH) 2> nul
    @$(CXX) $(CXXFLAGS)$(PCH_OBJ) /Yc"$(PCH)" $(PCH_STARTER).cpp

!endif

{$(SRCPATH)}.cpp{$(OBJPATH)}.obj:
    @-mkdir $(OBJPATH) 2> nul
    @$(CXX) $(CXXFLAGS)"$@" $<

{$(SRCPATH)}.c{$(OBJPATH)}.obj:
    @-mkdir $(OBJPATH) 2> nul
    @$(CXX) $(CXXFLAGS)$@ $<

all:
    $(OUTBIN)

clean:
    @-del $(OUTBIN) 2> nul
    @-del $(OUTPDB) 2> nul
    @-del $(OUTLIB) 2> nul
    @-rmdir /s /q $(OBJPATH) 2> nul

$(OBJS): $(PCH_TARGET) $(INCS)
