#ifndef _BW_FILTER_TASK_HPP_
#define _BW_FILTER_TASK_HPP_

#include <opencv2/opencv.hpp>

class BWFilterTask {
public:
    void operator()(const cv::Mat & inputFrame, cv::Mat &);
};

#endif
