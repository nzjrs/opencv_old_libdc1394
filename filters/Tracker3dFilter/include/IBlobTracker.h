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
// IBlobTracker.h 
// ////////////////////////////////////////////////////////////////////////////

#ifndef IBLOBTRACKER_H
#define IBLOBTRACKER_H

#include <objbase.h>

// {77B72A26-9CFD-4aa4-9EAE-E3D06277E629}
DEFINE_GUID(IID_IBlobTracker, 0x77b72a26, 0x9cfd, 0x4aa4, 0x9e, 0xae, 0xe3, 0xd0, 0x62, 0x77, 0xe6, 0x29);

// {AF372D83-6086-42a7-8923-854733D94B48}
DEFINE_GUID(CLSID_BlobTracker, 0xaf372d83, 0x6086, 0x42a7, 0x89, 0x23, 0x85, 0x47, 0x33, 0xd9, 0x4b, 0x48);

interface IBlobTracker : IUnknown
{
    enum OutputFlags { OUTPUT_CROSSHAIRS = 1, OUTPUT_COLOR_PIXELS = 2, OUTPUT_BOUNDING_BOX = 4 };
    
    STDMETHOD(GetNumberOfObjects)(int &num_objects) = 0;
    STDMETHOD(SetNumberOfObjects)(int num_objects) = 0;
    STDMETHOD(GetPixelThreshold)(int &threshold) = 0;
    STDMETHOD(SetPixelThreshold)(int threshold) = 0;
    STDMETHOD(GetSizeThreshold)(int &threshold) = 0;
    STDMETHOD(SetSizeThreshold)(int threshold) = 0;
    STDMETHOD(GetOutputOptions)(int &options) = 0;
    STDMETHOD(SetOutputOptions)(int options) = 0;
};

#endif // IBLOBTRACKER_H
