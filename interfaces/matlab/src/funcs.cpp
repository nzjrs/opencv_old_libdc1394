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

#include "funcs.h"

//-----------------------------------------------------------------------------

std::string get_string( const mxArray *mxArr )
{
	int l = mxGetN(mxArr);
	char* buf = new char[l + 2];
	mxGetString(mxArr, buf, l + 1);
	std::string s(buf);
	delete[] buf;
	return s;
}

//------------------------------------------------------------------------------
//                                verifyDepth
//
void verifyDepth( mxClassID classID, bool bAnyDepth, int &depth )
{
	int _depth = depth;
	switch(	classID )
	{
		case mxUINT8_CLASS:
			depth = IPL_DEPTH_8U;
			break;
		case mxUINT16_CLASS:
			depth = IPL_DEPTH_16U;
			break;
		case mxSINGLE_CLASS:
			depth = IPL_DEPTH_32F;
			break;
		case mxINT8_CLASS:
			depth = IPL_DEPTH_8S;
			break;
		case mxINT16_CLASS:
			depth = IPL_DEPTH_16S;
			break;
		case mxINT32_CLASS:
			depth = IPL_DEPTH_32S;
			break;
		default:
			throw_error( "Unsupported image format." );
			return;
			break;
	}
	if( !bAnyDepth && depth != _depth )
	{
		throw_error( "Incorrect image type." );
		return;
	}
}

//------------------------------------------------------------------------------
//                                verifyArray
//
void verifyArray( mxArray *mxArr, int *dims, int &ndim, bool bAllowEmpty,
				  bool bAnyClass, mxClassID &classID )
{
	bool bEmpty = mxIsEmpty( mxArr );
	if( !bAllowEmpty && bEmpty )
	{
		throw_error( "Array should be not empty." );
		return;
	}
	
	if( bEmpty )
	{
		if( ndim <= 0 )
		{
			ndim = mxGetNumberOfDimensions( mxArr );
		}
		int dim = 0;
		for( dim = 0; dim < ndim; dim++ )
		{
			if( dims[dim] < 0 )
			{
				dims[dim] = 0;
			}
		}
		return;
	}
	
	mxClassID _classID = classID;
	classID    = mxGetClassID( mxArr );
	if( !bAnyClass && classID != _classID )
	{
		throw_error( "Incorrect array type." );
		return;
	}
	
	int _ndim = ndim;
	ndim      = mxGetNumberOfDimensions( mxArr );
	if( _ndim > 0 && ndim != _ndim )
	{
		std::ostringstream ostr;
		ostr << "Incorrect array size. " <<
			"Array should have " << _ndim << " dimensions.";
		throw_error( ostr.str() );
		return;
	}
	const int *_dims = mxGetDimensions( mxArr );
	int dim = 0;
	for( dim = 0; dim < ndim; dim++ )
	{
		if( dims[dim] > 0 && dims[dim] != _dims[dim] )
		{
			std::ostringstream ostr;
			ostr << "Incorrect array size. " <<
				"Dimension: " << (dim + 1) << ".";
			throw_error( ostr.str() );
			return;
		}
		dims[dim] = _dims[dim];
	} // for each dimension
}

//------------------------------------------------------------------------------
//                                verifyMatrix
//
void verifyMatrix( mxArray *mxArr, int &rows, int &cols, bool bAllowEmpty,
				  bool bAnyClass, mxClassID &classID )
{
	int ndim = 2;
	int dims[2];
	dims[0] = rows;
	dims[1] = cols;
	verifyArray( mxArr, dims, ndim, bAllowEmpty, bAnyClass, classID );
	rows = dims[0];
	cols = dims[1];
}

//------------------------------------------------------------------------------
//                                verifyVector
//
void verifyVector( mxArray *mxArr, int &num, VecType &type, bool bAllowEmpty,
				   bool bAnyClass, mxClassID &classID )
{
	bool bEmpty = mxIsEmpty( mxArr );
	if( !bAllowEmpty && bEmpty )
	{
		throw_error( "Vector should be not empty." );
		return;
	}
	
	if( bEmpty )
	{
		if( num < 0 )
		{
			num = 0;
		}
		return;
	}
	
	mxClassID _classID = classID;
	classID    = mxGetClassID( mxArr );
	if( !bAnyClass && classID != _classID )
	{
		throw_error( "Incorrect vector type." );
		return;
	}
	
	const int *dims = mxGetDimensions( mxArr );
	if( type == any )
	{
		type = ( dims[0] > dims[1] ) ? col : row;
	}
	
	int _num = num;
	num = mxGetNumberOfElements( mxArr );
	if( _num > 0 && _num != num )
	{
		std::ostringstream ostr;
		ostr << "Incorrect vector size. " <<
			"Vector should have " << _num << " element(s).";
		throw_error( ostr.str() );
		return;
	}

	if( mxGetNumberOfDimensions( mxArr ) != 2 ||
		( type == row && dims[0] != 1 ) ||
		( type == col && dims[1] != 1 ) )
	{
		std::ostringstream ostr;
		ostr << "Should be " << (( type == row ) ? "row" : "column") <<
			" vector.";
		throw_error( ostr.str() );
		return;
	}
}

//------------------------------------------------------------------------------
//                                verifySeq
//
void verifySeq( mxArray *mxArr, int maxIndex, int index,
				mxClassID &dataClass, void *&dataPtr, int &dataNum,
				CvRect &rect, int &type,
				int &h_prev, int &h_next, int &v_prev, int &v_next )
{
	if( index >= maxIndex || index < 0 )
	{
		throw_error( "Invalid index." );
		return;
	}
	mxArray *mxFdata   = mxGetField( mxArr, index, "data" );
	mxArray *mxFrect   = mxGetField( mxArr, index, "rect" );
	mxArray *mxFtype   = mxGetField( mxArr, index, "type" );
	mxArray *mxFh_prev = mxGetField( mxArr, index, "h_prev" );
	mxArray *mxFh_next = mxGetField( mxArr, index, "h_next" );
	mxArray *mxFv_prev = mxGetField( mxArr, index, "v_prev" );
	mxArray *mxFv_next = mxGetField( mxArr, index, "v_next" );

	/* data */
	if( !mxFdata || mxIsEmpty( mxFdata ) || 
		mxGetNumberOfDimensions( mxFdata ) != 2 ||
		!mxIsNumeric( mxFdata ) )
	{
		std::ostringstream ostr;
		ostr << "Invalid 'data' field in struct number " << index
			 << ". It should be n by 2 numeric array.";
		throw_error( ostr.str() );
		return;
	}
	dataClass = mxGetClassID( mxFdata );
	dataPtr   = (void *)mxGetData( mxFdata );
	dataNum   = mxGetM( mxFdata );
	
	/* rect */
	if( !mxFrect || mxIsEmpty( mxFrect ) || 
		mxGetNumberOfDimensions( mxFrect ) != 2 ||
		!mxIsNumeric( mxFrect ) || mxGetM( mxFrect ) != 1 ||
		mxGetN( mxFrect ) != 4 )
	{
		std::ostringstream ostr;
		ostr << "Invalid 'rect' field in struct number " << index
			 << ". It should be 1 by 4 numeric array.";
		throw_error( ostr.str() );
		return;
	}
	MXTYPE_DATAPTR( mxGetClassID( mxFrect ), ;, rectData, mxGetData( mxFrect );
		castCopy( rectData, (int *)(&rect), 1, 4, 1, 1, 1, 1 ); );

	/* type */
	if( !mxFtype || mxIsEmpty( mxFtype ) || 
		mxGetNumberOfDimensions( mxFtype ) != 2 ||
		!mxIsNumeric( mxFtype ) || mxGetM( mxFtype ) != 1 ||
		mxGetN( mxFtype ) != 1 )
	{
		std::ostringstream ostr;
		ostr << "Invalid 'type' field in struct number " << index
			 << ". It should be 1 by 1 numeric array.";
		throw_error( ostr.str() );
		return;
	}
	MXTYPE_DATAPTR( mxGetClassID( mxFtype ), ;, typeData, mxGetData( mxFtype );
		castCopy( typeData, (int *)(&type), 1, 1, 1, 1, 1, 1 ); );

	/* h_prev */
	if( !mxFh_prev || mxIsEmpty( mxFh_prev ) || 
		mxGetNumberOfDimensions( mxFh_prev ) != 2 ||
		!mxIsNumeric( mxFh_prev ) || mxGetM( mxFh_prev ) != 1 ||
		mxGetN( mxFh_prev ) != 1 )
	{
		std::ostringstream ostr;
		ostr << "Invalid 'h_prev' field in struct number " << index
			 << ". It should be 1 by 1 numeric array.";
		throw_error( ostr.str() );
		return;
	}
	MXTYPE_DATAPTR( mxGetClassID( mxFh_prev ), ;, 
		h_prevData, mxGetData( mxFh_prev );
		castCopy( h_prevData, (int *)(&h_prev), 1, 1, 1, 1, 1, 1 ); );
	if( h_prev > maxIndex || h_prev < 0 )
	{
		std::ostringstream ostr;
		ostr << "Invalid 'h_prev' value in struct number " << index
			 << ".";
		throw_error( ostr.str() );
		return;
	}

	/* h_next */
	if( !mxFh_next || mxIsEmpty( mxFh_next ) || 
		mxGetNumberOfDimensions( mxFh_next ) != 2 ||
		!mxIsNumeric( mxFh_next ) || mxGetM( mxFh_next ) != 1 ||
		mxGetN( mxFh_next ) != 1 )
	{
		std::ostringstream ostr;
		ostr << "Invalid 'h_next' field in struct number " << index
			 << ". It should be 1 by 1 numeric array.";
		throw_error( ostr.str() );
		return;
	}
	MXTYPE_DATAPTR( mxGetClassID( mxFh_next ), ;, 
		h_nextData, mxGetData( mxFh_next );
		castCopy( h_nextData, (int *)(&h_next), 1, 1, 1, 1, 1, 1 ); );
	if( h_next > maxIndex || h_next < 0 )
	{
		std::ostringstream ostr;
		ostr << "Invalid 'h_next' value in struct number " << index
			 << ".";
		throw_error( ostr.str() );
		return;
	}

	/* v_prev */
	if( !mxFv_prev || mxIsEmpty( mxFv_prev ) || 
		mxGetNumberOfDimensions( mxFv_prev ) != 2 ||
		!mxIsNumeric( mxFv_prev ) || mxGetM( mxFv_prev ) != 1 ||
		mxGetN( mxFv_prev ) != 1 )
	{
		std::ostringstream ostr;
		ostr << "Invalid 'v_prev' field in struct number " << index
			 << ". It should be 1 by 1 numeric array.";
		throw_error( ostr.str() );
		return;
	}
	MXTYPE_DATAPTR( mxGetClassID( mxFv_prev ), ;, 
		v_prevData, mxGetData( mxFv_prev );
		castCopy( v_prevData, (int *)(&v_prev), 1, 1, 1, 1, 1, 1 ); );
	if( v_prev > maxIndex || v_prev < 0 )
	{
		std::ostringstream ostr;
		ostr << "Invalid 'v_prev' value in struct number " << index
			 << ".";
		throw_error( ostr.str() );
		return;
	}

	/* v_next */
	if( !mxFv_next || mxIsEmpty( mxFv_next ) || 
		mxGetNumberOfDimensions( mxFv_next ) != 2 ||
		!mxIsNumeric( mxFv_next ) || mxGetM( mxFv_next ) != 1 ||
		mxGetN( mxFv_next ) != 1 )
	{
		std::ostringstream ostr;
		ostr << "Invalid 'v_next' field in struct number " << index
			 << ". It should be 1 by 1 numeric array.";
		throw_error( ostr.str() );
		return;
	}
	MXTYPE_DATAPTR( mxGetClassID( mxFv_next ), ;, 
		v_nextData, mxGetData( mxFv_next );
		castCopy( v_nextData, (int *)(&v_next), 1, 1, 1, 1, 1, 1 ); );
	if( v_next > maxIndex || v_next < 0 )
	{
		std::ostringstream ostr;
		ostr << "Invalid 'v_next' value in struct number " << index
			 << ".";
		throw_error( ostr.str() );
		return;
	}
}
