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

//#include "qfrontend.h"

#include "qerr.h"
#include "hash.h"

#include "cv.h"

// !!!
// uncomment this function 
// if you wish that CV reports errors to Matlab and don't terminate programm

//------------- Error function ----------------------------------------------------------
char cvErrorMessage[1000]; // error message from some OpenCV function

extern "C"
{
	CVStatus GuiBoxReport_NotFatal( CVStatus status, const char *funcName, 
								   const char *context, const char *file, int line)
	{
		char mess[1000];
		//char *choice = 0;    
		const char* errText = cvErrorStr( status );
		
		if ( cvGetErrMode() != CV_ErrModeSilent ) 
		{
			if ( !funcName ) funcName = "<unknown>";
			if ( !context  ) context = "";
			if ( !file     ) file = "";
			if (  line < 0 ) line = 0;
			
			
			if ( status == CV_StsBackTrace)
				sprintf(mess,"Called from %s(): [file %s, line %d]\n%s\n%s\n(status:%d)\n",
				funcName, file,line,context, errText, status);
			else if ( status == CV_StsAutoTrace )
				sprintf(mess,"Called from %s(): [file %s, line %d]\n%s\n%s\n(status:%d)\n",
				funcName, file, line, context, errText, status);
			else
				sprintf(mess,"In function %s(): [file %s, line %d]\n%s\n%s\n(status:%d)\n",
				funcName, file, line, context, errText, status);
			
			sprintf(cvErrorMessage,"OpenCV Beta 1: %s\n%s",cvErrorStr(cvGetErrStatus()),mess);
			//throw_error(title);
		}
		
		return status;
	}
}
//-------------------------------------------------------------------------------------

//------------- Wrappers --------------------------------------------------------------
#include "funcs.h"
#include "tmacro.h"
#include "wrmacro.h"
#include "wrappers.cpp"

// !!!
// you should better to add all names of wrapper functions in map!

MapEntry function_map[] = 
  {
	  { "Erode"		, (void*) wrErode },
	  { "Dilate"		, (void*) wrDilate },
	  { "PyrUp"			, (void*) wrPyrUp },
	  { "PyrDown"		, (void*) wrPyrDown },
	  { "Laplace"		, (void*) wrLaplace },
	  { "Sobel"			, (void*) wrSobel },
	  { "DistTransform"			, (void*) wrDistTransform },
	  { "Canny"			, (void*) wrCanny },
	  { "PreCornerDetect"			, (void*) wrPreCornerDetect },
	  { "MatchTemplate"			, (void*) wrMatchTemplate },
	  { "UpdateMotionHistory"			, (void*) wrUpdateMotionHistory },
	  { "MinMaxLoc"			, (void *) wrMinMaxLoc },
	  { "Threshold"			, (void *)wrThreshold },
	  { "AdaptiveThreshold"			, (void*) wrAdaptiveThreshold },
	  { "FindContours"		, (void *)wrFindContours },
	  { "DrawContours"		, (void *)wrDrawContours },
	  { "ApproxPoly"		, (void *)wrApproxPoly },
	  { "ContourPerimeter"	, (void *)wrContourPerimeter },
	  { "ContourArea"	, (void *)wrContourArea },
	  { "FloodFill",          (void *)wrFloodFill },
	  { "CalcOpticalFlowPyrLK", (void *)wrCalcOpticalFlowPyrLK },
	  { "GoodFeaturesToTrack", (void *)wrGoodFeaturesToTrack },
	  { 0 , 0 }
  };
//-------------------------------------------------------------------------------------

extern "C" {

void mexFunction( int nlhs, mxArray* plhs[],
				  int nrhs, const mxArray* prhs[] ) {
	// uncomment these two calls
	// if you wish that CV report errors to Matlab and don't terminate programm

	cvRedirectError( GuiBoxReport_NotFatal ); 
	cvSetErrMode( CV_ErrModeParent );

	try
	{
		if(nrhs < 2)
			throw_error( "Internal wrapper error: main function must have more than one argument" );
		
		if( !mxIsChar( prhs[0] ) ) 
			throw_error( "Internal wrapper error: in main function zero argument must be string" );

		std::string funcname = get_string( prhs[0] );
				
		InitHashTable((MapEntry *)&function_map);
		MapEntry* entry = FindMapEntry(funcname.c_str());
		
		// if wrapper exist we call it
		if (entry)
		{
			void (*pfun) (int nlhs, mxArray* plhs[],int nrhs, const mxArray* prhs[])
				= (void (__cdecl *)(int nlhs, mxArray* plhs[],
									int nrhs, const mxArray* prhs[]))(entry->image);
			
			(*pfun)(nlhs, plhs, nrhs, prhs);
		} else
		{
			std::ostringstream ostr;
			ostr << "Internal wrapper error: "<< funcname << " function not found";
			throw_error( ostr.str() );
		}		
		return;
	}
	catch( Error e )
	{
		mexErrMsgTxt( e.get_error_messages() );
	}
	catch (...)
	{
		mexErrMsgTxt( "Unknown C++ exception occurred" );
	}
}

}