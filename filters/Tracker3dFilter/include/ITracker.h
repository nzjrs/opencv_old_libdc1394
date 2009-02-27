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


// ITracker.h

#ifndef ITRACKER_H
#define ITRACKER_H

#include <vector>
#include <objbase.h>
#include "cv.h"
#include "cvaux.h"

// ITracker is the interface used by the 3D Tracker filter to call the
// 2D trackers.
// {FC2DF436-EBBB-42b7-BC83-7D536D65F413}
DEFINE_GUID(IID_ITracker, 0xfc2df436, 0xebbb, 0x42b7, 0xbc, 0x83, 0x7d, 0x53, 0x6d, 0x65, 0xf4, 0x13);

// CATID_Trackers is the "Video Trackers" category of COM classes in the registry,
// which allows the 3D Tracker Filter Property Page to find the available trackers.
// {D9EFDB52-BCCA-4b4c-BC04-A78184FB2052}
DEFINE_GUID(CATID_Trackers, 0xd9efdb52, 0xbcca, 0x4b4c, 0xbc, 0x4, 0xa7, 0x81, 0x84, 0xfb, 0x20, 0x52);

interface ITracker : IUnknown
{
    typedef std::vector<Cv3dTracker2dTrackedObject> TrackingInfo;

    // CheckFormat: returns S_OK if the format is acceptable; otherwise a failure code.
    STDMETHOD(CheckFormat)(IplImage *format) = 0;

    // SetFormat: specifies the format of images that will be passed to Process.
    // This is guaranteed to be called before Process is called. The tracker may
    // use this to perform configuration or memory allocation based on the image
    // format and size. Returns a failure code if the image format is not
    // acceptable or if a configuration or allocation error occurs.
    STDMETHOD(SetFormat)(IplImage *image_header) = 0;

    // Process: perform tracking on the image. The format of the image will match
    // the format passed to SetFormat.
    STDMETHOD(Process)(IplImage *sample) = 0;

    // GetTrackedObjects: returns the objects found in the preceding call
    // to Process.
    STDMETHOD(GetTrackedObjects)(TrackingInfo &tracked_objects) = 0;

    // GetPropertyPage: returns the GUID of a property page object
    // that supports configuration of the tracker. This returns S_OK
    // if a property page is available; otherwise a failure code.
    STDMETHOD(GetPropertyPage)(GUID *page) = 0;
};

#endif // ITRACKER_H
