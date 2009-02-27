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
//  BlobTrkOut.cpp 
//
//    This file contains member functions of the BlobTracker class
//    that deal with output data.
//            FillDestinationData
//            GetTrackedObjects
//            DrawCross
// 
// ////////////////////////////////////////////////////////////////////////////

#include "cvstreams.h"
#include "BlobTracker.h"
#include "BlobTrkObject.h"

#undef min
#undef max
template <class T> static inline T min(T a, T b) { return a < b ? a : b; }
template <class T> static inline T max(T a, T b) { return a > b ? a : b; }

// ////////////////////////////////////////////////////////////////////////////
// BlobTracker::FillDestinationData()
//
// Fills in the destination image based on estimated object location and state.
// Markers are displayed to indicate object position and search windows and 
// other information of importance.
//
// Input: 
//	image.
//
// ////////////////////////////////////////////////////////////////////////////
void BlobTracker::FillDestinationData(IplImage *image)
{
    if (m_output_options != 0)
    {
        for (BlobTrackerObjectList::const_iterator object = m_objects->begin(); object != m_objects->end(); object++)
        {
#if 0
            if (m_output_options & IBlobTracker::OUTPUT_COLOR_PIXELS)
            {
                CvRect rect = object->GetRect();

                const unsigned char *pSrc = (const unsigned char *)m_image->imageData + rect.y * image->width;
                unsigned char *pDst = (unsigned char *)image->imageData + rect.y * image->width;
                for (int i = rect.y; i < rect.y + rect.height; i++)
                {
                    for (int j = rect.x; j < rect.x + rect.width; j++)
                    {
                        if (pSrc[j] == color)
                            pDst[j] = ((i ^ j) & 1) ? 255 : 0;
                    }
                    pSrc += image->width;
                    pDst += image->widthStep;
                }
            }
#endif

            if (m_output_options & IBlobTracker::OUTPUT_BOUNDING_BOX)
            {
                CvRect rect = object->GetRect();						
                cvRectangle(image, cvPoint(rect.x, rect.y), cvPoint(rect.x+rect.width, rect.y+rect.height), 0xffffff, 1);
            }

            if (m_output_options & IBlobTracker::OUTPUT_CROSSHAIRS)
            {
                // Draw a cross at the estimated Object location
                double color;
                switch (object->GetId())
                {
                case 0:
	                color = 0x0000ff;
	                break;
                case 1:
	                color = 0x00ff00;
	                break;
                case 2:
	                color = 0xff0000;
	                break;
                default:
	                color = 0xffffff;
	                break;
                }
                DrawCross(image, object->GetCenter(), color);
            }
        }
    }
}

STDMETHODIMP BlobTracker::GetTrackedObjects(ITracker::TrackingInfo &tracked_objects)
{
    tracked_objects.clear();

    for (BlobTrackerObjectList::const_iterator object = m_objects->begin(); object != m_objects->end(); object++)
        tracked_objects.push_back(cv3dTracker2dTrackedObject(object->GetId(), object->GetCenter()));

    return NOERROR;
}

// ////////////////////////////////////////////////////////////////////////////
// BlobTracker::DrawCross(IplImage *image, CvPoint point)
//
// Draws a cross on the image at the point indicated
//
// Inputs: 
//        image: image in which cross is being placed
//        point: coordinates of center of cross
//
// ////////////////////////////////////////////////////////////////////////////

void BlobTracker::DrawCross(IplImage *image, CvPoint point, double color)
{
    const int SIZE_OF_CROSS = 20;

    int Left = max((int)point.x - SIZE_OF_CROSS, 0);
    int Right = min((int)point.x + SIZE_OF_CROSS, (int)image->width - 1);
    int Top = max((int)point.y - SIZE_OF_CROSS, 0);
    int Bottom = min((int)point.y + SIZE_OF_CROSS, (int)image->height - 1);

    cvLine(image, cvPoint(point.x, Top), cvPoint(point.x, Bottom), color);
    cvLine(image, cvPoint(Left, point.y), cvPoint(Right, point.y), color);
}
