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
//M*/

#include "CV.h"
#include <windows.h>
#include <cvstreams.h>
#include <initguid.h>
#include <stdlib.h>
#include "syncfilter.h"
#include "syncfilteruids.h"
#include "isyncfilter.h"

const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
  &MEDIATYPE_Video,            // major type
  &MEDIASUBTYPE_NULL           // minor type
};

const AMOVIESETUP_PIN sudpPins[] =
{
    {
        L"Input1",          // String pin name
        FALSE,              // Is it rendered
        FALSE,              // Is it an output
        FALSE,              // Allowed none
        FALSE,              // Allowed many
        &CLSID_NULL,        // Connects to filter
        L"Output1",         // Connects to pin
        1,                  // Number of types
        &sudPinTypes        // The pin details
    },
    {
        L"Input2",          // String pin name
        FALSE,              // Is it rendered
        FALSE,              // Is it an output
        FALSE,              // Allowed none
        FALSE,              // Allowed many
        &CLSID_NULL,        // Connects to filter
        L"Output2",         // Connects to pin
        1,                  // Number of types
        &sudPinTypes        // The pin details
    },
    {
        L"Input3",          // String pin name
        FALSE,              // Is it rendered
        FALSE,              // Is it an output
        FALSE,              // Allowed none
        FALSE,              // Allowed many
        &CLSID_NULL,        // Connects to filter
        L"Output3",         // Connects to pin
        1,                  // Number of types
        &sudPinTypes        // The pin details
    },
    {
        L"Input4",          // String pin name
        FALSE,              // Is it rendered
        FALSE,              // Is it an output
        FALSE,              // Allowed none
        FALSE,              // Allowed many
        &CLSID_NULL,        // Connects to filter
        L"Output4",         // Connects to pin
        1,                  // Number of types
        &sudPinTypes        // The pin details
    },

    {
        L"Output1",         // String pin name
        TRUE,               // Is it rendered
        TRUE,               // Is it an output
        FALSE,              // Allowed none
        FALSE,              // Allowed many
        &CLSID_NULL,        // Connects to filter
        L"Input1",          // Connects to pin
        1,                  // Number of types
        &sudPinTypes        // The pin details
    },
    {
        L"Output2",         // String pin name
        TRUE,               // Is it rendered
        TRUE,               // Is it an output
        FALSE,              // Allowed none
        FALSE,              // Allowed many
        &CLSID_NULL,        // Connects to filter
        L"Input2",          // Connects to pin
        1,                  // Number of types
        &sudPinTypes        // The pin details
    },
    {
        L"Output3",         // String pin name
        TRUE,               // Is it rendered
        TRUE,               // Is it an output
        FALSE,              // Allowed none
        FALSE,              // Allowed many
        &CLSID_NULL,        // Connects to filter
        L"Input3",          // Connects to pin
        1,                  // Number of types
        &sudPinTypes        // The pin details
    },
    {
        L"Output4",         // String pin name
        TRUE,               // Is it rendered
        TRUE,               // Is it an output
        FALSE,              // Allowed none
        FALSE,              // Allowed many
        &CLSID_NULL,        // Connects to filter
        L"Input4",          // Connects to pin
        1,                  // Number of types
        &sudPinTypes        // The pin details
    }
};

const AMOVIESETUP_FILTER  sudMyFilter =
{
    &CLSID_SyncFilter                           // clsID
    , L"VideoSync Extended Filter for OpenCV"   // strName
    , MERIT_DO_NOT_USE                          // dwMerit
    , 4                                         // nPins
    , sudpPins                                  // lpPin
};

SyncFilter::SyncFilter(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr)
    :m_csFilter(), CBaseFilter(tszName, punk, &m_csFilter, CLSID_SyncFilter), m_NbConnected(0), m_callback2(NULL), m_callback3(NULL), m_callback4(NULL)
{
    m_ip[0] = new cvSyncInputPin(NAME ( "1-st input pin"),
                               this,
                               &m_csFilter,
                               &m_hr,
                               L"Input1");
    m_ip[1] = new cvSyncInputPin(NAME ( "2-nd input pin"),
                               this,
                               &m_csFilter,
                               &m_hr,
                               L"Input2");
    m_ip[2] = new cvSyncInputPin(NAME ( "3-rd input pin"),
                               this,
                               &m_csFilter,
                               &m_hr,
                               L"Input3");
    m_ip[3] = new cvSyncInputPin(NAME ( "4-th input pin"),
                               this,
                               &m_csFilter,
                               &m_hr,
                               L"Input4");

    m_op[0] = new cvSyncOutputPin(NAME("1-st output pin"),
                               this,
                               &m_csFilter,
                               &m_hr,
                               L"Output1");
    m_op[1] = new cvSyncOutputPin(NAME("2-nd output pin"),
                               this,
                               &m_csFilter,
                               &m_hr,
                               L"Output2");
    m_op[2] = new cvSyncOutputPin(NAME("3-rd output pin"),
                               this,
                               &m_csFilter,
                               &m_hr,
                               L"Output3");
    m_op[3] = new cvSyncOutputPin(NAME("4-th output pin"),
                               this,
                               &m_csFilter,
                               &m_hr,
                               L"Output4");

    for (int i=0;i<4;i++)
    {
        m_ip[i]->m_op = m_op[i];
        m_op[i]->m_ip = m_ip[i];
    }
}

CUnknown * WINAPI SyncFilter::CreateInstance(LPUNKNOWN punk, HRESULT *phr)
{
    SyncFilter *pNewObject = new SyncFilter(NAME("VideoSync Extended Filter for OpenCV"), punk, phr );

    if (pNewObject == NULL)
    {
        *phr = E_OUTOFMEMORY;
    }

    return pNewObject;
}

int SyncFilter::GetPinCount(void)
{
    return 8;
}

CBasePin* SyncFilter::GetPin(int n)
{
    if(n==0)
        return m_ip[0];
    if(n==1)
        return m_ip[1];
    if(n==2)
        return m_ip[2];
    if(n==3)
        return m_ip[3];
    if(n==4)
        return m_op[0];
    if(n==5)
        return m_op[1];
    if(n==6)
        return m_op[2];
    if(n==7)
        return m_op[3];

    return NULL;
}

HRESULT cvSyncOutputPin::CheckMediaType(const CMediaType* pmt)
{
    // Check this is a VIDEOINFO type

    if (*pmt->FormatType() != FORMAT_VideoInfo)
    {
        return E_INVALIDARG;
    }

    VIDEOINFO* vi = (VIDEOINFO*)pmt->Format();
    if(vi->bmiHeader.biBitCount != 24)
    {
        return E_INVALIDARG;
    }

    return NOERROR;
}

CFactoryTemplate g_Templates[]=

{
    { L"SyncFilter"
            , &CLSID_SyncFilter
            , SyncFilter::CreateInstance  // function called by class factory
            , NULL
            , &sudMyFilter }     // address of the AMOVIESETUP_FILTER structure,
                                 //   or NULL if no structure exists
};

int g_cTemplates = sizeof(g_Templates)/sizeof(g_Templates[0]);

//
// DllRegisterServer
//
// Handle registration of this filter
//
STDAPI DllRegisterServer()
{
    return AMovieDllRegisterServer2( TRUE );

} // DllRegisterServer

//
// DllUnregisterServer
//
STDAPI DllUnregisterServer()
{
   return AMovieDllRegisterServer2( FALSE );

} // DllUnregisterServer

SyncFilter::~SyncFilter()
{
    for (int i=0;i<4;i++)
    {
        delete m_ip[i];
        delete m_op[i];
    }
}

HRESULT cvSyncInputPin::BeginFlush()
{
    CAutoLock lock_it(m_pLock);
    HRESULT hr = NOERROR;
    hr = m_op->DeliverBeginFlush();
    if (FAILED(hr))
        return hr;

    return CBaseInputPin::BeginFlush();
}

HRESULT cvSyncInputPin::BreakConnect()
{
    // Release any allocator that we are holding

    if(m_pAllocator)
    {
        m_pAllocator->Release();

        m_pAllocator = NULL;
    }

    return NOERROR;
}

HRESULT cvSyncInputPin::CompleteConnect(IPin *pReceivePin)
{
    HRESULT hr = CBaseInputPin::CompleteConnect(pReceivePin);

    if (FAILED(hr))
    {
        return hr;
    }

    if (m_op->m_Connected != NULL)
    {
        if (m_mt != m_op->m_mt)
            m_pFilter->ReconnectPin(m_op, &m_mt);
    }
    else
    {
        m_op->m_mt = m_mt;
    }

    m_Connected->QueryInterface(IID_IAMStreamConfig,(void**)&m_pVSC);

    if(!(m_pVSC.is_valid()))
        return E_UNEXPECTED;

    return S_OK;
}

HRESULT cvSyncInputPin::EndFlush()
{
    CAutoLock lock_it(m_pLock);
    HRESULT hr = NOERROR;
    hr = m_op->DeliverEndFlush();
    if (FAILED(hr))
        return hr;

    return CBaseInputPin::EndFlush();
}

HRESULT cvSyncInputPin::EndOfStream()
{
    CAutoLock lock_it(m_pLock);
    HRESULT hr = NOERROR;

    hr = m_op->DeliverEndOfStream();
    if (FAILED(hr))
        return hr;

    return(NOERROR);
}

HRESULT cvSyncInputPin::NewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate)
{
    CAutoLock lock_it(m_pLock);
    HRESULT hr = NOERROR;
    hr = m_op->DeliverNewSegment(tStart, tStop, dRate);
    if (FAILED(hr))
        return hr;
    return CBaseInputPin::NewSegment(tStart, tStop, dRate);
}

STDMETHODIMP cvSyncInputPin::Receive(IMediaSample *pSample)
{
    CAutoLock lock_it(m_pLock);

    // Check that all is well with the base class

    if(m_pSample.value() == NULL)
    {
        m_pSample = pSample;
        m_pSample->AddRef();
    }

    HRESULT hr = NOERROR;

    hr = CBaseInputPin::Receive(pSample);

    if (hr != NOERROR)
        return hr;

    m_pFilter->Receive();

    return NOERROR;
}

STDMETHODIMP cvSyncInputPin::NotifyAllocator(IMemAllocator *pAllocator, BOOL bReadOnly)
{
    CAutoLock lock_it(m_pLock);
    if (pAllocator == NULL)
        return E_FAIL;

    // Free the old allocator if any
    if(m_pAllocator)
    {
        m_pAllocator->Release();
        m_pAllocator = NULL;
    }

    // Store away the new allocator

    pAllocator->AddRef();
    m_pAllocator = pAllocator;

    // Notify the base class about the allocator
    return CBaseInputPin::NotifyAllocator(pAllocator,bReadOnly);
}

HRESULT cvSyncOutputPin::Active()
{
    CAutoLock lock_it(m_pLock);
    //CAutoLock lock_it_to(m_ip->m_pFilter->m_pLock);
    HRESULT hr = NOERROR;

    // Make sure that the pin is connected
    if (m_Connected == NULL)
        return NOERROR;

    // Create the output queue if we have to
    if ((!(HIWORD(m_pOutputQueue))||(LOWORD(m_pOutputQueue))))
    {

        m_pOutputQueue = new COutputQueue(m_Connected, &hr, TRUE, FALSE);
        if (m_pOutputQueue == NULL)
            return E_OUTOFMEMORY;

        // Make sure that the constructor did not return any error
        if (FAILED(hr))
        {
            delete m_pOutputQueue;
            m_pOutputQueue = NULL;
            return hr;
        }
    }

    // Pass the call on to the base class
    CBaseOutputPin::Active();
    return NOERROR;
}

HRESULT cvSyncOutputPin::CompleteConnect(IPin *pReceivePin)
{
    CAutoLock lock_it(m_pLock);
    ASSERT(m_Connected == pReceivePin);
    HRESULT hr = NOERROR;

    hr = CBaseOutputPin::CompleteConnect(pReceivePin);
    if (FAILED(hr))
        return hr;

    // If the type is not the same as that stored for the input
    // pin then force the input pins peer to be reconnected

    if (m_mt != m_ip->m_mt)
    {
        hr = m_ip->m_pFilter->ReconnectPin(m_ip->m_Connected, &m_mt);

        if(FAILED(hr))
        {
            return hr;
        }
    }

    return NOERROR;
}

HRESULT cvSyncOutputPin::DecideAllocator(IMemInputPin *pPin, IMemAllocator **ppAlloc)
{
    *ppAlloc = NULL;

    // Tell the pin about our allocator, set by the input pin.
    HRESULT hr = NOERROR;
    hr = pPin->NotifyAllocator(m_ip->m_pAllocator,FALSE);
    if (FAILED(hr))
        return hr;

    // Return the allocator
    *ppAlloc = m_ip->m_pAllocator;
    m_ip->m_pAllocator->AddRef();
    return NOERROR;
}

HRESULT cvSyncOutputPin::Deliver(IMediaSample *pMediaSample)
{
    CAutoLock lock_it(m_pLock);
    pMediaSample->AddRef();

    // Make sure that we have an output queue
    if (!((HIWORD(m_pOutputQueue))&&(LOWORD(m_pOutputQueue))))
        return NOERROR;
    //MessageBox(NULL,"we have queue","", MB_OK);

    HRESULT hr =m_pOutputQueue->Receive(pMediaSample);

    return hr;
}

HRESULT cvSyncOutputPin::DeliverBeginFlush()
{
    // Make sure that we have an output queue
    if (!((HIWORD(m_pOutputQueue))&&(LOWORD(m_pOutputQueue))))
        return NOERROR;

    m_pOutputQueue->BeginFlush();
    return NOERROR;
}

HRESULT cvSyncOutputPin::DeliverNewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate)
{
    // Make sure that we have an output queue
    if (!((HIWORD(m_pOutputQueue))&&(LOWORD(m_pOutputQueue))))
        return NOERROR;

    m_pOutputQueue->NewSegment(tStart, tStop, dRate);
    return NOERROR;
}

HRESULT cvSyncOutputPin::SetMediaType(const CMediaType *pmt)
{
    // Make sure that we have an input connected
    if (m_ip->m_Connected == NULL)
        return VFW_E_NOT_CONNECTED;

    // Make sure that the base class likes it
    HRESULT hr = NOERROR;
    hr = CBaseOutputPin::SetMediaType(pmt);
    if (FAILED(hr))
        return hr;

    return NOERROR;
}

STDMETHODIMP cvSyncOutputPin::EnumMediaTypes(IEnumMediaTypes **ppEnum)
{
    CAutoLock lock_it(m_pLock);
    ASSERT(ppEnum);

    // Make sure that we are connected
    if (m_ip->m_Connected == NULL)
        return VFW_E_NOT_CONNECTED;

    // We will simply return the enumerator of our input pin's peer
    return m_ip->m_Connected->EnumMediaTypes(ppEnum);

}

HRESULT cvSyncOutputPin::DeliverEndFlush()
{
    // Make sure that we have an output queue
    if (!((HIWORD(m_pOutputQueue))&&(LOWORD(m_pOutputQueue))))
        return NOERROR;

    m_pOutputQueue->EndFlush();
    return NOERROR;
}

HRESULT cvSyncOutputPin::DeliverEndOfStream()
{
    // Make sure that we have an output queue
    if (!((HIWORD(m_pOutputQueue))&&(LOWORD(m_pOutputQueue))))
        return NOERROR;

    m_pOutputQueue->EOS();

    return NOERROR;
}

HRESULT cvSyncOutputPin::Inactive()
{
    CAutoLock lock_it(m_pLock);
    //CAutoLock lock_it_to(m_ip->m_pFilter->m_pLock);

    // Delete the output queus associated with the pin.
    if ((HIWORD(m_pOutputQueue))&&(LOWORD(m_pOutputQueue)))
    {
        //m_pOutputQueue->FreeSamples();

        delete m_pOutputQueue;
        m_pOutputQueue = NULL;
    }

    CBaseOutputPin::Inactive();

    return NOERROR;
}

cvSyncInputPin::~cvSyncInputPin()
{
        m_pVSC=NULL;
}

HRESULT SyncFilter::Receive()
{
    CAutoLock lock_it(m_pLock);
    REFERENCE_TIME te[4], ts[4]; //timestamps
    static REFERENCE_TIME MediaTime1, MediaTime2 = 1;
    IMediaSample *pims[4];
    HRESULT hr = NOERROR;
    int i;

    for (i=0;i<m_NbConnected;i++)
        if(!m_ip[i]->m_pSample.is_valid())
            return NOERROR;

    for (i=0;i<m_NbConnected;i++)
    {
        hr = m_ip[i]->m_pSample->GetTime(&ts[i], &te[i]);

        if FAILED(hr)
        {
            m_ip[i]->m_pSample=NULL;
            return hr;
        }
    }

    for (i=0;i<m_NbConnected;i++)
    {
        hr = m_ip[i]->m_pSample->SetMediaTime(&MediaTime1,&MediaTime2);
        if FAILED(hr)
        {
            m_ip[i]->m_pSample=NULL;
            return hr;
        }
    }

    for (i=0;i<m_NbConnected;i++)
        pims[i]=m_ip[i]->m_pSample.value();
    Transform(pims);

    for (i=0;i<m_NbConnected;i++)
    {
        hr = m_op[i]->Deliver(m_ip[i]->m_pSample.value());
        if(FAILED(hr))
            m_ip[i]->m_pSample = NULL;
    }

    MediaTime1++;
    MediaTime2++;

    for (i=0;i<m_NbConnected;i++)
        m_ip[i]->m_pSample = NULL;

    return NOERROR;
}

STDMETHODIMP SyncFilter::Stop()
{
    CAutoLock lock_it(m_pLock);

    CBaseFilter::Stop();
    m_State = State_Stopped;

    return NOERROR;
}

STDMETHODIMP SyncFilter::Pause()
{
    CAutoLock cObjectLock(m_pLock);
    HRESULT hr = CBaseFilter::Pause();
    bool NotConnected=false;
    int i;

    for (i=0;i<m_NbConnected;i++)
        if (m_ip[i]->IsConnected() == FALSE)
        {
            NotConnected=true;
            break;
        }

    if (NotConnected)
        for (i=0;i<m_NbConnected;i++)
            m_ip[0]->EndOfStream();

    for (i=0;i<m_NbConnected;i++)
        m_ip[i]->m_pSample = NULL;

    return hr;
}

STDMETHODIMP SyncFilter::Run(REFERENCE_TIME tStart)
{
    CAutoLock cObjectLock(m_pLock);
    HRESULT hr = CBaseFilter::Run(tStart);
    bool NotConnected=false;
    int i;

    for (i=0;i<m_NbConnected;i++)
        if (m_ip[i]->IsConnected() == FALSE)
        {
            NotConnected=true;
            break;
        }

    if (NotConnected)
        for (i=0;i<m_NbConnected;i++)
            m_ip[0]->EndOfStream();

    return hr;
}

HRESULT cvSyncInputPin::Inactive()
{
    m_pSample = NULL;

    return CBaseInputPin::Inactive();
}

STDMETHODIMP SyncFilter::SetCallBack(void (__cdecl *func)(void *,void *))
{
    return SetCallBack2(func);
}

STDMETHODIMP SyncFilter::SetCallBack2(void (__cdecl *func)(void *,void *))
{
    m_callback2 = func;
    m_NbConnected=2;
    return S_OK;
}

STDMETHODIMP SyncFilter::SetCallBack3(void (__cdecl *func)(void *,void *,void *))
{
    m_callback3 = func;
    m_NbConnected=3;
    return S_OK;
}

STDMETHODIMP SyncFilter::SetCallBack4(void (__cdecl *func)(void *,void *,void *,void *))
{
    m_callback4 = func;
    m_NbConnected=4;
    return S_OK;
}

STDMETHODIMP SyncFilter::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    CheckPointer(ppv,E_POINTER);

    if (riid == IID_ISyncFilter)
    {
        return GetInterface((ISyncFilter *) this, ppv);
    }
    else
    {
        return CBaseFilter::NonDelegatingQueryInterface(riid, ppv);
    }
}

HRESULT SyncFilter::Transform(IMediaSample *pims[4])
{
    BYTE*    pData[4];
    IplImage image[4];
    AM_MEDIA_TYPE* pType[4];
    VIDEOINFOHEADER *pvi[4];
    int cxImage[4];
    int cyImage[4];
    int stride[4];

    for (int i=0;i<m_NbConnected;i++)
    {
        pims[i]->GetPointer(&pData[i]);
        pType[i] = &m_ip[i]->m_mt;
        pvi[i] = (VIDEOINFOHEADER *) pType[i]->pbFormat;

        if (pvi[i]->bmiHeader.biBitCount != 24)
            return NOERROR;

        cxImage[i]    = pvi[i]->bmiHeader.biWidth;
        cyImage[i]    = abs(pvi[i]->bmiHeader.biHeight);
        stride[i]     = (cxImage[i] * sizeof( RGBTRIPLE ) + 3) & -4;

        cvInitImageHeader( &image[i], cvSize(cxImage[i], cyImage[i]), 8, 3, IPL_ORIGIN_BL, 4 );
        image[i].widthStep = stride[i];

        cvSetImageData( &image[i], pData[i], stride[i] );
    }

    if (2==m_NbConnected)
        m_callback2(&image[0], &image[1]);
    else if (3==m_NbConnected)
        m_callback3(&image[0], &image[1], &image[2]);
    else if (4==m_NbConnected)
        m_callback4(&image[0], &image[1], &image[2], &image[3]);

    return S_OK;
}
