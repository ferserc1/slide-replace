#ifndef _TIMEDSLIDE_REPLACE_TASK_HPP_
#define _TIMEDSLIDE_REPLACE_TASK_HPP_

#include <task.hpp>

#include <opencv2/opencv.hpp>

#include <path.hpp>

#include <string>
#include <vector>

class TimedSlideReplaceTask : public Task {
public:
    void execute(const cv::Mat &, cv::Mat &, uint32_t frameIndex, std::mutex & mutex, uint32_t passIndex);
    
    int32_t numberOfPasses() { return 2; }
    
    inline void setCommandLine(int argc, char ** argv) { setCommandLine(argc, const_cast<const char **>(argv)); }
    void setCommandLine(int argc, const char ** argv);
        
private:    
    cv::Mat _searchImage;
    cv::Mat _replacingImage;
    float _timestamp;
    float _treshold;
    uint32_t _expectedframe;
};

#endif