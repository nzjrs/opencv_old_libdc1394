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
//    BlobTracker.cpp
//
//    This file contains the bulk of the implementation of the 
//    BlobTracker class.
//
// ////////////////////////////////////////////////////////////////////////////

#include <stack>
#include <algorithm>
#include <comcat.h>
#include "cvstreams.h"
#include "BlobTracker.h"
#include "BlobTrkObject.h"
#include "BlobTrackerPropertyPage.h"

static void FloodFill(unsigned char *image, int width, int height, int start_x, int start_y, int new_color, CvRect &rect, int &area);


//
// CreateInstance
//
// Used by the DirectShow base classes to create instances
//
CUnknown *BlobTracker::CreateInstance(IUnknown *outer, HRESULT *phr)
{
    CUnknown *punk = new BlobTracker(outer, phr);
    if (punk == NULL) {
        *phr = E_OUTOFMEMORY;
    }
    return punk;

}


// ////////////////////////////////////////////////////////////////////////////
// BlobTracker::BlobTracker()
//
// Constructor.
//
// ////////////////////////////////////////////////////////////////////////////
BlobTracker::BlobTracker(IUnknown *outer, HRESULT *phr)
    : CUnknown(NAME("Blob Tracker"), outer)
{
    // Set member variables to defaults
    m_output_options = IBlobTracker::OUTPUT_CROSSHAIRS;
    m_num_objects = 0;
    m_pixel_threshold = 99; 
    m_size_threshold = 3;
    m_distance_threshold = 80;
    m_image = NULL;
    m_tmp_image = NULL;
    m_objects = new BlobTrackerObjectList;
}

// ////////////////////////////////////////////////////////////////////////////
// BlobTracker::~BlobTracker()
//
// Destructor.
//
// ////////////////////////////////////////////////////////////////////////////
BlobTracker::~BlobTracker()
{
    cvReleaseImage( &m_image );
    cvReleaseImage( &m_tmp_image );

    delete m_objects;
}


HRESULT BlobTracker::NonDelegatingQueryInterface(REFIID iid, void **ppv)
{
    if (iid == IID_IBlobTracker)
        return GetInterface((IUnknown *)(void *)static_cast<IBlobTracker *>(this), ppv);

    if (iid == IID_ITracker)
        return GetInterface((IUnknown *)(void *)static_cast<ITracker *>(this), ppv);

    return CUnknown::NonDelegatingQueryInterface(iid, ppv);
}


// ITracker methods

STDMETHODIMP BlobTracker::CheckFormat(IplImage *image_header)
{
    if (image_header->depth != IPL_DEPTH_8U
        || image_header->width % 4 != 0)
    {
        return E_FAIL;
    }

    if (image_header->nChannels == 1
        && strncmp(image_header->colorModel, "GRAY", 4) == 0)
    {
        return NOERROR;
    }

    if (image_header->nChannels == 3
        && strncmp(image_header->colorModel, "RGB", 3) == 0)
    {
        return NOERROR;
    }

    return E_FAIL;
}

STDMETHODIMP BlobTracker::SetFormat(IplImage *image_header)
{
    HRESULT hr = CheckFormat(image_header);
    if (FAILED(hr))
        return E_FAIL;

    m_image_format = *image_header;

    m_image = cvCreateImage( cvSize(image_header->width,image_header->height), IPL_DEPTH_8U, 1 );
    m_image->origin = image_header->origin;

    if (image_header->nChannels != 1)
    {
        m_tmp_image = cvCreateImage( cvSize(image_header->width,image_header->height), IPL_DEPTH_8U, 1 );
        m_image->origin = image_header->origin;
    }

    return NOERROR;
}


STDMETHODIMP BlobTracker::SetNumberOfObjects(int num_objects)
{
    if (num_objects < 1)
        return E_FAIL;

    m_num_objects = num_objects;

    return NOERROR;
}

STDMETHODIMP BlobTracker::GetPropertyPage(GUID *page)
{
    *page = CLSID_BlobTrackerPropertyPage;
    return NOERROR;
}

// ////////////////////////////////////////////////////////////////////////////
// BlobTracker::Process()
//
// Performs the processing of one frame.
//
// Inputs: 
//      Input frame data (src)
//
// ////////////////////////////////////////////////////////////////////////////

static inline bool MatchFormat(IplImage *image, IplImage *format)
{
    return (image->nChannels == format->nChannels
            && image->width == format->width
            && image->height == format->height
            && image->depth == format->depth
            && image->dataOrder == format->dataOrder
            && image->origin == format->origin
            && strncmp(image->channelSeq, format->channelSeq, 4) == 0);

}

// Used for sorting in descending order by size
static inline bool size_cmp(const BlobTrackerObject &o1, const BlobTrackerObject &o2)
{
    return o1.GetSize() > o2.GetSize();
}

// compute the square of the distance between two points.
static inline int distance(const CvPoint &p1, const CvPoint &p2)
{
    int dx = p1.x - p2.x;
    int dy = p1.y - p2.y;
    return dx*dx + dy*dy;
}

static inline bool InRect(const CvRect &rect, const CvPoint &p)
{
    return (p.x >= rect.x && p.x < rect.x + rect.width
            && p.y >= rect.y && p.y < rect.y + rect.height);
}

STDMETHODIMP BlobTracker::Process(IplImage *image)
{
    // Make sure we've gone through our initialization stage
    if (m_image == NULL || m_num_objects == 0)
        return E_FAIL;

    // Make sure the image format matches what is expected.
    if (!MatchFormat(image, &m_image_format))
        return E_FAIL;

    // Convert the image to gray if required.
    IplImage *gray_image = image;
    if (image->nChannels != 1)
    {
        cvCvtColor(image, m_tmp_image, CV_BGR2GRAY );
        gray_image = m_tmp_image;
    }

    // Dilation or blurring of the image enlarges small points to make them more visible
    // and connects any points with gaps so they become a single point.
    // Blurring has the advantage that it helps filter out noise, whereas dilation
    // exacerbates noise. However, blurring reduces the intensity of the bright spots,
    // so the threshold must be set lower.
    // Since background noise isn't a big problem with the cameras we use, on the
    // whole we determined that dilation improves tracking.
    cvDilate(gray_image, m_image, 0, 2);
    //iplBlur(gray_image, m_image, 3, 3, 1, 1);


    // Copy the modified image back into the input image, so the
    // image seen in the video window is the one that is actually processed.
    // This is fairly expensive, especially if color conversion is required
    // (i.e, if the input image is RGB); this step may be eliminated with no
    // loss of tracking functionality, in which case the video window will
    // show the original image rather than the processed image. (Crosshairs
    // are drawn on the input image in any case.)
    if (image->nChannels == 1)
        cvCopy(m_image, image);
    else
        cvCvtColor(m_image, image, CV_GRAY2BGR );


    // Search the frame for bright spots.
    BlobTrackerObjectList new_objects;
    FindObjects(new_objects);


    // Match up the objects just found with the ones found previously,
    // to try to achieve continuity of object ids across frames.
    BlobTrackerObjectList old_objects = *m_objects;
    m_objects->clear();
    unsigned long matched = 0;
    for (BlobTrackerObjectList::iterator old = old_objects.begin(); old != old_objects.end(); )
    {
        int min_d = m_distance_threshold;
        BlobTrackerObjectList::iterator min_o = new_objects.end();
        for (BlobTrackerObjectList::iterator o = new_objects.begin(); o != new_objects.end(); o++)
        {
            if (InRect(old->GetRect(), o->GetCenter()))
            {
                min_o = o;
                break;
            }

            int d = distance(o->GetCenter(), old->GetCenter());
            if (d < min_d)
            {
                min_d = d;
                min_o = o;
            }
        }

        if (min_o != new_objects.end())
        {
            int id = old->GetId();

            min_o->SetId(id);
            matched |= 1 << id;
            m_objects->push_back(*min_o);
            new_objects.erase(min_o);
            old_objects.erase(old);
        }
        else
        {
            old++;
        }
    }


    // For any objects that weren't matched up, assign ids in
    // order of decreasing size.
    if (m_objects->size() < m_num_objects && new_objects.size() > 0)
    {
        std::sort(new_objects.begin(), new_objects.end(), size_cmp);

        int id = 0;
        for (BlobTrackerObjectList::iterator o = new_objects.begin();
             o != new_objects.end() && m_objects->size() < m_num_objects;
             o++)
        {
            while (matched & (1 << id))
                id++;
            o->SetId(id);
            id++;
            m_objects->push_back(*o);
        }
    }

    // Generate output frame
    FillDestinationData(image);

    return NOERROR;
}


// (These functions are not very efficient, but they are called rarely enough
// that it is not a big impact on performance.)
static inline int GetPixel(const unsigned char *image, int width, int x, int y)
{
    return image[y*width + x];
}

static inline void SetPixel(unsigned char *image, int width, int x, int y, int color)
{
    image[y*width + x] = color;
}


// Search the image for spots that meet the brightness and size thresholds.
void BlobTracker::FindObjects(BlobTrackerObjectList &objects)
{
    for (int y = 0; y < m_image_format.height; y++)
    {
        for (int x = 0; x < m_image_format.width; x++)
        {
            // Check each pixel to see if it meets the brightness threshold.
            // (This call to GetPixel is the only one that is executed with enough frequency to have
            // any impact on performance, but optimizing it gave minimal performance improvement.)
            if (GetPixel((unsigned char *)m_image->imageData, m_image_format.width, x, y) >= m_pixel_threshold)
            {
                // Find all connected bright pixels. Set them all to black while keeping track
                // of the bounding rectangle and the number of pixels found.
                CvRect rect;
                int area;
                FloodFill((unsigned char *)m_image->imageData, m_image_format.width, m_image_format.height, x, y, m_pixel_threshold, rect, area);

                // Ignore any spots that don't meet the size threshold.
                if (area >= m_size_threshold)
                    objects.push_back(BlobTrackerObject(rect, area));
            }
        }
    }
}


// Filled horizontal segment of scanline y for xl$<=$x$<=$xr.
// Parent segment was on line y-dy. dy=l or -1
struct Segment 
{
    int y, xl, xr, dy;
    Segment(int a, int b, int c, int d) : y(a), xl(b), xr(c), dy(d) { };
};

static inline void push(std::stack<Segment> &stack, int height, int y, int xl, int xr, int dy)
{
    y += dy;
    if (y >= 0 && y < height)
        stack.push(Segment(y, xl, xr, dy));
}


static inline void SetPixel(unsigned char *image, int width, int x, int y, int color, CvRect &rect, int &area)
{
    if (x < rect.x)
        rect.x = x;
    else if (x >= rect.x+rect.width)
        rect.width = x+1-rect.x;
    if (y < rect.y)
        rect.y = y;
    else if (y >= rect.y+rect.height)
        rect.height = y+1-rect.y;
    SetPixel(image, width, x, y, color);
    area++;
}

#define GET_PIXEL(x, y) GetPixel(image, width, x, y)
#define SET_PIXEL(x, y) SetPixel(image, width, x, y, 0, rect, area)
#define PUSH(y, xl, xr, dy) push(stack, height, y, xl, xr, dy)


// FloodFill: set the pixel at (x,y) and all of its 4-connected neighbors
// that meet the brightness threshold to zero while keeping track
// of the bounding rectangle and the number of pixels found.
// A 4-connected neighbor is a pixel above, below, left, or right of
// a pixel.
// Algorithm adapted from one by Paul Heckbert 13 Sept 1982, 28 Jan 1987.

static void FloodFill(unsigned char *image, int width, int height, int start_x, int start_y, int threshold, CvRect &rect, int &area)
{
    CvRect init_rect = { start_x, start_y, 1, 1 };
    rect = init_rect;
    area = 0;

    std::stack<Segment> stack;
    PUSH(start_y, start_x, start_x, 1); /* needed in some cases */
    PUSH(start_y+1, start_x, start_x, -1); /* seed segment (popped 1st) */

    while (!stack.empty())
    {
        /* pop segment off stack and fill a neighboring scan line */
        const Segment seg = stack.top();
        stack.pop();

        // segment of scan line y-dy for xl<=x<=x2 was
        // previously filled,
        // now explore adjacent pixels in scan line y

        int l;
        int x;
        for (x = seg.xl; x >= 0 && GET_PIXEL(x, seg.y) >= threshold; x--)
            SET_PIXEL(x, seg.y);

        if (x >= seg.xl)
            goto skip;
        l = x+1;
        if (l < seg.xl)
            PUSH(seg.y, l, seg.xl-1, -seg.dy); /* leak on left? */
        x = seg.xl+1;
        do {
            for (; x < width && GET_PIXEL(x, seg.y) >= threshold; x++)
                SET_PIXEL(x, seg.y);
            PUSH(seg.y, l, x-1, seg.dy);
            if (x > seg.xr+1)
                PUSH(seg.y, seg.xr+1, x-1, -seg.dy); /* leak on right? */
skip:
            for (x++; x <= seg.xr && GET_PIXEL(x, seg.y) < threshold; x++)
                ;
            l = x;
        } while (x <= seg.xr);
    }
}


// Registration information

// List of class IDs and creator functions for the class factory. This
// provides the link between the COM entry point in the DLL and an object
// being created. The class factory will call the static CreateInstance.

CFactoryTemplate g_Templates[] = {
    { L"Blob Tracker", &CLSID_BlobTracker, BlobTracker::CreateInstance },
    { L"Blob Tracker Property Page", &CLSID_BlobTrackerPropertyPage, BlobTrackerPropertyPage::CreateInstance }
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);


//
// DllRegisterServer
//
// Register the COM objects (the tracker and the property page).
// Also add the tracker to the "Video Trackers" component category.
STDAPI DllRegisterServer()
{
    HRESULT hr = AMovieDllRegisterServer2( TRUE );
    if (FAILED(hr))
        return hr;

    ICatRegister *reg;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_ALL, IID_ICatRegister, (void **)&reg);
    if (FAILED(hr))
        return hr;

    CATEGORYINFO catinfo;
    catinfo.catid = CATID_Trackers;
    catinfo.lcid = 0x409;
    wcscpy(catinfo.szDescription, L"Video Trackers");
    reg->RegisterCategories(1, &catinfo);
    reg->RegisterClassImplCategories(CLSID_BlobTracker, 1, const_cast<GUID *>(&CATID_Trackers));

    reg->Release();

    return NOERROR;
} // DllRegisterServer


//
// DllUnregisterServer
//
// Unregister the COM objects (the tracker and the property page).
// Also remove the tracker from the "Video Trackers" component category.
STDAPI DllUnregisterServer()
{
    HRESULT hr = AMovieDllRegisterServer2( FALSE );
    if (FAILED(hr))
        return hr;

    ICatRegister *reg;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_ALL, IID_ICatRegister, (void **)&reg);
    if (FAILED(hr))
        return hr;

    reg->UnRegisterClassImplCategories(CLSID_BlobTracker, 1, const_cast<GUID *>(&CATID_Trackers));

    reg->Release();

    return NOERROR;
} // DllUnregisterServer
