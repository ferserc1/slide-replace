
#ifndef _SLIDE_MERGER_TASK_HPP_
#define _SLIDE_MERGER_TASK_HPP_

#include <opencv2/opencv.hpp>

#include <task.hpp>
#include <path.hpp>

#include <string>
#include <vector>

class SlideMergerTask : public Task {
public:
    // Setup function
    void setup(const std::vector<cv::Mat> &);
    
    // Main task function
    void execute(const cv::Mat &, cv::Mat &, std::mutex & mutex);

    inline void setCommandLine(int argc, char ** argv) { setCommandLine(argc, const_cast<const char **>(argv)); }
    void setCommandLine(int argc, const char ** argv);
    
    inline void setOriginalImagePath(const path & v) { _originalImagePath = v; }
    inline void setModifiedImagePath(const path & v) { _modifiedImagePath = v; }
    inline void setImagePrefix(const std::string & v) { _imagePrefix = v; }
    inline void setModifiedImagePrefix(const std::string & v) { _modifiedImagePrefix = v; }
    inline void setImageExtension(const std::string & v) { _imageExtension = v; }
    inline void setStartIndex(int startIndex) { _startIndex = startIndex; }
    inline const path & originalImagePath() const { return _originalImagePath; }
    inline const path & modifiedImagePath() const { return _modifiedImagePath; }
    inline const std::string & imagePrefix() const { return _imagePrefix; }
    inline const std::string & modifiedImagePrefix() const { return _modifiedImagePrefix; }
    inline const std::string & imageExtension() const { return _imageExtension; }
    inline int startIndex() const { return _startIndex; }
    inline void setTreshold(float t) { _treshold = t; }
    inline float treshold() const { return _treshold; }
    
    void loadResources();
    
protected:
    path _originalImagePath = "";
    path _modifiedImagePath = "";
    std::string _imagePrefix = "frame_";
    std::string _modifiedImagePrefix = "frame_alt_";
    std::string _imageExtension = "jpg";
    float _treshold = 60.0f;
    int _startIndex = 0;
    
    std::vector<cv::Mat> _originalImages;
    std::vector<cv::Mat> _modifiedImages;
    
private:
    uint32_t _currentImage = 0;
    const cv::Mat * _slideImage = nullptr;

    void loadImages(const path & basePath, const std::string & imgPrefix, const std::string & extension, std::vector<cv::Mat> & result);
    double imageSimilarity(const cv::Mat & img1, const cv::Mat & img2);
    size_t closestImageIndex(const cv::Mat & source, std::vector<cv::Mat> & images, size_t lastIndex);
    void combineTranslatedVideo(const cv::Mat & videoFrame, const cv::Mat & difference, const cv::Mat & translatedImage, float threshold, cv::Mat & imgResult);
};

#endif

