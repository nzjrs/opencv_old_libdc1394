/* Copyright (c) 2001-2002 by SoftIntegration, Inc. All Rights Reserved */
#include <cv.h>
#include <ch.h>

typedef  void* (CV_CDECL *allocFunc)(size_t, void*);
typedef  int (CV_CDECL *freeFunc)(void*, void*);

static ChInterp_t interp;
static void* allocFunc_funarg(size_t Arg1, void* Arg2);
static void* setallocFunc_funptr;

static int freeFunc_funarg(void* Arg1, void* Arg2);
static void* setfreeFunc_funptr;


EXPORTCH void cvSetMemoryManager_chdl(void *varg) {
    va_list ap;
    allocFunc allocFuncptr_ch, allocFuncptr_c=NULL;
    freeFunc freeFuncptr_ch, freeFuncptr_c=NULL;
    void *userdata = NULL;

    Ch_VaStart(interp, ap, varg);

    allocFuncptr_ch = Ch_VaArg(interp, ap, allocFunc );
    setallocFunc_funptr = (void *)allocFuncptr_ch;
    if (allocFuncptr_ch != NULL)  {
        allocFuncptr_c = (allocFunc)allocFunc_funarg;
    }

    freeFuncptr_ch = Ch_VaArg(interp, ap, freeFunc);
    setfreeFunc_funptr = (void *)freeFuncptr_ch;
    if (freeFuncptr_ch != NULL)  {
        freeFuncptr_c = (freeFunc)freeFunc_funarg;
    }

    userdata = Ch_VaArg(interp, ap, void*);

    cvSetMemoryManager(allocFuncptr_c, freeFuncptr_c, userdata);
    Ch_VaEnd(interp, ap);
}

static void*  allocFunc_funarg(size_t Arg1, void* Arg2)    {
     void*  retval;
     Ch_CallFuncByAddr(interp, setallocFunc_funptr, &retval, Arg1, Arg2);
     return retval;
}

static int  freeFunc_funarg(void* Arg1, void* Arg2)    {
   int retval;
   Ch_CallFuncByAddr(interp, setfreeFunc_funptr, &retval, Arg1, Arg2);
   return retval;
}
