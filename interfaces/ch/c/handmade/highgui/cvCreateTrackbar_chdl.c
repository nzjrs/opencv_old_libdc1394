#include <cv.h>
#include <highgui.h>
#include <ch.h>

////////////////////// create trackbar /////////////////////
static ChInterp_t interp;
static void* cvCreateTrackbar_chdl_callback_ptr[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

static void cvCreateTrackbar_chdl_callback0(int arg) {
  Ch_CallFuncByAddr(interp,cvCreateTrackbar_chdl_callback_ptr[0],NULL,arg);
}

static void cvCreateTrackbar_chdl_callback1(int arg) {
  Ch_CallFuncByAddr(interp,cvCreateTrackbar_chdl_callback_ptr[1],NULL,arg);
}

static void cvCreateTrackbar_chdl_callback2(int arg) {
  Ch_CallFuncByAddr(interp,cvCreateTrackbar_chdl_callback_ptr[2],NULL,arg);
}

static void cvCreateTrackbar_chdl_callback3(int arg) {
  Ch_CallFuncByAddr(interp,cvCreateTrackbar_chdl_callback_ptr[3],NULL,arg);
}

static void cvCreateTrackbar_chdl_callback4(int arg) {
  Ch_CallFuncByAddr(interp,cvCreateTrackbar_chdl_callback_ptr[4],NULL,arg);
}

static void cvCreateTrackbar_chdl_callback5(int arg) {
  Ch_CallFuncByAddr(interp,cvCreateTrackbar_chdl_callback_ptr[5],NULL,arg);
}

static void cvCreateTrackbar_chdl_callback6(int arg) {
  Ch_CallFuncByAddr(interp,cvCreateTrackbar_chdl_callback_ptr[6],NULL,arg);
}

static void cvCreateTrackbar_chdl_callback7(int arg) {
  Ch_CallFuncByAddr(interp,cvCreateTrackbar_chdl_callback_ptr[7],NULL,arg);
}


EXPORTCH int cvCreateTrackbar_chdl(void *varg) {
    static int idx = 0;
    va_list ap;
    const char* name;
    const char* window_name;
    int* value;
    int count;
    CvTrackbarCallback on_notify_ch = NULL, on_notify_c = NULL;
    int retval;

    Ch_VaStart(interp, ap, varg);
    name = Ch_VaArg(interp, ap, const char*);
    window_name = Ch_VaArg(interp, ap, const char*);
    value = Ch_VaArg(interp, ap, int*);
    count = Ch_VaArg(interp, ap, int);
    on_notify_ch = Ch_VaArg(interp, ap, CvTrackbarCallback);
    if (on_notify_ch != NULL)  {
        switch( idx )
        {
        case 0: on_notify_c = cvCreateTrackbar_chdl_callback0; break;
        case 1: on_notify_c = cvCreateTrackbar_chdl_callback1; break;
        case 2: on_notify_c = cvCreateTrackbar_chdl_callback2; break;
        case 3: on_notify_c = cvCreateTrackbar_chdl_callback3; break;
        case 4: on_notify_c = cvCreateTrackbar_chdl_callback4; break;
        case 5: on_notify_c = cvCreateTrackbar_chdl_callback5; break;
        case 6: on_notify_c = cvCreateTrackbar_chdl_callback6; break;
        case 7: on_notify_c = cvCreateTrackbar_chdl_callback7; break;
        }
        cvCreateTrackbar_chdl_callback_ptr[idx] = (void*)on_notify_ch;
        idx = (idx + 1) % 8;
    }
    retval = cvCreateTrackbar(name, window_name, value, count, on_notify_c);
    Ch_VaEnd(interp, ap);
    return retval;
}
