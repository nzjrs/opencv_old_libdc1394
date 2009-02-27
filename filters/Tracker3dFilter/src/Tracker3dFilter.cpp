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

#pragma warning(disable:4786)
#include <vector>
#include <algorithm>
#include <streams.h>
#include "autorelease.h"
#include <initguid.h>
#include "Tracker3dFilter.h"
#include "Tracker3dPropertyPage.h"
#include "cvaux.h"

#define ARRAY_SIZEOF(a) (sizeof(a)/sizeof((a)[0]))

// locks a critical section, and unlocks it automatically
// when the lock goes out of scope. Also allows explicit unlocking.
class AutoLock {
    CCritSec *m_lock;

public:
    AutoLock(CCritSec *lock)
    {
        m_lock = lock;
        m_lock->Lock();
    };

    ~AutoLock()
    {
        Unlock();
    };

    void Unlock()
    {
        if (m_lock)
        {
            m_lock->Unlock();
            m_lock = NULL;
        }
    };
};

//#define TRACE
#ifdef TRACE

#include <stdarg.h>
#include <stdio.h>

void Trace(const char *fmt, ...)
{
    char buf[200];
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    OutputDebugString(buf) ;
}
#endif

//
// CreateInstance
//
// Provide the way for COM to create a Tracker3dFilter object
//
CUnknown *Tracker3dFilter::CreateInstance(IUnknown *unk, HRESULT *phr)
{
    try
    {
        *phr = NOERROR;
        return new Tracker3dFilter(NAME("3d Tracker"), unk);
    }
    catch(HRESULT hr)
    {
        *phr = hr;
        return NULL;
    }
    catch (...)
    {
        *phr = E_OUTOFMEMORY;
        return NULL;
    }
} // CreateInstance


//
// Constructor
//
Tracker3dFilter::Tracker3dFilter(TCHAR *name, IUnknown *unk)
    : CBaseFilter(name, unk, &m_cs, CLSID_Tracker3dFilter)
{
    m_num_streams = 0;
    m_streams = NULL;
    m_camera_configuration_loaded = false;
    m_pCameraInfo = NULL;
    m_tracker_clsid = GUID_NULL;
    m_calibrate_cameras = 0;
    m_camera_intrinsics = NULL;
    m_viewing_stream = 0;
    m_preferred_size = SIZE_Any;
    m_end_of_stream_count = 0;
}

Tracker3dFilter::~Tracker3dFilter()
{
    for (int i = 0; i < m_callbacks.size(); i++)
        m_callbacks[i]->Release();

    delete [] m_streams;
    delete [] m_pCameraInfo;
    delete [] m_camera_intrinsics;
}


//
// NonDelegatingQueryInterface
//
STDMETHODIMP Tracker3dFilter::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    CheckPointer(ppv,E_POINTER);

    if (riid == IID_ITracker3dFilter)
        return GetInterface(static_cast<ITracker3dFilter *>(this), ppv);
    else if (riid == IID_ITracker3dInternal)
        return GetInterface(static_cast<ITracker3dInternal *>(this), ppv);
    else if (riid == IID_ISpecifyPropertyPages)
        return GetInterface(static_cast<ISpecifyPropertyPages *>(this), ppv);
    else if (riid == IID_IPersist)
        return GetInterface(static_cast<IPersist *>(static_cast<IPersistStream *>(this)), ppv);
    else if (riid == IID_IPersistStream)
        return GetInterface(static_cast<IPersistStream *>(this), ppv);
    else
        return CBaseFilter::NonDelegatingQueryInterface(riid, ppv);
}

int Tracker3dFilter::GetPinCount()
{
    return 2 * m_num_streams;
}

CBasePin *Tracker3dFilter::GetPin(int n)
{
    if (n > GetPinCount())
        return NULL;
    if (n % 2)
        return m_streams[n/2].output_pin;
    else
        return m_streams[n/2].input_pin;
}

static HRESULT InitImageHeader(IplImage *image_header, const CMediaType *mt)
{
    const GUID *subtype = mt->Subtype();
    ASSERT(*mt->FormatType() == FORMAT_VideoInfo);
    VIDEOINFO *fmt = (VIDEOINFO *)mt->Format();

    int channels;
    int origin;

    // We assume that RGB images are inverted and YUV images are not...
    if (*subtype == MEDIASUBTYPE_RGB24 || *subtype == MEDIASUBTYPE_RGB32)
    {
        channels = fmt->bmiHeader.biBitCount / 8;
        origin = IPL_ORIGIN_BL;
    }
    else if (*subtype == MEDIASUBTYPE_YVU9
             || *subtype == MEDIASUBTYPE_YV12
             || *subtype == MEDIASUBTYPE_IYUV)
    {
        // Note: these are planar images, which for our purposes we treat as
        // a single plane of Y8 and ignore the rest.
        channels = 1;
        origin = IPL_ORIGIN_TL;
    }
    else // Other trackers could support other formats, which should be handled here
    {
        return E_FAIL;
    }

    cvInitImageHeader(image_header, cvSize(fmt->bmiHeader.biWidth, fmt->bmiHeader.biHeight),
                      IPL_DEPTH_8U, channels, origin, 4);
    return NOERROR;
}

HRESULT Tracker3dFilter::CheckMediaType(int pin, const CMediaType *mt)
{
    Stream &stream = m_streams[pin];

    if (stream.media_type.IsValid())
        return *mt == stream.media_type ? S_OK : E_FAIL;

    if (*mt->Type() != MEDIATYPE_Video || *mt->FormatType() != FORMAT_VideoInfo)
        return E_FAIL;

    IplImage image_header;
    HRESULT hr = InitImageHeader(&image_header, mt);
    if (FAILED(hr))
        return hr;

    if (m_preferred_size == SIZE_320x240
        && (image_header.width != 320 || image_header.height != 240))
    {
        return E_FAIL;
    }

    if (m_preferred_size == SIZE_640x480
        && (image_header.width != 640 || image_header.height != 480))
    {
        return E_FAIL;
    }

    if (stream.tracker != NULL)
    {
        hr = stream.tracker->CheckFormat(&image_header);
        if (FAILED(hr))
            return hr;
    }

    return S_OK;
}


HRESULT Tracker3dFilter::SetMediaType(int pin, const CMediaType *mt)
{
    Stream &stream = m_streams[pin];

    if (stream.media_type.IsValid())
        return *mt == stream.media_type ? S_OK : E_FAIL;

    HRESULT hr = CheckMediaType(pin, mt);
    if (FAILED(hr))
        return hr;

    hr = InitImageHeader(&stream.image_header1, mt);
    if (FAILED(hr))
        return hr;
    stream.image_header2 = stream.image_header1;

    if (stream.tracker != NULL)
        stream.tracker->SetFormat(&stream.image_header1);

    stream.media_type = *mt;
    return NOERROR;
}

HRESULT Tracker3dFilter::GetMediaType(int pin, int pos, CMediaType *mt)
{
    if (pos < 0)
        return E_INVALIDARG;

#define IYUV 0x56555949
    static const struct
    {
        const GUID *subtype;
        FOURCC fourcc;
        int width, height, depth;
    } types[] = {
        { &MEDIASUBTYPE_IYUV,  IYUV,   640, 480, 12 },
        { &MEDIASUBTYPE_RGB24, BI_RGB, 640, 480, 24 },
        { &MEDIASUBTYPE_RGB24, BI_RGB, 640, 480, 32 },
        { &MEDIASUBTYPE_RGB32, BI_RGB, 640, 480, 32 },
        { &MEDIASUBTYPE_IYUV,  IYUV,   320, 240, 12 },
        { &MEDIASUBTYPE_RGB24, BI_RGB, 320, 240, 24 },
        { &MEDIASUBTYPE_RGB24, BI_RGB, 320, 240, 32 },
        { &MEDIASUBTYPE_RGB32, BI_RGB, 320, 240, 32 },
    };

    // If our media type has been set, we respond only with that type;
    // otherwise, we respond with each of the types we know about.
    if (m_streams[pin].media_type.IsValid())
    {
        if (pos == 0)
        {
            *mt = m_streams[pin].media_type;
            return S_OK;
        }
    }
    else if (pos < ARRAY_SIZEOF(types))
    {
        const GUID *subtype = types[pos].subtype;
        FOURCC fourcc = types[pos].fourcc;
        int width = types[pos].width;
        int height = types[pos].height;
        int depth = types[pos].depth;

        VIDEOINFOHEADER fmt = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, 30*width*height*depth, 0, 333333,
                                { sizeof(BITMAPINFOHEADER), width, height, 1, depth, fourcc, width*height*depth/8, 0, 0, 0, 0 } };
        mt->SetType(&MEDIATYPE_Video);
        mt->SetTemporalCompression(false);
        mt->SetSubtype(subtype);
        mt->SetSampleSize(width*height*depth/8);
        mt->SetFormatType(&FORMAT_VideoInfo);
        mt->SetFormat((BYTE *)&fmt, sizeof(fmt));
        return S_OK;
    }

    return VFW_S_NO_MORE_ITEMS;
}

STDMETHODIMP Tracker3dFilter::Run(REFERENCE_TIME t)
{
    if (!m_camera_configuration_loaded && m_calibrate_cameras == 0)
        LoadCameraConfiguration();
    return CBaseFilter::Run(t);
}

STDMETHODIMP Tracker3dFilter::Pause()
{
    return CBaseFilter::Pause();
}

STDMETHODIMP Tracker3dFilter::Stop()
{
    AutoLock lock1(&m_cs);
    AutoLock lock2(&m_recv_cs);

    HRESULT hr = CBaseFilter::Stop();
    if (FAILED(hr))
        return hr;

    for (int i = 0; i < m_num_streams; i++)
        Flush(i);
    m_end_of_stream_count = 0;

    return NOERROR;
}

HRESULT Tracker3dFilter::Flush(int pin)
{
    Stream::SampleQueue &q = m_streams[pin].queue;
    while (!q.empty())
        q.pop_front();
    return NOERROR;
}

Tracker3dFilter::Stream::Stream()
     : input_pin(NULL),
       output_pin(NULL),
       tracker(NULL),
       discarded_frames(0),
       total_frames(0)
{
}

Tracker3dFilter::Stream::~Stream()
{
    SAFE_RELEASE(input_pin);
    SAFE_RELEASE(output_pin);
    SAFE_RELEASE(tracker);
}

void Tracker3dFilter::Stream::operator =(Stream &s)
{
    SAFE_RELEASE(input_pin);
    SAFE_RELEASE(output_pin);
    SAFE_RELEASE(tracker);

    input_pin = s.input_pin;   s.input_pin = NULL;
    output_pin = s.output_pin; s.output_pin = NULL;
    tracker = s.tracker;       s.tracker = NULL;
    image_header1 = image_header2 = s.image_header1;
    media_type = s.media_type;
}

inline Tracker3dFilter::Stream::Sample::Sample(IMediaSample *s, const ITracker::TrackingInfo &t)
    : sample(s),
      tracking_info(t)
{
    sample->AddRef();
    sample->GetTime(&ts, &te);
    sample->GetPointer(&data);
}

inline Tracker3dFilter::Stream::Sample::Sample(const Sample &s)
    : sample(s.sample),
      ts(s.ts),
      te(s.te),
      data(s.data),
      tracking_info(s.tracking_info)
{
    sample->AddRef();
}

inline void Tracker3dFilter::Stream::Sample::operator=(const Sample &s)
{
    sample->Release();

    sample = s.sample;
    ts = s.ts;
    te = s.te;
    data = s.data;
    tracking_info = s.tracking_info;
    sample->AddRef();
}

inline Tracker3dFilter::Stream::Sample::~Sample()
{
    sample->Release();
}

//-----------------------
// This is an attempt to match the frames up across the streams which are connected to the tracker.
// The goal is to minimize the time differences between the frames in one set.  This is necessary since 
// there is no available syncronization mechanism for the independant USB cameras or other video sources.
//-----------------------
bool Tracker3dFilter::QueuesReady()
{
    int i;

    REFERENCE_TIME greatest = 0;
    REFERENCE_TIME least = 0;
    for (i = 0; i < m_num_streams; i++)
    {
        Stream::SampleQueue &q = m_streams[i].queue;
        if (q.empty())
            return false;
        REFERENCE_TIME ts = q.front().ts;
        if (ts > greatest)
            greatest = ts;
        if (ts < least || least == 0)
            least = ts;
    }

    // If the difference between the greatest timestamp and the least is less than
    // the specified difference (1/60 sec or 1/2 nominal frame time), all queues
    // are ready.
#define MAX_DIFF (10000000/60)
    if (greatest - least <= MAX_DIFF)
        return true;

    // Otherwise, check in each queue whether the 'greatest' frame is a closer time
    // match to the second frame in the queue than to the first frame in the queue.
    // If so, discard the first frame in the queue.
    // This algorithm requires a second frame in each queue, so it introduces one
    // full frame delay.
    for (i = 0; i < m_num_streams; i++)
    {
        Stream::SampleQueue &q = m_streams[i].queue;
        while (q.size() > 1 && abs(q.begin()->ts - greatest) > abs((q.begin() + 1)->ts - greatest))
        {
            m_streams[i].discarded_frames++;
#ifdef TRACE
            Trace("Discard:   %*s %5lu\n", 6*i, "", (unsigned long)q.front().ts/10000);
#endif
            q.pop_front();
        }

        if (q.size() > 1)
            continue;
        if (abs(greatest - q.front().ts) <= MAX_DIFF)
            continue;
        return false;
    }

    return true;
}

//-----------------------
// Performs the frame to frame work...
//-----------------------
HRESULT Tracker3dFilter::Receive(int pin, IMediaSample *sample)
{
    int i;

    Stream &stream = m_streams[pin];
    ITracker::TrackingInfo tracking_info;

    if (stream.tracker != NULL && m_calibrate_cameras == 0)
    {
        unsigned char *data;
        sample->GetPointer(&data);
        stream.image_header1.imageData = (char *)data;
        try
        {
            stream.tracker->Process(&stream.image_header1);
            stream.tracker->GetTrackedObjects(tracking_info);
        }
        catch (...)
        {
        }
        stream.image_header1.imageData = NULL;
    }

    AutoLock lock(&m_recv_cs);

    stream.queue.push_back(Stream::Sample(sample, tracking_info));

#ifdef TRACE
    Trace("Receive(%d):%*s %5lu\n", pin, 6*pin, "", (unsigned long)stream.queue.back().ts/10000);
#endif

    if (!QueuesReady())
        return NOERROR;

#ifdef TRACE
    char buf[100] = "Process:   ";
    char *p = buf + strlen(buf);
    REFERENCE_TIME greatest = 0;
    REFERENCE_TIME least = 0;
    for (i = 0; i < m_num_streams; i++)
    {
        REFERENCE_TIME ts = m_streams[i].queue.front().ts;
        if (ts > greatest)
            greatest = ts;
        if (ts < least || least == 0)
            least = ts;
        sprintf(p, " %5lu", (unsigned long)ts/10000);
        p += strlen(p);
    }
    sprintf(p, "  %4ld", (long)((greatest - least)/10000));
    p += strlen(p);
    static unsigned long processed_frames;
    sprintf(p, ";  %4lu", ++processed_frames);
    p += strlen(p);
    for (i = 0; i < m_num_streams; i++)
    {
        Stream &s = m_streams[i];
        sprintf(p, " %4lu (%2lu%%)", s.discarded_frames, (100*s.discarded_frames/(processed_frames+s.discarded_frames)));
        p += strlen(p);
    }

    Trace("%s\n", buf);
#endif

    if (m_calibrate_cameras != 0)
    {
        std::vector<IplImage *> samples(m_num_streams);
        for (i = 0; i < m_num_streams; i++)
        {
            m_streams[i].image_header2.imageData = (char *)m_streams[i].queue.front().data;
            samples[i] = &m_streams[i].image_header2;
        }
		//
		//  This is the second most significant call in the filter, this is how the relative locations of 
		//  all of the cameras are determined.  This information is vital to the later process of determining
		//  the location of the targets in 3D from the set of 2D results provided by the trackers.
		//
        if (cv3dTrackerCalibrateCameras(m_num_streams, m_camera_intrinsics, m_etalon_size,
                                        m_square_size, &samples[0], m_pCameraInfo))
        {
            if (m_calibrate_cameras == 1)
            {
                m_calibrate_cameras = 0;
                SaveCameraConfiguration();
            }
        }
    }
    else
    {
        int num_objects = 0;

        for (i = 0; i < m_num_streams; i++)
        {
            int n = m_streams[i].queue.front().tracking_info.size();
            if (n > num_objects)
                num_objects = n;
        }

        if (num_objects != 0)
        {
            std::vector<Cv3dTracker2dTrackedObject> tracking_info(m_num_streams * num_objects,
                                                                  cv3dTracker2dTrackedObject(-1, CvPoint()));
            for (i = 0; i < m_num_streams; i++)
            {
                ITracker::TrackingInfo &t = m_streams[i].queue.front().tracking_info;

                if (m_streams[i].image_header2.origin == IPL_ORIGIN_BL)
                    for (int j = 0; j < t.size(); j++)
                        t[j].p.y = m_streams[i].image_header2.height - 1 - t[j].p.y;

                for (int j = 0; j < t.size(); j++)
                    tracking_info[i*num_objects+j] = t[j];
            }

            m_tracked_objects.resize(num_objects);
			//
			//  This is it.  The whole point of this is this call.  
			//  For each matched set of results from the trackers this call will compute the 3D coordinates of the targets.
			//
            int n = cv3dTrackerLocateObjects(m_num_streams, num_objects, m_pCameraInfo, &tracking_info[0], &m_tracked_objects[0]);
            m_tracked_objects.resize(n);
        }
        else
            m_tracked_objects.clear();
    }

    // Copy viewing sample out of queue before releasing lock.
    Stream::Sample viewing_sample = m_streams[m_viewing_stream].queue.front();

    for (i = 0; i < m_num_streams; i++)
    {
        m_streams[i].output_pin->Deliver(m_streams[i].queue.front().sample);
        m_streams[i].queue.pop_front();
    }

    lock.Unlock();

    for (i = 0; i < m_callbacks.size(); i++)
        m_callbacks[i]->Callback(m_tracked_objects, viewing_sample.data, (IUnknown *)viewing_sample.sample);

    return NOERROR;
}

HRESULT Tracker3dFilter::EndOfStream(int pin)
{
    AutoLock lock(&m_recv_cs);

    m_end_of_stream_count++;

    if (m_end_of_stream_count == m_num_streams)
    {
        for (int i = 0; i < m_num_streams; i++)
        {
            Flush(i);
            m_streams[i].output_pin->DeliverEndOfStream();
        }
    }

    return NOERROR;
}

void Tracker3dFilter::SetNumberOfStreams(int num_streams)
{
    if (num_streams == m_num_streams)
        return;

    delete [] m_camera_intrinsics;
    m_camera_intrinsics = NULL;

    Stream *streams = NULL;
    Cv3dTrackerCameraInfo *camera_info = NULL;

    if (num_streams > 0)
    {
        int i;

        camera_info = new Cv3dTrackerCameraInfo [ num_streams ];
        streams = new Stream [ num_streams ];
        for (i = 0; i < num_streams; i++)
        {
            if (i < m_num_streams)
            {
                streams[i] = m_streams[i];
                camera_info[i] = m_pCameraInfo[i];
            }
            else
            {
                streams[i].input_pin = new Tracker3dInputPin(i, this, &m_cs);
                streams[i].output_pin = new Tracker3dOutputPin(i, this, &m_cs);
                if (m_tracker_clsid != GUID_NULL)
                {
                    // If CoCreateInstance fails, m_streams[i].tracker is left as NULL.
                    CoCreateInstance(m_tracker_clsid, NULL, CLSCTX_INPROC_SERVER, IID_ITracker, (void **)&streams[i].tracker);
                }
                camera_info[i].valid = false;
            }
        }
    }

    delete [] m_streams;
    m_streams = streams;

    delete [] m_pCameraInfo;
    m_pCameraInfo = camera_info;

    m_num_streams = num_streams;
    IncrementPinVersion();
}


//
// GetPages
//
// Returns the clsid's of the property pages we support
//
STDMETHODIMP Tracker3dFilter::GetPages(CAUUID *pPages)
{
    std::vector<GUID> pages;
    pages.push_back(CLSID_Tracker3dPropertyPage);

    // In addition to our property page, return a separate property page for each type of tracker.
    // Normally all the trackers will be the same, so only one property page will be returned,
    // but we call each tracker to see if it wants a different property page.
    // It is up to each property page to call GetTrackers and connect to all the trackers
    // that are of the appropriate type.
    if (m_streams != NULL)
    {
        for (int i = 0; i < m_num_streams; i++)
        {
            GUID page;
            if (m_streams[i].tracker != NULL
                && SUCCEEDED(m_streams[i].tracker->GetPropertyPage(&page)))
            {
                if (std::find(pages.begin(), pages.end(), page) == pages.end())
                    pages.push_back(page);
            }
        }
    }

    // Now that we've determined how many pages we need, allocate the memory
    // and fill it in.
    pPages->cElems = pages.size();
    pPages->pElems = (GUID *) CoTaskMemAlloc(pages.size() * sizeof(GUID));
    if (pPages->pElems == NULL)
        return E_OUTOFMEMORY;

    for (int i = 0; i < pages.size(); i++)
        pPages->pElems[i] = pages[i];

    return NOERROR;
}


// IPersistStream
STDMETHODIMP Tracker3dFilter::GetClassID(CLSID *clsid)
{
    *clsid = CLSID_Tracker3dFilter;
    return NOERROR;
}

STDMETHODIMP Tracker3dFilter::IsDirty()
{
    return S_OK;
}

static const int PERSIST_STREAM_VERSION0 = 0;
static const int PERSIST_STREAM_VERSION0_SIZE = 3;
static const int PERSIST_STREAM_VERSION1 = 1;
static const int PERSIST_STREAM_VERSION1_SIZE = 4;
static const int PERSIST_STREAM_VERSION2 = 2;
static const int PERSIST_STREAM_VERSION2_SIZE = PERSIST_STREAM_VERSION1_SIZE;
static const int PERSIST_STREAM_VERSION3 = 3;
static const int PERSIST_STREAM_VERSION3_SIZE = 3 + sizeof(GUID);

STDMETHODIMP Tracker3dFilter::Load(IStream *stream)
{
    unsigned long nread;
    unsigned char buf[3];

    HRESULT hr = stream->Read(buf, 3, &nread);
    if (FAILED(hr))
        return hr;

    if (nread != 3)
        return E_FAIL;

    int num_streams;

    if (buf[0] == PERSIST_STREAM_VERSION0 && buf[1] == PERSIST_STREAM_VERSION0_SIZE)
    {
        num_streams = buf[2];
    }
    else if (buf[0] == PERSIST_STREAM_VERSION1 && buf[1] == PERSIST_STREAM_VERSION1_SIZE)
    {
        num_streams = buf[2];
    }
    else if (buf[0] == PERSIST_STREAM_VERSION2 && buf[1] == PERSIST_STREAM_VERSION2_SIZE)
    {
        num_streams = buf[2] & 0x3f;
        m_preferred_size = (InputSize)((buf[2] >> 6) & 0x03);
    }
    else if (buf[0] == PERSIST_STREAM_VERSION3)
    {
        num_streams = buf[2] & 0x3f;
        m_preferred_size = (InputSize)((buf[2] >> 6) & 0x03);
        if (buf[1] == PERSIST_STREAM_VERSION3_SIZE)
        {
            GUID guid;
            hr = stream->Read(&guid, sizeof(GUID), &nread);
            if (FAILED(hr))
                return hr;
            if (nread != sizeof(GUID))
                return E_FAIL;
            m_tracker_clsid = guid;
        }
    }
    else
    {
        return E_FAIL;
    }

    if (buf[0] == PERSIST_STREAM_VERSION1 || buf[0] == PERSIST_STREAM_VERSION2)
    {
        unsigned char lib_name_len;

        hr = stream->Read(&lib_name_len, 1, &nread);
        if (FAILED(hr))
            return hr;
        if (nread != 1)
            return E_FAIL;

        if (lib_name_len > 0)
        {
            char lib_name[256];
            hr = stream->Read(lib_name, lib_name_len, &nread);
            // We don't support this format any more; ignore the
            // default tracker dll name, but allow the graph
            // to load.
            //if (SUCCEEDED(hr) && nread == lib_name_len)
            //{
            //    lib_name[lib_name_len] = '\0';
            //    SetDefaultTrackerDLL(lib_name);
            //}
        }
    }

    SetTrackers(num_streams, NULL);

    return NOERROR;
}

STDMETHODIMP Tracker3dFilter::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT hr;
    unsigned long dummy;
    unsigned char buf[PERSIST_STREAM_VERSION3_SIZE] = { PERSIST_STREAM_VERSION3,
                                                        PERSIST_STREAM_VERSION3_SIZE,
                                                        (unsigned char)((m_num_streams & 0x7f) | (m_preferred_size << 6))
                                                      };
    
    if (m_tracker_clsid != GUID_NULL)
        memcpy(buf+3, &m_tracker_clsid, sizeof(GUID));
    else
        buf[1] = 3;
    
    hr = stream->Write(&buf, buf[1], &dummy);
    if (FAILED(hr))
        return hr;

    return NOERROR;
}

STDMETHODIMP Tracker3dFilter::GetSizeMax(ULARGE_INTEGER *size)
{
    size->QuadPart = PERSIST_STREAM_VERSION3_SIZE;
    return NOERROR;
}


// ITracker3dFilter

STDMETHODIMP Tracker3dFilter::GetNumberOfCameras(int &num_streams)
{
    num_streams = m_num_streams;
    return NOERROR;
}

class AutoClose
{
    FILE ** m_ppfile;

public:
    AutoClose(FILE ** ppfile) : m_ppfile(ppfile) { };
    ~AutoClose()
    {
        if (*m_ppfile != NULL)
        {
            fclose(*m_ppfile);
            *m_ppfile = NULL;
        }
    };
};

#define AUTO_CLOSE(f) AutoClose AutoClose##f(&f)

STDMETHODIMP Tracker3dFilter::LoadCameraConfiguration(const char *filename)
{
    FILE *f = fopen(filename, "r"); AUTO_CLOSE(f);
    if (f == NULL)
        return E_FAIL;

    int num_streams;
    int r;
    r = fscanf(f, "%ld\n", &num_streams);
    if (r != 1 || num_streams <= 0)
        return E_FAIL;

    SetNumberOfStreams(num_streams);

    for (int c = 0; c < m_num_streams; c++)
    {
        Cv3dTrackerCameraInfo &camera = m_pCameraInfo[c];

        char camera_name[80];

        if (fscanf(f, "%[^:]:", camera_name) != 1)
            return E_FAIL;
        if (fscanf(f, "%g, %g;", &camera.principal_point.x, &camera.principal_point.y) != 2)
            return E_FAIL;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (fscanf(f, "%g", &camera.mat[i][j]) != 1)
                    return E_FAIL;
        //camera.camera_name = camera_name;
        camera.valid = true;
    }

    m_camera_configuration_loaded = true;

    return NOERROR;
}

STDMETHODIMP Tracker3dFilter::SaveCameraConfiguration(const char *filename)
{
    if (m_pCameraInfo == NULL)
        return E_FAIL;

    FILE *f = fopen(filename, "w");
    if (f == NULL)
        return E_FAIL;

    fprintf(f, "%lu\n", m_num_streams);

    for (int c = 0; c < m_num_streams; c++)
    {
        Cv3dTrackerCameraInfo &camera = m_pCameraInfo[c];

        fprintf(f, "Camera %d: ", c);
        //fprintf(f, "%s: ", camera.camera_name.c_str());
        fprintf(f, " %g, %g;", camera.principal_point.x, camera.principal_point.y);
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                fprintf(f, " %g", camera.mat[i][j]);
        fprintf(f, "\n");
    }

    fclose(f);

    return NOERROR;
}

static std::string subst(std::string s, char i, char o)
{
    std::string::size_type pos = 0;
    while ((pos = s.find(i, pos)) != std::string::npos)
        s.replace(pos, 1, 1, o);
    return s;
}

std::string Tracker3dFilter::CalibrationName(int i)
{
    std::string camera_name;
    GetCameraName(i, camera_name);
    return subst(camera_name, '\\', '#');
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

//------------------
// Once camera extrinsic parameters are determined they are stored in the registry in the key
// Software\\Intel\\VAI\\3d Tracker\\
// Read them back from here when the app starts.
//------------------
void Tracker3dFilter::LoadCameraConfiguration()
{
    LONG r;

    HKEY key = 0; AUTO_CLOSE_KEY(key);
    r = RegOpenKeyEx(HKEY_LOCAL_MACHINE, key_name, 0,KEY_READ, &key);
    if (r != ERROR_SUCCESS)
        return;

    int valid_count = 0;
    for (int c = 0; c < m_num_streams; c++)
    {
        Cv3dTrackerCameraInfo &camera = m_pCameraInfo[c];
        char buf[18*15];
        DWORD size = sizeof(buf);
        r = RegQueryValueEx(key, CalibrationName(c).c_str(), 0, NULL, (BYTE *)buf, &size);
        if (r != ERROR_SUCCESS)
            continue;

        if (sscanf(buf, "%g %g; %g %g %g %g, %g %g %g %g, %g %g %g %g, %g %g %g %g",
                        &camera.principal_point.x, &camera.principal_point.y,
                        &camera.mat[0][0], &camera.mat[0][1], &camera.mat[0][2], &camera.mat[0][3],
                        &camera.mat[1][0], &camera.mat[1][1], &camera.mat[1][2], &camera.mat[1][3],
                        &camera.mat[2][0], &camera.mat[2][1], &camera.mat[2][2], &camera.mat[2][3],
                        &camera.mat[3][0], &camera.mat[3][1], &camera.mat[3][2], &camera.mat[3][3]) != 18)
            continue;

        camera.valid = true;
        valid_count++;
    }

    m_camera_configuration_loaded = (valid_count == m_num_streams);
}

void Tracker3dFilter::SaveCameraConfiguration()
{
    LONG r;
    
    HKEY key = 0; AUTO_CLOSE_KEY(key);
    r = RegCreateKeyEx(HKEY_LOCAL_MACHINE, key_name, 0, NULL, 0, KEY_WRITE, NULL, &key, NULL);
    if (r != ERROR_SUCCESS)
        return;

    for (int c = 0; c < m_num_streams; c++)
    {
        Cv3dTrackerCameraInfo &camera = m_pCameraInfo[c];
        char buf[18*15];
        char *p = buf;
        int len = sprintf(p, "%g %g;", camera.principal_point.x, camera.principal_point.y);
        p += len;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                len = sprintf(p, " %g", camera.mat[i][j]);
                p += len;
            }
            *p++ = ',';
        }
        RegSetValueEx(key, CalibrationName(c).c_str(), 0, REG_SZ, (BYTE *)buf, p - buf + 1);
    }
}


STDMETHODIMP Tracker3dFilter::GetDefaultTracker(GUID &tracker_clsid)
{
    tracker_clsid = m_tracker_clsid;
    return NOERROR;
}

STDMETHODIMP Tracker3dFilter::SetDefaultTracker(const GUID &tracker_clsid)
{
    m_tracker_clsid = tracker_clsid;
    return NOERROR;
}

STDMETHODIMP Tracker3dFilter::SetTrackers(int num_streams, ITracker * const *trackers)
{
    SetNumberOfStreams(num_streams);

    if (trackers == NULL)
    {
        for (int i = 0; i < m_num_streams; i++)
        {
            SAFE_RELEASE(m_streams[i].tracker);

            if (m_tracker_clsid != GUID_NULL)
            {
                // If CoCreateInstance fails, m_streams[i].tracker is left as NULL.
                CoCreateInstance(m_tracker_clsid, NULL, CLSCTX_INPROC_SERVER, IID_ITracker, (void **)&m_streams[i].tracker);
                if (m_streams[i].media_type.IsValid())
                    m_streams[i].tracker->SetFormat(&m_streams[i].image_header1);
            }
        }
    }
    else
    {
        for (int i = 0; i < m_num_streams; i++)
        {
            SAFE_RELEASE(m_streams[i].tracker);

            // Store and addref new tracker
            m_streams[i].tracker = trackers[i];
            if (m_streams[i].tracker != NULL)
            {
                m_streams[i].tracker->AddRef();
                if (m_streams[i].media_type.IsValid())
                    m_streams[i].tracker->SetFormat(&m_streams[i].image_header1);
            }
        }
    }

    return NOERROR;
}


STDMETHODIMP Tracker3dFilter::GetTrackers(std::vector<ITracker *> &trackers)
{
    trackers.resize(m_num_streams);
    for (int i = 0; i < m_num_streams; i++)
    {
        trackers[i] = m_streams[i].tracker;
        if (trackers[i] != NULL)
            trackers[i]->AddRef();
    }
    return NOERROR;
}


//----------------------
// Read the intrinsics information for all of the from a list of files.
// The following is an example of the format that the file is reading.
// The top left 2 diagonal elements of the matrix are the horizontal and 
// vertical focal lengths and the top 2 elements in the last column 
// indicate the image center coordinates, x in the top row, y in the second.
// 
// The Distortion parameters indicate the radial and barrel distortion of the lens.
//
// All of these results are further discussed in the documentation concerning 
// the CalibFilter.
//----------------------

/*
Camera Matrix:
M[0.0]=         401.3238525M[0.1]=           0.0000000M[0.2]=         135.9540710
M[1.0]=           0.0000000M[1.1]=         403.9630737M[1.2]=         116.6698456
M[2.0]=           0.0000000M[2.1]=           0.0000000M[2.2]=           1.0000000


Distortion:
D[0]=-0.011825
D[1]=0.241698
D[2]=0.001189
D[3]=0.003923
*/

HRESULT Tracker3dFilter::ReadCameraIntrinsics(const char *filenames[])
{
    // read camera intrinsics for all cameras: focal length[2], principal_point.x&y, distortion[4]
    if (m_camera_intrinsics == NULL)
        m_camera_intrinsics = new Cv3dTrackerCameraIntrinsics [ m_num_streams ];

    for (int c = 0; c < m_num_streams; c++)
    {
        Cv3dTrackerCameraIntrinsics &camera = m_camera_intrinsics[c];

        FILE *file = fopen(filenames[c], "r"); AUTO_CLOSE(file);
        if (file == NULL)
            return E_FAIL;

#define BUF_SIZE 500
        char buffer[BUF_SIZE+1];
        int sz = fread( buffer, 1, BUF_SIZE, file );
        buffer[sz] = '\0';

        int i, j, k;
        float camera_matrix[3][3];
        char* ptr = buffer;
        /* read matrix */
        for( k = 0; k < 9; k++ )
        {
            ptr = strstr( ptr, "M[" );
            if( ptr )
            {
                int s = 0;
                ptr += 2;
                if( sscanf( ptr, "%d%*[.,]%d%n", &i, &j, &s ) == 2 && i == k/3 && j == k%3 )
                {
                    ptr += s;
                    ptr = strstr( ptr, "=" );
                    if( ptr )
                    {
                        s = 0;
                        ptr++;
                        if( sscanf( ptr, "%f%n", &camera_matrix[i][j], &s ) == 1 )
                        {
                            ptr += s;
                            continue;
                        }
                    }
                }
            }

            return E_FAIL;
        }

        camera.focal_length[0] = camera_matrix[0][0];
        camera.focal_length[1] = camera_matrix[1][1];

        camera.principal_point.x = camera_matrix[0][2];
        camera.principal_point.y = camera_matrix[1][2];

        /* read distortion */
        for( k = 0; k < 4; k++ )
        {
            ptr = strstr( ptr, "D[" );
            if( ptr )
            {
                int s = 0;
                ptr += 2;
                if( sscanf( ptr, "%d%n", &i, &s ) == 1 && i == k )
                {
                    ptr += s;
                    ptr = strstr( ptr, "=" );
                    if( ptr )
                    {
                        s = 0;
                        ptr++;
                        if( sscanf( ptr, "%f%n", &camera.distortion[k], &s ) == 1 )
                        {
                            ptr += s;
                            continue;
                        }
                    }
                }
            }

            return E_FAIL;
        }
    }

    return NOERROR;
}


//---------------------
// Initiate the camera calibration process.  Actual calibration call is made in Receive()
//---------------------
STDMETHODIMP Tracker3dFilter::CalibrateCameras(int checkerboard_width, int checkerboard_height,
                                               const char *camera_intrinsics_filenames[],
                                               float square_size,
                                               bool continuous)
{
    AutoLock lock(&m_recv_cs);
    HRESULT hr = ReadCameraIntrinsics(camera_intrinsics_filenames);
    if (FAILED(hr))
        return hr;
    m_tracked_objects.clear();
    m_etalon_size = cvSize(checkerboard_width-1, checkerboard_height-1);
    m_calibrate_cameras = continuous ? 2 : 1;
    m_square_size = square_size;
    for (int c = 0; c < m_num_streams; c++)
        m_pCameraInfo[c].valid = false;
    return NOERROR;
}
   

STDMETHODIMP Tracker3dFilter::GetTrackedObjects(std::vector<Cv3dTrackerTrackedObject> &tracked_objects)
{
    AutoLock lock(&m_recv_cs);
    tracked_objects = m_tracked_objects;
    return NOERROR;
}

STDMETHODIMP Tracker3dFilter::AddCallback(ITracker3dCallback *new_callback)
{
    if (new_callback == NULL)
        return E_INVALIDARG;

    new_callback->AddRef();
    m_callbacks.push_back(new_callback);

    return NOERROR;
}


STDMETHODIMP Tracker3dFilter::RemoveCallback(ITracker3dCallback *callback)
{
    if (callback == NULL)
        return E_INVALIDARG;

    std::vector<ITracker3dCallback *>::iterator i = std::find(m_callbacks.begin(), m_callbacks.end(), callback);
    if (i != m_callbacks.end())
    {
        m_callbacks.erase(i);
        callback->Release();
    }
    
    return NOERROR;
}



STDMETHODIMP Tracker3dFilter::SetViewingStream(int stream)
{
    if (stream < 0 || stream >= m_num_streams)
        return E_INVALIDARG;

    m_viewing_stream = stream;
    return NOERROR;
}

STDMETHODIMP Tracker3dFilter::GetViewingStream(int &stream)
{
    stream = m_viewing_stream;
    return NOERROR;
}

//--------------------
// Begin at input pin 'i' and work upstream in the graph to find the source filter.  Return 
// the name of the source in the string name.
//--------------------
STDMETHODIMP Tracker3dFilter::GetCameraName(int i, std::string &name)
{
    // Set up a default, in case we can't generate something better
    // (This could be improved.)
    name = (char)('0'+i);

    // Work upstream from input pin 'i' to find the capture filter
    HRESULT hr;
    IPin *input_pin = m_streams[i].input_pin; input_pin->AddRef(); AUTO_RELEASE(input_pin);
    IBaseFilter *capture_filter = NULL; AUTO_RELEASE(capture_filter);

    while (1)
    {
        IPin *output_pin = NULL; AUTO_RELEASE(output_pin);
        hr = input_pin->ConnectedTo(&output_pin);
        if (FAILED(hr))
            return hr;

        PIN_INFO pin_info;
        hr = output_pin->QueryPinInfo(&pin_info);
        if (FAILED(hr))
            return hr;
        IBaseFilter *filter = pin_info.pFilter; AUTO_RELEASE(filter);

        FILTER_INFO filter_info;
        hr = filter->QueryFilterInfo(&filter_info);
        filter_info.pGraph->Release(); filter_info.pGraph = NULL;

        IEnumPins *e = NULL; AUTO_RELEASE(e);
        hr = filter->EnumPins(&e);
        if (FAILED(hr))
            return hr;

        input_pin->Release(); input_pin = NULL;
        while (e->Next(1, &input_pin, NULL) == S_OK)
        {
            hr = input_pin->QueryPinInfo(&pin_info);
            if (FAILED(hr))
                return hr;
            pin_info.pFilter->Release();
            if (pin_info.dir == PINDIR_INPUT)
                break;
            input_pin->Release(); input_pin = NULL;
		}

        if (input_pin == NULL) // No input pin found; this must be the capture filter
        {
            capture_filter = filter;
            capture_filter->AddRef();
            break;
        }
    }

    // Use IPersistStream to save the filter data, which includes a text representation of the device id.
    IPersistStream *persist_stream = NULL; AUTO_RELEASE(persist_stream);
    hr = capture_filter->QueryInterface(IID_IPersistStream, (void **)&persist_stream);
    if (FAILED(hr))
        return hr;

    HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, 0);
    if (mem == 0)
        return E_OUTOFMEMORY;

    IStream *stream = NULL; AUTO_RELEASE(stream);
    CreateStreamOnHGlobal(mem, true, &stream);
    hr = persist_stream->Save(stream, false);
    if (FAILED(hr))
        return hr;

    STATSTG stat;
    hr = stream->Stat(&stat, STATFLAG_NONAME);
    if (FAILED(hr))
        return hr;

    int size = stat.cbSize.LowPart;

    wchar_t *p = (wchar_t *)GlobalLock(mem);
    wchar_t *end = p + size;
    wchar_t *id = NULL;
    while (p < end-8)
    {
        if (wcsncmp(p, L"@device:", 8) == 0)
        {
            id = p;
            break;
        }
        p++;
    }

    if (id == NULL)// Didn't find something we recognize
        return E_FAIL;

    p = id + 8;
    while (p < end && *p != L'\0')
        p++;
    int len = p - id;

    name.resize(len);

    for (i = 0; i < len; i++)
        name[i] = (char)id[i];

    return NOERROR;
}

STDMETHODIMP Tracker3dFilter::SetPreferredInputSize(InputSize size)
{
    m_preferred_size = size;
    return NOERROR;
}

STDMETHODIMP Tracker3dFilter::GetPreferredInputSize(InputSize &size)
{
    size = m_preferred_size;
    return NOERROR;
}

STDMETHODIMP Tracker3dFilter::IsConnected(bool &any_connected, bool &all_connected)
{
    any_connected = false;
    all_connected = true;
    for (int i = 0; i < m_num_streams; i++)
    {
        if (m_streams[i].input_pin->IsConnected())
            any_connected = true;
        else
            all_connected = false;

        if (m_streams[i].output_pin->IsConnected())
            any_connected = true;
        else
            all_connected = false;
    }
    return NOERROR;
}

// ITracker3dInternal
STDMETHODIMP Tracker3dFilter::GetCameraInfo(std::vector<Cv3dTrackerCameraInfo> &info)
{
    info.resize(m_num_streams);
    for (int i = 0; i < m_num_streams; i++)
        info[i] = m_pCameraInfo[i];
    return NOERROR;
}


// Implementation of pins for Tracker 3d Filter

static const wchar_t *PinName(PIN_DIRECTION pin_dir, int pin_number)
{
    static wchar_t buf[14];
    swprintf(buf, L"%s %d", pin_dir == PINDIR_INPUT ? L"Input" : L"Output", pin_number);
    return buf;
}


Tracker3dInputPin::Tracker3dInputPin(int pin_number, Tracker3dFilter *filter, CCritSec *cs)
    : CBaseInputPin("Tracker3dInputPin", filter, cs, NULL, PinName(PINDIR_INPUT, pin_number)),
      m_pin_number(pin_number),
      m_pFilter(filter),
      m_refcnt(1)
{
}

Tracker3dInputPin::~Tracker3dInputPin()
{
}

STDMETHODIMP_(ULONG) Tracker3dInputPin::NonDelegatingAddRef()
{
    InterlockedIncrement(&m_refcnt);
    return CBaseInputPin::NonDelegatingAddRef();
}

STDMETHODIMP_(ULONG) Tracker3dInputPin::NonDelegatingRelease()
{
    if (InterlockedDecrement(&m_refcnt) == 0)
    {
        delete this;
        return 0;
    }
    return CBaseInputPin::NonDelegatingRelease();
}

STDMETHODIMP Tracker3dInputPin::GetAllocator(IMemAllocator **allocator)
{
    if (m_pAllocator == NULL)
        return VFW_E_NO_ALLOCATOR;
    *allocator = m_pAllocator;
    (*allocator)->AddRef();
    return NOERROR;
}

STDMETHODIMP Tracker3dInputPin::NotifyAllocator(IMemAllocator *allocator, BOOL read_only)
{
    ALLOCATOR_PROPERTIES props, actual;
    allocator->GetProperties(&props);
    props.cBuffers = 5;
    allocator->SetProperties(&props, &actual);
    return CBaseInputPin::NotifyAllocator(allocator, read_only);
}

HRESULT Tracker3dInputPin::CheckMediaType(const CMediaType *mt)
{
    return m_pFilter->CheckMediaType(m_pin_number, mt);
}

HRESULT Tracker3dInputPin::SetMediaType(const CMediaType *mt)
{
    CBaseInputPin::SetMediaType(mt);
    return m_pFilter->SetMediaType(m_pin_number, mt);
}

HRESULT Tracker3dInputPin::GetMediaType(int pos, CMediaType *mt)
{
    return m_pFilter->GetMediaType(m_pin_number, pos, mt);
}


HRESULT Tracker3dInputPin::Receive(IMediaSample *sample)
{
    HRESULT hr = CBaseInputPin::Receive(sample);
    if (FAILED(hr))
        return hr;

    return m_pFilter->Receive(m_pin_number, sample);
}

HRESULT Tracker3dInputPin::EndOfStream()
{
    m_pFilter->EndOfStream(m_pin_number);
    return NOERROR;
}

HRESULT Tracker3dInputPin::BeginFlush()
{
    HRESULT hr = CBaseInputPin::BeginFlush();
    if (FAILED(hr))
        return hr;

    m_pFilter->Flush(m_pin_number);

    return static_cast<Tracker3dOutputPin *>(m_pFilter->GetPin(m_pin_number*2+1))->DeliverBeginFlush();
}

HRESULT Tracker3dInputPin::EndFlush()
{
    HRESULT hr =  static_cast<Tracker3dOutputPin *>(m_pFilter->GetPin(m_pin_number*2+1))->DeliverEndFlush();
    if (FAILED(hr))
        return hr;

    return CBaseInputPin::EndFlush();
}

HRESULT Tracker3dInputPin::NewSegment(REFERENCE_TIME start, REFERENCE_TIME stop, double rate)
{
    CBasePin::NewSegment(start, stop, rate);
    return static_cast<Tracker3dOutputPin *>(m_pFilter->GetPin(m_pin_number*2+1))->DeliverNewSegment(start, stop, rate);
}


Tracker3dOutputPin::Tracker3dOutputPin(int pin_number, Tracker3dFilter *filter, CCritSec *cs)
    : CBaseOutputPin("Tracker3dOutputPin", filter, cs, NULL, PinName(PINDIR_OUTPUT, pin_number)),
      m_pin_number(pin_number),
      m_pFilter(filter),
      m_refcnt(1)
{
}

Tracker3dOutputPin::~Tracker3dOutputPin()
{
}

STDMETHODIMP_(ULONG) Tracker3dOutputPin::NonDelegatingAddRef()
{
    InterlockedIncrement(&m_refcnt);
    return CBaseOutputPin::NonDelegatingAddRef();
}

STDMETHODIMP_(ULONG) Tracker3dOutputPin::NonDelegatingRelease()
{
    if (InterlockedDecrement(&m_refcnt) == 0)
    {
        delete this;
        return 0;
    }
    return CBaseOutputPin::NonDelegatingRelease();
}


HRESULT Tracker3dOutputPin::CheckMediaType(const CMediaType *mt)
{
    return m_pFilter->CheckMediaType(m_pin_number, mt);
}

HRESULT Tracker3dOutputPin::SetMediaType(const CMediaType *mt)
{
    CBaseOutputPin::SetMediaType(mt);
    return m_pFilter->SetMediaType(m_pin_number, mt);
}

HRESULT Tracker3dOutputPin::GetMediaType(int pos, CMediaType *mt)
{
    return m_pFilter->GetMediaType(m_pin_number, pos, mt);
}

HRESULT Tracker3dOutputPin::DecideAllocator(IMemInputPin *pPin, IMemAllocator **ppAlloc)
{
    *ppAlloc = NULL;

    Tracker3dInputPin *my_input_pin = static_cast<Tracker3dInputPin *>(m_pFilter->GetPin(m_pin_number*2));
    IMemAllocator *alloc;
    HRESULT hr = my_input_pin->GetAllocator(&alloc);
    if (FAILED(hr))
        return hr;

    // get downstream prop request
    ALLOCATOR_PROPERTIES props, request;
    alloc->GetProperties(&props);
    if (SUCCEEDED(pPin->GetAllocatorRequirements(&request)))
    {
        bool changed = false;
        if (request.cbAlign > props.cbAlign)
            props.cbAlign = request.cbAlign, changed = true;
        if (request.cBuffers > props.cBuffers)
            props.cBuffers = request.cBuffers, changed = true;
        if (request.cbBuffer > props.cbBuffer)
            props.cbBuffer = request.cbBuffer, changed = true;
        if (request.cbPrefix > props.cbPrefix)
            props.cbPrefix = request.cbPrefix, changed = true;
        if (changed)
        {
            ALLOCATOR_PROPERTIES actual;
            alloc->SetProperties(&props, &actual);
        }
    }

    hr = pPin->NotifyAllocator(alloc, my_input_pin->IsReadOnly());
    if (FAILED(hr))
    {
        alloc->Release();
        return hr;
    }

    *ppAlloc = alloc;
    return NOERROR;
}


// Setup information

const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
    &MEDIATYPE_Video,       // Major type
    &MEDIASUBTYPE_NULL      // Minor type
};

const AMOVIESETUP_PIN sudpPins[] =
{
    { L"Input",             // Pins string name
      FALSE,                // Is it rendered
      FALSE,                // Is it an output
      TRUE,                 // Are we allowed none
      TRUE,                 // And allowed many
      &CLSID_NULL,          // Connects to filter
      NULL,                 // Connects to pin
      1,                    // Number of types
      &sudPinTypes          // Pin information
    },
    { L"Output",            // Pins string name
      FALSE,                // Is it rendered
      TRUE,                 // Is it an output
      TRUE,                 // Are we allowed none
      TRUE,                 // And allowed many
      &CLSID_NULL,          // Connects to filter
      NULL,                 // Connects to pin
      1,                    // Number of types
      &sudPinTypes          // Pin information
    }
};

const AMOVIESETUP_FILTER sudFilter =
{
    &CLSID_Tracker3dFilter, // Filter CLSID
    L"3d Tracker",          // String name
    MERIT_DO_NOT_USE,       // Filter merit
    2,                      // Number of pins
    sudpPins                // Pin information
};


// List of class IDs and creator functions for the class factory. This
// provides the link between the OLE entry point in the DLL and an object
// being created. The class factory will call the static CreateInstance

CFactoryTemplate g_Templates[] = {
    { L"3d Tracker"
    , &CLSID_Tracker3dFilter
    , Tracker3dFilter::CreateInstance
    , NULL
    , &sudFilter }
  ,
    { L"3d Tracker Property Page"
    , &CLSID_Tracker3dPropertyPage
    , Tracker3dPropertyPage::CreateInstance }
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);


//
// DllRegisterServer
//
// Handles sample registry and unregistry
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
