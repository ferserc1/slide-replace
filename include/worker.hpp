#ifndef _WORKER_
#define _WORKER_

#include <functional>
#include <memory>

#include <opencv2/opencv.hpp>

#include <task.hpp>
#include <path.hpp>

class Worker {
public:
    typedef std::function<void()> SetupFunction;
    typedef std::function<void(const cv::Mat &, cv::Mat &)> TaskFunction;
    
    Worker(const path & src, const path & dst, int threads = 0);
    
    // Option 1: using lambdas
    inline void setSetup(SetupFunction f) { _setupFunction = f; }
    inline void setTask(TaskFunction f) { _taskFunction = f; }
    
    // Option 2: using a task instance
    inline void setTask(Task * task) { _taskInstance = task; }
    
    void run();

protected:
    SetupFunction _setupFunction;
    TaskFunction _taskFunction;
    Task * _taskInstance;
    path _src;
    path _dst;
    cv::VideoCapture _capture;
    cv::VideoWriter _writer;
    int _numberOfThreads;
    
    void openVideos();
};

#endif
