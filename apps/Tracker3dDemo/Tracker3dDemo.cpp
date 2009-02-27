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

#include <stdio.h>
#include <math.h>
#include <afxwin.h>
#include <objbase.h>
#include <streams.h>
#include <initguid.h>

#include "autorelease.h"
#include "ITracker3dFilter.h"
#include "IBlobTracker.h"
#include "ICamShiftTracker.h"
#include "resource.h"
#include "FilenamesDialog.h"

#define ARRAY_SIZEOF(a) (sizeof(a)/sizeof((a)[0]))

//----------------------
// Perform an action m for each BlobTracker connected to the Tracker3dFilter
//----------------------
#define FOR_EACHBLOB_TRACKER(m) { \
                                std::vector<ITracker *> trackers; \
                                m_pTracker3dFilter->GetTrackers(trackers); \
                                for (unsigned int i = 0; i < trackers.size(); i++) \
                                { \
                                    IBlobTracker *bt; \
                                    trackers[i]->QueryInterface(IID_IBlobTracker, (void **)&bt); \
                                    bt->m;\
                                    trackers[i]->Release(); \
                                    bt->Release(); \
                                } \
                            }
//----------------------
// Perform an action m for each CamShiftTracker connected to the Tracker3dFilter
//----------------------
#define FOR_EACHCAMSHIFT_TRACKER(m) { \
                                std::vector<ITracker *> trackers; \
                                m_pTracker3dFilter->GetTrackers(trackers); \
                                for (unsigned int i = 0; i < trackers.size(); i++) \
                                { \
                                    ICamShiftTracker *ct; \
                                    trackers[i]->QueryInterface(IID_ICamShiftTracker, (void **)&ct); \
                                    ct->m;\
                                    trackers[i]->Release(); \
                                    ct->Release();\
                                } \
                            }
//----------------------
// Definition for the MainWindow of the application.  
// Extends CDialog and implements the ITracker3dCallback interface in order to receive 
// notifications from the Tracker3dFilter.
//----------------------
class MainWindow : public CDialog, ITracker3dCallback
{

	//DirectShow Interfaces
    IGraphBuilder *m_pGraphBuilder;
    IMediaControl *m_pMediaControl;
    IMediaEventEx *m_pMediaEvent;

	//Tracker3d Interfaces
	ITracker3dFilter   *m_pTracker3dFilter;
    ITracker3dInternal *m_pTracker3dInternal;

    bool m_bShutdown;              //indicates the program has received a shutdown notification
    bool m_bBlob;                  //operate in blob tracker mode if true or in cam shift if false
    int  m_iCalibrating_camera;    //currently calibrating camera number

    bool m_bLog_camera_info;       //log the camera calibration info for debug or test purposes
    bool m_bLog_object_info;       //log the object tracking info for debug or test purposes
    FILE *m_logfile;               //file in which the logging information will be recorded
    int  m_iFrame_counter;         //count the frames in order to annotate the logging information

    std::vector<Cv3dTrackerCameraInfo>    m_vSaved_camera_info;  //storage for camera information in logging process
    std::vector<Cv3dTrackerTrackedObject> m_vSaved_objects;      //storage for object information in logging process

    BITMAPINFOHEADER m_bmiHeader;  //bitmap information for current frame
    std::vector<std::string> m_vCamera_intrinsics_filenames;  //filename from which to load the camera intrinsics data
															  //this data is obtained externally using the camera calibration 
	                                                          //routines and filter in the opencv library

    void CalibrateCameras(bool continuous);  
    
	// ITracker3dCallback
    ULONG __stdcall AddRef() { return 2; };
    ULONG __stdcall Release() { return 1; };

    STDMETHODIMP QueryInterface(REFIID iid, void **ppv)
    {
        if (iid == IID_ITracker3dCallback || iid == IID_IUnknown)
        {
            *ppv = (void *)static_cast<ITracker3dCallback *>(this);
            AddRef();
            return NOERROR;
        }
        return E_NOINTERFACE;
    };
    STDMETHODIMP Callback(const std::vector<Cv3dTrackerTrackedObject> &objects, const unsigned char *rgb, IUnknown *unk);

public:
    MainWindow(const std::vector<std::string> &argv);

    ~MainWindow()
    {
        m_bShutdown = true;
        SAFE_RELEASE(m_pMediaEvent);
        SAFE_RELEASE(m_pMediaControl);
        SAFE_RELEASE(m_pTracker3dFilter);
        SAFE_RELEASE(m_pTracker3dInternal);
        SAFE_RELEASE(m_pGraphBuilder);
        if (m_logfile != NULL)
            fclose(m_logfile);
    }

    void PostNcDestroy()
    {
        delete this;
    };

    //{{AFX_MSG(MainWindow)
    afx_msg void OnCalibrateCameras();
    afx_msg void OnCalibrateObjects();
    afx_msg void OnSetOutputOptions();
    afx_msg void OnChangeNumberOfObjects();
    afx_msg void OnChangeThreshold();
    afx_msg void OnChangeMinSize();
    afx_msg long OnMediaEvent(WPARAM, LPARAM);
    afx_msg long OnCallback(WPARAM, LPARAM);
    afx_msg void OnClose();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(MainWindow, CDialog)
    //{{AFX_MSG_MAP(MainWindow)
    ON_BN_CLICKED(IDC_CALIBRATE_CAMERAS, OnCalibrateCameras)
    ON_BN_CLICKED(IDC_CALIBRATE_OBJECTS, OnCalibrateObjects)
    ON_BN_CLICKED(IDC_COLOR_CLASSIFIED_PIXELS, OnSetOutputOptions)
    ON_BN_CLICKED(IDC_COLOR_BACKGROUND_PIXELS, OnSetOutputOptions)
    ON_BN_CLICKED(IDC_BOUNDING_BOX, OnSetOutputOptions)
    ON_BN_CLICKED(IDC_EXIT, OnClose)
    ON_WM_CLOSE()
    ON_EN_CHANGE(IDC_NUMBER_OF_OBJECTS, OnChangeNumberOfObjects)
    ON_MESSAGE(WM_APP, OnMediaEvent)
    ON_MESSAGE(WM_APP+1, OnCallback)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


//----------------------
// Iterate over the filters in the graph looking for the one with 
// REFIID _iid and assign it to the pointer _ppv.  Returns an HRESULT to
// indicate whether the operation succeeded or failed to find the filter
//----------------------
HRESULT GetFilter(IGraphBuilder *_pGraphBuilder, REFIID _iid, void** _ppv) 
{
    IEnumFilters* penum  = NULL; AUTO_RELEASE(penum);
    IBaseFilter* pfilter = NULL; AUTO_RELEASE(pfilter);

    //begin enumerating the filters in the filter graph
    HRESULT hr = _pGraphBuilder->EnumFilters(&penum);
    if (FAILED(hr))
        return hr;

    //retrieve one filter at a time and place it into pFilter
    while ( penum->Next(1, &pfilter, NULL) == S_OK )
    {
        hr = pfilter->QueryInterface(_iid, _ppv);

        //if the interface is found,
        if ( SUCCEEDED(hr) )
            break;
        
        //make sure the enumed filter is released
        SAFE_RELEASE(pfilter);
    }

    return hr;    
}

//----------------------
// Enumerate the pins on a DirectShow filter and find a pin with the specified properties 
// of direction (_pindir) and category (_pcategory). Returns false in the event the pin is 
// not found
//----------------------
BOOL GetPin(IBaseFilter* _pbasefilter, IPin* &_pPin, PIN_DIRECTION _pindir, const GUID* _pcategory)
{
    IEnumPins *pEnumPins = NULL; AUTO_RELEASE(pEnumPins);
    _pbasefilter->EnumPins( &pEnumPins );
    
    //retrieve one pin at a time and check for the output pin
    while ( pEnumPins->Next(1, &_pPin, NULL) == S_OK )
    {
        PIN_INFO PinInfo;
        IKsPropertySet *pPropSet = NULL; AUTO_RELEASE(pPropSet);

        _pPin->QueryPinInfo( &PinInfo );
        // Release the retrieved base filter ref;
        // we don't need it.
        if (PinInfo.pFilter != NULL)
            PinInfo.pFilter->Release();

        //if we have found the right pin direction
        if ( PinInfo.dir == _pindir )
        {
            //if a category was not passed in, we are done
            if (_pcategory == NULL)
                break;
        
            //check if the pin is the right category (preview, capture, etc.)
            GUID guid;
            DWORD retbytes;
            if (_pPin->QueryInterface(IID_IKsPropertySet, (void **)&pPropSet) == S_OK
                && pPropSet->Get(AMPROPSETID_Pin, //property set GUID 
                     AMPROPERTY_PIN_CATEGORY, //Identifier of the property within the property set
                     NULL, //Pointer to instance data for the property
                     0, //Number of bytes in the buffer to which pInstanceData points
                     &guid, //Pointer to the retrieved buffer 
                     sizeof(GUID), //Number of bytes in the buffer to which pPropData points
                     &retbytes  //number of bytes returned in the buffer 
                ) == S_OK
            && guid == *_pcategory)
            {
                //break from while loop with pointer to pin
                break;
            }
        }

        SAFE_RELEASE(_pPin);
    }

    //if the pin was not found, return false
    return _pPin != NULL;
}

//----------------------
// Obtain a pointer to a specific filter (_n) with GUID _trackerID that is employed 
// by the Tracker3dFilter referenced by the pointer _pTracker3dFilter.  
// Returns a pointer to the tracker
//----------------------
ITracker *GetTracker(ITracker3dFilter *_pTracker3dFilter, const _GUID _trackerID, int _n)
{
    std::vector<ITracker *> trackers;
    _pTracker3dFilter->GetTrackers(trackers);

    ITracker *tracker;
    trackers[_n]->QueryInterface(_trackerID, (void **)&tracker);

    for (unsigned int i = 0; i < trackers.size(); i++)
        trackers[i]->Release();

    return tracker;
}

//----------------------
// process the vector of arguments, create and configure the Tracker3DFilter
//----------------------
MainWindow::MainWindow(const std::vector<std::string> &argv)
{
    HRESULT hr;

    int num_objects = 1;
    int threshold = 100;
    int size = 3;
    int checkerboard_width = 6, checkerboard_height = 4;
    bool calibrate_cameras = false;
    bool continuous = false;
    wchar_t graph_name[MAX_PATH];

    m_pGraphBuilder = NULL;
    m_pMediaControl = NULL;
    m_pMediaEvent = NULL;
    m_pTracker3dFilter = NULL;
    m_pTracker3dInternal = NULL;

    m_bShutdown = false;
    m_logfile = NULL;
    m_bLog_camera_info = false;
    m_bLog_object_info = false;
    m_iFrame_counter = 0;
    m_bBlob = false;
    //m_auto_calibration = false;

    for (unsigned int i = 1; i < argv.size(); i++)
    {
        if (argv[i][0] == '-')
        {
            char c = argv[i][1];
            try
            {
                std::string s;
                if (argv[i].length() > 2)
                    s = argv[i].substr(2);
                else if (i + 1 < argv.size())
                    s = argv[++i];
                else
                    throw argv[i];

                int m, n;
                switch (c)
                {
                    case 'c': // calibrate cameras
                    case 'C': // calibrate cameras continuously
                        m_vCamera_intrinsics_filenames.push_back(s);
                        calibrate_cameras = true;
                        if (argv[i][1] == 'C')
                            continuous = true;
                        break;

                    case 's': // checkerboard size
                        if (sscanf(s.c_str(), "%d,%d", &m, &n) == 2)
                        {
                            checkerboard_width = m;
                            checkerboard_height = n;
                        }
                        else
                            throw std::string("-s ") + s;
                        break;

                    case 'n': // number of objects
                        if (sscanf(s.c_str(), "%d", &n) == 1)
                        {
                            num_objects = n;
                        }
                        else
                            throw std::string("-n ") + s;
                        break;

                    case 'm': // min size
                        if (sscanf(s.c_str(), "%d", &n) == 1)
                        {
                            size = n;
                        }
                        else
                            throw std::string("-m ") + s;
                        break;

                    case 't': // threshold
                        if (sscanf(s.c_str(), "%d", &n) == 1)
                        {
                            threshold = n;
                        }
                        else
                            throw std::string("-t ") + s;
                        break;

                    case 'l': // logfile
                        m_logfile = fopen(s.c_str(), "w");
                        if (m_logfile == NULL)
                            if (::MessageBox(0, ("can't open logfile " + s).c_str(), "Tracker 3d Test", MB_OKCANCEL) == IDCANCEL)
                                throw 0;
                        m_bLog_object_info = true;
                        m_bLog_camera_info = true;
                        break;

                    case 'b': //operate in blob mode
                        m_bBlob = true;
                        break;
                    
                    default:
                        throw argv[i];
                }
            }
            catch (std::string e)
            {
                if (::MessageBox(0, ("argument \"" + e + "\" invalid -- ignored").c_str(), "Tracker 3d Test", MB_OKCANCEL) == IDCANCEL)
                    throw 0;
            }
        }
        else
            mbstowcs(graph_name, argv[i].c_str(), MAX_PATH);
    }
 
	// create an instance of the GraphBuilder interface

	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
                                  IID_IGraphBuilder, (void **)&m_pGraphBuilder);
    if (FAILED(hr))
        throw 0;

	// render the graph file specified as a command line argument

    if (!SUCCEEDED(m_pGraphBuilder->RenderFile(graph_name, NULL)))
    {
		// if the attempt to render the file failed then we have a fallback approach

        IStorage *storage = NULL; AUTO_RELEASE(storage);
        IStream *stream = NULL; AUTO_RELEASE(stream);
        IPersistStream *persist_stream = NULL; AUTO_RELEASE(persist_stream);

        hr = StgOpenStorage(graph_name, NULL, STGM_READ|STGM_SHARE_EXCLUSIVE, NULL, 0, &storage);
        if (FAILED(hr))
            throw 0;

        hr = storage->OpenStream(L"ActiveMovieGraph", NULL, STGM_READ|STGM_SHARE_EXCLUSIVE, 0, &stream);
        if (FAILED(hr))
            throw 0;

        hr = m_pGraphBuilder->QueryInterface(IID_IPersistStream, (void **)&persist_stream);
        if (FAILED(hr))
            throw 0;

        hr = persist_stream->Load(stream);
        if (FAILED(hr))
        throw std::string("loadgraph");
    }

	// if still no luck then throw an error
    if (FAILED(hr))
        throw 0;

    //get IMediaControl interface, so we can stop and start the graph
    hr = m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void**)&m_pMediaControl);
    if (FAILED(hr))
        throw 0;

	//get the IMediaEvent interface, so that we can get the graph to signal the app via events
    hr = m_pGraphBuilder->QueryInterface(IID_IMediaEventEx,(void**)&m_pMediaEvent);
    if (FAILED(hr))
        throw 0;
	//get the ITracker3dFilter interface from the graph
    hr = GetFilter(m_pGraphBuilder, IID_ITracker3dFilter, (void **)&m_pTracker3dFilter);
    if (FAILED(hr))
        throw 0;

	//get the ITracker3dInternal interface from the Tracker3dFilter
    hr = m_pTracker3dFilter->QueryInterface(IID_ITracker3dInternal, (void **)&m_pTracker3dInternal);
    if (FAILED(hr))
        throw 0;

	//get the BaseFilter interface from the Tracker3dFilter
    IBaseFilter *filter = NULL; AUTO_RELEASE(filter);

    hr = m_pTracker3dFilter->QueryInterface(IID_IBaseFilter, (void **)&filter);
    if (FAILED(hr))
        throw 0;

    //retrieve an input pin of the 3d tracker filter
    IPin* pPin = NULL; AUTO_RELEASE(pPin);
    if ( !GetPin(filter, pPin, PINDIR_INPUT, NULL) )
        throw 0;

    //get the media type
    AM_MEDIA_TYPE mediatype;
    hr = pPin->ConnectionMediaType(&mediatype);
    if (FAILED(hr))
        throw 0;

    VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *)(mediatype.pbFormat);

    int width = pvi->bmiHeader.biWidth;
    int height = pvi->bmiHeader.biHeight;
    int depth = pvi->bmiHeader.biBitCount;

    //fill the main bmih structure
    m_bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_bmiHeader.biWidth = width;
    m_bmiHeader.biHeight = height;
    m_bmiHeader.biBitCount = depth;
    m_bmiHeader.biPlanes = 1;
    m_bmiHeader.biSizeImage = 0; 
    m_bmiHeader.biCompression = BI_RGB;
    m_bmiHeader.biClrImportant = 0;
    m_bmiHeader.biClrUsed = 0;
    m_bmiHeader.biXPelsPerMeter = 0;
    m_bmiHeader.biYPelsPerMeter = 0;

    Create(IDD_MAIN);

    SetDlgItemInt(IDC_NUMBER_OF_OBJECTS, num_objects);
    SetDlgItemInt(IDC_MIN_SIZE, size);
    SetDlgItemInt(IDC_THRESHOLD, threshold);
    SetDlgItemInt(IDC_CHECKERBOARD_WIDTH, checkerboard_width);
    SetDlgItemInt(IDC_CHECKERBOARD_HEIGHT, checkerboard_height);
    OnSetOutputOptions();

	//add a callback to the Tracker3d filter 
    hr = m_pTracker3dFilter->AddCallback(this);
    if (FAILED(hr))
    {
        MessageBox("m_pTracker3dFilter->AddCallback(m_pCallback)", "Tracker 3d Test");
        throw 0;
    }

	//calibrate the cameras once or continuously as specified by the options on the command line
    if (calibrate_cameras)
        CalibrateCameras(continuous);

    //have the graph signal event via window message
    m_pMediaEvent->SetNotifyWindow((OAHWND)m_hWnd, WM_APP, 0);

	//run the graph
    hr = m_pMediaControl->Run();
    if (FAILED(hr))
        throw 0;
}

//----------------------
// Respond to the Calibrate Cameras Button
// Create the dialog for the selection of the camera intrinsics files and if that return is successful then 
// call the calibrate cameras routine.
//----------------------
void MainWindow::OnCalibrateCameras() 
{
    int num_cameras;
    m_pTracker3dFilter->GetNumberOfCameras(num_cameras);
    bool continuous = GetKeyState(VK_CONTROL) < 0;
    bool dialog = (GetKeyState(VK_SHIFT) < 0 || m_vCamera_intrinsics_filenames.size() != num_cameras);
    if (!dialog || FilenamesDialog(m_hWnd, num_cameras, m_vCamera_intrinsics_filenames))
        CalibrateCameras(continuous);
}

//----------------------
// Get the checkerboard parameters from the applicaiton dialog and the number of cameras from the Tracker3dFilter
// Set the names of the camera instrinsics for each of the cameras according to the values in m_vCamera_intrinsics_filenames
// Call the Calibrate Cameras method on the Tracker3dFilter with the checkerboard dimensions m and n, the names of the 
// intrinsics files and whether or not to run continuously
//----------------------
void MainWindow::CalibrateCameras(bool continuous)
{
    int m = GetDlgItemInt(IDC_CHECKERBOARD_WIDTH);
    int n = GetDlgItemInt(IDC_CHECKERBOARD_HEIGHT);
    int num_cameras;
    m_pTracker3dFilter->GetNumberOfCameras(num_cameras);
    if (m_vCamera_intrinsics_filenames.size() != num_cameras)
        MessageBox("Not enough camera intrinsics file names supplied", "Tracker 3d Test");
    std::vector<const char *> names(num_cameras);
    for (int i = 0; i < num_cameras; i++)
        names[i] = m_vCamera_intrinsics_filenames[i].c_str();
    if (m_pTracker3dFilter->CalibrateCameras(m, n, &names[0], 1.0f, continuous) != NOERROR)
        MessageBox("Error in Calibrate_Cameras", "Tracker 3d Test");

}

//----------------------
// Set the output options for the trackers
// This only applies to the blob tracker, there are no options in the CamShiftTracker to set
// Uses the FOR_EACHBLOB_TRACER(m) macro to make the same call on each tracker in the Filter
//----------------------
void MainWindow::OnSetOutputOptions() 
{
    if(m_bBlob)
    {
        unsigned long output_options = IBlobTracker::OUTPUT_CROSSHAIRS;
        if (IsDlgButtonChecked(IDC_COLOR_CLASSIFIED_PIXELS))
            output_options |= IBlobTracker::OUTPUT_COLOR_PIXELS;

        if (IsDlgButtonChecked(IDC_BOUNDING_BOX))
            output_options |= IBlobTracker::OUTPUT_BOUNDING_BOX;

        FOR_EACHBLOB_TRACKER(SetOutputOptions(output_options));
    }
    else
        return;
}


//----------------------
// Close the application and stop the graph
//----------------------
void MainWindow::OnClose()
{
    m_bShutdown = true; // causes subsequent events to be ignored
    if (m_pMediaControl != NULL)
        m_pMediaControl->Stop();
    DestroyWindow();
    PostQuitMessage(0);
}

//----------------------
// Process a media event (from the Filter)
// if a shutdown has already been started (m_bShutdown==true) then ignore the event
// if the Event is EC_COMPLETE (no more frames to process) then initiate the shutdown
//----------------------
long MainWindow::OnMediaEvent(WPARAM, LPARAM)
{
    if (m_bShutdown)
        return 0;

    long code, param1, param2;
    while (SUCCEEDED(m_pMediaEvent->GetEvent(&code, &param1, &param2, 0)))
    {
        m_pMediaEvent->FreeEventParams(code, param1, param2);
        if(code == EC_COMPLETE)
        {
            OnClose();
            break;
        }
    }
    return 0;
}

//----------------------
// Calibrate the CamShiftTracker attached to each camera in turn
// with the BlobTracker there is nothing to calibrate
//----------------------
void MainWindow::OnCalibrateObjects() 
{
	if(!m_bBlob)
    {    
        FOR_EACHCAMSHIFT_TRACKER(Calibrate());
    }
}

//----------------------
// Change the number of targets that the BlobTracker is looking for, use the 
// FOR_EACHBLOB_TRACKER macro to apply the changes
// this doesn't apply the the CamShiftTracker
//----------------------
void MainWindow::OnChangeNumberOfObjects() 
{
    int num_objects = GetDlgItemInt(IDC_NUMBER_OF_OBJECTS);
    if(m_bBlob)
        FOR_EACHBLOB_TRACKER(SetNumberOfObjects(num_objects));

}

//----------------------
// Change the threshold value for each of the BlobTrackers, doesn't apply to the CamShiftTracker
//----------------------
void MainWindow::OnChangeThreshold() 
{
    int threshold = GetDlgItemInt(IDC_THRESHOLD);
    if(m_bBlob)
        FOR_EACHBLOB_TRACKER(SetSizeThreshold(threshold));
}

//----------------------
// Change the min size value for each of the BlobTrackers, doesn't apply to the CamShiftTracker
//----------------------
void MainWindow::OnChangeMinSize() 
{
    int min_size = GetDlgItemInt(IDC_MIN_SIZE);
    if(m_bBlob)
        FOR_EACHBLOB_TRACKER(SetSizeThreshold(min_size));

}

//----------------------
// Define a struct to package the callback arguments
//----------------------
struct callback_args
{
    std::vector<Cv3dTrackerCameraInfo> camera_info;
    std::vector<Cv3dTrackerTrackedObject> objects;
};

//----------------------
// compare two CameraTrackerInfo objects and return true iff their matrices are substantially identical
// used to evaluate performance in testing
//----------------------
static inline bool operator ==(const Cv3dTrackerCameraInfo &arg1, const Cv3dTrackerCameraInfo &arg2)
{
    if (arg1.valid != arg2.valid)
        return false;

    if (arg1.valid)
    {
        int count = 0;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                float diff = (float) fabs((arg1.mat[i][j] - arg2.mat[i][j]) / arg1.mat[i][j]);
                if (diff > 0.1 || (diff > 0.02 && ++count > 2))
                    return false;
            }
        }
    }

    return true;
}

//----------------------
// Define the complementary != operator for the comparison of two CameraInfo objects
//----------------------
static inline bool operator !=(const Cv3dTrackerCameraInfo &arg1, const Cv3dTrackerCameraInfo &arg2)
{
    return !(arg1 == arg2);
}

//----------------------
// Define the == operator for TrackedObjects, again used in evaluating performance during testing
// determines that for Tracked objects the id, the x and the y coord results are the same
//----------------------
static inline bool operator ==(const Cv3dTrackerTrackedObject &arg1, const Cv3dTrackerTrackedObject &arg2)
{
    return arg1.id == arg2.id
               && arg1.p.x == arg2.p.x
               && arg1.p.y == arg2.p.y;
}

//----------------------
// Defines the complementary != operator for TrackedObjects
//----------------------
static inline bool operator !=(const Cv3dTrackerTrackedObject &arg1, const Cv3dTrackerTrackedObject &arg2)
{
    return !(arg1 == arg2);
}

//----------------------
//
// NOTE:  If you need to determine the format of the rgb image pointer grab the input pin of the filter and check the format there
// 
//----------------------
STDMETHODIMP MainWindow::Callback(const std::vector<Cv3dTrackerTrackedObject> &objects, const unsigned char *rgb, IUnknown *unk)
{
    if (m_bShutdown)
        return NOERROR;

    callback_args *args = new callback_args;
    args->objects = objects;
    m_pTracker3dInternal->GetCameraInfo(args->camera_info);
    PostMessage(WM_APP+1, 0, (LPARAM)args);
    return NOERROR;
}

//----------------------
// Respond to OnCallback Event
// process the callback args structure passed back as l, set the coordinates in the dialog
// box for the tracked objects
// disable the elements of the dialog for those objects not found.
// if logging is enabled then also log the results, camera parameters are also passed back
// as a part of the callback_args struct and are written to the log file from here.
//----------------------
LRESULT MainWindow::OnCallback(WPARAM w, LPARAM l)
{
    callback_args *args = (callback_args *)l;

#define MAX_OBJECTS 3
    unsigned int i;

    for (i = 0; i < MAX_OBJECTS; i++)
        GetDlgItem(IDC_LOCATION1 + i)->EnableWindow(false);

    for (i = 0; i < args->objects.size(); i++)
    {
        const Cv3dTrackerTrackedObject &o = args->objects[i];
        if (o.id < MAX_OBJECTS)
        {
            char text[80];
            sprintf(text, "(%#6.1f, %#6.1f, %#6.1f)", o.p.x, o.p.y, o.p.z);
            SetDlgItemText(IDC_LOCATION1 + o.id, text);
            GetDlgItem(IDC_LOCATION1 + o.id)->EnableWindow(true);
        }
    }

    if (m_logfile != NULL)
    {
        bool flag = false;
        m_iFrame_counter++;

        if (m_bLog_camera_info)
        {
            if (args->camera_info != m_vSaved_camera_info)
            {
                if (!flag)
                {
                    fprintf(m_logfile, "Frame %d\n", m_iFrame_counter);
                    flag = true;
                }
                for (unsigned int i = 0; i < args->camera_info.size(); i++)
                {
                    Cv3dTrackerCameraInfo &camera_info = args->camera_info[i];
                    fprintf(m_logfile, "%d: %s\n", i, camera_info.valid ? "valid" : "not valid");
                    if (camera_info.valid)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            fprintf(m_logfile, "[ %#10.3g %#10.3g %#10.3g %#10.3g ]\n",
                                    camera_info.mat[j][0],
                                    camera_info.mat[j][1],
                                    camera_info.mat[j][2],
                                    camera_info.mat[j][3]);
                        }
                    }
                }
                m_vSaved_camera_info = args->camera_info;
            }
        }

        if (m_bLog_object_info)
        {
            if (args->objects != m_vSaved_objects)
            {
                if (!flag)
                {
                    fprintf(m_logfile, "Frame %d\n", m_iFrame_counter);
                    flag = true;
                }
                if (args->objects.size() == 0)
                    fprintf(m_logfile, "no objects\n");
                else
                {
                    for (unsigned int i = 0; i < args->objects.size(); i++)
                    {
                        Cv3dTrackerTrackedObject &object = args->objects[i];
                        fprintf(m_logfile, "%d: (%#6.1f %#6.1f %#6.1f)\n",
                                object.id, object.p.x, object.p.y, object.p.z);
                    }
                }
                m_vSaved_objects = args->objects;
            }
        }
    }

    delete args;

    return 0;
}

static void ParseCommandLine(const char *cmd_line, std::vector<std::string> &argv);

//----------------------
// Main Application just parses the command line and creates a MainWindow with the
// resulting parameters
//----------------------
WINOLEAPI  CoInitializeEx(IN LPVOID pvReserved, IN DWORD dwCoInit);

class Tracker3dDemoApp : public CWinApp
{
    std::vector<std::string> argv;

public:
    BOOL InitInstance()
    {
        ::CoInitializeEx(0, 0);

        ::ParseCommandLine(m_lpCmdLine, argv);

        try
        {
            m_pMainWnd = new MainWindow(argv);
        }
        catch (...)
        {
            return FALSE;
        }

        return TRUE;
    };

    int Tracker3dDemoApp::ExitInstance() 
    {
        CoUninitialize();

        return CWinApp::ExitInstance();
    };

    //{{AFX_VIRTUAL(Tracker3dDemoApp)
    //}}AFX_VIRTUAL
    //{{AFX_MSG(Tracker3dDemoApp)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


BEGIN_MESSAGE_MAP(Tracker3dDemoApp, CWinApp)
    //{{AFX_MSG_MAP(Tracker3dDemoApp)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


Tracker3dDemoApp app;

//----------------------
// Parse the command line arguments out
// the assumption is that each flag has a following argument, therefore for -b which enables 
// blob mode for example, write -blob and "lob" is the argument.
//----------------------
static void ParseCommandLine(const char *cmd_line, std::vector<std::string> &argv)
{
    argv.clear();
    argv.push_back("Unknown");
    
    if (cmd_line != NULL)
    {
        while (*(cmd_line += strspn(cmd_line, " \t")) != '\0')
        {
            int copy_len;
            int skip_len;

            if (*cmd_line == '"' || *cmd_line == '\'')
            {
                char quote = *cmd_line++;
                const char *end;
                end = strchr(cmd_line, quote);
                if (end == NULL)
                    skip_len = copy_len = strlen(cmd_line);
                else
                    skip_len = 1 + (copy_len = end - cmd_line);
            }
            else
                skip_len = copy_len = strcspn(cmd_line, " \t");

            argv.push_back(std::string(cmd_line, copy_len));
            cmd_line += skip_len;
        }
    }
}
