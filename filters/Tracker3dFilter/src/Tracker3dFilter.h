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

#include <vector>
#include <queue>
#include <string>
#include "ITracker.h"
#include "ITracker3dFilter.h"
#include "cv.h"
#include "cvaux.h"

class Tracker3dInputPin;
class Tracker3dOutputPin;

class Tracker3dFilter : public CBaseFilter, ITracker3dFilter, ITracker3dInternal, ISpecifyPropertyPages, IPersistStream
{
    // Properties
    int m_num_streams;
    int m_viewing_stream;
    std::vector<ITracker3dCallback *> m_callbacks;
    InputSize m_preferred_size;
    GUID m_tracker_clsid;

    // Camera calibration stuff
    int m_calibrate_cameras; // 0 - not calibrating;
                             // 1 - calibrate until successful
                             // 2 - calibrate continuously
    Cv3dTrackerCameraIntrinsics *m_camera_intrinsics;
    CvSize m_etalon_size; // size of the checkerboard used to calibrate cameras
    float m_square_size;

    // Results of camera calibration
    Cv3dTrackerCameraInfo *m_pCameraInfo;

    // Results of tracking
    std::vector<Cv3dTrackerTrackedObject> m_tracked_objects;

    // Filter stuff
    CCritSec m_cs;
    CCritSec m_recv_cs;
    struct Stream
    {
        struct Sample
        {
            Sample(IMediaSample *, const ITracker::TrackingInfo &);
            ~Sample();
            Sample(const Sample &s);
            void operator=(const Sample &);
            IMediaSample *sample;
            REFERENCE_TIME ts, te;
            unsigned char *data;
            ITracker::TrackingInfo tracking_info;
        };
        Tracker3dInputPin *input_pin;
        Tracker3dOutputPin *output_pin;
        CMediaType media_type;
        ITracker *tracker;
        IplImage image_header1, image_header2;
        typedef std::deque<Sample> SampleQueue;
        SampleQueue queue;
        unsigned long discarded_frames, total_frames;
        Stream();
        ~Stream();
        void operator =(Stream &);
    }; 
    Stream *m_streams;
    int m_end_of_stream_count; // used to determine when to pass
                               // end of stream to all output pins

    bool m_camera_configuration_loaded;

    Tracker3dFilter(TCHAR *name, IUnknown *unk);
    ~Tracker3dFilter();

    void SetNumberOfStreams(int num_streams);
    HRESULT ReadCameraIntrinsics(const char *filenames[]);
    bool QueuesReady();

    std::string CalibrationName(int i);
    void LoadCameraConfiguration();
    void SaveCameraConfiguration();

public:
    static CUnknown * WINAPI CreateInstance(IUnknown *unk, HRESULT *phr);
    DECLARE_IUNKNOWN;
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

    // Called by pins
    HRESULT CheckMediaType(int pin, const CMediaType *mt);
    HRESULT SetMediaType(int pin, const CMediaType *mt);
    HRESULT GetMediaType(int pin, int pos, CMediaType *mt);
    HRESULT Receive(int pin, IMediaSample *);
    HRESULT EndOfStream(int pin);
    HRESULT Flush(int pin);

    // Overridden from CBaseFilter
    CBasePin *GetPin(int n);
    int GetPinCount();
    STDMETHODIMP Run(REFERENCE_TIME);
    STDMETHODIMP Pause();
    STDMETHODIMP Stop();

    // ISpecifyPropertyPages interface
    STDMETHODIMP GetPages(CAUUID *pPages);

    // IPersistStream
    STDMETHODIMP GetClassID(CLSID *clsid);
    STDMETHODIMP IsDirty();
    STDMETHODIMP Load(IStream *);
    STDMETHODIMP Save(IStream *, BOOL clear_dirty);
    STDMETHODIMP GetSizeMax(ULARGE_INTEGER *size);

    // ITracker3dFilter
    STDMETHODIMP GetNumberOfCameras(int &num_cameras);
    STDMETHODIMP LoadCameraConfiguration(const char *filename);
    STDMETHODIMP SaveCameraConfiguration(const char *filename);
    STDMETHODIMP GetDefaultTracker(GUID &filename);
    STDMETHODIMP SetDefaultTracker(const GUID &filename);
    STDMETHODIMP SetTrackers(int numstreams, ITracker * const *trackers);
    STDMETHODIMP GetTrackers(std::vector<ITracker *> &trackers);
    STDMETHODIMP CalibrateCameras(int checkerboard_width, int checkerboard_height,
                                  const char *camera_intrinsics_filenames[],
                                  float square_size,
                                  bool continuous);
    STDMETHODIMP GetTrackedObjects(std::vector<Cv3dTrackerTrackedObject> &tracked_objects);
    STDMETHODIMP SetViewingStream(int stream);
    STDMETHODIMP GetViewingStream(int &stream);
    STDMETHODIMP GetCameraName(int i, std::string &name);
    STDMETHODIMP SetPreferredInputSize(InputSize size);
    STDMETHODIMP GetPreferredInputSize(InputSize &size);
    STDMETHODIMP AddCallback(ITracker3dCallback *);
    STDMETHODIMP RemoveCallback(ITracker3dCallback *);
    STDMETHODIMP IsConnected(bool &any_connected, bool &all_connected);

    // ITracker3dInternal
    STDMETHODIMP GetCameraInfo(std::vector<Cv3dTrackerCameraInfo> &info);
};

class Tracker3dInputPin : public CBaseInputPin
{
    Tracker3dFilter *m_pFilter;
    int m_pin_number;
    long m_refcnt;

public:
    Tracker3dInputPin(int pin_number, Tracker3dFilter *filter, CCritSec *cs);
    ~Tracker3dInputPin();

    STDMETHODIMP_(ULONG) NonDelegatingAddRef();
    STDMETHODIMP_(ULONG) NonDelegatingRelease();

    HRESULT CheckMediaType(const CMediaType *mt);
    HRESULT SetMediaType(const CMediaType *mt);
    HRESULT GetMediaType(int pos, CMediaType *mt);
    STDMETHODIMP GetAllocator(IMemAllocator **);
    STDMETHODIMP NotifyAllocator(IMemAllocator *allocator, BOOL read_only);
    STDMETHODIMP Receive(IMediaSample *sample);
    STDMETHODIMP Notify(IBaseFilter * pSender, Quality q) { return E_NOTIMPL; };
    STDMETHODIMP EndOfStream();
    STDMETHODIMP BeginFlush();
    STDMETHODIMP EndFlush();
    STDMETHODIMP NewSegment(REFERENCE_TIME start, REFERENCE_TIME stop, double rate);
};

class Tracker3dOutputPin : public CBaseOutputPin
{
    Tracker3dFilter *m_pFilter;
    int m_pin_number;
    long m_refcnt;

public:
    Tracker3dOutputPin(int pin_number, Tracker3dFilter *filter, CCritSec *cs);
    ~Tracker3dOutputPin();

    STDMETHODIMP_(ULONG) NonDelegatingAddRef();
    STDMETHODIMP_(ULONG) NonDelegatingRelease();

    HRESULT CheckMediaType(const CMediaType *mt);
    HRESULT SetMediaType(const CMediaType *mt);
    HRESULT GetMediaType(int pos, CMediaType *mt);
    HRESULT DecideAllocator(IMemInputPin *, IMemAllocator **);
    HRESULT DecideBufferSize(IMemAllocator *, ALLOCATOR_PROPERTIES *) { return E_NOTIMPL; };
    STDMETHODIMP Notify(IBaseFilter * pSender, Quality q) { return E_NOTIMPL; };
};
