/**
 * @brief   Code for interfacing and visualizing the output of a camera from
 *          The Imaging Source using opencv and gstreamer. The code also
 *          calculates a focus measure of the image in real time while displaying
 *          it in the video window.
 * @param   Number of the camera to use according to the cameralist.txt file
 *          included with this code. Number is 1-indexed. Default is 1.
 * @param   If 1 the focus measures are saved in a local text file named
 *          'focus_log.txt'. Default is 0.
 * @example $./viewcam 4 1          (selects camera 4 and saves data to file)
 */


#include <opencv2/opencv.hpp>

#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>

#include <libv4l2.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <fcntl.h>


#define FRAME_RATE              ( 30 )  // camera frame rate
#define FOCUSUPDATE_RATE        ( 10 )  // update rate focus measure in frames per second

using namespace cv;
using namespace std;


string cameraList[12];
FILE *f;


/**
 * @brief   Loads camera list from text file
 */
void loadCameraList(void)
{
    int i =0;
    string line;
    ifstream file("cameralist.txt");

    if(file.is_open())
    {
        while (!file.eof())
        {
            getline (file, line);
            cameraList[i] = line;
            //cout << cameraList[i] << endl;
            i++;
        }
        file.close();
    }
    else printf("Unable to open file\n");
}


/**
 * @brief   Configures camera
 * @param   String with device name
 */
void configureCamera(string deviceName)
{
    const char* cameraId = deviceName.c_str();
    //int fd = v4l2_open(cameraId, O_RDWR);
    int fd = open(cameraId, O_RDWR /* required */ | O_NONBLOCK, 0);
    
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
    ctrl.value = 100;
    v4l2_ioctl(fd, VIDIOC_S_CTRL, &ctrl);

    // set trigger
    ctrl.id = V4L2_CID_PRIVACY;
    ctrl.value = 1;
    v4l2_ioctl(fd, VIDIOC_S_CTRL, &ctrl);

    // set frame rate
    struct v4l2_streamparm parm;
    parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    parm.parm.capture.timeperframe.numerator = 1;
    parm.parm.capture.timeperframe.denominator = FRAME_RATE;
    v4l2_ioctl(fd, VIDIOC_S_PARM, &parm);

    //v4l2_close(fd);
    close(fd);
}


/**
 * @brief   Focus measure implementation using the 'Variance of laplacian (Pech2000)'.
 *          Information taken from:
 *          - S. Pertuz et al., Analysis of focus measure operators for shape-from-focus.
 *            Pattern Recognition, 46(5):1415:1432, 2013
 * @param   Image to process
 * @return  Focus measure
 */
float fmeasure( const Mat &src )
{
    Point anchor( 0, 0 );
    double delta = 0;
    int ddepth = -1;
    Scalar mean, stddev;

    // laplacian filter kernel
    float kernel[3][3] = { {0.1667, 0.6667, 0.1667} , {0.6667, -3.3333, 0.6667} , {0.1667, 0.6667, 0.1667} };

    Mat ker = Mat( 3, 3, CV_32F, &kernel );
    Mat dst = Mat( src.size(), src.type() );

    // apply filter to image
    filter2D( src, dst, ddepth , ker, anchor, delta, BORDER_REPLICATE );

    // calculate standard deviation
    meanStdDev(dst, mean, stddev);

    // return focus measure as the standard deviation squared
    return static_cast<float>( pow(stddev.val[0], 2) );
}


int main ( int argc, char** argv )
{
    int     camIndex    = 1;    // default camera
    int     saveToText  = 0;    // default setting
    float   focus       = 0.0;
    int     wKey        = -1;
    int     updateCount = 0;

    if( argc > 1)
        camIndex = atoi(argv[1]);
    
    if( argc > 2)
        saveToText = atoi(argv[2]);

    printf("Program started\n");
    
    // load camera list from external text file
    loadCameraList();

    // configure camera
    //configureCamera(cameraList[camIndex-1]);

    // build gstreamer command
    string cmd = "v4l2src device="
               + cameraList[camIndex-1]
               + " ! video/x-raw,format=GRAY8,width=1280,height=960,framerate=30/1 ! videoconvert ! appsink";
    
    // open camera
    VideoCapture cap(cmd);

    // if not success, exit program
    if (cap.isOpened() == false)  
    {
        printf("Cannot open the video camera\n");
        return -1;
    }

    // create a window for display
    string window_name = "Display window";
    namedWindow(window_name, WINDOW_AUTOSIZE);

    printf("Start capturing\n");

    // if flag is set, open text file to save data
    if (saveToText) f = fopen("focus_log.txt", "w");

    while(wKey == -1 )
    {
        Mat frame;
        bool bSuccess = cap.read(frame); // read a new frame from video 

        // breaking the while loop if the frames cannot be captured
        if (bSuccess == false) 
        {
            printf("Video camera is disconnected\n");
            break;
        }

        // calculate focus
        if (++updateCount == FRAME_RATE / FOCUSUPDATE_RATE) {
            updateCount = 0;
            focus = fmeasure(frame);
            if (saveToText) fprintf(f, "%f\n", focus);
        }

        // convert grey image to color image
        Mat frame_rgb(frame.size(), CV_8UC3);
        cvtColor(frame, frame_rgb, CV_GRAY2RGB);

        // display text overlay with focus measure
        stringstream stream;
        stream << focus;
        string text = "focus=" + stream.str();
        putText(frame_rgb, text, cvPoint(30,30), FONT_HERSHEY_DUPLEX, 1.2, CV_RGB(255,30,0), 1, CV_AA);
        
        // show frame
        imshow(window_name, frame_rgb);

        wKey =  waitKey(10);
    }

    cap.release();
    destroyWindow(window_name);
    if (saveToText) fclose(f);

    printf("\nProgram ended\n");

    return 0;
}
