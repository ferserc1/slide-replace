#ifndef _TASK_HPP_
#define _TASK_HPP_

#include <opencv2/opencv.hpp>

class Task {
public:
    virtual void setup() {}
    virtual void execute(const cv::Mat &, cv::Mat &) = 0;

    virtual ~Task() {}
};

#endif
