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
#include <stdlib.h>
#include <varargs.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>

#include <eic.h>
#ifdef malloc /* This macros is defined in eic.h */
#undef malloc
#endif /* malloc */
#ifdef realloc
#undef realloc
#endif /* realloc */
#ifdef free
#undef free
#endif /* free */

#ifdef WIN32
#undef WIN32
#include <highgui.h>
#define WIN32
#else
#include <highgui.h>
#endif

#include "hawkwrap.h"

static jmp_buf* highgui_mark = NULL;
static errlevel_t highgui_errlevel = unsafe;
int trackbar_count = 0;
void (**trackbar_notifies)(int) = NULL;

//define many small trackbar callbacks
#define TB_CB(i) void tb_cb##i(int val)                 \
{                                                       \
    setArg(0, (code_t*)trackbar_notifies[i], int, val); \
    EiC_callBack(trackbar_notifies[i]);                 \
}                                        
TB_CB(0); TB_CB(1); TB_CB(2); TB_CB(3); TB_CB(4); 
TB_CB(5); TB_CB(6); TB_CB(7); TB_CB(8); TB_CB(9); 
TB_CB(10); TB_CB(11); TB_CB(12); TB_CB(13); TB_CB(14); 
TB_CB(15); TB_CB(16); TB_CB(17); TB_CB(18); TB_CB(19); 
TB_CB(20); TB_CB(21); TB_CB(22); TB_CB(23); TB_CB(24); 
TB_CB(25); TB_CB(26); TB_CB(27); TB_CB(28); TB_CB(29); 
TB_CB(30); TB_CB(31); TB_CB(32); TB_CB(33); TB_CB(34); 
TB_CB(35); TB_CB(36); TB_CB(37); TB_CB(38); TB_CB(39);

void(*tb_cbs[40])(int) = 
{ tb_cb0, tb_cb1, tb_cb2, tb_cb3, tb_cb4, 
  tb_cb5, tb_cb6, tb_cb7, tb_cb8, tb_cb9,
  tb_cb10, tb_cb11, tb_cb12, tb_cb13, tb_cb14, 
  tb_cb15, tb_cb16, tb_cb17, tb_cb18, tb_cb19,
  tb_cb20, tb_cb21, tb_cb22, tb_cb23, tb_cb24, 
  tb_cb25, tb_cb26, tb_cb27, tb_cb28, tb_cb29,
  tb_cb30, tb_cb31, tb_cb32, tb_cb33, tb_cb34, 
  tb_cb35, tb_cb36, tb_cb37, tb_cb38, tb_cb39 }; //array of callbacks

static val_t eic_create_trackbar(void)
{
	val_t v;
    void* pnotify;

    if( trackbar_count == 40 )  
    {
        char str[1024];
        sprintf(str, "The maximal number of trackbars is 40");
        WRAP_THROW_MESSAGE(highgui_mark, str);
    }

    pnotify = arg(4,getargs(),ptr_t).p;
	v.ival = cvCreateTrackbar((const char*)arg(0,getargs(),ptr_t).p,
        (const char*)arg(1,getargs(),ptr_t).p,
        (int*)arg(2,getargs(),ptr_t).p,
        arg(3,getargs(),int),
        pnotify ? tb_cbs[trackbar_count] : NULL);
    trackbar_count++;
    trackbar_notifies = (void(**)(int))realloc(trackbar_notifies, trackbar_count*sizeof(char*));
    trackbar_notifies[trackbar_count - 1] = (void(*)(int))pnotify;
	return v;
}

int mouse_callback_count = 0;
void (**mouse_notifies)(int,int,int,int) = NULL;

//define many small mouse callbacks
#define M_CB(i) void m_cb##i(int event, int x, int y, int flag, void* userdata )  \
{                                                        \
    setArg(0, (code_t*)mouse_notifies[i], int, event);   \
    setArg(1, (code_t*)mouse_notifies[i], int, x);       \
    setArg(2, (code_t*)mouse_notifies[i], int, y);       \
    setArg(3, (code_t*)mouse_notifies[i], int, flag);    \
    setArg(4, (code_t*)mouse_notifies[i], void*, userdata); \
    EiC_callBack(mouse_notifies[i]);                     \
}                                        

M_CB(0); M_CB(1); M_CB(2); M_CB(3); M_CB(4); 
M_CB(5); M_CB(6); M_CB(7); M_CB(8); M_CB(9); 
M_CB(10); M_CB(11); M_CB(12); M_CB(13); M_CB(14); 
M_CB(15); M_CB(16); M_CB(17); M_CB(18); M_CB(19); 
M_CB(20); M_CB(21); M_CB(22); M_CB(23); M_CB(24); 
M_CB(25); M_CB(26); M_CB(27); M_CB(28); M_CB(29); 
M_CB(30); M_CB(31); M_CB(32); M_CB(33); M_CB(34); 
M_CB(35); M_CB(36); M_CB(37); M_CB(38); M_CB(39);

void(*m_cbs[40])(int) = 
{ m_cb0, m_cb1, m_cb2, m_cb3, m_cb4, 
  m_cb5, m_cb6, m_cb7, m_cb8, m_cb9,
  m_cb10, m_cb11, m_cb12, m_cb13, m_cb14, 
  m_cb15, m_cb16, m_cb17, m_cb18, m_cb19,
  m_cb20, m_cb21, m_cb22, m_cb23, m_cb24, 
  m_cb25, m_cb26, m_cb27, m_cb28, m_cb29,
  m_cb30, m_cb31, m_cb32, m_cb33, m_cb34, 
  m_cb35, m_cb36, m_cb37, m_cb38, m_cb39 }; //array of callbacks

static val_t eic_set_mouse_callback(void)
{
	val_t v;
    void* pnotify;

    if( mouse_callback_count == 40 )  
    {
        char str[1024];
        sprintf(str, "The maximal number of mouse callbacks is 40");
        WRAP_THROW_MESSAGE(highgui_mark, str);
    }

    pnotify = arg(1,getargs(),ptr_t).p;
	cvSetMouseCallback((const char*)arg(0,getargs(),ptr_t).p,
                       pnotify ? m_cbs[mouse_callback_count] : NULL,NULL);
    mouse_callback_count++;
    mouse_notifies = (void(**)(int,int,int,int))realloc(mouse_notifies, 
                                       mouse_callback_count*sizeof(char*));
    mouse_notifies[mouse_callback_count - 1] = (void(*)(int,int,int,int))pnotify;
	return v;
}

/**********************************/

void module_highguiwrap()
{
    //wrap highgui functions which have callbacks as parameters
    EiC_add_builtinfunc("cvCreateTrackbar",eic_create_trackbar); 
    EiC_add_builtinfunc("cvSetMouseCallback",eic_set_mouse_callback); 
}

void set_highgui_mark(jmp_buf* mark)
{
    highgui_mark = mark;
}

void set_highgui_errlevel(errlevel_t level)
{
    highgui_errlevel = level;
}
/**********************************/

