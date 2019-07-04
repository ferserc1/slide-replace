
#include <tools.hpp>

namespace tools {
    
double imageSimilarity(const cv::Mat & img1, const cv::Mat & img2) {
    cv::Mat diff;
    absdiff(img1, img2, diff);
    bitwise_not(diff, diff);
    std::vector<cv::Mat> planes;
    split(diff, planes);
    
    int histSize = 256;
    float range[] = { 0.0f, 256.0f };
    const float * histRange = { range };
    cv::Mat bHist, gHist, rHist;
    cv::calcHist(&planes[0], 1, 0, cv::Mat(), bHist, 1, &histSize, &histRange, true, false);
    cv::calcHist(&planes[1], 1, 0, cv::Mat(), gHist, 1, &histSize, &histRange, true, false);
    cv::calcHist(&planes[2], 1, 0, cv::Mat(), rHist, 1, &histSize, &histRange, true, false);
    
    float value = 0.0f;
    for (int i = 0; i<histSize/3; ++i) {
        auto histColor = (bHist.at<float>(i) + gHist.at<float>(i) + bHist.at<float>(i)) / 3.0f;
        value += histColor;
    }
    return value;
}

void combineImages(const cv::Mat & videoFrame, const cv::Mat & difference, const cv::Mat & replacingImage, float threshold, cv::Mat & imgResult) {
    float dist;
    for (int j = 0; j<videoFrame.rows; ++j) {
        for (int i = 0; i<videoFrame.cols; ++i) {
            cv::Vec3b pix = difference.at<cv::Vec3b>(j,i);
            
            dist = sqrtf(static_cast<float>(pix[0] * pix[0] + pix[2] * pix[1] + pix[2] * pix[2]));
            if (dist>threshold) {
                imgResult.at<cv::Vec3b>(j,i) = videoFrame.at<cv::Vec3b>(j,i);
            }
            else {
                imgResult.at<cv::Vec3b>(j,i) = replacingImage.at<cv::Vec3b>(j,i);
            }
        }
    }
}

}
