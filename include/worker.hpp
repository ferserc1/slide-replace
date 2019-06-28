#ifndef _WORKER_
#define _WORKER_

#include <functional>
#include <memory>

#include <opencv2/opencv.hpp>

#include <task.hpp>
#include <path.hpp>

class Worker {
public:
    struct VideoData {
        friend class Worker;
        
        const uint32_t fps() const { return _fps; }
        const uint32_t frameCount() const { return _frameCount; }
        const uint32_t width() const { return _width; }
        const uint32_t height() const { return _height; }
        
    private:
        uint32_t _fps = 0;
        uint32_t _frameCount = 0;
        uint32_t _width = 0;
        uint32_t _height = 0;
        
        void setVideoData(const cv::VideoCapture & cap);
    } videoData;
    
    typedef std::function<void()> SetupFunction;
    typedef std::function<void(const cv::Mat &, cv::Mat &)> TaskFunction;
    
    Worker(const path & src, const path & dst, int threads = 0);
    
    // Option 1: using lambdas
    inline void setSetup(SetupFunction f) { _setupFunction = f; }
    inline void setTask(TaskFunction f) { _taskFunction = f; }
    
    // Option 2: using a task instance
    inline void setTask(Task * task) {
        if (_taskInstance && _taskInstance->_worker) {
            _taskInstance->_worker = nullptr;
        }
        _taskInstance = task;
        if (_taskInstance) {
            _taskInstance->_worker = this;
        }
    }
    
    void run(int argc, const char ** argv);
    inline void run(int argc, char ** argv) { run(argc, const_cast<const char **>(argv)); }

protected:
    SetupFunction _setupFunction;
    TaskFunction _taskFunction;
    Task * _taskInstance = nullptr;
    path _src;
    path _dst;
    cv::VideoCapture _capture;
    cv::VideoWriter _writer;
    int _numberOfThreads;
    
    void openVideos();
};

#endif
