#ifndef _TASK_HPP_
#define _TASK_HPP_

#include <opencv2/opencv.hpp>
#include <vector>
#include <functional>
#include <mutex>

class Task {
public:

    virtual void setCommandLine(int argc,const char ** argv) {}
    virtual void setup(const std::vector<cv::Mat> &) {}
    virtual void execute(const cv::Mat &, cv::Mat &, std::mutex & mutex) = 0;

    virtual ~Task() {}
};

#endif
