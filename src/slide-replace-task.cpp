
#include <slide-replace-task.hpp>

TaskRegistrar<SlideReplaceTask> slideReplaceFactory("slideReplace");

void SlideReplaceTask::setCommandLine(int argc, const char **argv) {
    const cv::String keys =
    "{help h                   |                      | show this message                                   }"
    "{searchImage s            |                      | image that will be searched in the video to replace }"
    "{replacingImage r         |                      | mage that will be replaced in the video             }"
    ;
    
    cv::CommandLineParser parser(argc,argv,keys);
    
    if (!parser.has("searchImage")) {
        parser.printMessage();
        throw std::invalid_argument("searchImage parameter is required");
    }
    
    if (!parser.has("")) {
        parser.printMessage();
        throw std::invalid_argument("replacingImage parameter is required");
    }
    
    std::string searchImagePath = parser.get<cv::String>("searchImage");
    std::string replacingImagePath = parser.get<cv::String>("replacingImage");
    
    _searchImage = cv::imread(searchImagePath, cv::IMREAD_COLOR);
    if (_searchImage.empty()) {
        throw std::ios_base::failure("No such image with name " + searchImagePath);
    }
    
    _replacingImage = cv::imread(replacingImagePath, cv::IMREAD_COLOR);
    if (_replacingImage.empty()) {
        throw std::ios_base::failure("No such image with name " + replacingImagePath);
    }
    
    _tresshold = 10.0f; // TODO: find a good thressold
}

void SlideReplaceTask::setup(const std::vector<cv::Mat> &) {
    
}

void SlideReplaceTask::execute(const cv::Mat & srcImage, cv::Mat & dstImage, std::mutex &mutex) {
    // Compare input image
    
    // if thresshold<_thresshold
    //    extract difference
    //    combine replacing image with the difference into dstImage
}
