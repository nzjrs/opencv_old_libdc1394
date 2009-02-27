/* Copyright (c) 2001-2002 by SoftIntegration, Inc. All Rights Reserved */
#include <windows.h>
#include <cvcam.h>
#include <ch.h>

typedef  void (*callback)(void *);
static ChInterp_t interp;
static void cvcamPlayAVI_chdl_funarg(void* arg);
static void *cvcamPlayAVI_chdl_funptr;

EXPORTCH int cvcamPlayAVI_chdl(void *varg) {
    va_list ap;
    const char* file;
    void* window;
    int width;
    int height;
    callback callback_ch, callback_c=NULL;
    int retval;

    Ch_VaStart(interp, ap, varg);
    file = Ch_VaArg(interp, ap, const char*);
    window = Ch_VaArg(interp, ap, void*);
    width = Ch_VaArg(interp, ap, int);
    height = Ch_VaArg(interp, ap, int);
    callback_ch = Ch_VaArg(interp, ap, callback);
    cvcamPlayAVI_chdl_funptr = (void *)callback_ch;
    if (callback_ch != NULL)  {
        callback_c = (callback) cvcamPlayAVI_chdl_funarg;
    }
    retval = cvcamPlayAVI(file, window, width, height, callback_c);
    Ch_VaEnd(interp, ap);
    return retval;
}

static void cvcamPlayAVI_chdl_funarg(void* arg)   {
  Ch_CallFuncByAddr(interp,cvcamPlayAVI_chdl_funptr,NULL,arg);
}
