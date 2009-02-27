#
# This is helper makefile that can build DLL or EXE for i386, amd64 or ia64 platform.
# User of the makefile should specify the following symbols:
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
#

!ifndef MY_CXX
MY_CXX = bcc32
!endif

CXX = $(MY_CXX)

!ifndef MY_LINK
MY_LINK = bcc32
!endif

LINK = $(MY_LINK)

# Analyze binary type

!ifndef BINTYPE
BINTYPE = DLL
!endif

!if "$(BINTYPE)"=="DLL"

!ifndef VER
VER = 110
!endif

EXT = .dll
CXXFLAGS_DLL = -D"CVAPI_EXPORTS"
LINKFLAGS_DLL = -tWD

!else

EXT = .exe
CXXFLAGS_DLL =

!if "$(BINTYPE)"=="CONSOLE"
LINKFLAGS_DLL = -tWC
!else if "$(BINTYPE)"=="APP"
LINKFLAGS_DLL = -tW
!endif

!endif

# Check debug/release

!ifndef DEBUG
DEBUG = 0
!endif

!if $(DEBUG)

CXXFLAGS_DBG = -D"_DEBUG" -Od -v
TEMP_DBG_SUFFIX = _Dbg
DBG = d

!else

CXXFLAGS_DBG = -D"NDEBUG" -O2 -v
TEMP_DBG_SUFFIX = _Rls
DBG = 

!endif

OBJPATH = $(ROOT)\_temp\$(TARGET)$(TEMP_DBG_SUFFIX)_bcc
OUTBIN = $(ROOT)\bin\$(TARGET)$(VER)$(DBG)$(EXT)
OUTLIB = $(ROOT)\lib\$(TARGET)$(DBG).lib
LIBS = kernel32.lib user32.lib gdi32.lib advapi32.lib

I=-I
LIBPATH=-L

CXXFLAGS = -q -3 -a8 -b -c -d -RT- -tWM -tW -w-inl -w-aus -j200 -D"WIN32" -D"_WINDOWS" $(CXXFLAGS_DLL) $(CXXFLAGS_DBG) $(CXXFLAGS_PROJ) -o 
LINKFLAGS = -q -lq -v -lv -e$(OUTBIN) $(LINKFLAGS_DLL) $(LIBS_PROJ) $(MY_LINKFLAGS)

!ifndef SRCPATH
SRCPATH = .
!endif

.path.c = $(SRCPATH)
.path.cpp = $(SRCPATH)
.path.obj = $(OBJPATH)

.autodepend

!if "$(BINTYPE)"=="DLL"

$(OUTBIN): $(OBJS)
    @echo ********* Linking $(TARGET) ...
    @-mkdir $(ROOT)\bin 2> nul
    @-mkdir $(ROOT)\lib 2> nul
    @$(LINK) $(LINKFLAGS) $** $(LIBS) 
    @implib $(OUTLIB) $(OUTBIN)

!else

$(OUTBIN): $(OBJS)
    @echo ********* Linking $(TARGET) ...
    @-mkdir $(ROOT)\bin 2> nul
    @$(LINK) $(LINKFLAGS) $** $(LIBS) 

!endif

.cpp.obj:
    @-mkdir $(OBJPATH) 2> nul
    @$(CXX) $(CXXFLAGS)"$@" $<

.c.obj:
    @-mkdir $(OBJPATH) 2> nul
    @$(CXX) $(CXXFLAGS)"$@" $<

all:
    $(OUTBIN)

clean:
    @-del $(OUTBIN) 2> nul
    @-del $(OUTPDB) 2> nul
    @-del $(OUTLIB) 2> nul
    @-rmdir /s /q $(OBJPATH) 2> nul

$(OBJS): $(INCS)
