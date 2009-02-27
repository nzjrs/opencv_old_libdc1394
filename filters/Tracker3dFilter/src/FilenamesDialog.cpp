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

#pragma warning(disable: 4786)
#include "FilenamesDialog.h"

template<class T> static inline void AddTemplate(char *&p, const T &item)
{
    *(T *)p = item;
    p += sizeof(T);
}

static void AddTemplate(char *&p, const wchar_t *item)
{
    size_t sz = (wcslen(item) + 1) * 2;
    memcpy(p, item, sz);
    p += sz;
}

void AddItem(char *&p, const DLGITEMTEMPLATE &item_tmpl, WORD item_class, const wchar_t *title)
{
    // make sure the beginning of each DLGITEMTEMPLATE is DWORD aligned.
    if ((int)p & 2)
        p += 2;
    AddTemplate(p, item_tmpl);
    AddTemplate(p, (WORD)0xffff); // item class
    AddTemplate(p, item_class);
    AddTemplate(p, title);
    AddTemplate(p, (WORD)0);   // creation data
}

#define ID_LABEL  20
#define ID_EDIT   40
#define ID_BROWSE 60

static BOOL CALLBACK DlgProc(HWND dlg, UINT msg, WPARAM w, LPARAM l);
static void LoadCameraIntrinsicsFilenames(std::vector<std::string> &filenames);
static void SaveCameraIntrinsicsFilenames(const std::vector<std::string> &filenames);

bool FilenamesDialog(HWND parent, int num_filenames, std::vector<std::string> &filenames)
{
    int i;

    if (num_filenames > 20)
        return false;

    if (filenames.size() == 0)
        LoadCameraIntrinsicsFilenames(filenames);

    filenames.resize(num_filenames);

    int name_size = 0;
    for (i = 0; i < num_filenames; i++)
        name_size += filenames[i].length() + 1;

    int num_items = num_filenames*3 + 2;
    int width = 200;
    int height = 35 + num_filenames * 20;
    size_t sz = sizeof(DLGTEMPLATE)+90 + num_items * (sizeof(DLGITEMTEMPLATE)+18) + name_size*2;
    char *dlgtmpl = (char *)malloc(sz);
    char *p = dlgtmpl;
    DLGTEMPLATE dlgtemplate = { WS_POPUP|WS_CAPTION|DS_MODALFRAME|DS_SETFONT|DS_SETFOREGROUND, 0, num_items, 0, 0, width, height };
    AddTemplate(p, dlgtemplate);
    AddTemplate(p, (WORD)0); // menu
    AddTemplate(p, (WORD)0); //  window class
    AddTemplate(p, L"Camera Intrinsics Filenames"); // title
    AddTemplate(p, (WORD)8); // font size
    AddTemplate(p, L"MS Sans Serif"); // font

#define BUTTON 0x80
#define EDIT   0x81
#define STATIC 0x82
    for (i = 0; i < num_filenames; i++)
    {
        DLGITEMTEMPLATE label = { SS_LEFT|WS_CHILD|WS_VISIBLE, 0, 10, 12+i*20, 40, 13, ID_LABEL+i };
        DLGITEMTEMPLATE edit = { ES_LEFT|WS_BORDER|WS_TABSTOP|WS_CHILD|WS_VISIBLE, 0, 50, 10+i*20, width-75, 13, ID_EDIT+i };
        DLGITEMTEMPLATE browse = { BS_PUSHBUTTON|WS_TABSTOP|WS_CHILD|WS_VISIBLE, 0, width-25, 10+i*20, 13, 13, ID_BROWSE+i };
        wchar_t text[MAX_PATH];
        swprintf(text, L"Camera %d", i+1);
        AddItem(p, label, STATIC, text);
        mbstowcs(text, filenames[i].c_str(), sizeof(text));
        AddItem(p, edit, EDIT, text);
        AddItem(p, browse, BUTTON, L"...");
    }
    DLGITEMTEMPLATE ok = { BS_DEFPUSHBUTTON|WS_TABSTOP|WS_CHILD|WS_VISIBLE, 0, 40, height - 20, 50, 14, IDOK };
    DLGITEMTEMPLATE cancel = { BS_PUSHBUTTON|WS_TABSTOP|WS_CHILD|WS_VISIBLE, 0, width-90, height - 20, 50, 14, IDCANCEL };
    AddItem(p, ok, BUTTON, L"OK");
    AddItem(p, cancel, BUTTON, L"Cancel");

    int r = DialogBoxIndirectParam(0, (DLGTEMPLATE *)dlgtmpl, parent, DlgProc, (LPARAM)&filenames);
    free(dlgtmpl);

    if (r == IDOK)
        SaveCameraIntrinsicsFilenames(filenames);

    return r == IDOK;
}

static BOOL CALLBACK DlgProc(HWND dlg, UINT msg, WPARAM w, LPARAM l)
{
    char filename[MAX_PATH];
    switch (msg)
    {
        case WM_INITDIALOG:
            SetWindowLong(dlg, GWL_USERDATA, l);
            return true;

        case WM_COMMAND:
            int control = LOWORD(w);
            if (control == IDOK)
            {
                std::vector<std::string> *filenames = (std::vector<std::string> *)GetWindowLong(dlg, GWL_USERDATA);
                for (int i = 0; i < filenames->size(); i++)
                {
                    GetWindowText(GetDlgItem(dlg, ID_EDIT+i), filename, sizeof(filename));
                    (*filenames)[i] = filename;
                }

                EndDialog(dlg, IDOK);
                return true;
            }
            else if (control == IDCANCEL)
            {
                EndDialog(dlg, IDCANCEL);
                return true;
            }
            else if (control >= ID_BROWSE && control < ID_BROWSE+20)
            {
                const char filter[] = "*.txt\0*.txt\0All files\0*.*\0";
                int index = control-ID_BROWSE;
                HWND edit = GetDlgItem(dlg, ID_EDIT+index);
                GetWindowText(edit, filename, sizeof(filename));
                OPENFILENAME ofn = { sizeof(OPENFILENAME), dlg, 0, filter, NULL, 0, 1,
                                     filename, sizeof(filename), NULL, 0, NULL, "Camera Intrinsics File",
                                     OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR|OFN_HIDEREADONLY };
                if (GetOpenFileName(&ofn))
                    SetWindowText(edit, filename);
                return true;
            }
    }
    return false;
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


static void LoadCameraIntrinsicsFilenames(std::vector<std::string> &filenames)
{
    LONG r;
    
    HKEY key = 0; AUTO_CLOSE_KEY(key);
    r = RegOpenKeyEx(HKEY_LOCAL_MACHINE, key_name, 0, KEY_READ, &key);
    if (r != ERROR_SUCCESS)
        return;

    char names[2000];
    DWORD size = sizeof(names);
    r = RegQueryValueEx(key, "CameraIntrinsicsFilenames", 0, NULL, (BYTE *)names, &size);
    if (r != ERROR_SUCCESS)
        return;

    filenames.resize(0);
    const char *p = names;
    while (*p != '\0')
    {
        filenames.push_back(p);
        p += strlen(p) + 1;
    }
}

static void SaveCameraIntrinsicsFilenames(const std::vector<std::string> &filenames)
{
    LONG r;
    
    HKEY key = 0; AUTO_CLOSE_KEY(key);
    r = RegCreateKeyEx(HKEY_LOCAL_MACHINE, key_name, 0, NULL, 0, KEY_WRITE, NULL, &key, NULL);
    if (r != ERROR_SUCCESS)
        return;

    char names[2000];
    // Warning: no overflow protection
    char *p = names;
    for (int i = 0; i < filenames.size(); i++)
    {
        strcpy(p, filenames[i].c_str());
        p += filenames[i].length() + 1;
    }
    *p = '\0';

    DWORD size = p - names + 1;

    RegSetValueEx(key, "CameraIntrinsicsFilenames", 0, REG_MULTI_SZ, (BYTE *)names, size);
}
