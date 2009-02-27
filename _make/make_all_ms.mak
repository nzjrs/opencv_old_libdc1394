OPT = MY_CXX=$(MY_CXX) MY_CXXFLAGS=$(MY_CXXFLAGS) MY_LINK=$(MY_LINK) \
    MY_LINKFLAGS=$(MY_LINKFLAGS) DEBUG=$(DEBUG) PLATFORM=$(PLATFORM) PDBNAME=$(PDBNAME) MS=ms

all:
    @cd ..\cxcore\src
    @nmake /nologo -f makefile.ms $(OPT)
    @cd ..\..\_make
    @cd ..\cv\src
    @nmake /nologo -f makefile.ms $(OPT)
    @cd ..\..\_make
    @cd ..\cvaux\src
    @nmake /nologo -f makefile.ms $(OPT)
    @cd ..\..\_make
    @cd ..\ml\src
    @nmake /nologo -f makefile.ms $(OPT)
    @cd ..\..\_make
    @cd ..\otherlibs\highgui
    @nmake /nologo -f makefile.ms $(OPT)
    @cd ..\..\_make
    @cd ..\tests\cxts
    @nmake /nologo -f makefile.ms $(OPT)
    @cd ..\..\_make
    @cd ..\tests\cxcore\src
    @nmake /nologo -f makefile.ms $(OPT)
    @cd ..\..\..\_make
    @cd ..\tests\cv\src
    @nmake /nologo -f makefile.ms $(OPT)
    @cd ..\..\..\_make

clean:
    @cd ..\cxcore\src
    @nmake /nologo -f makefile.ms clean $(OPT)
    @cd ..\..\_make
    @cd ..\cv\src
    @nmake /nologo -f makefile.ms clean $(OPT)
    @cd ..\..\_make
    @cd ..\cvaux\src
    @nmake /nologo -f makefile.ms clean $(OPT)
    @cd ..\..\_make
    @cd ..\ml\src
    @nmake /nologo -f makefile.ms clean $(OPT)
    @cd ..\..\_make
    @cd ..\otherlibs\highgui
    @nmake /nologo -f makefile.ms clean $(OPT)
    @cd ..\..\_make
    @cd ..\tests\cxts
    @nmake /nologo -f makefile.ms clean $(OPT)
    @cd ..\..\_make
    @cd ..\tests\cxcore\src
    @nmake /nologo -f makefile.ms clean $(OPT)
    @cd ..\..\..\_make
    @cd ..\tests\cv\src
    @nmake /nologo -f makefile.ms clean $(OPT)
    @cd ..\..\..\_make

