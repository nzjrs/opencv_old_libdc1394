/* Copyright (c) 2001-2002 by SoftIntegration, Inc. All Rights Reserved */
#include <windows.h>
#include <string.h>
#include <cvcam.h>
#include <ch.h>

typedef  void (*callback)(void *);
static ChInterp_t interp;
static void cvcamSetProperty_chdl_funarg(void* arg);
static void *cvcamSetProperty_chdl_funptr;

EXPORTCH int cvcamSetProperty_chdl(void *varg) {
    va_list ap;
    int camera;
    const char* property;
    void* value;
    callback callback_ch, callback_c;
    int retval;

    Ch_VaStart(interp, ap, varg);
    camera = Ch_VaArg(interp, ap, int);
    property = Ch_VaArg(interp, ap, const char*);

    if  ( (strcmp(property, CVCAM_PROP_CALLBACK) == 0) || (strcmp(property, CVCAM_STEREO_CALLBACK) == 0) )
    {
         callback_ch = Ch_VaArg(interp, ap, callback); 
         cvcamSetProperty_chdl_funptr = (void *)callback_ch;
         if (callback_ch != NULL)  {
            callback_c = (callback) cvcamSetProperty_chdl_funarg;
          }
         retval = cvcamSetProperty(camera, property, callback_c);
     }
    else 
    {
        value = Ch_VaArg(interp, ap, void*); 
        retval = cvcamSetProperty(camera, property, value);
     }

    Ch_VaEnd(interp, ap);
    return retval;
}

static void cvcamSetProperty_chdl_funarg(void* arg)   {
  Ch_CallFuncByAddr(interp,cvcamSetProperty_chdl_funptr,NULL,arg);
}
