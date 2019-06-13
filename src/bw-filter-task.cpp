
#include <bw-filter-task.hpp>

void BWFilterTask::operator()(const cv::Mat & inputFrame, cv::Mat & outputFrame) {
    for (int j = 0; j<inputFrame.rows; ++j) {
        for (int i = 0; i<inputFrame.cols; ++i) {
            cv::Vec3b pix = inputFrame.at<cv::Vec3b>(j, i);
            uint8_t bw = static_cast<uint8_t>(
                0.30f * static_cast<float>(pix[0]) +
                0.59f * static_cast<float>(pix[1]) +
                0.11f * static_cast<float>(pix[2])
            );
            
            outputFrame.at<cv::Vec3b>(j,i) = cv::Vec3b(bw,bw,bw);
        }
    }
}
