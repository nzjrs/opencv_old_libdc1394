OPT :=

ifdef CXXFLAGS
	OPT := $(OPT) "CXXFLAGS_EXTRA=$(CXXFLAGS)"
endif

ifndef DEBUG
    DEBUG := 0
endif

OPT := $(OPT) DEBUG=$(DEBUG)

ifneq ($(DEBUG),0)
    DBG := d
    OPT := $(OPT) DBG=d
    DR := _Dbg
else
    DBG :=
    DR := _Rls
endif

ifndef MAKE
    MAKE := mingw32-make
endif

all:
	@-mkdir ..\_temp
	@-mkdir ..\_temp\cxcore$(DR)
	@$(MAKE) $(OPT) --directory=../_temp/cxcore$(DR) --makefile=../../cxcore/src/makefile.gnu
	@-mkdir ..\_temp\cv$(DR)
	@$(MAKE) $(OPT) --directory=../_temp/cv$(DR) --makefile=../../cv/src/makefile.gnu
	@-mkdir ..\_temp\cvaux$(DR)
	@$(MAKE) $(OPT) --directory=../_temp/cvaux$(DR) --makefile=../../cvaux/src/makefile.gnu
	@-mkdir ..\_temp\ml$(DR)
	@$(MAKE) $(OPT) --directory=../_temp/ml$(DR) --makefile=../../ml/src/makefile.gnu
	@-mkdir ..\_temp\highgui$(DR)
	@$(MAKE) $(OPT) --directory=../_temp/highgui$(DR) --makefile=../../otherlibs/highgui/makefile.gnu
	@-mkdir ..\_temp\cxts$(DR)
	@$(MAKE) $(OPT) --directory=../_temp/cxts$(DR) --makefile=../../tests/cxts/makefile.gnu
	@-mkdir ..\_temp\cxcoretest$(DR)
	@$(MAKE) $(OPT) --directory=../_temp/cxcoretest$(DR) --makefile=../../tests/cxcore/src/makefile.gnu
	@-mkdir ..\_temp\cvtest$(DR)
	@$(MAKE) $(OPT) --directory=../_temp/cvtest$(DR) --makefile=../../tests/cv/src/makefile.gnu

clean:
	@-rmdir /s /q ..\_temp\cxcore$(DR)
	@-del ..\bin\cxcore???$(DBG).dll
	@-del ..\lib\cxcore$(DBG).lib
	@-rmdir /s /q ..\_temp\cv$(DR)
	@-del ..\bin\cv???$(DBG).dll
	@-del ..\lib\cv$(DBG).lib
	@-rmdir /s /q ..\_temp\cvaux$(DR)
	@-del ..\bin\cvaux???$(DBG).dll
	@-del ..\lib\cvaux$(DBG).lib
	@-rmdir /s /q ..\_temp\ml$(DR)
	@-del ..\bin\ml???$(DBG).dll
	@-del ..\lib\ml$(DBG).lib
	@-rmdir /s /q ..\_temp\highgui$(DR)
	@-del ..\bin\highgui???$(DBG).dll
	@-del ..\lib\highgui$(DBG).lib
	@-rmdir /s /q ..\_temp\cxts$(DR)
	@-del ..\bin\cxts???$(DBG).dll
	@-del ..\lib\cxts$(DBG).lib
	@-rmdir /s /q ..\_temp\cxcoretest$(DR)
	@-del ..\bin\cxcoretest$(DBG).exe
	@-rmdir /s /q ..\_temp\cvtest$(DR)
	@-del ..\bin\cvtest$(DBG).exe
