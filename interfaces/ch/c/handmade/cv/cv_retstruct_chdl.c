/* Copyright (c) 2001-2002 by SoftIntegration, Inc. All Rights Reserved */
#include <cv.h>
#include <ch.h>

EXPORTCH CvRect* cvBoundingRect_chdl(void *varg) {
    ChInterp_t interp;
    va_list ap;
    CvArr* array;
    int update;
    CvRect *retval;

    Ch_VaStart(interp, ap, varg);
    array = Ch_VaArg(interp, ap, CvArr*);
    update = Ch_VaArg(interp, ap, int);
    retval = (CvRect*)malloc(sizeof(*retval));
    *retval = cvBoundingRect(array, update);
    Ch_VaEnd(interp, ap);
    return retval;
}

EXPORTCH CvBox2D* cvFitEllipse2_chdl(void *varg) {
    ChInterp_t interp;
    va_list ap;
    const CvArr* array;
    CvBox2D *retval;

    Ch_VaStart(interp, ap, varg);
    array = Ch_VaArg(interp, ap, const CvArr*);
    retval = (CvBox2D*)malloc(sizeof(*retval));
    *retval = cvFitEllipse2(array);
    Ch_VaEnd(interp, ap);
    return retval;
}



EXPORTCH CvBox2D* cvMinAreaRect2_chdl(void *varg) {
    ChInterp_t interp;
    va_list ap;
    const CvArr* array;
    CvMemStorage* storage;
    CvBox2D *retval;

    Ch_VaStart(interp, ap, varg);
    array = Ch_VaArg(interp, ap, const CvArr*);
    storage = Ch_VaArg(interp, ap, CvMemStorage*);
    retval = (CvBox2D*)malloc(sizeof(*retval));
    *retval = cvMinAreaRect2(array, storage);
    Ch_VaEnd(interp, ap);
    return retval;
}

