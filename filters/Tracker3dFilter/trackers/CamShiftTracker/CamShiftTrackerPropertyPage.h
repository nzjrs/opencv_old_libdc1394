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

#pragma once

// ////////////////////////////////////////////////////////////////////////////
//
// CamShiftTrackerPropertyPage
//
// This is a property page for the COM wrapper for the OpenCV CamShift tracker.
// This is intended as sample code only. It only supports calibration of
// the tracker.
// A complete implementation would have additional controls to control the
// CamShift tracker.
//
// ////////////////////////////////////////////////////////////////////////////

#include <vector>
#include "ICamShiftTracker.h"

// {5C4C56C6-5739-42ac-973F-5A1CC0844407}
DEFINE_GUID(CLSID_CamShiftTrackerPropertyPage, 0x5c4c56c6, 0x5739, 0x42ac, 0x97, 0x3f, 0x5a, 0x1c, 0xc0, 0x84, 0x44, 0x7);

class CamShiftTrackerPropertyPage : public CBasePropertyPage
{
    std::vector<ICamShiftTracker *> m_trackers;

    CamShiftTrackerPropertyPage(IUnknown *outer, HRESULT *phr);
    ~CamShiftTrackerPropertyPage();

public:
    static CUnknown * WINAPI CreateInstance(IUnknown *outer, HRESULT *phr);

    BOOL OnReceiveMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
    HRESULT OnConnect(IUnknown *pUnknown);
    HRESULT OnDisconnect();
    HRESULT OnActivate();
    HRESULT OnDeactivate();
    HRESULT OnApplyChanges();
};
