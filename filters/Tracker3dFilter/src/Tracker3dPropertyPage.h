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

#include <vector>
#include <string>
#include "ITracker.h"
#include "ITracker3dFilter.h"

// {9AC294E2-5F53-4e08-AF35-69B9BE5B4723}
DEFINE_GUID(CLSID_Tracker3dPropertyPage, 0x9ac294e2, 0x5f53, 0x4e08, 0xaf, 0x35, 0x69, 0xb9, 0xbe, 0x5b, 0x47, 0x23);

class Tracker3dPropertyPage : public CBasePropertyPage, ITracker3dCallback
{
    enum { MAX_OBJECTS = 4 };
    ITracker3dFilter *m_pITracker3dFilter;
    int m_num_trackers;
    bool m_internal_change;
    std::vector<std::string> m_camera_intrinsics_filenames;

    void SetDirty();
    void RecreateTrackers(int size);
    void LoadTrackers();
    void SelectTracker(const GUID &tracker_clsid);
    void GetSelectedTracker(GUID &tracker_clsid);

    Tracker3dPropertyPage(IUnknown *outer, HRESULT *phr);
    ~Tracker3dPropertyPage();

public:
    static CUnknown * WINAPI CreateInstance(IUnknown *outer, HRESULT *phr);

    DECLARE_IUNKNOWN;
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

    BOOL OnReceiveMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
    HRESULT OnConnect(IUnknown *pUnknown);
    HRESULT OnDisconnect();
    HRESULT OnActivate();
    HRESULT OnDeactivate();
    HRESULT OnApplyChanges();

    // ITracker3dCallback
    STDMETHODIMP Callback(const std::vector<Cv3dTrackerTrackedObject> &tracked_objects, const unsigned char *, IUnknown *);
};
