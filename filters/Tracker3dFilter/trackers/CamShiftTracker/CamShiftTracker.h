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
// CamShiftTracker.h 
// ////////////////////////////////////////////////////////////////////////////

#ifndef CAMSHIFTTRACKER_H
#define CAMSHIFTTRACKER_H

#include "ITracker.h"
#include "ICamShiftTracker.h"
#include "cv.h"
#include "cv.hpp"

// ////////////////////////////////////////////////////////////////////////////
//
// CamShiftTracker
//
// This is a COM wrapper for the OpenCV CamShift tracker.
// It implements ITracker so that CamShift can be used by the 3d Tracker filter.
// This is intended as sample code only.
// A complete implementation would have additional methods (in ICamShiftTracker)
// to control the CamShift tracker.
//
// ////////////////////////////////////////////////////////////////////////////

class CamShiftTracker : ITracker, ICamShiftTracker, CvCamShiftTracker, CUnknown
{
    IplImage m_image_format;
    int /*bool*/ m_calibrate;

    CamShiftTracker(IUnknown *outer, HRESULT *phr);
    ~CamShiftTracker();

public:
    static CUnknown * WINAPI CreateInstance(IUnknown *outer, HRESULT *phr);

    DECLARE_IUNKNOWN;
    STDMETHODIMP NonDelegatingQueryInterface(REFIID iid, void **ppv);

    // ITracker methods
    STDMETHODIMP CheckFormat(IplImage *image_header);
    STDMETHODIMP SetFormat(IplImage *image_header);
    STDMETHODIMP Process(IplImage *image);
    STDMETHODIMP GetTrackedObjects(ITracker::TrackingInfo &tracked_objects);
    STDMETHODIMP GetPropertyPage(GUID *page);

    // ICamShiftTracker methods
    STDMETHODIMP Calibrate();
};

#endif // CAMSHIFTTRACKER_H
