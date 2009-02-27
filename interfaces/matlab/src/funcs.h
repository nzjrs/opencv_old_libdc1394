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

#ifndef FUNCS_H
#define FUNCS_H


#include "cv.h"

#include "qerr.h"
#include "tmacro.h"

typedef enum {
	any, /* any type */
	row, /* only row */
	col, /* only column */
} VecType; // vector type

/*F/////////////////////////////////////////////////////////////////////////////
//  Name:        verifyDepth
//  Purpose:     verify depth of image
//  Parameters:
//  Returns:
//  Description: Depth is determined from classID and returned via
//    depth parameter. If classID is not supported or bAnyDepth == false and
//    determined depth is not equal to input desired depth then error is thrown.
//
//F*/
void verifyDepth( mxClassID classID, bool bAnyDepth, int &depth );

/*F/////////////////////////////////////////////////////////////////////////////
//  Name:        verifyArray
//  Purpose:     verify mxArray
//  Parameters:
//  Returns:
//  Description: If dims[i] > 0, then it is desired number of elements
//    If dims[i] < 0, then it is filled with number of elements
//    If ndim > 0 then it is desired number of dimensions, otherwise it is
//    filled with number of dimensions.
//
//F*/
void verifyArray( mxArray *mxArr, int *dims, int &ndim, bool bAllowEmpty,
				  bool bAnyClass, mxClassID &classID );

void verifyMatrix( mxArray *mxArr, int &rows, int &cols, bool bAllowEmpty,
				   bool bAnyClass, mxClassID &classID );

void verifyVector( mxArray *mxArr, int &num, VecType &type, bool bAllowEmpty,
				   bool bAnyClass, mxClassID &classID );

void verifySeq( mxArray *mxArr, int maxIndex, int index,
				mxClassID &dataClass, void *&dataPtr, int &dataNum,
				CvRect &rect, int &type,
				int &h_prev, int &h_next, int &v_prev, int &v_next );

std::string get_string( const mxArray *mxArr );

#endif /* FUNCS_H */

