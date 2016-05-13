#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "cv.h"

#include "Configs.h"
#include "Utils.h"

char srcImagePath[1000];

int main( int argc, char **argv )
{
    using namespace cv;
    using namespace std;

    if ( argc >= 2 ) {
        sprintf( srcImagePath, "%s", argv[1] );
    } else {
        sprintf( srcImagePath, "%s", DATA_DIR "/lena.jpg" );
    }

    int step = 32;
    if ( argc >= 3 ) {
        step = atoi( argv[2] );
    }

    Mat src, dst;
    src = imread( srcImagePath, 1 );
    dst = src.clone();

    if ( src.empty() ) {
        puts( "image loading failed." );
        exit(1);
    }

    Utils::drawContours( src, dst, step );
    imshow( "src", src );
    imshow( "dst", dst );
    cvWaitKey( 0 );
}