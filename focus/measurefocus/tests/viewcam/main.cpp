/**
 * Code for interfacing and visualizing the output of a camera from
 * The Imaging Source using opencv and without gstreamer
 * 
 * Based on code taken from
 * https://gist.github.com/TIS-Edgar/10f04501f49b6b3bf75e
 */

#include "v4ldevice.h"

#include <opencv2/opencv.hpp>

#include <iostream>
#include <stdio.h>

#include <libv4l2.h>
#include <linux/videodev2.h>

using namespace cv;

void configureCamera(int fd)
{
    struct v4l2_control ctrl;

    // set exposure
    ctrl.id = V4L2_CID_BRIGHTNESS;
    ctrl.value = 12;
    v4l2_ioctl(fd, VIDIOC_S_CTRL, &ctrl);

    // set gain
    ctrl.id = V4L2_CID_GAIN;
    ctrl.value = 36;
    v4l2_ioctl(fd, VIDIOC_S_CTRL, &ctrl);

    // set exposure
    ctrl.id = V4L2_CID_EXPOSURE_ABSOLUTE;
    ctrl.value = 2;
    v4l2_ioctl(fd, VIDIOC_S_CTRL, &ctrl);

    // set trigger
    ctrl.id = V4L2_CID_PRIVACY;
    ctrl.value = 0;
    v4l2_ioctl(fd, VIDIOC_S_CTRL, &ctrl);
}

int main ()
{
    IplImage* pOpenCVImage;
    IplImage* pColorCVImage;

    CvSize ImageSize;
    unsigned char* ImageBuffer = NULL;
    int wKey = -1;

    ImageSize.width = 1280;
    ImageSize.height = 960;

    cvNamedWindow( (char*)"Camera", 1 );

    printf("Program started\n");

    pOpenCVImage = cvCreateImage(ImageSize , IPL_DEPTH_8U, 1 ); // Grayscale

    int fd = open_device((char*)"/dev/v4l/by-id/usb-The_Imaging_Source_Europe_GmbH_DMM_42BUC03-ML_47710685-video-index0");

    configureCamera(fd);

    init_device(ImageSize.width, ImageSize.height);

    printf("Start capturing\n");

    start_capturing();

    while(wKey == -1 )
    {
        ImageBuffer = snapFrame();

        if( ImageBuffer != NULL )
        {
            memcpy( pOpenCVImage->imageData, ImageBuffer, pOpenCVImage->imageSize);

            cvShowImage( (char*)"Camera", pOpenCVImage);
            wKey = cvWaitKey(10);
        }
        else
        {
            printf("No image buffer retrieved.\n");
            break;
        }
    }

    cvDestroyWindow( (char*)"Camera" );
    stop_capturing();
    uninit_device();
    close_device();

    printf("Program ended\n");

    return 0;
}