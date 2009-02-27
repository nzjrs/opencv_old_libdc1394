!ifdef DEBUG
DBG_OPT=-D"DEBUG=1"
!else
DBG_OP=
!endif

OPT = $(DBG_OPT) -DMS=bc

all:
    @cd ..\cxcore\src
    @make -f makefile.ms $(OPT)
    @cd ..\..\_make
    @cd ..\cv\src
    @make -f makefile.ms $(OPT)
    @cd ..\..\_make
    @cd ..\cvaux\src
    @make -f makefile.ms $(OPT)
    @cd ..\..\_make
    @cd ..\ml\src
    @make -f makefile.ms $(OPT)
    @cd ..\..\_make
    @cd ..\otherlibs\highgui
    @make -f makefile.ms $(OPT)
    @cd ..\..\_make
    @cd ..\tests\cxts
    @make -f makefile.ms $(OPT)
    @cd ..\..\_make
    @cd ..\tests\cxcore\src
    @make -f makefile.ms $(OPT)
    @cd ..\..\..\_make
    @cd ..\tests\cv\src
    @make -f makefile.ms $(OPT)
    @cd ..\..\..\_make

clean:
    @cd ..\cxcore\src
    @make -f makefile.ms $(OPT) clean
    @cd ..\..\_make
    @cd ..\cv\src
    @make -f makefile.ms $(OPT) clean
    @cd ..\..\_make
    @cd ..\cvaux\src
    @make -f makefile.ms $(OPT) clean
    @cd ..\..\_make
    @cd ..\ml\src
    @make -f makefile.ms $(OPT) clean
    @cd ..\..\_make
    @cd ..\otherlibs\highgui
    @make -f makefile.ms $(OPT) clean
    @cd ..\..\_make
    @cd ..\tests\cxts
    @make -f makefile.ms $(OPT) clean
    @cd ..\..\_make
    @cd ..\tests\cxcore\src
    @make -f makefile.ms $(OPT) clean
    @cd ..\..\..\_make
    @cd ..\tests\cv\src
    @make -f makefile.ms $(OPT) clean
    @cd ..\..\..\_make
