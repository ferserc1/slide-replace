
#ifndef _SLIDE_REPLACE_TASK_HPP_
#define _SLIDE_REPLACE_TASK_HPP_

#include <task.hpp>

#include <opencv2/opencv.hpp>

#include <path.hpp>

#include <string>
#include <vector>

class SlideReplaceTask : public Task {
public:
    void execute(const cv::Mat &, cv::Mat &, std::mutex & mutex);
    
    inline void setCommandLine(int argc, char ** argv) { setCommandLine(argc, const_cast<const char **>(argv)); }
    void setCommandLine(int argc, const char ** argv);
        
private:
    cv::Mat _searchImage;
    cv::Mat _replacingImage;
    float _treshold;
};

#endif

