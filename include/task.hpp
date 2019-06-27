#ifndef _TASK_HPP_
#define _TASK_HPP_

#include <opencv2/opencv.hpp>
#include <vector>
#include <functional>
#include <mutex>
#include <map>

#include <path.hpp>

class Task {
public:

    inline virtual void setupPaths(const std::string & videoIn, const std::string & videoOut) { _videoIn = videoIn; _videoOut = videoOut; }
    inline const path & videoIn() const { return _videoIn; }
    inline const path & videoOut() const { return _videoOut; }
    
    virtual void setCommandLine(int argc,const char ** argv) {}
    virtual void setup(const std::vector<cv::Mat> &, uint32_t passIndex) {}
    virtual void execute(const cv::Mat &, cv::Mat &, std::mutex & mutex, uint32_t passIndex) {};
    virtual int32_t numberOfPasses() { return 1; }

    virtual ~Task() {}

protected:
    path _videoIn;
    path _videoOut;
};

class TaskFactory {
public:
    typedef std::function<Task *()> FactoryLambda;
    
    static TaskFactory * Get();
    
    static Task * Instantiate(const std::string & task, const std::string & videoIn, const std::string & videoOut);
    
    void registerFactory(const std::string & name, FactoryLambda factory);
    
    Task * instantiate(const std::string & task, const std::string & videoIn, const std::string & videoOut);
    
protected:
    static TaskFactory * s_singleton;
    
    std::unordered_map<std::string,FactoryLambda> _factoryMap;
};

template <class T>
class TaskRegistrar {
public:
    TaskRegistrar(const std::string & taskName) {
        TaskFactory::Get()->registerFactory(taskName, []() { return new T(); });
    }
};

#endif
