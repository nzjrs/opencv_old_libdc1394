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


// ////////////////////////////////////////////////////////////////////////////
// BlobTrkObject.h
//
// This file contains the class for Blob Tracker Objects 
//
// ////////////////////////////////////////////////////////////////////////////

#ifndef BLOBTRKOBJECT_H
#define BLOBTRKOBJECT_H

//--------------------
// Confidence
//--------------------

// What percentage of algorithm state and location confidences is based on hysteresis (HISTORY) 
// what percentage on the current frame information (PIXEL)
const double LOCATION_HISTORY_PERC = 0.50;
const double LOCATION_PIXEL_PERC = 0.50;

// Determines how much influence past states or locations have 
// over confidence of current algorithm state and location
// (example: if 2, that means that for frame A, 
// the state of frame A-1 has twice the influence of frame A-2 in algorithm state confidence
const double DECAY = 2.0;

// Distance from previous location that is without confidence penalty.
// Measured in pixels.
const double DISTANCE_FREE = 20.0;

// Used to determine Location Pixel Confidence, as in the equation:
// (total # of non-background pixels in centroid window) 
// / (total # non-background pixels in frame + LOCATION_CONFIDENCE_WEIGHT)
const unsigned long LOCATION_CONFIDENCE_WEIGHT = 1;

// What numbers are considered 100% confidence for location confidence
const double LOCATION_100_PERCENT = 100.0;

// How many frames of historical data should be saved to determine confidence
const unsigned long HISTORY = 2;
//--------------------


// Object Definition
class BlobTrackerObject
{
    int m_id;
    CvPoint m_Center;
    CvRect m_Rectangle;
    unsigned long m_Size;

    static CvPoint RectCenter(const CvRect &rect)
    {
        CvPoint p = { rect.x + rect.width/2, rect.y + rect.height/2 };
        return p;
    };

public:
    BlobTrackerObject(const CvRect &rect, unsigned long size)
        : m_id(-1),
          m_Rectangle(rect),
          m_Size(size),
          m_Center(RectCenter(rect))
    {
    };

    void SetId(int id) { m_id = id; };

    // Get Functions
    int GetId() const { return m_id; };
    CvPoint GetCenter() const { return m_Center; };
    CvRect GetRect() const { return m_Rectangle; };
    unsigned long GetSize() const { return m_Size; };
};

class BlobTrackerObjectList : public std::vector<BlobTrackerObject>
{
};

#endif // BLOBTRKOBJECT_H
