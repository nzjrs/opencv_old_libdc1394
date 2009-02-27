#include <cxcore.h>
#include <ch.h>

EXPORTCH void cvCalcCovarMatrix_chdl(void *varg) {
    ChInterp_t interp;
    va_list ap;
    const CvArr** vects;
    int count;
    CvArr* cov_mat;
    CvArr* avg;
    int flags;

    Ch_VaStart(interp, ap, varg);
    vects = Ch_VaArg(interp, ap, const CvArr**);
    count = Ch_VaArg(interp, ap, int);
    cov_mat = Ch_VaArg(interp, ap, CvArr*);
    avg = Ch_VaArg(interp, ap, CvArr*);
    flags = Ch_VaArg(interp, ap, int);
    cvCalcCovarMatrix(vects, count, cov_mat, avg, flags);
    Ch_VaEnd(interp, ap);
}

EXPORTCH void cvGetModuleInfo_chdl(void *varg) {
    ChInterp_t interp;
    va_list ap;
    char* module_name;
    const char** version;
    const char** loaded_addon_plugins;

    Ch_VaStart(interp, ap, varg);
    module_name = Ch_VaArg(interp, ap, char*);
    version = Ch_VaArg(interp, ap, const char**);
    loaded_addon_plugins = Ch_VaArg(interp, ap, const char**);
    cvGetModuleInfo(module_name, version, loaded_addon_plugins);
    Ch_VaEnd(interp, ap);
}

EXPORTCH int cvGetErrInfo_chdl(void *varg) {
    ChInterp_t interp;
    va_list ap;
    const char** errcode_desc;
    const char** description;
    const char** filename;
    int* line;
    int retval;

    Ch_VaStart(interp, ap, varg);
    errcode_desc = Ch_VaArg(interp, ap, const char**);
    description = Ch_VaArg(interp, ap, const char**);
    filename = Ch_VaArg(interp, ap, const char**);
    line = Ch_VaArg(interp, ap, int*);
    retval = cvGetErrInfo(errcode_desc, description, filename, line);
    Ch_VaEnd(interp, ap);
    return retval;
}


EXPORTCH void* cvLoad_chdl(void *varg) {
    ChInterp_t interp;
    va_list ap;
    char* filename;
    CvMemStorage* memstorage;
    char* name;
    const char** real_name;
    void*retval;

    Ch_VaStart(interp, ap, varg);
    filename = Ch_VaArg(interp, ap, char*);
    memstorage = Ch_VaArg(interp, ap, CvMemStorage*);
    name = Ch_VaArg(interp, ap, char*);
    real_name = Ch_VaArg(interp, ap, const char**);
    retval = cvLoad(filename, memstorage, name, real_name);
    Ch_VaEnd(interp, ap);
    return retval;
}
