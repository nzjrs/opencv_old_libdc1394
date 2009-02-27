/******************************************************************   
* This program uses function histogram() in Ch Professional Edition
*******************************************************************/
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <numeric.h>

int main(int argc, char** argv) {
    char filename[] = "baboon.jpg";
    char windowname[] = "Gray image";
    IplImage *image1 = NULL, *image2 = NULL;

    // Load the source image. 
    image1 = cvLoadImage(argc == 2 ? argv[1] : filename, 1);
    if(!image1) {
        printf("Image was not loaded.\n");
        return -1;
    }
    image2 = cvCreateImage(cvSize(image1->width,image1->height), 
              IPL_DEPTH_8U, 1);
    cvCvtColor(image1,image2, CV_BGR2GRAY);
    cvNamedWindow(windowname, 0);
    cvShowImage(windowname, image2);

    unsigned char x[128], y[image2->imageSize]; 
    linspace(x, 0, 255);
    memcpy(y, image2->imageData, image2->imageSize); 
    histogram(y, x);

    cvWaitKey(0);
    cvReleaseImage(&image1);
    cvReleaseImage(&image2);
    cvDestroyWindow(windowname);
    return 0;
}
