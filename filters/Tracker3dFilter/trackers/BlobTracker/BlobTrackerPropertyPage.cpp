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

#include <string>
#include <stdio.h>
#include "cvstreams.h"
#include <initguid.h>
#include "autorelease.h"
#include "ITracker3dFilter.h"
#include "IBlobTracker.h"
#include "BlobTrackerPropertyPage.h"
#include "resource.h"

//
// CreateInstance
//
// Used by the DirectShow base classes to create instances
//
CUnknown *BlobTrackerPropertyPage::CreateInstance(IUnknown *outer, HRESULT *phr)
{
    CUnknown *punk = new BlobTrackerPropertyPage(outer, phr);
    if (punk == NULL) {
        *phr = E_OUTOFMEMORY;
    }
    return punk;

}

//
// Constructor
//
BlobTrackerPropertyPage::BlobTrackerPropertyPage(IUnknown *outer, HRESULT *phr) :
    CBasePropertyPage(NAME("Blob Tracker Property Page"), outer, IDD_BlobTrackerPropertyPage, IDS_TITLE),
    m_en_change(false),
    m_output_options(IBlobTracker::OUTPUT_CROSSHAIRS),
    m_num_objects(0)
{
}

BlobTrackerPropertyPage::~BlobTrackerPropertyPage()
{
    for (int i = 0; i < m_trackers.size(); i++)
        SAFE_RELEASE(m_trackers[i]);
}

//
// OnReceiveMessage
//
// Handles the messages for our property window
//
BOOL BlobTrackerPropertyPage::OnReceiveMessage(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
    int i;
    if (msg == WM_COMMAND)
    {
        switch (HIWORD(w))
        {
            case BN_CLICKED:
                switch (LOWORD(w))
                {
                    case IDC_COLOR_CLASSIFIED_PIXELS:
                    case IDC_BOUNDING_BOX:
                        SetOutputOptions();
                        break;
                }
                break;

            case EN_CHANGE:
                if (!m_en_change)
                {
                    m_en_change = true;

                    char text[20];
                    GetWindowText(GetDlgItem(m_Dlg, LOWORD(w)), text, sizeof(text));
                    int n = atoi(text);

                    switch (LOWORD(w))
                    {
                        case IDC_NUMBER_OF_OBJECTS:
                            if (n != m_num_objects)
                            {
                                m_num_objects = n;

                                for (int i = 0; i < m_trackers.size(); i++)
                                    m_trackers[i]->SetNumberOfObjects(m_num_objects);
                            }
                            break;

                        case IDC_MIN_SIZE:
                            for (i = 0; i < m_trackers.size(); i++)
                                m_trackers[i]->SetSizeThreshold(n);
                            break;

                        case IDC_THRESHOLD:
                            for (i = 0; i < m_trackers.size(); i++)
                                m_trackers[i]->SetPixelThreshold(n);
                            break;
                    }

                    m_en_change = false;
                }
                break;
        }
    }

    return CBasePropertyPage::OnReceiveMessage(hwnd, msg, w, l);
}


// SetOutputOptions
void BlobTrackerPropertyPage::SetOutputOptions()
{
    m_output_options = IBlobTracker::OUTPUT_CROSSHAIRS;

    if (IsDlgButtonChecked(m_Dlg, IDC_COLOR_CLASSIFIED_PIXELS))
        m_output_options |= IBlobTracker::OUTPUT_COLOR_PIXELS;

    if (IsDlgButtonChecked(m_Dlg, IDC_BOUNDING_BOX))
        m_output_options |= IBlobTracker::OUTPUT_BOUNDING_BOX;

    for (int i = 0; i < m_trackers.size(); i++)
        m_trackers[i]->SetOutputOptions(m_output_options);
}

//
// OnConnect
//
// Called when we connect to the filter
//
HRESULT BlobTrackerPropertyPage::OnConnect(IUnknown *pUnknown)
{
    ITracker3dFilter *tracker_filter;

    HRESULT hr = pUnknown->QueryInterface(IID_ITracker3dFilter, (void **) &tracker_filter);
    if (FAILED(hr))
        return hr;
    
    std::vector<ITracker *> trackers;
    tracker_filter->GetTrackers(trackers);
    m_trackers.resize(0);
    m_trackers.reserve(trackers.size());
    for (int i = 0; i < trackers.size(); i++)
    {
        // Ensure that we are the correct property page for each filter
        // Ignore any that request a different page
        GUID page = GUID_NULL;
        trackers[i]->GetPropertyPage(&page);
        if (page == CLSID_BlobTrackerPropertyPage)
        {
            IBlobTracker *p;
            trackers[i]->QueryInterface(IID_IBlobTracker, (void **)&p);
            m_trackers.push_back(p);
        }
        trackers[i]->Release();
    }

    tracker_filter->Release();

    return NOERROR;
}


//
// OnDisconnect
//
// Called when we disconnect from the filter
//
HRESULT BlobTrackerPropertyPage::OnDisconnect()
{
    for (int i = 0; i < m_trackers.size(); i++)
        SAFE_RELEASE(m_trackers[i]);
    m_trackers.clear();

    return NOERROR;
}

//
// OnActivate
//
// We are being activated
//
HRESULT BlobTrackerPropertyPage::OnActivate()
{
    SetWindowText(GetDlgItem(m_Dlg, IDC_NUMBER_OF_OBJECTS), "1");
    SetOutputOptions();

    if (m_trackers.size() > 0 && m_trackers[0] != NULL)
    {
        int i;
        m_en_change = true;

        m_trackers[0]->GetPixelThreshold(i);
        SetDlgItemInt(m_Dlg, IDC_THRESHOLD, i, false);

        m_trackers[0]->GetSizeThreshold(i);
        SetDlgItemInt(m_Dlg, IDC_MIN_SIZE, i, false);

        m_en_change = false;
    }
    
    return NOERROR;
}

//
// OnDeactivate
//
// We are being deactivated
//
HRESULT BlobTrackerPropertyPage::OnDeactivate(void)
{
    return NOERROR;
}

//
// OnApplyChanges
//
// Apply any changes so far made
//
HRESULT BlobTrackerPropertyPage::OnApplyChanges()
{
    return NOERROR;
}
