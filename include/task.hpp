#ifndef _TASK_HPP_
#define _TASK_HPP_

#include <opencv2/opencv.hpp>
#include <vector>

class Task {
public:
    virtual void setup(const std::vector<cv::Mat> &) {}
    virtual void execute(const cv::Mat &, cv::Mat &) = 0;

    virtual ~Task() {}
};

#endif
