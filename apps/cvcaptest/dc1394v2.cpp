#include "cv.h"
#include "highgui.h"

#include <stdio.h>

int main( int argc, char* argv[] )
{
    CvCapture* capture = NULL;
    IplImage *frame = NULL;

    capture = cvCreateCameraCapture(300);

    if( capture )
    {
        cvNamedWindow( "result", CV_WINDOW_AUTOSIZE );

        for(;;)
        {
            frame = cvQueryFrame( capture );
            if( !frame ) {
                printf("sadf\n");
                break;
            }

            cvShowImage( "result", frame );

            if( cvWaitKey( 10 ) >= 0 )
                break;
        }
        cvReleaseCapture( &capture );
        cvDestroyWindow("result");
    }
    
    return 0;
}

