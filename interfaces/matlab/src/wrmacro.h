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

#ifndef CV_WRAP_MACRO
#define CV_WRAP_MACRO

#include "cv.h"
#include <mex.h>

#include "tmacro.h"

#ifndef IPL_DEPTH_MASK
#define IPL_DEPTH_MASK 255
#endif

//----------------------------------- Classes ---------------------------------
template <class T>
class CArr
{
public:
	T *pData;
	bool ToClearData;
	
	CArr();
	~CArr();
	inline void  Create( int num, bool tocleardata);
	inline operator T*()
	{
		return pData;
	}

	inline operator void*()
	{
		return pData;
	}

	inline operator T()
	{
		return pData[0];
	}

	inline T operator[](int n) const
	{
		return pData[n];
	}
	
	inline T* operator+(int n) const
	{
		return pData + n;
	}
};

template <class T>
inline CArr<T>::CArr()  
{
	pData = NULL;
	ToClearData = false;
}

template <class T>
inline CArr<T>::~CArr() 
{
	if (ToClearData && pData)
		free(pData);
}

template <class T>
void  inline CArr<T>::Create( int num, bool tocleardata)
{
	pData = (T*) calloc(1,sizeof(T)*num);
	ToClearData = tocleardata;
}

//-------------------------------------------------
class CIplImage
{
public:
	IplImage *pHeader;
	void *pData;
	bool ToClearData;
	
	CIplImage();
	~CIplImage();
	void  CreateImageHeader( CvSize size, int depth, int channels );
	void  SetImageData( void* data, int step , bool tocleardata);
	inline operator IplImage*() const
	{
		return pHeader;
	}
};

inline CIplImage::CIplImage() 
{
	pHeader = NULL;
	pData = NULL;
	ToClearData = false;
}

inline CIplImage::~CIplImage() 
{
	if (ToClearData && pData)
		free(pData);
	
	if (pHeader)
		cvReleaseImageHeader(&pHeader);
}

inline void CIplImage::CreateImageHeader( CvSize size, int depth, int channels )
{
	if (!pHeader)
		pHeader = cvCreateImageHeader( size, depth, channels );
	return;
}

inline void CIplImage::SetImageData( void* data, int step, bool tocleardata )
{
	if (pHeader && !pData)
	{
		pData = data;
		ToClearData = tocleardata;
		cvSetImageData( pHeader, data, step );
	}
	return;
}

//----------------------------------- Macroses---------------------------------

/*#define WRAPPER( nargin , nargout, wrapper_name ) \
void  wrapper_name (const CV::FrontEnd::ArgValType& arginhandle, CV::FrontEnd::ReturnValType& argouthandle){ \
		arginhandle.verify_nargin(nargin); \
		argouthandle.verify_nargout(nargout)*/

#define WRAPPER( nargin , nargout, wrapper_name ) \
void  wrapper_name (int nlhs, mxArray* plhs[], int nrhs, mxArray* prhs[]) { \
	if (nrhs != nargin) \
	{ \
	std::ostringstream ostr; \
		ostr << "Function expected " << nargin << " input argument"; \
		if (nargin != 1) \
			ostr << "s"; \
		throw_error(ostr.str()); \
	} \
	if (nlhs != nargout) \
	{ \
	std::ostringstream ostr; \
		ostr << "Function expected " << nargout << " output argument"; \
		if (nargout != 1) \
			ostr << "s"; \
		throw_error(ostr.str()); \
	}

#define RET \
	return;	\
	}

#define PARAM( name, param ) name##_##param

// {{ Macros for row or column vector of numbers
#define VECTOR_IN( n, type, name, size, cond ) \
	int  name##_num        = size;  \
	bool name##_anyClass   = true;  \
	bool name##_allowEmpty = false; \
	bool name##_inPlace    = false; /* use matlab data directly */ \
	mxClassID name##_class = mxDOUBLE_CLASS; \
	VecType name##_vecType = any; /* type of vector */ \
	 \
	CArray< type > name; \
	 \
	cond; \
	 \
	_ENHANCE_ERROR( \
		verifyVector( prhs[n], name##_num, name##_vecType, name##_allowEmpty, \
			name##_anyClass, name##_class ); \
		if( !mxIsEmpty( prhs[n] ) ) \
		{ \
			if( !name##_inPlace ) \
			{ \
				name.create( name##_num ); \
				MXTYPE_DATAPTR( name##_class, ;, _srcData, \
					mxGetData( prhs[n] ); \
					castCopy( _srcData, (type *)name, 1, name##_num, \
						1, 1, 1, 1 ) ); \
			} \
			else \
			{ \
				name.set( (type *)mxGetData( prhs[n]) ); \
			} \
		} \
		, \
		name ) 

#define VECTOR_OUT( n, type, name, size, cond ) \
	int  name##_num        = size;  \
	bool name##_anyClass   = true;  \
	bool name##_inPlace    = false; /* use matlab data directly */ \
	mxClassID name##_class = mxDOUBLE_CLASS; \
	int  name##_n          = n; \
	VecType name##_vecType = any; /* type of vector */ \
	 \
	CArray< type > name; \
	 \
	cond; \
	 \
	_ENHANCE_ERROR( \
			if( !name##_inPlace ) \
			{ \
				name.create( name##_num ); \
			} \
			else \
			{ \
				_CREATE_VECTOR( name ); \
				name.set( (type *)mxGetData( plhs[n]) ); \
			} \
			, \
			name ) 



#define PUT_VECTOR( name ) \
	if( !name##_inPlace ) \
	{ \
		_CREATE_VECTOR( name ); \
		MXTYPE_DATAPTR( name##_class, ;, _dstData, \
			mxGetData( plhs[name##_n] ); \
			castCopy( name.getData(), _dstData, 1, name##_num, \
					1, 1, 1, 1 ) ); \
	}

// }} Macros for row or column vector of numbers

// {{ Macros for single number
#define NUM_IN( n, type, name ) \
	mxClassID name##_class = mxDOUBLE_CLASS; \
	type name; \
	_ENHANCE_ERROR( \
		int _num = 1; \
		verifyMatrix( prhs[n], _num, _num, false, \
			true, name##_class ); \
		MXTYPE_DATAPTR( name##_class, ;,  _srcData, mxGetData( prhs[n] ); \
			castCopy( _srcData, (type *)&name, 1, 1, 1, 1, 1, 1 ) ); \
		, \
		name ) 

#define NUM_OUT( n, type, name, cond ) \
	mxClassID name##_class = mxDOUBLE_CLASS; \
	int  name##_n          = n; \
	 \
	type name; \
	 \
	cond; \

#define PUT_NUM( name ) \
	_CREATE_MATRIX( name##_n, 1, 1, name##_class ); \
	MXTYPE_DATAPTR( name##_class, ;, _dstData, mxGetData( plhs[name##_n] ); \
		castCopy( &name, _dstData, 1, 1, 1, 1, 1, 1 ) );
// }} Macros for single number

// {{ Macros for C array of struct
#define ARRAY_IN( n, type, name, size, cond ) \
	int  name##_num        = size;  \
	bool name##_anyClass   = true;  \
	bool name##_allowEmpty = false; \
	bool name##_inPlace    = false; /* use matlab data directly */ \
	mxClassID name##_class = mxDOUBLE_CLASS; \
	 \
	CArray< type > name; \
	\
	cond; \
	\
	_ENHANCE_ERROR( \
		int _numFields = type##NUMFIELDS; \
		verifyMatrix( prhs[n], name##_num, _numFields, name##_allowEmpty, \
			name##_anyClass, name##_class ); \
		 \
		if( !mxIsEmpty( prhs[n] ) ) \
		{ \
			if( !name##_inPlace ) \
			{ \
				name.create( name##_num ); \
				MXTYPE_DATAPTR( name##_class, ;,  _srcData, mxGetData( prhs[n] ); \
					convert( _srcData, (type *)name, name##_num ) ); \
			} \
			else \
			{ \
				name.set( (type *)mxGetData( prhs[n]) ); \
			} \
		} \
		, \
		name ) 

#define ARRAY_OUT( n, type, name, size, cond ) \
	int  name##_num        = size;  \
	bool name##_anyClass   = true;  \
	bool name##_inPlace    = false; /* use matlab data directly */ \
	mxClassID name##_class = mxDOUBLE_CLASS; \
	int  name##_n          = n; \
	int  name##_numFields  = type##NUMFIELDS; \
	 \
	CArray< type > name; \
	 \
	cond; \
	 \
	_ENHANCE_ERROR( \
		mxDestroyArray( plhs[n] ); \
		if( !name##_inPlace ) \
		{ \
			name.create( name##_num ); \
		} \
		else \
		{ \
			_CREATE_MATRIX( n, name##_num, name##_numFields, name##_class ); \
			name.set( (type *)mxGetData( plhs[n]) ); \
		} \
		, \
		name ) 

#define PUT_ARRAY( name ) \
	if( !name##_inPlace ) \
	{ \
		_CREATE_MATRIX( name##_n, name##_num, name##_numFields, name##_class ); \
		MXTYPE_DATAPTR( name##_class, ;, _dstData, mxGetData( plhs[name##_n] ); \
			convert( name.getData(), _dstData, name##_num ) ); \
	}

// create temporary array
#define TEMP_ARRAY( type, name, size ) \
	CArray< type > name; \
	name.create( size );
// }} Macros for C array of struct

// {{ Macros for image
#define IMG_IN( n, name, cond ) \
	CvSize    name##_size        = cvSize( -1, -1 ); \
	int       name##_channels    = -1; \
	bool      name##_anyClass    = true; \
	mxClassID name##_class       = mxDOUBLE_CLASS; \
	bool      name##_anyDepth    = true; \
	int       name##_depth       = IPL_DEPTH_8U; \
	bool      name##_allowEmpty  = false; \
	bool      name##_inPlace     = true; \
	 \
	cond; \
	 \
    CImage name; \
	 \
	_ENHANCE_ERROR( \
		int dims[3]; \
		dims[0] = name##_size.width; \
		dims[1] = name##_size.height; \
		dims[2] = name##_channels; \
		int ndim = ( name##_channels < 1 ) ? -1 : \
			( ( name##_channels != 1 ) ? 3 : 2 ); \
		verifyArray( prhs[n], dims, ndim, \
			name##_allowEmpty, name##_anyClass, name##_class ); \
		name##_size.width  = dims[0]; \
		name##_size.height = dims[1]; \
		name##_channels    = ( ndim == 2 ) ? 1 : dims[2]; \
		 \
		if( !mxIsEmpty( prhs[n] ) ) \
		{ \
			verifyDepth( name##_class, name##_anyDepth, name##_depth ); \
			 \
				void *name##_data      = mxGetData( prhs[n] ); \
				int name##_typeSize    = \
					( IPL_DEPTH_MASK & name##_depth ) / 8; \
				int name##_step        = name##_typeSize * name##_size.width; \
				int name##_channelSize = name##_step * name##_size.height; \
			 \
			name.createHeader( name##_size, name##_depth, name##_channels ); \
			if( name##_channels == 3 ) \
			{ \
				CImage name##_r; \
				name##_r.createHeader( name##_size, name##_depth, 1 ); \
				name##_r.setData( name##_data, name##_step ); \
				CImage name##_g; \
				name##_g.createHeader( name##_size, name##_depth, 1 ); \
				name##_g.setData( (char *)name##_data + name##_channelSize, \
					name##_step ); \
				CImage name##_b; \
				name##_b.createHeader( name##_size, name##_depth, 1 ); \
				name##_b.setData( \
					(char *)name##_data + 2 * name##_channelSize, \
					name##_step ); \
				name.createData(); \
				cvCvtPlaneToPix( (IplImage *)name##_r, (IplImage *)name##_g, \
					(IplImage *)name##_b, 0, (IplImage *)name ); \
			} \
			else \
			{ /* single channel */ \
				if( name##_inPlace ) \
				{ \
					name.setData( name##_data, name##_step ); \
				} \
				else \
				{ \
					CImage tmpImage; \
					tmpImage.createHeader( name##_size, \
						name##_depth, name##_channels ); \
					tmpImage.setData( name##_data, name##_step ); \
					name.createData(); \
					cvCopy( (IplImage *)tmpImage, (IplImage *)name ); \
				} \
			} \
		} \
		, \
		name ) 

#define IMG_OUT( n, name, cond ) \
	CvSize    name##_size        = cvSize( -1, -1 ); \
	int       name##_channels    = -1; \
	mxClassID name##_class       = mxDOUBLE_CLASS; \
	int       name##_depth       = IPL_DEPTH_8U; \
	bool      name##_inPlace     = true; \
	int       name##_n           = n; \
	 \
	cond; \
	 \
    CImage name; \
	 \
	_ENHANCE_ERROR( \
		verifyDepth( name##_class, true, name##_depth ); \
		name.createHeader( name##_size, name##_depth, name##_channels ); \
		if( name##_channels == 3 || !name##_inPlace ) \
		{ \
			name.createData(); \
		} \
		else \
		{ /* single channel */ \
			if( name##_inPlace ) \
			{ \
				_CREATE_IMAGE( name ); \
				void *name##_data      = mxGetData( plhs[name##_n] ); \
				int name##_typeSize    = \
					( IPL_DEPTH_MASK & name##_depth ) / 8; \
				int name##_step        = name##_typeSize * name##_size.width; \
				int name##_channelSize = name##_step * name##_size.height; \
				name.setData( name##_data, name##_step ); \
			} \
		} \
		, \
		name ) 

#define PUT_IMG( name ) \
	if( name##_channels == 3 || !name##_inPlace ) \
	{ \
		_CREATE_IMAGE( name ); \
		void *name##_data      = mxGetData( plhs[name##_n] ); \
		int name##_typeSize    = ( IPL_DEPTH_MASK & name##_depth ) / 8; \
		int name##_step        = name##_typeSize * name##_size.width; \
		int name##_channelSize = name##_step * name##_size.height; \
		if( name##_channels == 3 ) \
		{ \
			CImage name##_r; \
			name##_r.createHeader( name##_size, name##_depth, 1 ); \
			name##_r.setData( name##_data, name##_step ); \
			CImage name##_g; \
			name##_g.createHeader( name##_size, name##_depth, 1 ); \
			name##_g.setData( (char *)name##_data + name##_channelSize, \
				name##_step ); \
			CImage name##_b; \
			name##_b.createHeader( name##_size, name##_depth, 1 ); \
			name##_b.setData( (char *)name##_data + 2 * name##_channelSize, \
				name##_step ); \
			cvCvtPixToPlane( (IplImage *)name, \
				(IplImage *)name##_r, \
				(IplImage *)name##_g, \
				(IplImage *)name##_b, \
				0 ); \
		} \
		else \
		{ \
			CImage tmpImage; \
			tmpImage.createHeader( name##_size, name##_depth, \
			    name##_channels ); \
			tmpImage.setData( name##_data, name##_step ); \
			cvCopy( (IplImage *)name, (IplImage *)tmpImage ); \
		} \
	}

// create temporary image and allocate image data
#define TEMP_IMAGE( name, size, depth, channels ) \
	CImage name; \
	name.createHeader( size, depth, channels ); \
	name.createData();

// }} Macros for image

// {{ Macros for sequences
#define SEQ_IN( n, first, name, pointType, rectType, cond ) \
	bool name##_recursive  = true; \
	int  name##_maxLevel   = 0; \
	bool name##_allowEmpty = false; \
	int  name##_num        = -1; /* number of sequences */ \
	int  name##_headerSize = sizeof( CvContour ); \
	 \
	cond; \
	 \
	CStorage   name##_storage; \
	CvContour *name = 0; \
	 \
	_ENHANCE_ERROR( \
		int _rows = 1; \
		mxClassID mxClass = mxSTRUCT_CLASS; \
		verifyMatrix( prhs[n], _rows, name##_num, name##_allowEmpty, false, \
			mxClass ); \
		if( name##_num > 0 ) \
		{ \
			int icur = 0; /* current index (1 based) */ \
			 \
			CvSeq *prev_contour = 0; \
			CvSeq *parent = 0; \
			CvSeq *cur   = 0; \
			int isrc_seq = first; /* starting point */ \
			int level    = 1; \
			int maxLevel = name##_maxLevel; \
			mxClassID   dataClass; \
			void       *dataPtr = 0; \
			int         dataNum = 0; \
			CvRect      rect; \
			int         type  = 0; \
			int         flags = 0; \
			CvSeqWriter writer; \
			int         nelem = 0; \
			 \
			TEMP_ARRAY( int, h_prev, name##_num ); \
			TEMP_ARRAY( int, h_next, name##_num ); \
			TEMP_ARRAY( int, v_prev, name##_num ); \
			TEMP_ARRAY( int, v_next, name##_num ); \
			 \
			while( isrc_seq != 0 ) \
			{ \
				icur++; \
				verifySeq( prhs[n], name##_num, isrc_seq-1, \
					dataClass, dataPtr, dataNum, rect, \
					type, h_prev[isrc_seq-1], h_next[isrc_seq-1], \
					v_prev[isrc_seq-1], v_next[isrc_seq-1] ); \
				flags = CV_SEQ_CONTOUR | \
					( ( type == 1 ) ? CV_SEQ_FLAG_HOLE : 0 ); \
				cvStartWriteSeq( flags, name##_headerSize, sizeof( CvPoint ), \
					name##_storage, &writer ); \
				TEMP_ARRAY( pointType, tmpData, dataNum ); \
				MXTYPE_DATAPTR( dataClass, ;, dataData, dataPtr; \
					convert( dataData, tmpData, dataNum ); ); \
				for( nelem = 0; nelem < dataNum; nelem++ ) \
				{ \
					CV_WRITE_SEQ_ELEM( (tmpData[nelem]), writer ); \
				} /* for each elem in arr */ \
				cur = (CvSeq *)cvEndWriteSeq( &writer ); \
				if( !name ) name = (CvContour *)cur; \
				convert( (int *)&rect, \
					(rectType *)&(((CvContour *)cur)->rect), 1 ); \
				 \
				if( maxLevel < 0 && !name##_recursive ) \
				{ \
					h_next[isrc_seq-1] = 0; \
					maxLevel = -maxLevel; \
				} \
				 \
				cur->v_prev = parent; \
				cur->h_prev = prev_contour; \
				 \
				cur->v_next = 0; \
				cur->h_next = 0; \
				 \
				if( maxLevel == 0 && !name##_recursive ) \
				{ \
					break; \
				} \
				 \
				if( prev_contour ) \
				{ \
					prev_contour->h_next = cur; \
				} \
				else if( parent ) \
				{ \
					parent->v_next = cur; \
				} \
				prev_contour = cur; \
				 \
				if( v_next[isrc_seq-1] != 0 && \
					( level < maxLevel || name##_recursive ) ) \
				{ \
					assert( prev_contour != 0 ); \
					parent = prev_contour; \
					prev_contour = 0; \
					isrc_seq = v_next[isrc_seq-1]; \
					level++; \
				} \
				else \
				{ \
					while( h_next[isrc_seq-1] == 0 ) \
					{ \
						isrc_seq = v_prev[isrc_seq-1]; \
						level--; \
						if( level == 0 && !name##_recursive ) \
						{ \
							isrc_seq = 0; \
						} \
						if( isrc_seq == 0 ) \
						{ \
							break; \
						} \
						prev_contour = parent; \
						if( parent ) \
						{ \
							parent = parent->v_prev; \
						} \
					} \
					if( isrc_seq ) \
					{ \
						isrc_seq = h_next[isrc_seq-1]; \
					} \
				} \
			} \
			name##_num = icur; \
 		} /* not empty */ \
		, \
		name )
/* end SEQ_IN( ... ) */

#define SEQ_OUT( n, name, cond ) \
	int       name##_n          = n; \
	int       name##_num        = 0; /* number of sequences */ \
	mxClassID name##_pointClass = mxDOUBLE_CLASS; \
	int       name##_headerSize = sizeof( CvContour ); \
	 \
	cond; \
	 \
	CStorage   name##_storage; \
	CvContour *name = 0;

#define PUT_SEQ( name, pointType, rectType ) \
	_ENHANCE_ERROR( \
		mxDestroyArray( plhs[name##_n] ); \
		const char *fieldnames[7]; \
		fieldnames[0] = "data"; \
		fieldnames[1] = "rect"; \
		fieldnames[2] = "type"; \
		fieldnames[3] = "h_prev"; \
		fieldnames[4] = "h_next"; \
		fieldnames[5] = "v_prev"; \
		fieldnames[6] = "v_next"; \
		 \
		plhs[name##_n] = mxCreateStructMatrix( (name##_num > 0) ? 1 : 0, \
			name##_num, \
			sizeof( fieldnames ) / sizeof( fieldnames[0] ), \
			fieldnames ); \
		if( name##_num > 0 ) \
		{ \
			TEMP_ARRAY( int,     h_prev, name##_num ); \
			TEMP_ARRAY( int,     h_next, name##_num ); \
			TEMP_ARRAY( int,     v_prev, name##_num ); \
			TEMP_ARRAY( int,     v_next, name##_num ); \
			TEMP_ARRAY( CvSeq *, seq,    name##_num ); \
			 \
			/* indexes of the previous and parent sequences (1 based) */ \
			int iprev_contour = 0; \
			int iparent = 0; \
			int icur = 0; /* current index (1 based) */ \
			CvSeq *src_seq = (CvSeq *)name; /* starting point */ \
			 \
			while( src_seq != 0 ) \
			{ \
				icur++; \
				if( icur > name##_num ) \
				{ \
					throw_error( "Too many sequences. May be broken links." ); \
					return; \
				} \
				/* put src_seq into StructArray[icur] */ \
				seq[icur-1] = (CvSeq *)src_seq; \
				 \
				v_prev[icur-1] = iparent; \
				h_prev[icur-1] = iprev_contour; \
				v_next[icur-1] = 0; \
				h_next[icur-1] = 0; \
				 \
				if( iprev_contour ) \
				{ \
					h_next[iprev_contour-1] = icur; \
				} \
				else if( iparent ) \
				{ \
					v_next[iparent-1] = icur; \
				} \
				iprev_contour = icur; \
				 \
				if( src_seq->v_next ) \
				{ \
					assert( iprev_contour != 0 ); \
					iparent = iprev_contour; \
					iprev_contour = 0; \
					src_seq = src_seq->v_next; \
				} \
				else \
				{ \
					while( src_seq->h_next == 0 ) \
					{ \
						src_seq = src_seq->v_prev; \
						if( src_seq == 0 ) \
						{ \
							break; \
						} \
						iprev_contour = iparent; \
						if( iparent ) \
						{ \
							/* iparent = parent->v_prev */ \
							iparent = v_prev[iparent-1]; \
						} \
					} \
					if( src_seq ) \
					{ \
						src_seq = src_seq->h_next; \
					} \
				} \
			} \
			if( icur < name##_num ) \
			{ \
				/* shorten struct array if there are */ \
				/* less than name##_num  seqences */ \
				name##_num = icur; \
				mxSetM( plhs[name##_n], icur ); \
				mxSetN( plhs[name##_n], (icur == 0) ? 0 : 1 ); \
			} \
			/* copy data into MATLAB */ \
			/* data */ \
			MXTYPE_DATAPTR( name##_pointClass, \
				mxArray     *mxFdata = 0; \
				for( icur = 0; icur < name##_num; icur++ ) \
				{ \
					mxFdata = mxCreateNumericMatrix( seq[icur]->total, \
						pointType##NUMFIELDS, name##_pointClass, mxREAL );, \
					dataData, mxGetData( mxFdata ); \
					TEMP_ARRAY( pointType, tmpArr, seq[icur]->total ); \
					cvCvtSeqToArray( (CvSeq *)seq[icur], \
						(void *)(pointType *)tmpArr ); \
					convert( (pointType *)tmpArr, dataData, seq[icur]->total );\
					mxSetField( plhs[name##_n], icur, "data", mxFdata ); \
				} \
				); \
			 \
			/* rect, type, references */ \
			mxArray *mxFrect   = 0; \
			mxArray *mxFtype   = 0; \
			mxArray *mxFh_prev = 0; \
			mxArray *mxFh_next = 0; \
			mxArray *mxFv_prev = 0; \
			mxArray *mxFv_next = 0; \
			 \
			double *rectData   = 0; \
			double *typeData   = 0; \
			double *h_prevData = 0; \
			double *h_nextData = 0; \
			double *v_prevData = 0; \
			double *v_nextData = 0; \
			int     type       = 0; \
			for( icur = 0; icur < name##_num; icur++ ) \
			{ \
				/* rect */ \
				mxFrect = mxCreateNumericMatrix( \
					1, rectType##NUMFIELDS, mxDOUBLE_CLASS, mxREAL ); \
				rectData = (double *) mxGetData( mxFrect ); \
				convert( (rectType *)&(((CvContour *)seq[icur])->rect), \
						 rectData, 1 ); \
				mxSetField( plhs[name##_n], icur, "rect", mxFrect ); \
				/* type */ \
				type = ( CV_IS_SEQ_HOLE( seq[icur] ) ) ? 1 : 0; \
				mxFtype = mxCreateNumericMatrix( \
					1, 1, mxDOUBLE_CLASS, mxREAL ); \
				typeData = (double *)mxGetData( mxFtype ); \
				castCopy( &type, typeData, 1, 1, 1, 1, 1, 1 ); \
				mxSetField( plhs[name##_n], icur, "type", mxFtype ); \
				/* h_prev */ \
				mxFh_prev = mxCreateNumericMatrix( \
					1, 1, mxDOUBLE_CLASS, mxREAL ); \
				h_prevData = (double *)mxGetData( mxFh_prev ); \
				castCopy( &(h_prev[icur]), h_prevData, \
					1, 1, 1, 1, 1, 1 ); \
				mxSetField( plhs[name##_n], icur, "h_prev", mxFh_prev ); \
				/* h_next */ \
				mxFh_next = mxCreateNumericMatrix( \
					1, 1, mxDOUBLE_CLASS, mxREAL ); \
				h_nextData = (double *)mxGetData( mxFh_next ); \
				castCopy( &(h_next[icur]), h_nextData, \
					1, 1, 1, 1, 1, 1 ); \
				mxSetField( plhs[name##_n], icur, "h_next", mxFh_next ); \
				/* v_prev */ \
				mxFv_prev = mxCreateNumericMatrix( \
					1, 1, mxDOUBLE_CLASS, mxREAL ); \
				v_prevData = (double *)mxGetData( mxFv_prev ); \
				castCopy( &(v_prev[icur]), v_prevData, \
					1, 1, 1, 1, 1, 1 ); \
				mxSetField( plhs[name##_n], icur, "v_prev", mxFv_prev ); \
				/* v_next */ \
				mxFv_next = mxCreateNumericMatrix( \
					1, 1, mxDOUBLE_CLASS, mxREAL ); \
				v_nextData = (double *)mxGetData( mxFv_next ); \
				castCopy( &(v_next[icur]), v_nextData, \
					1, 1, 1, 1, 1, 1 ); \
				mxSetField( plhs[name##_n], icur, "v_next", mxFv_next ); \
			} \
		} /* if name##_num > 0 */ \
		, \
		name )
/* end PUT_SEQ( ... ) */
// }} Macros for sequences

// {{ Macros for structuring element
#define STRUCTELEM_IN( n1, n2, name, cond ) \
	int            name##_nCols      = 0; \
	int            name##_nRows      = 0; \
	int            name##_anchorX    = 0; \
	int            name##_anchorY    = 0; \
	CvElementShape name##_shape      = CV_SHAPE_CUSTOM; \
	int           *_name##_values    = 0; \
	bool           name##_allowEmpty = false; \
	 \
	CStructElem    name; \
	 \
	cond; \
	 \
	_ENHANCE_ERROR( \
		MATRIX_IN( n1, int, name##Values, -1, -1, \
			PARAM( name##Values, transpose )  = false; \
			PARAM( name##Values, allowEmpty ) = name##_allowEmpty; ); \
		if( (int *)name##Values ) \
		{ \
			/* not empty */ \
			if( PARAM( name##Values, nRows ) == 1 && \
				PARAM( name##Values, nCols ) == 1 ) \
			{ \
				/* predefined kernel */ \
				name##_shape = (CvElementShape)(int)name##Values[0]; \
				VECTOR_IN( n2, int, name##Size, 4, ; ); \
				name##_nCols   = name##Size[0]; \
				name##_nRows   = name##Size[1]; \
				name##_anchorX = name##Size[2] - 1; \
				name##_anchorY = name##Size[3] - 1; \
			} \
			else \
			{ \
				/* user-defined kernel */ \
				name##_nCols   = PARAM( name##Values, nRows ); \
				name##_nRows   = PARAM( name##Values, nCols ); \
				VECTOR_IN( n2, int, name##Size, 2, ; ); \
				name##_anchorX = name##Size[0] - 1; \
				name##_anchorY = name##Size[1] - 1; \
				_name##_values = (int *)name##Values; \
			} \
			name.create( name##_nRows, name##_nCols, \
				name##_anchorY, name##_anchorX, name##_shape, _name##_values );\
		} \
		, \
		name )
// }} Macros for structuring element

// {{ Macros for matrix
#define MATRIX_IN( n, type, name, nRows, nCols, cond ) \
	int  name##_nRows      = nRows;  \
	int  name##_nCols      = nCols;  \
	bool name##_anyClass   = true;  \
	bool name##_allowEmpty = false; \
	bool name##_inPlace    = false; /* use matlab data directly */ \
	bool name##_transpose  = true; \
	mxClassID name##_class = mxDOUBLE_CLASS; \
	 \
	CArray< type > name; \
	\
	cond; \
	\
	_ENHANCE_ERROR( \
		verifyMatrix( prhs[n], name##_nRows, name##_nCols, name##_allowEmpty, \
			name##_anyClass, name##_class ); \
		 \
		if( !mxIsEmpty( prhs[n] ) ) \
		{ \
			if( !name##_inPlace ) \
			{ \
				name.create( name##_nRows * name##_nCols ); \
				MXTYPE_DATAPTR( name##_class, ;,\
					_srcData, mxGetData( prhs[n] ); \
					if( name##_transpose ) \
					{ \
						castCopy( _srcData, (type *)name, \
							name##_nRows, name##_nCols, 1, name##_nRows, \
							name##_nCols, 1 ); \
					} \
					else \
					{ \
						castCopy( _srcData, (type *)name, \
							1, name##_nRows * name##_nCols, 1, 1, \
							1, 1 ); \
					} \
					); \
			} \
			else \
			{ \
				name.set( (type *)mxGetData( prhs[n]) ); \
			} \
		} \
		, \
		name ) 

// }} Macros for matrix

#endif // CV_WRAP_MACRO
