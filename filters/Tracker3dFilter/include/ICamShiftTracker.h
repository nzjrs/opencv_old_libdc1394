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
// ICamShiftTracker.h
//
// This interface is implemented by the COM wrapper for the CamShift tracker,
// and is used by Tracker3dFilter and Tracker3dDemo.
// This is intended as sample code only.
// A complete implementation would have additional methods
// to control the CamShift tracker.
//
// ////////////////////////////////////////////////////////////////////////////

#ifndef ICAMSHIFTTRACKER_H
#define ICAMSHIFTTRACKER_H

#include <objbase.h>

// {073531E4-1D5E-4990-9DD6-595929C8AD50}
DEFINE_GUID(IID_ICamShiftTracker, 0x73531e4, 0x1d5e, 0x4990, 0x9d, 0xd6, 0x59, 0x59, 0x29, 0xc8, 0xad, 0x50);

// {04C17309-FD54-473a-B2B7-5899592BFDE2}
DEFINE_GUID(CLSID_CamShiftTracker, 0x4c17309, 0xfd54, 0x473a, 0xb2, 0xb7, 0x58, 0x99, 0x59, 0x2b, 0xfd, 0xe2);

interface ICamShiftTracker : IUnknown
{
    STDMETHOD(Calibrate)() = 0;
};

#endif // ICAMSHIFTTRACKER_H
