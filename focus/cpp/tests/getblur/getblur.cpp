#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace cv;

float calcBlurriness( const Mat &src )
{
    Mat Gx, Gy;
    Sobel( src, Gx, CV_32F, 1, 0 );
    Sobel( src, Gy, CV_32F, 0, 1 );
    double normGx = norm( Gx );
    double normGy = norm( Gy );
    double sumSq = normGx * normGx + normGy * normGy;
    return static_cast<float>( 1. / ( sumSq / src.size().area() + 1e-6 ));
}

int main( int argc, char** argv )
{
    char* filename = argc >= 2 ? argv[1] : (char*)"../bird.jpg";
    
    Mat src = imread( filename );

    if(src.data)
    {
        std::cout << "blur on image : " << calcBlurriness( src ) << std::endl;
    }
    return 0;
}
