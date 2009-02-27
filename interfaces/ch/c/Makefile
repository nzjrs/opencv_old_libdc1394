# OPENCV path is set assuming this folder is OPENCV_HOME/ch/c
OPENCV =../..
INC1   = -I$(OPENCV)/cv/include -I$(OPENCV)/cxcore/include -I$(OPENCV)/cvaux/include
INC2 = -I$(OPENCV)/otherlibs/highgui
LFLAG1  = -L/usr/local/lib -lcxcore -lcv -lcvaux
LFLAG2 = -lhighgui


target: libcv.dl libhighgui.dl

# for libcv.dl
libcv.dl: cv_chdl.o cv_retstruct_chdl.o cxcore_retstruct_chdl.o cvSetIPLAllocators_chdl.o \
	cvSetMemoryManager_chdl.o cxcore_chdl.o cvaux_chdl.o constcharp2_chdl.o
	ch dllink libcv.dl cv_chdl.o cv_retstruct_chdl.o cxcore_retstruct_chdl.o cvSetIPLAllocators_chdl.o \
	cvSetMemoryManager_chdl.o cxcore_chdl.o cvaux_chdl.o constcharp2_chdl.o  $(LFLAG1) 

cv_chdl.o: cv_chdl.c
	ch dlcomp libcv.dl -cplusplus cv_chdl.c $(INC1)

cxcore_chdl.o: cxcore_chdl.c
	ch dlcomp libcv.dl -cplusplus cxcore_chdl.c $(INC1)

cvaux_chdl.o: cvaux_chdl.c
	ch dlcomp libcv.dl cvaux_chdl.c $(INC1)

cv_retstruct_chdl.o: handmade/cv/cv_retstruct_chdl.c
	ch dlcomp libcv.dl -cplusplus handmade/cv/cv_retstruct_chdl.c $(INC1)

cxcore_retstruct_chdl.o: handmade/cxcore/cxcore_retstruct_chdl.c
	ch dlcomp libcv.dl -cplusplus handmade/cxcore/cxcore_retstruct_chdl.c $(INC1)

cvSetIPLAllocators_chdl.o: handmade/cxcore/cvSetIPLAllocators_chdl.c
	ch dlcomp libcv.dl -cplusplus handmade/cxcore/cvSetIPLAllocators_chdl.c $(INC1)

cvSetMemoryManager_chdl.o: handmade/cxcore/cvSetMemoryManager_chdl.c
	ch dlcomp libcv.dl -cplusplus handmade/cxcore/cvSetMemoryManager_chdl.c $(INC1)

constcharp2_chdl.o: handmade/cxcore/constcharp2_chdl.c
	ch dlcomp libcv.dl -cplusplus handmade/cxcore/constcharp2_chdl.c $(INC1)


# for libhighgui.dl
libhighgui.dl: highgui_chdl.o  cvCreateTrackbar_chdl.o cvSetMouseCallback_chdl.o
	ch dllink libhighgui.dl highgui_chdl.o  cvCreateTrackbar_chdl.o cvSetMouseCallback_chdl.o \
                  $(LFLAG1) $(LFLAG2)

highgui_chdl.o: highgui_chdl.c
	ch dlcomp highgui.dl -cplusplus highgui_chdl.c $(INC1) $(INC2)

cvSetMouseCallback_chdl.o: handmade/highgui/cvSetMouseCallback_chdl.c
	ch dlcomp highgui.dl -cplusplus handmade/highgui/cvSetMouseCallback_chdl.c $(INC1) $(INC2)

cvCreateTrackbar_chdl.o: handmade/highgui/cvCreateTrackbar_chdl.c
	ch dlcomp highgui.dl -cplusplus handmade/highgui/cvCreateTrackbar_chdl.c $(INC1) $(INC2)


clean:
	rm -rf *.o *.obj *.dl *.exp *.lib
