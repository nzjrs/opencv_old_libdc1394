# use the default c++ flags
AM_CXXFLAGS=@DEF_CXXFLAGS@

bin_PROGRAMS = opencv-cvcap

ALL_CV_LIBS =                                       \
    $(top_builddir)/otherlibs/highgui/libhighgui.la \
    $(top_builddir)/cv/src/libcv.la                 \
    $(top_builddir)/cxcore/src/libcxcore.la

opencv_cvcap_SOURCES = cvcap.cpp
opencv_cvcap_LDADD = $(ALL_CV_LIBS)

EXTRA_DIST =

INCLUDES =                                \
    -I$(top_srcdir)/cxcore/include    \
    -I$(top_srcdir)/cv/include        \
    -I$(top_srcdir)/otherlibs/highgui \
    -I$(srcdir)/../include            \
    -I.

#CXX=@CXXOPENMP@
