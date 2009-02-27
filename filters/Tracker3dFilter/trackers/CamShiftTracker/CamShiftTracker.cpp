/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2002, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

// ////////////////////////////////////////////////////////////////////////////
//    CamShiftTracker.cpp
//
// This is a COM wrapper for the OpenCV CamShift tracker.
// It implements ITracker so that CamShift can be used by the 3d Tracker filter.
// This is intended as sample code only.
// A complete implementation would have additional methods (in ICamShiftTracker)
// to control the CamShift tracker.
//
// ////////////////////////////////////////////////////////////////////////////

#include <cvstreams.h>
#include <comcat.h>
#include "CamShiftTracker.h"
#include "CamShiftTrackerPropertyPage.h"

static void DrawCross(IplImage *image, CvPoint point);


//
// CreateInstance
//
// Used by the DirectShow base classes to create instances
//
CUnknown *CamShiftTracker::CreateInstance(IUnknown *outer, HRESULT *phr)
{
    CUnknown *punk = new CamShiftTracker(outer, phr);
    if (punk == NULL) {
        *phr = E_OUTOFMEMORY;
    }
    return punk;

}

// ////////////////////////////////////////////////////////////////////////////
// CamShiftTracker::CamShiftTracker()
//
// Constructor.
//
// ////////////////////////////////////////////////////////////////////////////
CamShiftTracker::CamShiftTracker(IUnknown *outer, HRESULT *phr)
    : CUnknown(NAME("CamShift Tracker"), outer)
{
    // set up default tracking params
    int dims[] = { 20 };
    set_hist_dims( 1, dims );
    set_hist_bin_range( 0, 1, 180 );
    set_threshold( 0 );
    set_min_ch_val( 1, 20 );   // S MIN
    set_max_ch_val( 1, 255 );  // S MAX
    set_min_ch_val( 2, 40 );   // V MIN
    set_max_ch_val( 2, 240 );  // V MAX

    Calibrate();
}

// ////////////////////////////////////////////////////////////////////////////
// CamShiftTracker::~CamShiftTracker()
//
// Destructor.
//
// ////////////////////////////////////////////////////////////////////////////
CamShiftTracker::~CamShiftTracker()
{
}


HRESULT CamShiftTracker::NonDelegatingQueryInterface(REFIID iid, void **ppv)
{
    if (iid == IID_ICamShiftTracker)
        return GetInterface((IUnknown *)(void *)static_cast<ICamShiftTracker *>(this), ppv);

    if (iid == IID_ITracker)
        return GetInterface((IUnknown *)(void *)static_cast<ITracker *>(this), ppv);

    return CUnknown::NonDelegatingQueryInterface(iid, ppv);
}

STDMETHODIMP CamShiftTracker::CheckFormat(IplImage *image_header)
{
    if (image_header->depth != IPL_DEPTH_8U
        || image_header->width % 4 != 0)
    {
        return E_FAIL;
    }

    if (image_header->nChannels == 3
        && strncmp(image_header->colorModel, "RGB", 3) == 0)
    {
        return NOERROR;
    }

    return E_FAIL;
}

STDMETHODIMP CamShiftTracker::SetFormat(IplImage *image_header)
{
    HRESULT hr = CheckFormat(image_header);
    if (FAILED(hr))
        return hr;

    m_image_format = *image_header;

    set_window(cvRect(0, 0, image_header->width, image_header->height));

    return NOERROR;
}


STDMETHODIMP CamShiftTracker::GetPropertyPage(GUID *page)
{
    *page = CLSID_CamShiftTrackerPropertyPage;
    return NOERROR;
}

static inline bool MatchFormat(IplImage *image, IplImage *format)
{
    return (image->nChannels == format->nChannels
            && image->width == format->width
            && image->height == format->height
            && image->depth == format->depth
            && image->dataOrder == format->dataOrder
            && image->origin == format->origin
            && strncmp(image->channelSeq, format->channelSeq, 4) == 0);

}

STDMETHODIMP CamShiftTracker::Calibrate()
{
    m_calibrate = 30;
    reset_histogram();
    return NOERROR;
}

STDMETHODIMP CamShiftTracker::Process(IplImage *image)
{
    HRESULT hr = MatchFormat(image, &m_image_format);
    if (FAILED(hr))
        return hr;

    if (m_calibrate > 0)
    {
        CvRect rect = cvRect(image->width*0.47, image->height*0.47, image->width*0.06, image->height*0.07);
        cvRectangle(image, cvPoint(rect.x, rect.y), cvPoint(rect.x+rect.width, rect.y+rect.height), 0xffffff, 1);
        set_window(rect);
        update_histogram(static_cast<CvImage *>(image));
        m_calibrate--;
    }
    //else
    {
        track_object(static_cast<CvImage *>(image));

        CvRect rect = get_window();
        CvPoint center = cvPoint(rect.x + rect.width/2, rect.y + rect.height/2);
        DrawCross(image, center);
        cvRectangle(image, cvPoint(rect.x, rect.y), cvPoint(rect.x+rect.width, rect.y+rect.height), 0xffffff, 1);
    }

    return NOERROR;
}

STDMETHODIMP CamShiftTracker::GetTrackedObjects(ITracker::TrackingInfo &tracked_objects)
{
    CvRect rect = get_window();
    CvPoint center = cvPoint(rect.x + rect.width/2, rect.y + rect.height/2);
    tracked_objects.resize(1);
    tracked_objects[0] = cv3dTracker2dTrackedObject(0, center);
    return NOERROR;
}


static void DrawCross(IplImage *image, CvPoint point)
{
    const int SIZE_OF_CROSS = 20;

    int Left = MAX((int)point.x - SIZE_OF_CROSS, 0);
    int Right = MIN((int)point.x + SIZE_OF_CROSS, (int)image->width - 1);
    int Top = MAX((int)point.y - SIZE_OF_CROSS, 0);
    int Bottom = MIN((int)point.y + SIZE_OF_CROSS, (int)image->height - 1);

    cvLine(image, cvPoint(point.x, Top), cvPoint(point.x, Bottom), 0xffffff);
    cvLine(image, cvPoint(Left, point.y), cvPoint(Right, point.y), 0xffffff);
}


// Setup information

// List of class IDs and creator functions for the class factory. This
// provides the link between the OLE entry point in the DLL and an object
// being created. The class factory will call the static CreateInstance

CFactoryTemplate g_Templates[] = {
    { L"CamShift Tracker", &CLSID_CamShiftTracker, CamShiftTracker::CreateInstance },
    { L"CamShift Tracker Property Page", &CLSID_CamShiftTrackerPropertyPage, CamShiftTrackerPropertyPage::CreateInstance }
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);


//
// DllRegisterServer
//
// Register the COM objects (the tracker and the property page).
// Also add the tracker to the "Video Trackers" component category.
STDAPI DllRegisterServer()
{
    HRESULT hr = AMovieDllRegisterServer2( TRUE );
    if (FAILED(hr))
        return hr;

    ICatRegister *reg;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_ALL, IID_ICatRegister, (void **)&reg);
    if (FAILED(hr))
        return hr;

    CATEGORYINFO catinfo;
    catinfo.catid = CATID_Trackers;
    catinfo.lcid = 0x409;
    wcscpy(catinfo.szDescription, L"Video Trackers");
    reg->RegisterCategories(1, &catinfo);
    reg->RegisterClassImplCategories(CLSID_CamShiftTracker, 1, const_cast<GUID *>(&CATID_Trackers));

    reg->Release();

    return NOERROR;
} // DllRegisterServer


//
// DllUnregisterServer
//
// Unregister the COM objects (the tracker and the property page).
// Also remove the tracker from the "Video Trackers" component category.
STDAPI DllUnregisterServer()
{
    HRESULT hr = AMovieDllRegisterServer2( FALSE );
    if (FAILED(hr))
        return hr;

    ICatRegister *reg;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_ALL, IID_ICatRegister, (void **)&reg);
    if (FAILED(hr))
        return hr;

    reg->UnRegisterClassImplCategories(CLSID_CamShiftTracker, 1, const_cast<GUID *>(&CATID_Trackers));

    reg->Release();

    return NOERROR;
} // DllUnregisterServer
