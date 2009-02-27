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


#ifndef QERR_H
#define QERR_H

#include <sstream>

extern "C" {
#include "mex.h" /* to build MATLAB wrappers you need to have MATLAB C/C++ package installed.
                    see opencv\interfaces\matlab\readme.txt */
}

const int ERROR_BUF_LEN = 2000;

class Error {
	public:
		Error( const Error& e )
		{
			for( int i = 0; i < ERROR_BUF_LEN; i++ )
			{
				msg[i] = e.msg[i];
				if( !msg[i] )
				{
					break;
				}
			}
		}
  
		~Error() {;}
		const char *get_error_messages() { return msg; };
		friend void throw_error( const std::string& s );

	private:
		char msg[ERROR_BUF_LEN];
		// Only throw_error can create this
		explicit Error( const std::string& errstr )
		{
			int l = errstr.length();
			if( l >= ERROR_BUF_LEN )
			{
				l = ERROR_BUF_LEN - 1;
			}
			for( int i = 0; i < l; i++ )
			{
				msg[i] = errstr[i];
			}
			msg[l] = '\0';
		}
};


void throw_error( const std::string& s );

#endif
