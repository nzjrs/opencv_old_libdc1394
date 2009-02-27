/* Copyright (c) 2001-2002 by SoftIntegration, Inc. All Rights Reserved */
#include <windows.h>
#include <cv.h>
#include <highgui.h>
#include <ch.h>

static ChInterp_t interp;
static void set_postprocess_chdl_funarg(HWND arg1, UINT arg2, WPARAM arg3, LPARAM arg4, int* arg5);
static void *set_postprocess_chdl_funptr;

/* EXPORTCH void set_postprocess_func_chdl(void *varg) { */
EXPORTCH void cvSetPostprocessFuncWin32_chdl(void *varg) {
    va_list ap;
    CvWin32WindowCallback on_postprocess_ch, on_postprocess_c;

    Ch_VaStart(interp, ap, varg);
    on_postprocess_ch = Ch_VaArg(interp, ap, CvWin32WindowCallback);
    set_postprocess_chdl_funptr = (void *)on_postprocess_ch;
    if (on_postprocess_ch != NULL)  {
        on_postprocess_c = (CvWin32WindowCallback) set_postprocess_chdl_funarg;
    }
    cvSetPostprocessFuncWin32(on_postprocess_c);

    Ch_VaEnd(interp, ap);
}

static void set_postprocess_chdl_funarg(HWND arg1, UINT arg2, WPARAM arg3, LPARAM arg4, int* arg5)   {  
   Ch_CallFuncByAddr(interp,set_postprocess_chdl_funptr,NULL,arg1,arg2,arg3,arg4,arg5);
}

