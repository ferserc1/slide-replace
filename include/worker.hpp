#ifndef _WORKER_
#define _WORKER_

#include <functional>

#include <opencv2/opencv.hpp>

class Worker {
public:
    typedef std::function<void(const cv::Mat &, cv::Mat &)> TaskFunction;
    
    Worker(cv::VideoCapture & cap, cv::VideoWriter & writer, int threads = 0);
    
    inline void setTask(TaskFunction f) { _taskFunction = f; }
    
    void run();

protected:
    TaskFunction _taskFunction;
    cv::VideoCapture _capture;
    cv::VideoWriter & _writer;
    int _numberOfThreads;
};

#endif
