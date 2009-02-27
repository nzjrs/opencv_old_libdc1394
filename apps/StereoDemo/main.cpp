/****************************************************************************************\
*                                                                                        *
*                                        Stereo Demo                                     *
*                                                                                        *
\****************************************************************************************/

#include "cvaux.h"
#include "highgui.h"
#include <limits.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>    

volatile int g_undistort = 0;
volatile int g_calib = 0;
volatile int g_rectify = 0;
volatile int g_save = 0;
volatile int g_calc_stereo = 1;
volatile int g_view_rectified = 0;
volatile int g_snap = 0;
volatile int g_write_avi = 0;    

void callback( IplImage* image1,IplImage* image2 );
void callback1( IplImage* image );

CvCalibFilter CalibFilter;
double EtalonParams[3] = { 6, 8, 3 };
IplImage* vis_disp = NULL;
//StereoMatcher stereo_matcher;

CvVideoWriter* VideoWriterL = NULL;
CvVideoWriter* VideoWriterR = NULL;
CvVideoWriter* VideoWriterAux = NULL;


void toggle_calib(void);
void toggle_undistort(void);

int maxdisp = 100;

void calc_dense_disparity(IplImage* image1, IplImage* image2);

void print_help(void)
{
    printf("Help:\n"
                   "h - this help screen\n"
                   "g - start/stop calibration\n"
	           "spacebar - take a snapshot to send to calibration\n"
                   "s - save camera parameters to camera.txt\n"
                   "l - load camera parameters from camera.txt\n"
                   "u - toggle undistortion\n"
                   "r - toggle rectification\n"
                   "t - reverse the cameras order\n"
                   "n - save the couple of frames\n"
                   "d - on/off computing disparity\n"
                   "v - toggle view of the rectified images\n"
                   "a - start/stop writing avi files\n"
	);
}

#define SOURCE_AVI 1
#define SOURCE_CAM 2

#ifndef FLTK
int main( int argc, char** argv )
{
    int source = SOURCE_CAM; 
    if( argc == 3 )
    {   
        source = SOURCE_AVI;
    }   
    g_calc_stereo = 1;  

    CvCapture* captureL; //left 
    CvCapture* captureR; //right

    IplImage* left_gray = 0;
    IplImage* right_gray = 0;

    CalibFilter.SetEtalon( CV_CALIB_ETALON_CHESSBOARD, EtalonParams );
    CalibFilter.SetCameraCount(2);
    CalibFilter.SetFrames(15);

    if( source == SOURCE_CAM ) //input from cameras
    {
        captureL = cvCaptureFromCAM( 0 ); //first camera
        captureR = cvCaptureFromCAM( 1 ); //second camera
    }
    else
    {
        captureL = cvCaptureFromAVI( argv[1] ); //first avi file
        captureR = cvCaptureFromAVI( argv[2] ); //second avi file
    }   
    
    if( !captureL || !captureR ) 
    {  printf("can't run camera/avi\n");  return 1; }
        
    cvNamedWindow( "source left", 1 );
    cvNamedWindow( "source right", 1 );
    
    print_help();

    for(;;)
    {
        int cmd = cvWaitKey( 2 );
        if( cmd == '\x1b' )
            break;

        //frame processing
        cvGrabFrame( captureL );
        //int time2 = clock();
        cvGrabFrame( captureR );
        //int time1 = clock();
        //printf("time diff = %d", time1 - time2);
        IplImage* frameL = cvRetrieveFrame( captureL );
        IplImage* frameR = cvRetrieveFrame( captureR );

        if( !frameL || !frameR )
        {  
            cvReleaseVideoWriter( &VideoWriterAux );

            if( g_write_avi == 1 )
            {
                cvReleaseVideoWriter( &VideoWriterL );
                cvReleaseVideoWriter( &VideoWriterR );
            }
            break;
        }

        if( source == SOURCE_AVI )
        {
            frameL->origin = frameR->origin = IPL_ORIGIN_TL;
            cvFlip( frameR, frameR, 0 );
            cvFlip( frameL, frameL, 0 );
        }

        //convert color to gray
        if( !left_gray && !right_gray )
        {
            left_gray = cvCreateImage(cvSize( frameL->width,frameL->height), IPL_DEPTH_8U, 1 );
            right_gray = cvCreateImage(cvSize( frameL->width,frameL->height), IPL_DEPTH_8U, 1 );   
            left_gray->origin = right_gray->origin = frameL->origin;
        }
    
        cvCvtColor( frameL, left_gray, CV_BGR2GRAY );
        cvCvtColor( frameR, right_gray, CV_BGR2GRAY );
        
        callback( left_gray, right_gray ); 

        switch( tolower(cmd) )
        {
        case '1':
        case '2':
            break;
        case 'g':
            toggle_calib();
            break;
        case 's':
            CalibFilter.SaveCameraParams("cameras.txt");
            break;
        case 'l':
            CalibFilter.LoadCameraParams("cameras.txt");
            g_rectify ^= 1;
            break;
        case 'u':
            g_undistort ^= 1;
            break;
        case 'r':
            g_rectify ^= 1;
            break;
	case 'v':
	    g_view_rectified ^= 1;
	    break;
        case 'n':
            g_save = 1;
            break;
        case 't': {
            CvCapture* tmp = captureL;
            captureL = captureR;
            captureR = tmp;
            break; }
        case 'h':
            print_help();
            break;
        case 'd':
            g_calc_stereo ^=1;
            break;           
        case 'a':
            g_write_avi ^=1;
            if( g_write_avi )
            {
                VideoWriterL = cvCreateVideoWriter( "StereoL.avi", -1,
                                        10, cvSize(frameL->width,frameL->height)  );
                VideoWriterR = cvCreateVideoWriter( "StereoR.avi", -1,
                                        10, cvSize(frameL->width,frameL->height)  );
            }
            else
            {
                cvReleaseVideoWriter( &VideoWriterL );
                cvReleaseVideoWriter( &VideoWriterR );
            }           
            break;           
	case ' ':
	    g_snap = 1;
	    break;
        }
    }
   

    cvReleaseCapture( &captureL );
    cvReleaseCapture( &captureR );

    cvReleaseImage( &left_gray );
    cvReleaseImage( &right_gray );

    cvReleaseImage(&vis_disp);

    
    return 0;
}
#endif

void toggle_undistort( void )
{
    g_undistort ^= 1;
}


int prev_time = 0;

void toggle_calib( void )
{
    prev_time = clock() - 1000;
    if( !g_calib )
        g_undistort = 0;
    g_calib ^= 1;
}   

void SaveStereoPair( IplImage** images, const char* rootname )
{
    int i = 0;
    char name[1000];

    for( i = 0; i < 1000; i++ )
    {
        sprintf( name, "%s%03dL.bmp", rootname, i );
        FILE* f = fopen( name, "rb" );

        if( f )
        {
            fclose(f);
            continue;
        }
        
        cvSaveImage( name, images[0] );

        sprintf( name, "%s%03dR.bmp", rootname, i );
        cvSaveImage( name, images[1] );

        break;
    }
}

void callback( IplImage* image1, IplImage* image2 )
{
    IplImage* images[] = { image1, image2 };
    int save = g_save != 0;
           
#if 1
    if( g_calib && g_snap )
    {
        bool found = CalibFilter.FindEtalon( images );

	g_view_rectified = 0;

        if( !save )
            CalibFilter.DrawPoints( images );

        if( found )
        {
            int cur_time = clock();
            if( cur_time >= prev_time + 1000 )
            {
                prev_time = cur_time;
                CalibFilter.Push();
                cvXorS( image1, cvScalarAll(255), image1 );
                cvXorS( image2, cvScalarAll(255), image2 );
                g_undistort = 0;
            }

            if( CalibFilter.IsCalibrated() )
            {
                g_calib ^= 1;
                g_undistort = 1;
                g_rectify = 1;
            }
        }
    }

    if( !g_view_rectified )
    {
	cvDestroyWindow( "rectified left" );
	cvDestroyWindow( "rectified right" );	
    }

    //if( g_undistort )
    //    CalibFilter.Undistort( images, images );

    cvShowImage( "source left", image1 );
    cvShowImage( "source right", image2 );

    if ( g_calib && g_snap ){
	g_snap = 0;
	cvWaitKey( 1000 );
    }

    if( save )
    {
        SaveStereoPair( images, "imgU" );
    }

    if( g_rectify && CalibFilter.IsCalibrated() )
    {
        CalibFilter.Rectify( images, images );

        //calc_disparity(image1, image2);
        if( save )
        {
            SaveStereoPair( images, "imgR" );
        }
    }

    if( CalibFilter.IsCalibrated() && g_view_rectified)
    {
	IplImage *l;
	l = cvCreateImage( cvSize(image1->width, image1->height),
			   IPL_DEPTH_8U, 1 );

	cvConvertScale( image1, l, 1, 0 );
	cvNamedWindow( "rectified left", 1 );
	cvNamedWindow( "rectified right", 1 );
        cvShowImage( "rectified left", l );
	cvConvertScale(image2, l, 1, 0);
        cvShowImage( "rectified right", l );
	cvReleaseImage( &l );
    }

    if( g_calc_stereo )
    {
        calc_dense_disparity(image1, image2); 
    }                                                                          

    if( g_write_avi )
    {
        cvWriteFrame( VideoWriterL, image1 );
        cvWriteFrame( VideoWriterR, image2 ); 
    }      
#endif
    g_save = 0;
}
int dense_inited = 0;
void calc_dense_disparity(IplImage* image1, IplImage* image2)
{
    if(!dense_inited)
    {
        cvNamedWindow("disparity", 1);
        cvCreateTrackbar( "maxdisp", "disparity", &maxdisp, 255, 0 );
        dense_inited = 1;
        vis_disp = cvCreateImage(cvSize(image1->width, image1->height), IPL_DEPTH_8U, 1);
    }

    vis_disp->origin = image1->origin;
    cvFindStereoCorrespondence( 
                   image1, image2,
                   CV_DISPARITY_BIRCHFIELD,
                   vis_disp,
                   maxdisp );
    cvConvertScale(vis_disp, vis_disp, 255.f/maxdisp);
    cvShowImage("disparity", vis_disp);
}

/*void calc_disparity_matcher(IplImage* left, IplImage* right)
{
    if( !stereo_matcher.IsInited() )
    {
        stereo_matcher.Init( cvSize(left->width, left->height ),
            60, 
            STEREO_DP
            //STEREO_ID
            //STEREO_SEGM
            //STEREO_SEGM_MULTY 
            );
        cvvNamedWindow("disparity", 1);
        cvvWaitKey(0);
    }
    if( stereo_matcher.m_max_disparity != thresh1 )
        stereo_matcher.Init( cvSize(left->width, left->height ),
            thresh1, STEREO_DP); 
            
    stereo_matcher.OnFrames( left, right );
    
    //scale disparity
    iplMultiplyS( stereo_matcher.GetDisparity(), 
                  stereo_matcher.GetDisparity(),
                  255/stereo_matcher.m_max_disparity );

   // iplBlur( stereo_matcher.GetDisparity(), 
     //             stereo_matcher.GetDisparity(), 3,3,1,1 );

    cvErode( stereo_matcher.GetDisparity(), 
                  stereo_matcher.GetDisparity(), NULL, 1 );
    //cvDilate( stereo_matcher.GetDisparity(), 
    //              stereo_matcher.GetDisparity(), NULL, 1 );
     

    
    cvvShowImage("left", left );
    cvvShowImage("right", right );
    cvvShowImage("disparity", stereo_matcher.GetDisparity() );



    if( !VideoWriterAux )
    {
        VideoWriterAux = cvCreateVideoWriter( "StereoLD.avi", -1,
                                        5, cvSize(left->width*2,left->height)  );
    }
    IplImage* merged = cvCreateImage( cvSize(left->width*2,left->height), 
                                      IPL_DEPTH_8U, 3 );
    IplROI roi;
    roi.coi = 0; roi.height = left->height; roi.width = left->width;
    roi.yOffset = 0;
    roi.xOffset = 0;
    
    left->roi = &roi;
    merged->roi = &roi;
    iplCopy( left, merged);
    
    IplROI roi2 = roi;
    stereo_matcher.GetDisparity()->roi = &roi2;
    roi.xOffset = left->width;
    
    iplCopy( stereo_matcher.GetDisparity(), merged);    

    merged->roi = left->roi = stereo_matcher.GetDisparity()->roi = NULL;
    
    cvWriteFrame( VideoWriterAux, merged );

    //visualize disparity
    //cvvWaitKeyEx("left", 1000);

}
*/
