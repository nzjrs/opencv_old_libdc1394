#include "cv.h"
#include "highgui.h"

#include <stdlib.h>
#include <stdio.h>

static void usage(char *me)
{
    printf(
        "Usage:\n%s\t<base+index>\n"
        "Base:\n"
        "0\tAutodetect\n"
        "100\tMIL proprietary drivers\n"
        "200\tPlatform native (VFW, VFL2, etc)\n"
        "300\tIEEE 1394 drivers\n"
        "400\tTYZX proprietary drivers\n"
        "500\tQuickTime\n"
        "600\tUnicap drivers\n"
        "700\tDirectShow (via videoInput)\n", me);
}

int main( int argc, char* argv[] )
{
    CvCapture* capture = NULL;
    IplImage *frame = NULL;
    int index = -1;

    if (argc != 2) {
        usage(argv[0]);
        exit(1);
    }

    index = strtol(argv[1], NULL, 0);
    if (index < 0 || index > 800) {
        usage(argv[0]);
        exit(2);
    }        

    capture = cvCreateCameraCapture(index);

    if( capture )
    {
        cvNamedWindow( "result", CV_WINDOW_AUTOSIZE );

        for(;;)
        {
            frame = cvQueryFrame( capture );
            if( !frame )
                break;

            cvShowImage( "result", frame );

            if( cvWaitKey( 10 ) >= 0 )
                break;
        }
        cvReleaseCapture( &capture );
        cvDestroyWindow("result");
    }
    
    return 0;
}

