#ifndef UTILS_H
#define UTILS_H

namespace cv {
    class Mat;
    class RNG;
}

class QImage;

class Utils
{
public:
    static cv::RNG rng;
    static QImage cvMat2QImage(const cv::Mat& mat);
    static cv::Mat QImage2cvMat( const QImage &image );
    static void drawContours( const cv::Mat &src, cv::Mat &dst, int step = 32 );
};

#endif  // UTILS_H