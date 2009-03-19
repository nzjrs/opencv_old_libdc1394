# use the default c++ flags
AM_CXXFLAGS=@DEF_CXXFLAGS@

bin_PROGRAMS = opencv-cvcap-dc1394v2 

ALL_CV_LIBS =                                       \
    $(top_builddir)/otherlibs/highgui/libhighgui.la \
    $(top_builddir)/cv/src/libcv.la                 \
    $(top_builddir)/cxcore/src/libcxcore.la

opencv_cvcap_dc1394v2_SOURCES = dc1394v2.cpp
opencv_cvcap_dc1394v2_LDADD = $(ALL_CV_LIBS)

EXTRA_DIST =

INCLUDES =                                \
    -I$(top_srcdir)/cxcore/include    \
    -I$(top_srcdir)/cv/include        \
    -I$(top_srcdir)/otherlibs/highgui \
    -I$(srcdir)/../include            \
    -I.

#CXX=@CXXOPENMP@
