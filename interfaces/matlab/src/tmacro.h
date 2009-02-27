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

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _TMACRO_H_
#define _TMACRO_H_

template< class T >
class CArray
{
	public:
		CArray() : data( 0 ), bReleaseData( false ) {}
		~CArray()
		{
			if( bReleaseData && data )
			{
				delete[] data;
				data = 0;
			}
		}

		// create data array of size _num
		void create( int _num = 0 )
		{
			assert( !data );
			if( _num > 0 )
			{
				data = new T[_num];
			}
			bReleaseData = true;
		}

		// Set external data
		void set( T *_data )
		{
			assert( !data );
			data = _data;
			bReleaseData = false;
		}

		operator T *() { return( data ); }
		T *getData() { return( data ); }
		operator T() { return( data[0] ); }

		T operator=( T val ) { data[0] = val; return( val ); }
		
		T &operator[]( int index ) { return( data[index] ); }

	protected:
		T *data;
		bool bReleaseData;
		
		// prevent from copying
		CArray( const CArray &arr );
		CArray &operator=( const CArray &arr );
};

class CImage
{
	public:
		CImage() : header( 0 ), bReleaseData( false ) {}
		~CImage()
		{
			if( header )
			{
				if( bReleaseData )
				{
					cvReleaseData( header );
					bReleaseData = false;
				}
				cvReleaseImageHeader( & header );
				header = 0;
			}
		}
		
		void  createHeader( CvSize size, int depth, int channels )
		{
			assert( !header );
			header = cvCreateImageHeader( size, depth, channels );
		}
		
		// Set external data
		void setData( void *data, int step )
		{
			assert( header );
			assert( !bReleaseData );
			cvSetData( header, data, step );
		}

		// Create own data. It will be automatically released
		void createData()
		{
			assert( header );
			assert( !bReleaseData );
			cvCreateData( header );
			bReleaseData = true;
		}
		operator IplImage *() { return( header ); }

	private:
		IplImage *header;
		bool bReleaseData;

		// prevent from copying
		CImage( const CImage &arr );
		CImage &operator=( const CImage &arr );
};

class CStorage
{
	public:
		CStorage() { storage = cvCreateMemStorage(); }
		~CStorage() { cvReleaseMemStorage( &storage ); }
		operator CvMemStorage *() { return( storage ); }
	private:
		CvMemStorage *storage;

		// prevent from copying
		CStorage( const CStorage &obj );
		CStorage &operator=( const CStorage &obj );
};

class CStructElem
{
	public:
		CStructElem() : structElem( 0 ) {}
		~CStructElem()
		{
			if( structElem )
			{
				cvReleaseStructuringElement( &structElem );
				structElem = 0;
			}
		}
		
		void create( int cols, int rows, int anchorX, int anchorY, 
			CvElementShape shape, int *values = 0 )
		{
			assert( !structElem );
			structElem = cvCreateStructuringElementEx( cols, rows, 
				anchorX, anchorY, shape, values );
		}

		operator IplConvKernel *() { return( structElem ); }
	private:
		IplConvKernel *structElem;

		// prevent from copying
		CStructElem( const CStructElem &obj );
		CStructElem &operator=( const CStructElem &obj );
};

// castCopy
template< class T1, class T2 >
void castCopy( T1 *src, T2 *dst, int rowNum, int colNum,
 			   int srcRowStep, int srcColStep,
			   int dstRowStep, int dstColStep )
{
	for( int r = 0; r < rowNum; r++ )
	{
		for( int c = 0; c < colNum; c++ )
		{
			dst[r * dstRowStep + c * dstColStep] = 
				static_cast< T2 > ( src[r * srcRowStep + c * srcColStep] );
		}
	}
}

#define MXTYPE_DATAPTR( type, part1, ptrName, part2 ) \
	switch(	type ) \
	{ \
		case mxDOUBLE_CLASS: \
		{ \
			part1; \
			double *ptrName = (double *)part2; \
			break; \
		} \
		case mxSINGLE_CLASS: \
		{ \
			part1; \
			float *ptrName = (float *)part2; \
			break; \
		} \
		case mxINT32_CLASS: \
		{ \
			part1; \
			signed int *ptrName = (signed int *)part2; \
			break; \
		} \
		case mxUINT32_CLASS: \
		{ \
			part1; \
			unsigned int *ptrName = (unsigned int *)part2; \
			break; \
		} \
		case mxINT16_CLASS: \
		{ \
			part1; \
			signed short *ptrName = (signed short *)part2; \
			break; \
		} \
		case mxUINT16_CLASS: \
		{ \
			part1; \
			unsigned short *ptrName = (unsigned short *)part2; \
			break; \
		} \
		case mxINT8_CLASS: \
		{ \
			part1; \
			signed char *ptrName = (signed char *)part2; \
			break; \
		} \
		case mxUINT8_CLASS: \
		{ \
			part1; \
			unsigned char *ptrName = (unsigned char *)part2; \
			break; \
		} \
		default: \
			throw_error( "Parameter should be numeric." ); \
			break; \
	} 

// Execute callfun. If an error occured, add to the error
// #name and rethrow it.
#define _ENHANCE_ERROR( callfun, name ) \
	try \
	{ \
		callfun; \
	} \
	catch( Error _e ) \
	{ \
		std::ostringstream ostr; \
		ostr << _e.get_error_messages() << " Parameter: " << #name; \
		throw_error( ostr.str() ); \
	}

#define _CREATE_MATRIX( n, rows, cols, class ) \
	plhs[n] = mxCreateNumericMatrix( rows, cols, class, mxREAL );

#define _CREATE_VECTOR( name ) \
	int _m = ( name##_vecType == any || name##_vecType == row ) ? \
		1 : name##_num; \
	int _n = ( name##_vecType == any || name##_vecType == row ) ? \
		name##_num : 1; \
	_CREATE_MATRIX( name##_n, _m, _n, name##_class ); 

#define _CREATE_IMAGE( name ) \
	if( name##_channels == 3 ) \
	{ \
		const int ndim = 3; \
		int dims[ndim]; \
		dims[0] = name##_size.width; \
		dims[1] = name##_size.height; \
		dims[2] = name##_channels; \
		plhs[name##_n] = \
			mxCreateNumericArray(ndim, dims, name##_class, mxREAL );\
	} \
	else \
	{ \
		_CREATE_MATRIX( name##_n, name##_size.width, name##_size.height, \
		    name##_class ); \
	}

/* Macro for convert functions of certain type */
#define FUNC_CONVERT( type, \
	getDecl, getParam, getAssign, \
	putDecl, putAssign, putParam ) \
	 \
	template< class T1 > \
	void convert( T1 *src, type *dst, int num ) \
	{ \
		\
		getDecl; \
		\
		int nelem = 0; \
		for( nelem = 0; nelem < num; nelem++ ) \
		{ \
			castCopy( src+nelem, getParam, 1, type##NUMFIELDS, 1, num, 1, 1 ); \
			\
			getAssign; \
			\
		} /* for each elem in arr */ \
	} \
	 \
	template< class T2 > \
	void convert( type *src, T2 *dst, int num ) \
	{ \
		\
		putDecl; \
		\
		int nelem = 0; \
		for( nelem = 0; nelem < num; nelem++ ) \
		{ \
			\
			putAssign; \
			\
			castCopy( putParam, dst+nelem, 1, type##NUMFIELDS, 1, 1, 1, num ); \
		} /* for each elem in arr */ \
	}

// type CvPoint
#define CvPointNUMFIELDS 2
FUNC_CONVERT( CvPoint,
			  ;,
			  (int *)(dst+nelem),
			  ;,
			  ;,
			  ;,
			  (int *)(src + nelem) )

// type CvPoint0S
// Convert from 1-based to 0-based and swap coordinates 
struct CvPoint0S : public CvPoint {};
#define CvPoint0SNUMFIELDS 2
FUNC_CONVERT( CvPoint0S,
			  int _tmp,
			  (int *)(dst+nelem),
			  _tmp = dst[nelem].x - 1; dst[nelem].x = dst[nelem].y - 1;
			  dst[nelem].y = _tmp;,
			  CvPoint point,
			  point.x = src[nelem].y + 1; point.y = src[nelem].x + 1;,
			  (int *)&point )

// type CvSizeS
// Swap coordinates 
struct CvSizeS : public CvSize {};
#define CvSizeSNUMFIELDS 2
FUNC_CONVERT( CvSizeS,
			  int _tmp,
			  (int *)(dst+nelem),
			  _tmp = dst[nelem].width; dst[nelem].width = dst[nelem].height;
			  dst[nelem].height = _tmp;,
			  CvSize elem,
			  elem.width = src[nelem].height; elem.height = src[nelem].width;,
			  (int *)&elem )

// type CvSlice0
// Convert from 1-based to 0-based
struct CvSlice0 : public CvSlice {};
#define CvSlice0NUMFIELDS 2
FUNC_CONVERT( CvSlice0,
			  ;,
			  (int *)(dst+nelem),
			  dst[nelem].startIndex = ( dst[nelem].startIndex > 0 ) ?
				  dst[nelem].startIndex - 1 : dst[nelem].startIndex;
			  dst[nelem].endIndex = ( dst[nelem].endIndex > 0 ) ?
				  dst[nelem].endIndex - 1 : dst[nelem].endIndex;,
			  CvSlice slice,
			  slice.startIndex = src[nelem].startIndex + 
				  ( src[nelem].startIndex >= 0 ) ? 1 : 0;
			  slice.endIndex = src[nelem].endIndex + 
				  ( src[nelem].endIndex >= 0 ) ? 1 : 0;,
			  (int *)&slice )

// type CvPoint2D32f
#define CvPoint2D32fNUMFIELDS 2
FUNC_CONVERT( CvPoint2D32f,
			  ;,
			  (float *)(dst+nelem),
			  ;,
			  ;,
			  ;,
			  (float *)(src + nelem) )

// type CvPoint2D32fS
// Swap coordinates 
struct CvPoint2D32fS : public CvPoint2D32f {};
#define CvPoint2D32fSNUMFIELDS 2
FUNC_CONVERT( CvPoint2D32fS,
			  float _tmp,
			  (float *)(dst+nelem),
			  _tmp = dst[nelem].x; dst[nelem].x = dst[nelem].y;
			  dst[nelem].y = _tmp;,
			  CvPoint2D32f point,
			  point.x = src[nelem].y; point.y = src[nelem].x;,
			  (float *)&point )

// type CvRect
#define CvRectNUMFIELDS 4
FUNC_CONVERT( CvRect,
			  ;,
			  (int *)(dst+nelem),
			  ;,
			  ;,
			  ;,
			  (int *)(src+nelem) )

// type CvRect0S
// Convert from 1-based to 0-based and swap coordinates 
struct CvRect0S : public CvRect {};
#define CvRect0SNUMFIELDS 4
FUNC_CONVERT( CvRect0S,
			  int _tmp,
			  (int *)(dst+nelem),
			  _tmp = dst[nelem].x - 1; dst[nelem].x = dst[nelem].y - 1;
			  dst[nelem].y = _tmp;
			  _tmp = dst[nelem].width - 1;
			  dst[nelem].width = dst[nelem].height - 1;
			  dst[nelem].height = _tmp;,
			  CvRect elem,
			  elem.x = src[nelem].y + 1; elem.y = src[nelem].x + 1;
			  elem.width = src[nelem].height;
			  elem.height = src[nelem].width;,
			  (int *)&elem )

// type CvTermCriteria
#define CvTermCriteriaNUMFIELDS 2
FUNC_CONVERT( CvTermCriteria,
			  double elem[2];,
			  (double *)elem,
			  dst[nelem].type = 0;
			  dst[nelem].maxIter = 0;
			  dst[nelem].epsilon = 0.0f;
			  if( elem[0] > 0.0 )
			  {
				  dst[nelem].type |= CV_TERMCRIT_ITER;
				  dst[nelem].maxIter = static_cast< int > ( elem[0] );
			  }
			  if( elem[1] > 0.0 )
			  {
				  dst[nelem].type |= CV_TERMCRIT_EPS;
				  dst[nelem].epsilon = static_cast< float > ( elem[1] );
			  },
			  double elem[2];,
			  elem[0] = ( ( src[nelem].type & CV_TERMCRIT_ITER ) ? 
			  static_cast< double > ( src[nelem].maxIter ) : 0.0 );
			  elem[1] = ( ( src[nelem].type & CV_TERMCRIT_EPS ) ? 
			  static_cast< double > ( src[nelem].epsilon ) : 0.0 );,
			  (double *)elem )

#endif /* _TMACRO_H_ */