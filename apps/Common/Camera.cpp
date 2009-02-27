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
//M*/// Camera.cpp: implementation of the CCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Camera.h"
#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCamera::CCamera()
{
    m_capWnd = 0;
    m_isRunning = false;
    m_hic = 0;
    m_fourcc = -1;
}

CCamera::~CCamera()
{
    Uninitialize();
}


void CCamera::UpdateParent( bool whole )
{
    HWND parent = GetParent( m_capWnd );
    RECT r = { 0, 0, 0, 0 };
    IplImage* img = m_frame.GetImage();

    if( img )
    {
        r.right = img->width;
        r.bottom = img->height;
    }
    
    InvalidateRect( parent, whole ? 0 : &r, whole );
    UpdateWindow( parent );
}


// Frame callback
LRESULT PASCAL FrameCallbackProc( HWND hWnd, VIDEOHDR* hdr ) 
{ 
    BITMAPINFO vfmt;
    CCamera* camera = (CCamera*)capGetUserData(hWnd);
    int sz;

    if (!hWnd) return FALSE;
	if (camera && !camera->IsRunning() ) return FALSE;
	
    sz = capGetVideoFormat( hWnd, &vfmt, sizeof(vfmt));
    
    if( hdr && hdr->lpData && sz != 0 && camera )
        camera->PutCompressedFrame( hdr, &vfmt );

    return (LRESULT)TRUE; 
}


void CCamera::OnFrame()
{
    UpdateParent(0);
}

void CCamera::PutCompressedFrame( VIDEOHDR* hdr, BITMAPINFO* vfmt )
{
    long code = ICERR_OK;
    char* frame_data = (char*)hdr->lpData;
        
    if( vfmt->bmiHeader.biCompression != BI_RGB ||
        vfmt->bmiHeader.biBitCount != 24 )
    {
        BITMAPINFOHEADER& vfmt0 = vfmt->bmiHeader;
        BITMAPINFOHEADER vfmt1;
        code = ICERR_ERROR;

        memset( &vfmt1, 0, sizeof(vfmt1));
        vfmt1.biSize = sizeof(vfmt1);
        vfmt1.biWidth = vfmt0.biWidth;
        vfmt1.biHeight = vfmt0.biHeight;
        vfmt1.biBitCount = 24;
        vfmt1.biCompression = BI_RGB;
        vfmt1.biPlanes = 1;

        if( m_hic == 0 || m_fourcc != vfmt0.biCompression || m_frame.GetImage() == 0 ||
            vfmt0.biWidth != m_frame.GetImage()->width ||
            vfmt0.biHeight != m_frame.GetImage()->height )
        {
            if( m_hic )
            {
                ICDecompressEnd( m_hic );
                ICClose( m_hic );
            }
            m_hic = ICOpen( MAKEFOURCC('V','I','D','C'),
                          vfmt0.biCompression, ICMODE_DECOMPRESS );

            if( m_hic && ICDecompressBegin( m_hic, &vfmt0, &vfmt1 ) == ICERR_OK )
            {
                m_frame.Create( vfmt0.biWidth, vfmt0.biHeight, 24 );
                m_frame.GetImage()->origin = IPL_ORIGIN_BL;

                code = ICDecompress( m_hic, 0, &vfmt0, hdr->lpData,
                                     &vfmt1, m_frame.GetImage()->imageData );
            }
        }
    }
    else
    {
        m_frame.Create( vfmt->bmiHeader.biWidth, vfmt->bmiHeader.biHeight, 24 );
        memcpy( m_frame.GetImage()->imageData, hdr->lpData, m_frame.GetImage()->imageSize );
    }

    /*m_frame.GetImage()->origin = IPL_ORIGIN_BL;*/
    cvFlip( m_frame.GetImage(), m_frame.GetImage(), 0 );

    OnFrame();
} 


// Initialize camera input
bool  CCamera::Initialize( int width, int height, int format, HWND parent, int wIndex )
{
    char szDeviceName[80];
    char szDeviceVersion[80];
    HWND hWndC = 0;
    
    if( (unsigned)wIndex < 10 )
        goto init_camera;

    for( wIndex = 0; wIndex < 10; wIndex++ ) 
    {
init_camera:        
        if( capGetDriverDescription( wIndex, szDeviceName, 
            sizeof (szDeviceName), szDeviceVersion, 
            sizeof (szDeviceVersion))) 
        {
            hWndC = capCreateCaptureWindow ( "My Own Capture Window", 
                  WS_CHILD | WS_VISIBLE , 0, 0, 160, 120, parent, 0);

            if( capDriverConnect (hWndC, wIndex))
            {
                /*BITMAPINFO bmi;
 
                //capPreviewRate(hWndC, 66);  // rate, in milliseconds
                memset( &bmi.bmiHeader, 0, sizeof(bmi.bmiHeader));
                bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
                bmi.bmiHeader.biBitCount = format == 0 ? 24 : 12;
                bmi.bmiHeader.biWidth = width;
                bmi.bmiHeader.biHeight = height;
                bmi.bmiHeader.biPlanes = 1;
                bmi.bmiHeader.biCompression = format;
                bmi.bmiHeader.biSizeImage = ((bmi.bmiHeader.biWidth*
                                              bmi.bmiHeader.biBitCount/8 + 3)&-4)*
                                              bmi.bmiHeader.biHeight;
                if( format == -1 || capSetVideoFormat( hWndC, &bmi, sizeof(bmi)-4)) break;
                capDriverDisconnect( hWndC );*/
                break;
            }
            DestroyWindow( hWndC );
            hWndC = 0;
        }
    }
    
    if( hWndC )
    {
        m_capWnd = hWndC;
        
        memset( &m_caps, 0, sizeof(m_caps));
        capDriverGetCaps( hWndC, &m_caps, sizeof(m_caps));
        capSetUserData( hWndC, (long)this );
        capSetCallbackOnFrame( m_capWnd, FrameCallbackProc ); 
        ::MoveWindow( m_capWnd, 0, 0, 1, 1, TRUE );
        CAPTUREPARMS p;
        capCaptureGetSetup(hWndC,&p,sizeof(CAPTUREPARMS));
        p.dwRequestMicroSecPerFrame = 1;
        capCaptureSetSetup(hWndC,&p,sizeof(CAPTUREPARMS));
        capPreviewScale(hWndC,FALSE);
        capPreviewRate(hWndC,1);
    }
    return m_capWnd != 0;
}


// Uninitialize camera input
void  CCamera::Uninitialize()
{
    Stop();
    capSetCallbackOnFrame( m_capWnd, NULL ); 
    capDriverDisconnect( m_capWnd );
    DestroyWindow( m_capWnd );

    if( m_hic )
    {
        ICDecompressEnd( m_hic );
        ICClose( m_hic );
        m_hic = 0;
    }
}

// Start capture
void  CCamera::Start()
{
    if( m_capWnd )
    {
        m_isRunning = true;
    }
}

// Stop capture
void  CCamera::Stop()
{
    if( m_capWnd )
    {
        m_isRunning = false;
    }
}


void  CCamera::VideoFormatDlg()
{
    if( m_capWnd && m_caps.fHasDlgVideoFormat )
    {
        capDlgVideoFormat( m_capWnd );
    }
}


void  CCamera::VideoSourceDlg()
{
    if( m_capWnd && m_caps.fHasDlgVideoSource )
    {
        capDlgVideoSource( m_capWnd );
    }
}
