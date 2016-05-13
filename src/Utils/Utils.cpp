#include "Utils.h"

#include <QImage>
#include <QDebug>

#include <vector>

#include <cv.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "Configs.h"
#ifndef DEFAULT_INPUT_IMAGE_DIR
    #define DEFAULT_INPUT_IMAGE_DIR ""
#endif

using namespace cv;

cv::RNG Utils::rng(12345);

// from: http://blog.csdn.net/liyuanbhu/article/details/46662115
QImage Utils::cvMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1  
    if(mat.type() == CV_8UC1)  {  

        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);  
        // Set the color table (used to translate colour indexes to qRgb values)  
        image.setColorCount(256);  
        for(int i = 0; i < 256; i++)  {  
            image.setColor(i, qRgb(i, i, i));  
        }  
        // Copy input Mat  
        uchar *pSrc = mat.data;  
        for(int row = 0; row < mat.rows; row ++) {  
            uchar *pDest = image.scanLine(row);  
            memcpy(pDest, pSrc, mat.cols);  
            pSrc += mat.step;  
        }  
        return image;  

    }  else if(mat.type() == CV_8UC3)  {  

        // 8-bits unsigned, NO. OF CHANNELS = 3  
        // Copy input Mat  
        const uchar *pSrc = (const uchar*)mat.data;  
        // Create QImage with same dimensions as input Mat  
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);  
        return image.rgbSwapped();  

    }  else if(mat.type() == CV_8UC4)  {  

        qDebug() << "CV_8UC4";  
        // Copy input Mat  
        const uchar *pSrc = (const uchar*)mat.data;  
        // Create QImage with same dimensions as input Mat  
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);  
        return image.copy();  

    }  else  {  

        qDebug() << "ERROR: Mat could not be converted to QImage.";  
        return QImage();  

    }  
}

// from: http://blog.csdn.net/liyuanbhu/article/details/46662115
cv::Mat Utils::QImage2cvMat( const QImage &image )
{
    cv::Mat mat;  
    qDebug() << image.format();  
    switch(image.format()) {  
    case QImage::Format_ARGB32:  
    case QImage::Format_RGB32:  
    case QImage::Format_ARGB32_Premultiplied:  
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());  
        break;  
    case QImage::Format_RGB888:  
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());  
        cv::cvtColor(mat, mat, CV_BGR2RGB);  
        break;  
    case QImage::Format_Indexed8:  
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());  
        break;  
    }  
    return mat;  
}

// based on LU XiaoHu's code
void Utils::drawContours( const cv::Mat &src, cv::Mat &dst, int step /* = 32 */ )
{
    cv::Mat gray;
    cvtColor( src, gray, cv::COLOR_RGB2GRAY );

    int levelStep = step < 0 ? 10 : step > 255? 255 : step;
    std::vector<std::vector<std::vector<cv::Point> > > levelLines; // all contours

    int curLevel = levelStep;
    while( curLevel <= 255 ) {

        // gray mask
        cv::Mat mask( gray.rows, gray.cols, CV_8UC1, cv::Scalar(0) );
        uchar *ptrI = gray.data;
        uchar *ptrM = mask.data;
        int imgSize = gray.rows * gray.cols;
        int count = 0;
        for ( int i=0; i<imgSize; ++i ) {
            if ( *ptrI < curLevel ) {
                *ptrM = 255;
                ++count;
            }
            ++ptrI;
            ++ptrM;
        }

        // static char filepath[1000];
        // sprintf( filepath, "%s-mask-%03d.bmp", DATA_DIR "/lena", curLevel );
        // cv::imwrite( filepath, mask );

        // find contours on mask
        std::vector<std::vector<cv::Point> > contours;
        cv::findContours( mask, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
        levelLines.push_back( contours );

        curLevel += levelStep;
    }

    vector<Vec4i> hierarchy;
    for ( int i=0; i<levelLines.size(); ++i ) {
        for ( int j=0; j<levelLines.at(i).size(); ++j ) {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            cv::drawContours( dst, levelLines.at(i), j, color, 1, 8, hierarchy, 0, Point() );
        }
    }

    /*
    uchar *ptr = dst.data;
    for ( int i=0; i<levelLines.size(); ++i ) {
        // draw with random color
        int R = rand() % 255;
        int G = rand() % 255;
        int B = rand() % 255;

        for ( int j=0; j<levelLines[i].size(); ++j ) {
            for ( int m=0; m<levelLines[i][j].size(); ++m ) {
                int x = levelLines[i][j][m].x;
                int y = levelLines[i][j][m].y;
                int loc = y * dst.cols + x;
                ptr[3*loc+0] = B;
                ptr[3*loc+1] = G;
                ptr[3*loc+2] = R;
            }
        }
    }
    */
}