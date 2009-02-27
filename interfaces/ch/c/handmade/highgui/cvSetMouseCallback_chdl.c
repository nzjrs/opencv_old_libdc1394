/* Copyright (c) 2001-2002 by SoftIntegration, Inc. All Rights Reserved */
#include <cv.h>
#include <highgui.h>
#include <ch.h>

static ChInterp_t interp;
static void* cvSetMouseCallback_chdl_callback_ptr[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

static void cvSetMouseCallback_chdl_callback0(int event, int x, int y, int flags) {
  Ch_CallFuncByAddr(interp,cvSetMouseCallback_chdl_callback_ptr[0],NULL, event, x, y, flags);
}


static void cvSetMouseCallback_chdl_callback1(int event, int x, int y, int flags) {
  Ch_CallFuncByAddr(interp,cvSetMouseCallback_chdl_callback_ptr[1],NULL, event, x, y, flags);
}

static void cvSetMouseCallback_chdl_callback2(int event, int x, int y, int flags) {
  Ch_CallFuncByAddr(interp,cvSetMouseCallback_chdl_callback_ptr[2],NULL, event, x, y, flags);
}

static void cvSetMouseCallback_chdl_callback3(int event, int x, int y, int flags) {
  Ch_CallFuncByAddr(interp,cvSetMouseCallback_chdl_callback_ptr[3],NULL, event, x, y, flags);
}
static void cvSetMouseCallback_chdl_callback4(int event, int x, int y, int flags) {
  Ch_CallFuncByAddr(interp,cvSetMouseCallback_chdl_callback_ptr[4],NULL, event, x, y, flags);
}

static void cvSetMouseCallback_chdl_callback5(int event, int x, int y, int flags) {
  Ch_CallFuncByAddr(interp,cvSetMouseCallback_chdl_callback_ptr[5],NULL, event, x, y, flags);
}

static void cvSetMouseCallback_chdl_callback6(int event, int x, int y, int flags) {
  Ch_CallFuncByAddr(interp,cvSetMouseCallback_chdl_callback_ptr[6],NULL, event, x, y, flags);
}

static void cvSetMouseCallback_chdl_callback7(int event, int x, int y, int flags) {
  Ch_CallFuncByAddr(interp,cvSetMouseCallback_chdl_callback_ptr[7],NULL, event, x, y, flags);
}



EXPORTCH void cvSetMouseCallback_chdl(void *varg) {
    static int idx = 0;
    va_list ap;
    char* window_name;
    CvMouseCallback on_mouse_ch = NULL, on_mouse_c = NULL;

    Ch_VaStart(interp, ap, varg);
    window_name = Ch_VaArg(interp, ap, char*);
    on_mouse_ch = Ch_VaArg(interp, ap, CvMouseCallback);
    if (on_mouse_ch != NULL)  {
        switch( idx )
        {
        case 0: on_mouse_c = cvSetMouseCallback_chdl_callback0; break;
        case 1: on_mouse_c = cvSetMouseCallback_chdl_callback1; break;
        case 2: on_mouse_c = cvSetMouseCallback_chdl_callback2; break;
        case 3: on_mouse_c = cvSetMouseCallback_chdl_callback3; break;
        case 4: on_mouse_c = cvSetMouseCallback_chdl_callback4; break;
        case 5: on_mouse_c = cvSetMouseCallback_chdl_callback5; break;
        case 6: on_mouse_c = cvSetMouseCallback_chdl_callback6; break;
        case 7: on_mouse_c = cvSetMouseCallback_chdl_callback7; break;
        }
        cvSetMouseCallback_chdl_callback_ptr[idx] = (void*)on_mouse_ch;
        idx = (idx + 1) % 8;
    }
    cvSetMouseCallback(window_name, on_mouse_c);
    Ch_VaEnd(interp, ap);
    return;
}
