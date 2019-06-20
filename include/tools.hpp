#ifndef _TOOLS_HPP_
#define _TOOLS_HPP_

#include <opencv2/opencv.hpp>

namespace tools {
    
double imageSimilarity(const cv::Mat & img1, const cv::Mat & img2);
    
void combineImages(const cv::Mat & videoFrame, const cv::Mat & difference, const cv::Mat & replacingImage, float threshold, cv::Mat & imgResult);
    
}

#endif

