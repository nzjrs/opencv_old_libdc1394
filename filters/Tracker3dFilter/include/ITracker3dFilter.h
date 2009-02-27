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
#pragma warning(disable:4786)

#include <objbase.h>
#include <vector>
#include <string>
#include "ITracker.h"

// {7CD9FE5C-AEDB-4d66-A22F-EA1A48256D5F}
DEFINE_GUID(IID_ITracker3dFilter, 0x7cd9fe5c, 0xaedb, 0x4d66, 0xa2, 0x2f, 0xea, 0x1a, 0x48, 0x25, 0x6d, 0x5f);

// {C4A7F22C-DD1A-4ad8-81CA-773C10BEA6D2}
DEFINE_GUID(IID_ITracker3dCallback, 0xc4a7f22c, 0xdd1a, 0x4ad8, 0x81, 0xca, 0x77, 0x3c, 0x10, 0xbe, 0xa6, 0xd2);

// {0CFDE75C-F623-4320-A46F-2F751EA8712A}
DEFINE_GUID(IID_ITracker3dInternal, 0xcfde75c, 0xf623, 0x4320, 0xa4, 0x6f, 0x2f, 0x75, 0x1e, 0xa8, 0x71, 0x2a);

// {E08D9471-8C14-4547-9B61-0D8A89E7C42B}
DEFINE_GUID(CLSID_Tracker3dFilter, 0xe08d9471, 0x8c14, 0x4547, 0x9b, 0x61, 0xd, 0x8a, 0x89, 0xe7, 0xc4, 0x2b);


// The application and/or property page registers for callbacks from the filter by
// implementing this interface and passing it to ITracker3dFilter::AddCallback.
interface ITracker3dCallback : IUnknown
{
    // Callback is called by the filter after processing each set of frames.
    // The image argument is the raw image data, with no header.
    // If the image is needed past the time the callback returns,
    // this function should AddRef the IUnknown pointer and release it when
    // it is done with the image.
    // If tracked_objects is needed past the time the callback returns,
    // it must be copied.
    STDMETHOD(Callback)(const std::vector<Cv3dTrackerTrackedObject> &tracked_objects, const unsigned char *image, IUnknown *unk) = 0;
};

interface ITracker3dFilter : IUnknown
{
    STDMETHOD(GetNumberOfCameras)(int &num_cameras) = 0;

    // Load/SaveCameraConfiguration allows the camera position and
    // orientation information to be saved in a file. This allows
    // tracking to begin without having to recalibrate the camera.
    // The filter also saves the camera configuration in the registry
    // and will load it from there if LoadCameraConfiguration is not
    // called.
    STDMETHOD(LoadCameraConfiguration)(const char *filename) = 0;
    STDMETHOD(SaveCameraConfiguration)(const char *filename) = 0;

    // The default tracker is used when SetTrackers is called with the
    // second argument set to NULL. The default tracker is saved when
    // the filter graph is saved.
    STDMETHOD(GetDefaultTracker)(GUID &filename) = 0;
    STDMETHOD(SetDefaultTracker)(const GUID &filename) = 0;

    // SetTrackers is used to change the number of streams (cameras)
    // as well as to specify which trackers to use. If the trackers
    // argument is NULL, an instance of the default tracker is created
    // for each stream. If the trackers argument is non-NULL, it is
    // an array of num_streams pointers to trackers. If any of the
    // pointers is NULL, no tracker is assigned to that stream; the
    // stream is effectively ignored.
    // SetTrackers should not be called if the filter's pins are connected,
    // unless num_streams is unchanged and the new trackers are guaranteed
    // to accept the same image format as the current trackers.
    STDMETHOD(SetTrackers)(int num_streams, ITracker * const *trackers) = 0;
    STDMETHOD(GetTrackers)(std::vector<ITracker *> &trackers) = 0;

    // CalibrateCameras begins camera calibration. Calibration continues
    // until all cameras are calibrated simultaneously. The array of camera
    // intrinsics filenames has an entry for each camera. The camera
    // intrinsics files are text files in the format created by CalibFilter.
    // For testing purposes, if continuous is set to true, calibration
    // continues even after calibration succeeds, until this method is called
    // again with continuous set to false.
    STDMETHOD(CalibrateCameras)(int checkerboard_width, int checkerboard_height,
                                const char *camera_intrinsics_filenames[],
                                float square_size = 1.0f,
                                bool continuous = false) = 0;

    // GetTrackedObjects returns the last set of objects found. This is the
    // same as the informotion passed to the callback function.
    STDMETHOD(GetTrackedObjects)(std::vector<Cv3dTrackerTrackedObject> &tracked_objects) = 0;

    // Only one image is passed to the callback function. ViewingStream
    // controls which camera's image that is.
    STDMETHOD(SetViewingStream)(int stream) = 0;
    STDMETHOD(GetViewingStream)(int &stream) = 0;

    // GetCameraName returns a unique ASCII identifier for the specified
    // camera (not intended to be human readable).
    STDMETHOD(GetCameraName)(int camera, std::string &name) = 0;

    // PreferredInputSize specifies what image size should be accepted.
    // To be effective, this must be set before the input pins are connected.
    enum InputSize { SIZE_Any, SIZE_320x240, SIZE_640x480 };
    STDMETHOD(SetPreferredInputSize)(InputSize size) = 0;
    STDMETHOD(GetPreferredInputSize)(InputSize &size) = 0;

    // AddCallback is used to register a callback which will be called
    // after each frame is processed.
    STDMETHOD(AddCallback)(ITracker3dCallback *) = 0;
    STDMETHOD(RemoveCallback)(ITracker3dCallback *) = 0;

    // IsConnected reports whether any or all of the filter's pins are
    // connected. If any pins are connected, some methods should not be
    // called.
    STDMETHOD(IsConnected)(bool &any_connected, bool &all_connected) = 0;
};

interface ITracker3dInternal : IUnknown
{
    // GetCameraInfo returns the current calibration information for the cameras.
    STDMETHOD(GetCameraInfo)(std::vector<Cv3dTrackerCameraInfo> &info) = 0;
};
