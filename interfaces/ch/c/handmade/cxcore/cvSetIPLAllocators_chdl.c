/* Copyright (c) 2001-2002 by SoftIntegration, Inc. All Rights Reserved */
#include <cv.h>
#include <ch.h>

static ChInterp_t interp;
static IplImage* Cv_iplCreateImageHeader_funarg(int Arg1, int Arg2, int Arg3, char* Arg4, char* Arg5, int Arg6, int Arg7, int Arg8, int Arg9, int Arg10,  IplROI* Arg11, IplImage* Arg12, void* Arg13, IplTileInfo* Arg14);
static void *setiplCreateImageHeader_funptr;

static void Cv_iplAllocateImageData_funarg(IplImage* Arg1, int Arg2, int Arg3);
static void *setiplAllocateImageData_funptr;

static void Cv_iplDeallocate_funarg(IplImage* Arg1, int Arg2);
static void *setiplDeallocate_funptr;

static IplROI* Cv_iplCreateROI_funarg(int Arg1, int Arg2 ,int Arg3, int Arg4, int Arg5);
static void * setiplCreateROI_funptr;

static IplImage*  Cv_iplCloneImage_funarg(const IplImage* Arg1);
static void *setiplCloneImage_funptr;

EXPORTCH void cvSetIPLAllocators_chdl(void *varg) {
    va_list ap;
    Cv_iplCreateImageHeader createHeader_ch, createHeader_c;
    Cv_iplAllocateImageData allocateData_ch, allocateData_c;
    Cv_iplDeallocate deallocate_ch, deallocate_c;
    Cv_iplCreateROI createROI_ch, createROI_c;
    Cv_iplCloneImage cloneImage_ch, cloneImage_c;
    Ch_VaStart(interp, ap, varg);

    createHeader_ch = Ch_VaArg(interp, ap, Cv_iplCreateImageHeader);
    setiplCreateImageHeader_funptr = (void *)createHeader_ch;
    if (createHeader_ch != NULL)  {
        createHeader_c = (Cv_iplCreateImageHeader)Cv_iplCreateImageHeader_funarg;
    }

    allocateData_ch = Ch_VaArg(interp, ap, Cv_iplAllocateImageData);  
    setiplAllocateImageData_funptr = (void *)allocateData_ch;
    if (allocateData_ch != NULL)  {
        allocateData_c = (Cv_iplAllocateImageData)Cv_iplAllocateImageData_funarg;
    }

    deallocate_ch = Ch_VaArg(interp, ap, Cv_iplDeallocate);
    setiplDeallocate_funptr = (void *)deallocate_ch;
    if (deallocate_ch != NULL)  {
        deallocate_c = (Cv_iplDeallocate)Cv_iplDeallocate_funarg;
    }

    createROI_ch = Ch_VaArg(interp, ap, Cv_iplCreateROI);
    setiplCreateROI_funptr = (void *)createROI_ch;
    if (createROI_ch != NULL)  {
        createROI_c = (Cv_iplCreateROI)Cv_iplCreateROI_funarg;
    }

    cloneImage_ch = Ch_VaArg(interp, ap, Cv_iplCloneImage);
    setiplCreateROI_funptr = (void *)cloneImage_ch;
    if (cloneImage_ch != NULL)  {
        cloneImage_c = (Cv_iplCloneImage)Cv_iplCloneImage_funarg;
    }

    cvSetIPLAllocators(createHeader_c, allocateData_c, deallocate_c, createROI_c, cloneImage_c);
    Ch_VaEnd(interp, ap);
}

static IplImage* Cv_iplCreateImageHeader_funarg(int Arg1, int Arg2, int Arg3, char* Arg4, char* Arg5, int Arg6, int Arg7, int Arg8, int Arg9, int Arg10,  IplROI* Arg11, IplImage* Arg12, void* Arg13, IplTileInfo* Arg14)    {
     IplImage*  retval;
     Ch_CallFuncByAddr(interp, setiplCreateImageHeader_funptr, &retval, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14);
     return retval;
}

static void Cv_iplAllocateImageData_funarg(IplImage* Arg1, int Arg2, int Arg3)   {
     Ch_CallFuncByAddr(interp, setiplAllocateImageData_funptr, NULL, Arg1, Arg2, Arg3);
}

static void Cv_iplDeallocate_funarg(IplImage* Arg1, int Arg2)   {
     Ch_CallFuncByAddr(interp, setiplDeallocate_funptr, NULL, Arg1, Arg2);
}

static IplROI* Cv_iplCreateROI_funarg(int Arg1, int Arg2 ,int Arg3, int Arg4, int Arg5)    {
     IplROI*  retval;
     Ch_CallFuncByAddr(interp, setiplCreateROI_funptr, &retval, Arg1, Arg2, Arg3, Arg4, Arg5);
     return retval;
}

static IplImage*  Cv_iplCloneImage_funarg(const IplImage* Arg1)   {
     IplImage*  retval;
     Ch_CallFuncByAddr(interp, setiplCloneImage_funptr, &retval, Arg1);
     return retval;
}
