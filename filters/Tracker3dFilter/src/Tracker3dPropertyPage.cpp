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
#include <streams.h>
#include <shellapi.h>
#include <comcat.h>
#include "autorelease.h"
#include "ITracker3dFilter.h"
#include "Tracker3dPropertyPage.h"
#include "resource.h"
#include "FilenamesDialog.h"

//
// CreateInstance
//
// Used by the DirectShow base classes to create instances
//
CUnknown *Tracker3dPropertyPage::CreateInstance(IUnknown *outer, HRESULT *phr)
{
    CUnknown *punk = new Tracker3dPropertyPage(outer, phr);
    if (punk == NULL) {
        *phr = E_OUTOFMEMORY;
    }
    return punk;

}

//
// Constructor
//
Tracker3dPropertyPage::Tracker3dPropertyPage(IUnknown *outer, HRESULT *phr) :
    CBasePropertyPage(NAME("Tracker3d Property Page"), outer, IDD_Tracker3dPropertyPage, IDS_TITLE),
    m_pITracker3dFilter(NULL),
    m_internal_change(false),
    m_num_trackers(0)
{
}

Tracker3dPropertyPage::~Tracker3dPropertyPage()
{
    SAFE_RELEASE(m_pITracker3dFilter);
}

STDMETHODIMP Tracker3dPropertyPage::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    if (riid == IID_ITracker3dCallback)
        return GetInterface(static_cast<ITracker3dCallback *>(this), ppv);
    else
        return CBasePropertyPage::NonDelegatingQueryInterface(riid, ppv);
}

void Tracker3dPropertyPage::SetDirty()
{
    if (!m_internal_change)
    {
        m_bDirty = true;
        if (m_pPageSite)
            m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
    }
}

//
// OnReceiveMessage
//
// Handles the messages for our property window
//
BOOL Tracker3dPropertyPage::OnReceiveMessage(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
    switch (msg)
    {
        case WM_COMMAND:
            switch (HIWORD(w))
            {
                case BN_CLICKED:
                    switch (LOWORD(w))
                    {
                        case IDC_ANY_SIZE:
                        case IDC_640x480:
                        case IDC_320x240:
                            SetDirty();
                            break;

                        case IDC_CALIBRATE_CAMERAS:
                            bool continuous = (GetKeyState(VK_CONTROL) < 0);
                            bool dialog = (GetKeyState(VK_SHIFT) < 0 || m_camera_intrinsics_filenames.size() != m_num_trackers);
                            int m = GetDlgItemInt(m_Dlg, IDC_CHECKERBOARD_WIDTH, NULL, false);
                            int n = GetDlgItemInt(m_Dlg, IDC_CHECKERBOARD_HEIGHT, NULL, false);
                            if (m > 0 && n > 0)
                            {
                                if (!dialog || FilenamesDialog(m_Dlg, m_num_trackers, m_camera_intrinsics_filenames)) // returns false if cancelled by user
                                {
                                    std::vector<const char *> filenames(m_num_trackers);
                                    for (int i = 0; i < m_num_trackers; i++)
                                        filenames[i] = m_camera_intrinsics_filenames[i].c_str();

                                    if (m_pITracker3dFilter->CalibrateCameras(m, n, &filenames[0], 1.0f, continuous) != NOERROR)
                                        MessageBox(hwnd, "Error in Calibrate_Cameras", "Tracker3d", 0);
                                }
                            }
                            break;
                    }
                    break;

                case EN_CHANGE:
                    switch (LOWORD(w))
                    {
                        case IDC_NUMBER_OF_CAMERAS:
                            SetDirty();
                            break;
                    }
                    break;

                case CBN_SELCHANGE:
                case CBN_EDITCHANGE:
                    switch (LOWORD(w))
                    {
                        case IDC_TRACKER_DLL:
                            SetDirty();
                            break;
                    }
                    break;
            }
            break;

        case WM_APP:
        {
            HWND dlg = GetParent(GetParent(m_Dlg));
            RECT rect;
            GetWindowRect(dlg, &rect);
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;
            RECT screen_rect;
            GetWindowRect(GetDesktopWindow(), &screen_rect);
            int screen_width = screen_rect.right - screen_rect.left;
            int screen_height = screen_rect.bottom - screen_rect.top;
            int top = (screen_height - 40) - height;
            MoveWindow(dlg, screen_width - 10 - width, top, width, height, true);
            HWND parent = GetParent(dlg);
            char text[100];
            GetWindowText(parent, text, sizeof(text));
            if (strstr(text, "GraphEdit") != NULL)
                MoveWindow(GetParent(dlg), 10, top, screen_width - 30 - width, height, true);
            return 0;
        }
    }

    return CBasePropertyPage::OnReceiveMessage(hwnd, msg, w, l);
}

STDMETHODIMP Tracker3dPropertyPage::Callback(const std::vector<Cv3dTrackerTrackedObject> &objects, const unsigned char *, IUnknown *)
{
    unsigned long i;

    for (i = 0; i < MAX_OBJECTS; i++)
        EnableWindow(GetDlgItem(m_Dlg, IDC_LOCATION1 + i), false);

    for (i = 0; i < objects.size(); i++)
    {
        char text[80];
        sprintf(text, "(%#6.1f, %#6.1f, %#6.1f)", objects[i].p.x, objects[i].p.y, objects[i].p.z);
        SetDlgItemText(m_Dlg, IDC_LOCATION1 + objects[i].id, text);
        EnableWindow(GetDlgItem(m_Dlg, IDC_LOCATION1 + objects[i].id), true);
    }
    return NOERROR;
}

//
// OnConnect
//
// Called when we connect to the filter
//
HRESULT Tracker3dPropertyPage::OnConnect(IUnknown *pUnknown)
{
    SAFE_RELEASE(m_pITracker3dFilter); // shouldn't happen

    HRESULT hr = pUnknown->QueryInterface(IID_ITracker3dFilter, (void **) &m_pITracker3dFilter);
    if (FAILED(hr))
        return hr;

    hr = m_pITracker3dFilter->AddCallback(this);
    if (FAILED(hr))
       return hr;

    return NOERROR;
}


//
// OnDisconnect
//
// Called when we disconnect from the filter
//
HRESULT Tracker3dPropertyPage::OnDisconnect()
{
    if (m_pITracker3dFilter == NULL)
        return E_UNEXPECTED;

    m_pITracker3dFilter->RemoveCallback(this);
    SAFE_RELEASE(m_pITracker3dFilter);

    return NOERROR;
}

//
// OnActivate
//
// We are being activated
//
HRESULT Tracker3dPropertyPage::OnActivate()
{
    PostMessage(m_Dlg, WM_APP, 0, 0);

    m_internal_change = true;

    LoadTrackers();

    GUID tracker_clsid;
    m_pITracker3dFilter->GetDefaultTracker(tracker_clsid);
    if (tracker_clsid != GUID_NULL)
        SelectTracker(tracker_clsid);

    ITracker3dFilter::InputSize size;
    m_pITracker3dFilter->GetPreferredInputSize(size);
    if (size == ITracker3dFilter::SIZE_640x480)
        CheckRadioButton(m_Dlg, IDC_ANY_SIZE, IDC_640x480, IDC_640x480);
    else if (size == ITracker3dFilter::SIZE_320x240)
        CheckRadioButton(m_Dlg, IDC_ANY_SIZE, IDC_640x480, IDC_320x240);
    else
        CheckRadioButton(m_Dlg, IDC_ANY_SIZE, IDC_640x480, IDC_ANY_SIZE);

    m_pITracker3dFilter->GetNumberOfCameras(m_num_trackers);
    SetDlgItemInt(m_Dlg, IDC_NUMBER_OF_CAMERAS, m_num_trackers, false);

    SetDlgItemInt(m_Dlg, IDC_CHECKERBOARD_WIDTH, 6, false);
    SetDlgItemInt(m_Dlg, IDC_CHECKERBOARD_HEIGHT, 4, false);

    bool any_connected, all_connected;
    m_pITracker3dFilter->IsConnected(any_connected, all_connected);
    if (any_connected)
    {
        EnableWindow(GetDlgItem(m_Dlg, IDC_ANY_SIZE), false);
        EnableWindow(GetDlgItem(m_Dlg, IDC_640x480), false);
        EnableWindow(GetDlgItem(m_Dlg, IDC_320x240), false);
        EnableWindow(GetDlgItem(m_Dlg, IDC_NUMBER_OF_CAMERAS), false);
        EnableWindow(GetDlgItem(m_Dlg, IDC_TRACKER_DLL), false);
    }

    m_internal_change = false;

    return NOERROR;
}

//
// OnDeactivate
//
// We are being deactivated
//
HRESULT Tracker3dPropertyPage::OnDeactivate(void)
{
    return NOERROR;
}

//
// OnApplyChanges
//
// Apply any changes so far made
//
HRESULT Tracker3dPropertyPage::OnApplyChanges()
{
    if (IsDlgButtonChecked(m_Dlg, IDC_ANY_SIZE))
        m_pITracker3dFilter->SetPreferredInputSize(ITracker3dFilter::SIZE_Any);
    else if (IsDlgButtonChecked(m_Dlg, IDC_640x480))
        m_pITracker3dFilter->SetPreferredInputSize(ITracker3dFilter::SIZE_640x480);
    else if (IsDlgButtonChecked(m_Dlg, IDC_320x240))
        m_pITracker3dFilter->SetPreferredInputSize(ITracker3dFilter::SIZE_320x240);

    int new_num_trackers = GetDlgItemInt(m_Dlg, IDC_NUMBER_OF_CAMERAS, NULL, false);
    GUID new_tracker, prev_tracker;
    GetSelectedTracker(new_tracker);
    m_pITracker3dFilter->GetDefaultTracker(prev_tracker);

    if (new_num_trackers != m_num_trackers || prev_tracker != new_tracker)
    {
        m_pITracker3dFilter->SetDefaultTracker(new_tracker);
        m_pITracker3dFilter->SetTrackers(new_num_trackers, NULL); // force it to reallocate the trackers
        m_num_trackers = new_num_trackers;
    }

    return NOERROR;
}



class AutoCloseKey
{
    HKEY *m_pkey;

public:
    AutoCloseKey(HKEY *pkey) : m_pkey(pkey) { };
    ~AutoCloseKey()
    {
        if (*m_pkey != 0)
        {
            RegCloseKey(*m_pkey);
            *m_pkey = 0;
        }
    };
};

#define AUTO_CLOSE_KEY(f) AutoCloseKey AutoCloseKey##f(&f)

static const char key_name[] = "Software\\Intel\\VAI\\3d Tracker\\";


void Tracker3dPropertyPage::LoadTrackers()
{
    HRESULT hr;

    ICatInformation *catman;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_ALL, IID_ICatInformation, (void **)&catman);
    if (FAILED(hr))
        return;

    IEnumCLSID *e;
    hr = catman->EnumClassesOfCategories(1, const_cast<GUID *>(&CATID_Trackers), -1, NULL, &e);
    if (FAILED(hr))
    {
        catman->Release();
        return;
    }

    CLSID clsid;
    while (e->Next(1, &clsid, NULL) == S_OK)
    {
        char tracker_name[80];

        LONG r;

        wchar_t textw[50];
        StringFromGUID2(clsid, textw, 50);

        char key_name[80];
        wsprintf(key_name, "CLSID\\%ls", textw);
    
        HKEY key = 0; AUTO_CLOSE_KEY(key);
        r = RegOpenKeyEx(HKEY_CLASSES_ROOT, key_name, 0, KEY_READ, &key);
        if (r == ERROR_SUCCESS)
        {
            DWORD size = sizeof(tracker_name);
            r = RegQueryValueEx(key, "", 0, NULL, (BYTE *)tracker_name, &size);
        }

        if (r != ERROR_SUCCESS)
        {
            // Since we don't have a name, we just have to add the CLSID text
            wcstombs(tracker_name, textw, 50);
        }

        GUID *guid = new GUID;
        *guid = clsid;
        int item = SendDlgItemMessage(m_Dlg, IDC_TRACKER_DLL, CB_ADDSTRING, 0, (LPARAM)tracker_name);
        SendDlgItemMessage(m_Dlg, IDC_TRACKER_DLL, CB_SETITEMDATA, item, (LPARAM)guid);
    }

    e->Release();
    catman->Release();
}

void Tracker3dPropertyPage::SelectTracker(const GUID &tracker_clsid)
{
    // See if it's already there
    int item;
    int n = SendDlgItemMessage(m_Dlg, IDC_TRACKER_DLL, CB_GETCOUNT, 0, 0);
    for (item = 0; item < n; item++)
        if (tracker_clsid == *(GUID *)SendDlgItemMessage(m_Dlg, IDC_TRACKER_DLL, CB_GETITEMDATA, item, 0))
            break;

    // If it wasn't found, add it.
    // Since we don't have a name, we just have to add the CLSID text
    if (item == n)
    {
        char text[50];
        wchar_t textw[50];
        StringFromGUID2(tracker_clsid, textw, 50);
        wcstombs(text, textw, 50);
        item = SendDlgItemMessage(m_Dlg, IDC_TRACKER_DLL, CB_ADDSTRING, 0, (LPARAM)text);
        GUID *guid = new GUID;
        *guid = tracker_clsid;
        SendDlgItemMessage(m_Dlg, IDC_TRACKER_DLL, CB_SETITEMDATA, item, (LPARAM)guid);
    }
    
    SendDlgItemMessage(m_Dlg, IDC_TRACKER_DLL, CB_SETCURSEL, item, 0);
}

void Tracker3dPropertyPage::GetSelectedTracker(GUID &tracker_clsid)
{
    int item = SendDlgItemMessage(m_Dlg, IDC_TRACKER_DLL, CB_GETCURSEL, 0, 0);
    tracker_clsid = *(GUID *)SendDlgItemMessage(m_Dlg, IDC_TRACKER_DLL, CB_GETITEMDATA, item, 0);
}