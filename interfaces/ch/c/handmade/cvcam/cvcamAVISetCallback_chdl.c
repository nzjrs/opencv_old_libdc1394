/* Copyright (c) 2001-2002 by SoftIntegration, Inc. All Rights Reserved */
#include <windows.h>
#include <cvcam.h>
#include <ch.h>

typedef  void (*callback)(void *);
static ChInterp_t interp;
static void cvcamAVISetCallback_chdl_funarg(void* arg);
static void *cvcamAVISetCallback_chdl_funptr;

EXPORTCH int cvcamAVISetCallback_chdl(void *varg) {
    va_list ap;
    cvcamAVIFILE file;
    callback callback_ch, callback_c;
    int retval;

    Ch_VaStart(interp, ap, varg);
    file = Ch_VaArg(interp, ap, cvcamAVIFILE);
    callback_ch = Ch_VaArg(interp, ap, callback);

    cvcamAVISetCallback_chdl_funptr = (void *)callback_ch;
    if (callback_ch != NULL)  {
        callback_c = (callback) cvcamAVISetCallback_chdl_funarg;
    }

    retval = cvcamAVISetCallback(file, callback_c);
    Ch_VaEnd(interp, ap);
    return retval;
}

static void cvcamAVISetCallback_chdl_funarg(void* arg)   {
  Ch_CallFuncByAddr(interp,cvcamAVISetCallback_chdl_funptr,NULL,arg);
}
