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
// BlobTracker.h 
// ////////////////////////////////////////////////////////////////////////////

#ifndef BLOBTRACKER_H
#define BLOBTRACKER_H

#include "ITracker.h"
#include "IBlobTracker.h"

// ////////////////////////////////////////////////////////////////////////////
//
// BlobTracker
//
// Definition of the tracker class.
//
// ////////////////////////////////////////////////////////////////////////////

class BlobTrackerObjectList;

class BlobTracker : ITracker, IBlobTracker, CUnknown
{
    // image format
    IplImage m_image_format;

    int m_num_objects;
    int m_output_options;
    int m_pixel_threshold;
    int m_size_threshold;
    int m_distance_threshold;

    IplImage *m_image;
    IplImage *m_tmp_image;

    BlobTrackerObjectList *m_objects;

    void FindObjects(BlobTrackerObjectList &objects);

    // BlobTrkOut.cpp
    void FillDestinationData(IplImage *image);
    void DrawCross(IplImage *image, CvPoint point, double color);

    BlobTracker(IUnknown *outer, HRESULT *phr);
    ~BlobTracker();

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

    // IBlobTracker methods
    STDMETHODIMP GetNumberOfObjects(int &num_objects) { num_objects = m_num_objects; return NOERROR; };
    STDMETHODIMP SetNumberOfObjects(int num_objects);
    STDMETHODIMP GetPixelThreshold(int &threshold) { threshold = m_pixel_threshold; return NOERROR; };
    STDMETHODIMP SetPixelThreshold(int threshold) { m_pixel_threshold = threshold; return NOERROR; };
    STDMETHODIMP GetSizeThreshold(int &threshold) { threshold = m_size_threshold; return NOERROR; };
    STDMETHODIMP SetSizeThreshold(int threshold) { m_size_threshold = threshold; return NOERROR; };
    STDMETHODIMP GetOutputOptions(int &options) { options = m_output_options; return NOERROR; };
    STDMETHODIMP SetOutputOptions(int options) { m_output_options = options; return NOERROR; };
};

#endif // BLOBTRACKER_H
