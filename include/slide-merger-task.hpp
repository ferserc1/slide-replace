
#ifndef _SLIDE_MERGER_TASK_HPP_
#define _SLIDE_MERGER_TASK_HPP_

#include <opencv2/opencv.hpp>

#include <string>
#include <vector>

class SlideMergerTask {
public:
    void operator()(const cv::Mat &, cv::Mat &);

    inline void setInputPath(const std::string & v) { _inputPath = v; }
    inline void setInputPrefix(const std::string & v) { _inputPrefix = v; }
    inline void setImageExtension(const std::string & v) { _imageExtension = v; }
    inline void setStartIndex(int startIndex) { _startIndex = startIndex; }
    inline const std::string & inputPath() const { return _inputPath; }
    inline const std::string & inputPrefix() const { return _inputPrefix; }
    inline const std::string & imageExtension() const { return _imageExtension; }
    inline int startIndex() const { return _startIndex; }
    
protected:
    std::string _inputPath = "";
    std::string _inputPrefix = "frame_";
    std::string _imageExtension = "jpg";
    int _startIndex = 0;
    
    void loadImages(std::vector<cv::Mat> & result);
};

#endif

