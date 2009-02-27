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

#define CV_MCALL( func ) func; \
	if( cvGetErrStatus() != CV_StsOk ) \
	{ \
		OPENCV_RSTERR(); \
		throw_error( cvErrorMessage ); \
	}

///////////////////////////////////////////////////////////////////////////////
// IMAGE = dllname('PyrUp', IMAGE, NUMERIC filter_type);
//
WRAPPER (3, 1, wrPyrUp);
IMG_IN( 1,      src, ; );
NUM_IN( 2, int, filter );

IMG_OUT( 0, dst, 
	PARAM( dst, size ).width  = PARAM( src, size ).width  * 2;
	PARAM( dst, size ).height = PARAM( src, size ).height * 2;
	PARAM( dst, channels )    = PARAM( src, channels );
	PARAM( dst, class )       = PARAM( src, class ); );

CV_MCALL( cvPyrUp( (IplImage *)src, (IplImage *)dst, (CvFilter)filter ) );
PUT_IMG( dst );
RET
		   
///////////////////////////////////////////////////////////////////////////////
// IMAGE = dllname('PyrDown', IMAGE, NUMERIC filter_type);
//
WRAPPER (3, 1, wrPyrDown);	
IMG_IN( 1,      src, ; );
NUM_IN( 2, int, filter );

IMG_OUT( 0, dst, 
	PARAM( dst, size ).width  = PARAM( src, size ).width  / 2;
	PARAM( dst, size ).height = PARAM( src, size ).height / 2;
	PARAM( dst, channels ) = PARAM( src, channels );
	PARAM( dst, class )       = PARAM( src, class ); );

CV_MCALL( cvPyrDown( (IplImage *)src, (IplImage *)dst, (CvFilter)filter ) );
PUT_IMG( dst );
RET

///////////////////////////////////////////////////////////////////////////////
// IMAGE = dllname('Laplace', IMAGE, NUMERIC);
//
WRAPPER (3, 1, wrLaplace);	
IMG_IN( 1,      src, ; );
NUM_IN( 2, int, apertureSize );

IMG_OUT( 0, dst, 
	PARAM( dst, size )     = PARAM( src, size );
	PARAM( dst, channels ) = PARAM( src, channels );
	PARAM( dst, class )    = (( PARAM( src, class ) == mxSINGLE_CLASS ) ? 
		mxSINGLE_CLASS : mxINT16_CLASS); );

CV_MCALL( cvLaplace((IplImage *) src, (IplImage *) dst, apertureSize) );

PUT_IMG( dst );
RET

///////////////////////////////////////////////////////////////////////////////
// IMAGE = dllname('Sobel', IMAGE, NUMERIC, NUMERIC, NUMERIC);
//
WRAPPER (5, 1, wrSobel);	
IMG_IN( 1,      src, ; );
NUM_IN( 2, int, xorder );
NUM_IN( 3, int, yorder );
NUM_IN( 4, int, apertureSize );

IMG_OUT( 0, dst, 
	PARAM( dst, size )     = PARAM( src, size );
	PARAM( dst, channels ) = PARAM( src, channels );
	PARAM( dst, class )    = (( PARAM( src, class ) == mxSINGLE_CLASS ) ? 
		mxSINGLE_CLASS : mxINT16_CLASS); );

CV_MCALL( cvSobel( (IplImage *)src, (IplImage *)dst,
		 yorder, xorder, apertureSize ) );
PUT_IMG( dst );
RET

///////////////////////////////////////////////////////////////////////////////
// IMAGE = dllname( 'Erode', IMAGE src, 
//                  elementValues,// one of the predefined
//                                // structuring elements or
//                                // matrix for user-defined structuring element
//                  elementSize,  // [nRows, nCols, anchorRow, anchorCol] for
//                                // the predefined element or
//                                // [anchorRow, anchorCol] for
//                                // the user-defined element
//                  iterations );
//
WRAPPER( 5, 1, wrErode );
IMG_IN( 1, src, ; );
STRUCTELEM_IN( 2, 3, element, PARAM( element, allowEmpty ) = true; );
NUM_IN( 4, int, iterations );

IMG_OUT( 0, dst, PARAM( dst, size )     = PARAM( src, size );
				 PARAM( dst, channels ) = PARAM( src, channels );
				 PARAM( dst, class )    = PARAM( src, class ); );
CV_MCALL( cvErode( (IplImage *)src, (IplImage *)dst, (IplConvKernel *)element,
		 iterations ) );
PUT_IMG( dst );
RET

///////////////////////////////////////////////////////////////////////////////
// IMAGE = dllname( 'Dilate', IMAGE src, 
//                  elementValues,// one of the predefined
//                                // structuring elements or
//                                // matrix for user-defined structuring element
//                  elementSize,  // [nRows, nCols, anchorRow, anchorCol] for
//                                // the predefined element or
//                                // [anchorRow, anchorCol] for
//                                // the user-defined element
//                  iterations );
//
WRAPPER( 5, 1, wrDilate );	
IMG_IN( 1, src, ; );
STRUCTELEM_IN( 2, 3, element, PARAM( element, allowEmpty ) = true; );
NUM_IN( 4, int, iterations );

IMG_OUT( 0, dst, PARAM( dst, size )     = PARAM( src, size );
				 PARAM( dst, channels ) = PARAM( src, channels );
				 PARAM( dst, class )    = PARAM( src, class ); );
CV_MCALL( cvDilate( (IplImage *)src, (IplImage *)dst, (IplConvKernel *)element,
		  iterations ) );
PUT_IMG( dst );
RET

///////////////////////////////////////////////////////////////////////////////
// IMAGE dst = dllname( 'DistTransform', IMAGE src, 
//                   dist,    // [disType] or 
//                            // 1 by 2 (3) array of numbers that characterize
//                            // user-defined metric
//                   maskSize // ignored if metric is user-defined
//                );
//
WRAPPER (4, 1, wrDistTransform);	
IMG_IN(    1,        src, ; );
VECTOR_IN( 2, float, dist, -1, ; );

int        _maskSize = 0;
CvDisType disType  = CV_DIST_USER;
float     *mask     = 0;

if( PARAM( dist, num ) == 1 )
{
	// predefined metric
	NUM_IN( 3, int, maskSize );
	_maskSize = maskSize;
	disType  = (CvDisType)(int)dist[0];
}
else
{
	// user-defined metric
	switch( PARAM( dist, num ) )
	{
		case 2: { _maskSize = 3; break; }
		case 3: { _maskSize = 5; break; }
		default: { throw_error( "Invalid parameter 'dist'." ); }
	}
	mask     = (float *)dist;
}

IMG_OUT( 0, dst, PARAM( dst, size )     = PARAM( src, size );
				 PARAM( dst, channels ) = PARAM( src, channels );
				 PARAM( dst, class )    = mxSINGLE_CLASS );
CV_MCALL( cvDistTransform( (IplImage *)src, (IplImage *)dst,
				 disType, _maskSize, mask ) );
PUT_IMG( dst );
RET

///////////////////////////////////////////////////////////////////////////////
// IMAGE dst = dllname('Canny', IMAGE 8U src, DOUBLE low_thresh,
//		   DOUBLE high_thresh, INT aperture);
//
WRAPPER (5, 1, wrCanny);
IMG_IN( 1,         src, ; );
NUM_IN( 2, double, lowThreshold );
NUM_IN( 3, double, highThreshold );
NUM_IN( 4, int,    apertureSize );

IMG_OUT( 0, dst, PARAM( dst, size )     = PARAM( src, size );
				 PARAM( dst, channels ) = PARAM( src, channels );
				 PARAM( dst, class )    = PARAM( src, class ); );

CV_MCALL( cvCanny( (IplImage *)src, (IplImage *)dst, 
		 lowThreshold, highThreshold, apertureSize ) );
PUT_IMG( dst );
RET

///////////////////////////////////////////////////////////////////////////////
// IMAGE dst = dllname('PreCornerDetect', IMAGE 32F src, INT aperture);
//
WRAPPER (3, 1, wrPreCornerDetect);
IMG_IN( 1,      src, ; );
NUM_IN( 2, int, apertureSize );

IMG_OUT( 0, dst, PARAM( dst, size )     = PARAM( src, size );
				 PARAM( dst, channels ) = PARAM( src, channels );
				 PARAM( dst, class )    = mxSINGLE_CLASS );

CV_MCALL( cvPreCornerDetect( (IplImage *)src, (IplImage *)dst, apertureSize ) );
PUT_IMG( dst );
RET

///////////////////////////////////////////////////////////////////////////////
// IMAGE result = dllname( 'MatchTemplate', 
//                         IMAGE array, IMAGE templ, ENUM method );
//
WRAPPER (4, 1, wrMatchTemplate);
IMG_IN( 1,      array, ; );
IMG_IN( 2,      templ, ; );
NUM_IN( 3, int, method );

IMG_OUT( 0, result,
	PARAM( result, size ).width = 
		PARAM( array, size ).width - PARAM( templ, size ).width + 1;
	PARAM( result, size ).height = 
		PARAM( array, size ).height - PARAM( templ, size ).height + 1;
	PARAM( result, channels ) = PARAM( array, channels );
	PARAM( result, class )    = mxSINGLE_CLASS; );

CV_MCALL( cvMatchTemplate( (IplImage *)array, (IplImage *)templ,
		 (IplImage *)result, (CvTemplMatchMethod)method ) );

PUT_IMG( result );
RET

///////////////////////////////////////////////////////////////////////////////
// IMAGE mhi = dllname( 'UpdateMotionHistory', IMAGE silhouette,
//                      DOUBLE timestamp,
//			            DOUBLE mhiduration );
//
WRAPPER (4, 1, wrUpdateMotionHistory);
IMG_IN( 1,         silhouette, ; );
NUM_IN( 2, double, timestamp );
NUM_IN( 3, double, mhiDuration );

IMG_OUT( 0, mhi, PARAM( mhi, size )     = PARAM( silhouette, size );
				 PARAM( mhi, channels ) = PARAM( silhouette, channels );
				 PARAM( mhi, class )    = mxSINGLE_CLASS; );

CV_MCALL( cvUpdateMotionHistory( (IplImage *)silhouette, (IplImage *)mhi, 
					   timestamp, mhiDuration ) );
PUT_IMG( mhi );
RET

///////////////////////////////////////////////////////////////////////////////
// [min, max, [Xmin Ymin], [Xmax, Ymax]]=
//			dllname( 'MinMaxLoc', IMAGE src, IMAGE mask );
//
WRAPPER (3, 4, wrMinMaxLoc);
IMG_IN( 1, src, ; );
IMG_IN( 2, mask, PARAM( mask, allowEmpty ) = true; );

NUM_OUT( 0, double, minVal, ; );
NUM_OUT( 1, double, maxVal, ; );
ARRAY_OUT( 2, CvPoint0S, minLoc, 1, ; );
ARRAY_OUT( 3, CvPoint0S, maxLoc, 1, ; );

CV_MCALL( cvMinMaxLoc( (IplImage *)src, &minVal, &maxVal,
			 (CvPoint *)minLoc, (CvPoint *)maxLoc, (IplImage *)mask ) );

PUT_NUM( minVal );
PUT_NUM( maxVal );
PUT_ARRAY( minLoc );
PUT_ARRAY( maxLoc );
RET

///////////////////////////////////////////////////////////////////////////////
// IMAGE dst = dllname( 'Threshold', IMAGE src, DOUBLE thresh, DOUBLE maxValue,
//     ENUM type );
//
WRAPPER( 5, 1, wrThreshold );
IMG_IN( 1,         src, ; );
NUM_IN( 2, double, thresh );
NUM_IN( 3, double, maxValue );
NUM_IN( 4, int,    type );

IMG_OUT( 0, dst, PARAM( dst, size )     = PARAM( src, size );
				 PARAM( dst, channels ) = PARAM( src, channels );
				 PARAM( dst, class )    = PARAM( src, class ); );

CV_MCALL( cvThreshold( (IplImage *)src, (IplImage *)dst, thresh, maxValue, 
		 (CvThreshType)(type) ) );

PUT_IMG( dst );
RET

///////////////////////////////////////////////////////////////////////////////
// IMAGE dst = dllname( 'AdaptiveThreshold',
//                 IMAGE src, 
//                 maxValue,
//                 method,
//                 type,
//                 parameters );
//
WRAPPER (6, 1, wrAdaptiveThreshold);	
IMG_IN( 1,         src, ; );
NUM_IN( 2, double, maxValue );
NUM_IN( 3, int,    method );
NUM_IN( 4, int,    type );
NUM_IN( 5, int,    blockSize );
NUM_IN( 6, double, param1 );

IMG_OUT( 0, dst, PARAM( dst, size )     = PARAM( src, size );
				 PARAM( dst, channels ) = PARAM( src, channels );
				 PARAM( dst, class )    = PARAM( src, class ); );

CV_MCALL( cvAdaptiveThreshold( (IplImage *)src, (IplImage *)dst, 
		     maxValue, (CvAdaptiveThreshMethod)method, 
			 (CvThreshType)type, blockSize, param1 ));
RET

///////////////////////////////////////////////////////////////////////////////
// SEQARRAY dst = dllname( 'FindContours', IMAGE src, ENUM mode, ENUM method );
//
WRAPPER( 4, 1, wrFindContours );
IMG_IN(  1,      src,    ; );
NUM_IN(  2, int, mode );
NUM_IN(  3, int, method );

SEQ_OUT( 0, dst, ; );

TEMP_IMAGE( tmpImg, PARAM( src, size ), 
					PARAM( src, depth ), PARAM( src, channels ) );
cvCopy( (IplImage *)src, (IplImage *)tmpImg );
PARAM( dst, num ) = CV_MCALL( cvFindContours( (IplImage *)tmpImg,
								 PARAM( dst, storage ),
								 (CvSeq **)&dst, PARAM( dst, headerSize ),
								 (CvContourRetrievalMode)(mode),
								 (CvChainApproxMethod)(method) ) );
PUT_SEQ( dst, CvPoint0S, CvRect0S );
RET

///////////////////////////////////////////////////////////////////////////////
// IMAGE dst = dllname( 'DrawContours',
//     IMAGE img,            // image
//     SEQARRAY seq,         // array of sequences
//	   INT first,            // index of the first sequence
//     DOUBLE externalColor 
//     DOUBLE holeColor
//     INT maxLevel,
//     INT thickness,
//     INT connectivity );
//
WRAPPER( 9, 1, wrDrawContours );
IMG_IN( 1,         img, ; );
NUM_IN( 3, int,    first );
SEQ_IN( 2, first,  seq, CvPoint0S, CvRect0S, PARAM( seq, allowEmpty ) = true; );
NUM_IN( 4, double, externalColor );
NUM_IN( 5, double, holeColor );
NUM_IN( 6, int,    maxLevel );
NUM_IN( 7, int,    thickness );
NUM_IN( 8, int,    connectivity );

IMG_OUT( 0, dst, PARAM( dst, size )     = PARAM( img, size );
				 PARAM( dst, channels ) = PARAM( img, channels );
				 PARAM( dst, class )    = PARAM( img, class ); );

cvCopy( (IplImage *)img, (IplImage *)dst );
if( (CvSeq *)seq )
{
	CV_MCALL( cvDrawContours( (IplImage *)dst, (CvSeq *)seq, 
				 externalColor, holeColor, maxLevel, thickness,
				 connectivity ) );
}

PUT_IMG( dst );
RET

///////////////////////////////////////////////////////////////////////////////
// SEQARRAY dst = dllname( 'ApproxPoly',
//     SEQARRAY seqArray,    // array of sequences
//	   INT first,            // index of the first sequence
//     ENUM method,          // method
//     DOUBLE parameter,     // parameter
//     INT recursive );
//
WRAPPER( 6, 1, wrApproxPoly );
NUM_IN( 2, int,    first );
SEQ_IN( 1, first,  seq, CvPoint, CvRect, PARAM( seq, allowEmpty ) = true; );
NUM_IN( 3, int,    method );
NUM_IN( 4, double, parameter );
NUM_IN( 5, int,    recursive );

SEQ_OUT( 0, dst, ; );

if( (CvSeq *)seq )
{
	PARAM( dst, num ) = PARAM( seq, num );
	dst = (CvContour *)CV_MCALL( cvApproxPoly( (CvSeq *)seq,
		PARAM( seq, headerSize ),
		PARAM( seq, storage ), (CvPolyApproxMethod)(method),
		parameter, recursive ) );
}
PUT_SEQ( dst, CvPoint, CvRect );
RET

///////////////////////////////////////////////////////////////////////////////
// perimeter = dllname( 'ContourPerimeter',
//     points,    // n by 2 array of points
//	   slice );   // [firstIdx lastIdx]
//
WRAPPER( 3, 1, wrContourPerimeter );
ARRAY_IN( 1, CvPoint,  points, -1, ; );
ARRAY_IN( 2, CvSlice0, slice,   1, ; );

NUM_OUT(  0, double,   perimeter,  ; );

CvSeq seq;
CvSeqBlock block;
cvMakeSeqHeaderForArray( CV_SEQ_CONTOUR, sizeof( seq ),
  sizeof( CvPoint ),
  (CvArr *)((CvPoint *)points), PARAM( points, num ), &seq, &block );
perimeter = CV_MCALL( cvContourPerimeter(&seq) );

PUT_NUM( perimeter );
RET

///////////////////////////////////////////////////////////////////////////////
// area = dllname( 'ContourArea',
//     points,    // n by 2 array of points
//	   slice );   // [firstIdx lastIdx]
//
WRAPPER( 3, 1, wrContourArea );
ARRAY_IN( 1, CvPoint,  points, -1, ; );
ARRAY_IN( 2, CvSlice0, slice,   1, ; );

NUM_OUT(  0, double,   area,  ; );

CvSeq seq;
CvSeqBlock block;
cvMakeSeqHeaderForArray( CV_SEQ_CONTOUR, sizeof( seq ),
  sizeof( CvPoint ),
  (CvArr *)((CvPoint *)points), PARAM( points, num ), &seq, &block );
area = CV_MCALL( cvContourArea( &seq, *((CvSlice *)slice) ) );

PUT_NUM( area );
RET

///////////////////////////////////////////////////////////////////////////////
// [IMAGE dst, DOUBLE area, DOUBLE value, [x y width height]] = 
//   dllname( 'FloodFill',
//            IMAGE src,
//            seedPoint,
//            DOUBLE newVal,
//            DOUBLE loDiff,
//            DOUBLE upDiff,
//            INT connectivity );
//
WRAPPER( 7, 4, wrFloodFill );
IMG_IN(    1,            src, ; );
ARRAY_IN(  2, CvPoint0S, seedPoint, 1, ; );
NUM_IN(    3, double,    newVal );
NUM_IN(    4, double,    loDiff );
NUM_IN(    5, double,    upDiff );
NUM_IN(    6, int,       connectivity );

IMG_OUT(   0,            dst, PARAM( dst, size )     = PARAM( src, size );
							  PARAM( dst, channels ) = PARAM( src, channels );
							  PARAM( dst, class )    = PARAM( src, class ); );
NUM_OUT(   1, double,    area, ; );
NUM_OUT(   2, double,    value, ; );
ARRAY_OUT( 3, CvRect0S,  rect, 1, ; );

cvCopy( (IplImage *)src, (IplImage *)dst );
CvConnectedComp comp;
CV_MCALL( cvFloodFill( (IplImage *)dst, *((CvPoint *)seedPoint), 
	newVal, loDiff, upDiff,
	&comp, connectivity ) );
area               = (double)comp.area;
value              = (double)comp.value;
*((CvRect *)rect)  = (CvRect)comp.rect;

PUT_IMG(   dst   );
PUT_NUM(   area  );
PUT_NUM(   value );
PUT_ARRAY( rect  );
RET

///////////////////////////////////////////////////////////////////////////////
// [featuresB, error] = 
//   dllname( 'CalcOpticalFlowPyrLK',
//            IMAGE imgA, IMAGE imgB, featuresA, guessFeaturesB,
//            winSize, INT level,
//            [maxIter epsilon] );
//
WRAPPER( 8, 2, wrCalcOpticalFlowPyrLK );
IMG_IN(   1,                 imgA, ; );
IMG_IN(   2,                 imgB, ; );
ARRAY_IN( 3, CvPoint2D32fS,  featuresA, -1, ; );
ARRAY_IN( 4, CvPoint2D32fS,  guessFeaturesB, -1,
								 PARAM( guessFeaturesB, allowEmpty ) = true; );
ARRAY_IN( 5, CvSizeS,        winSize, 1, ; );
NUM_IN(   6, int,            level );
ARRAY_IN( 7, CvTermCriteria, criteria, 1, ; );

ARRAY_OUT(  0, CvPoint2D32fS, featuresB, PARAM( featuresA, num), ; );
VECTOR_OUT( 1, float,         error,     PARAM( featuresA, num ),
								 PARAM( error, vecType ) = col; );

int flags = 0;
TEMP_ARRAY( char, status, PARAM( featuresA, num) );
if( PARAM( featuresA, num) == PARAM( guessFeaturesB, num) )
{
   // initial guessess
   memcpy( (void *)((CvPoint2D32f *)featuresB),
	   (void *)((CvPoint2D32f *)guessFeaturesB),
	   sizeof( CvPoint2D32f ) * PARAM( featuresA, num) );
   flags |= CV_LKFLOW_INITIAL_GUESSES;
}
CV_MCALL( cvCalcOpticalFlowPyrLK(
   (IplImage *)imgA, (IplImage *)imgB,
   0, 0,
   (CvPoint2D32f *)featuresA, (CvPoint2D32f *)featuresB,
   PARAM( featuresA, num),
   *((CvSize *)winSize), level,
   (char *)status, (float *)error,
   *((CvTermCriteria *)criteria), flags ) );

PUT_ARRAY(  featuresB );
PUT_VECTOR( error );

// fill points for which flow was not found with NaN value
double NaN = mxGetNaN();
double *data0 = (double *)mxGetData( plhs[0] );
double *data1 = (double *)mxGetData( plhs[1] );
int step0 = mxGetM( plhs[0] );
for( int i = 0; i < PARAM( featuresA, num); i++ )
{
   if( *((char *)status + i) == 0 )
   {
	   data0[i] = NaN;
	   data0[i + step0] = NaN;
	   data1[i] = NaN;
   }
}
RET

///////////////////////////////////////////////////////////////////////////////
// corners = dllname( 'GoodFeaturesToTrack',
//             IMAGE image, INT cornerCount,
//             DOUBLE qualityLevel, DOUBLE minDistance, 
//             IMAGE mask // may be empty matrix
//           );
//
WRAPPER( 6, 1, wrGoodFeaturesToTrack );
IMG_IN( 1,         image,        ; );
NUM_IN( 2, int,    cornerCount );
NUM_IN( 3, double, qualityLevel );
NUM_IN( 4, double, minDistance );
IMG_IN( 5,         mask, PARAM( mask, allowEmpty ) = true; );

TEMP_IMAGE( eigImage,  PARAM( image, size ), IPL_DEPTH_32F, 1 );
TEMP_IMAGE( tempImage, PARAM( image, size ), IPL_DEPTH_32F, 1 );

ARRAY_OUT( 0, CvPoint2D32fS, corners, cornerCount, ; );

CV_MCALL( cvGoodFeaturesToTrack( (IplImage *)image, 
	(IplImage *)eigImage, (IplImage *)tempImage,
	(CvPoint2D32f *)corners, &cornerCount, qualityLevel,
				   minDistance, (IplImage *)mask ) );
PARAM( corners, num ) = cornerCount;
PUT_ARRAY( corners );
RET
