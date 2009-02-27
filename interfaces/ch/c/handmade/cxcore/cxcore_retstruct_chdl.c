/* Copyright (c) 2001-2002 by SoftIntegration, Inc. All Rights Reserved */
#include <cv.h>
#include <ch.h>

EXPORTCH CvScalar* cvColorToScalar_chdl(void *varg) {
    ChInterp_t interp;
    va_list ap;
    double packed_color;
    int arrtype;
    CvScalar *retval;

    Ch_VaStart(interp, ap, varg);
    packed_color = Ch_VaArg(interp, ap, double);
    arrtype = Ch_VaArg(interp, ap, int);
    retval = (CvScalar*)malloc(sizeof(*retval));
    *retval = cvColorToScalar(packed_color, arrtype);
    Ch_VaEnd(interp, ap);
    return retval;
}

EXPORTCH CvScalar* cvAvg_chdl(void *varg) {
    ChInterp_t interp;
    va_list ap;
    const CvArr* array;
    const CvArr* mask;
    CvScalar *retval;

    Ch_VaStart(interp, ap, varg);
    array = Ch_VaArg(interp, ap, const CvArr*);
    mask = Ch_VaArg(interp, ap, const CvArr*);
    retval = (CvScalar*)malloc(sizeof(*retval));
    *retval = cvAvg(array, mask);
    Ch_VaEnd(interp, ap);
    return retval;
}

EXPORTCH CvScalar* cvGet1D_chdl(void *varg) {
    ChInterp_t interp;
    va_list ap;
    const CvArr* array;
    int index;
    CvScalar *retval;

    Ch_VaStart(interp, ap, varg);
    array = Ch_VaArg(interp, ap, const CvArr*);
    index = Ch_VaArg(interp, ap, int);
    retval = (CvScalar*)malloc(sizeof(*retval));
    *retval = cvGet1D(array,index);
    Ch_VaEnd(interp, ap);
    return retval;
}

EXPORTCH CvScalar* cvGet2D_chdl(void *varg) {
    ChInterp_t interp;
    va_list ap;
    const CvArr* array;
    int index1, index2;
    CvScalar *retval;

    Ch_VaStart(interp, ap, varg);
    array = Ch_VaArg(interp, ap, const CvArr*);
    index1 = Ch_VaArg(interp, ap, int);
    index2 = Ch_VaArg(interp, ap, int);
    retval = (CvScalar*)malloc(sizeof(*retval));
    *retval = cvGet2D(array,index1,index2);
    Ch_VaEnd(interp, ap);
    return retval;
}

EXPORTCH CvScalar* cvGet3D_chdl(void *varg) {
    ChInterp_t interp;
    va_list ap;
    const CvArr* array;
    int index1, index2, index3;
    CvScalar *retval;

    Ch_VaStart(interp, ap, varg);
    array = Ch_VaArg(interp, ap, const CvArr*);
    index1 = Ch_VaArg(interp, ap, int);
    index2 = Ch_VaArg(interp, ap, int);
    index3 = Ch_VaArg(interp, ap, int);
    retval = (CvScalar*)malloc(sizeof(*retval));
    *retval = cvGet3D(array,index1,index2,index3);
    Ch_VaEnd(interp, ap);
    return retval;
}

EXPORTCH CvRect* cvGetImageROI_chdl(void *varg) {
    ChInterp_t interp;
    va_list ap;
    const IplImage* array;
    CvRect *retval;

    Ch_VaStart(interp, ap, varg);
    array = Ch_VaArg(interp, ap, const IplImage*);
    retval = (CvRect*)malloc(sizeof(*retval));
    *retval = cvGetImageROI(array);
    Ch_VaEnd(interp, ap);
    return retval;
}

EXPORTCH CvScalar* cvGetND_chdl(void *varg) {
    ChInterp_t interp;
    va_list ap;
    const CvArr* array;
    int* index;
    CvScalar *retval;

    Ch_VaStart(interp, ap, varg);
    array = Ch_VaArg(interp, ap, const CvArr*);
    index = Ch_VaArg(interp, ap, int*);
    retval = (CvScalar*)malloc(sizeof(*retval));
    *retval = cvGetND(array,index);
    Ch_VaEnd(interp, ap);
    return retval;
}

EXPORTCH CvSize* cvGetSize_chdl(void *varg) {
    ChInterp_t interp;
    va_list ap;
    const CvArr* array;
    CvSize *retval;

    Ch_VaStart(interp, ap, varg);
    array = Ch_VaArg(interp, ap, const CvArr*);
    retval = (CvSize*)malloc(sizeof(*retval));
    *retval = cvGetSize(array);
    Ch_VaEnd(interp, ap);
    return retval;
}

EXPORTCH CvScalar* cvSum_chdl(void *varg) {
    ChInterp_t interp;
    va_list ap;
    const CvArr* array;
    CvScalar *retval;

    Ch_VaStart(interp, ap, varg);
    array = Ch_VaArg(interp, ap, const CvArr*);
    retval = (CvScalar*)malloc(sizeof(*retval));
    *retval = cvSum(array);
    Ch_VaEnd(interp, ap);
    return retval;
}

EXPORTCH CvScalar* cvTrace_chdl(void *varg) {
    ChInterp_t interp;
    va_list ap;
    const CvArr* array;
    CvScalar *retval;

    Ch_VaStart(interp, ap, varg);
    array = Ch_VaArg(interp, ap, const CvArr*);
    retval = (CvScalar*)malloc(sizeof(*retval));
    *retval = cvTrace(array);
    Ch_VaEnd(interp, ap);
    return retval;
}

