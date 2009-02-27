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
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
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
//M*/// HMMDemoView.h : interface of the CHMMDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HMMDEMOVIEW_H__80FE7EAC_1D5B_4166_B0DF_5991B563B473__INCLUDED_)
#define AFX_HMMDEMOVIEW_H__80FE7EAC_1D5B_4166_B0DF_5991B563B473__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHMMDemoView : public CView
{
protected: // create from serialization only
	CHMMDemoView();
	DECLARE_DYNCREATE(CHMMDemoView)

// Attributes
public:
    CHMMDemoDoc* GetDocument() { return (CHMMDemoDoc*)m_pDocument; };

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMMDemoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL SetTestPath( char* path );
	virtual ~CHMMDemoView();
    bool IsCaptureEnabled() { m_camera.IsInitialized(); }
    void CheckUpdate();
    CCamera& Camera() { return m_camera; }
    void  SetSelection( RECT* sel );
    CRect GetSelection() { return m_sel; }
    void  SetImageList( CStringList* list );
    CStringList*  GetImageList() { return m_imageList; }
    CString GetTestPath() { return m_TestPath; }
    

    
protected:
    CRect m_sel, m_tmp_sel;
    CCamera m_camera;

    //list of files which was opened in HMMDemoView 
    CStringList* m_imageList;

// Generated message map functions
protected:
	CString m_TestPath;
	//{{AFX_MSG(CHMMDemoView)
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMMDEMOVIEW_H__80FE7EAC_1D5B_4166_B0DF_5991B563B473__INCLUDED_)
