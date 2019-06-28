
#include <timedslide-replace-task.hpp>
#include <tools.hpp>
#include <worker.hpp>

TaskRegistrar<TimedSlideReplaceTask> timedSlideReplaceFactory("timedSlideReplace");

void TimedSlideReplaceTask::setCommandLine(int argc, const char **argv) {
    const cv::String keys =
    "{help h                   |                      | show this message                                   }"
    "{timestamp t            |                      | moment that will be used as sample to replace afterwards }"
    "{replacingImage r         |                      | image that will be replaced in the video             }"    
    ;
    
    cv::CommandLineParser parser(argc,argv,keys);
    
    if (!parser.has("timestamp")) {
        parser.printMessage();
        throw std::invalid_argument("timestamp parameter is required");
    }
    
    if (!parser.has("replacingImage")) {
        parser.printMessage();
        throw std::invalid_argument("replacingImage parameter is required");
    }
    
    _timestamp = parser.get<int>("timestamp");
    std::string replacingImagePath = parser.get<cv::String>("replacingImage");
    
    _replacingImage = cv::imread(replacingImagePath, cv::IMREAD_COLOR);
    if (_replacingImage.empty()) {
        throw std::ios_base::failure("No such image with name " + replacingImagePath);
    }
    
    _expectedframe = _timestamp * worker()->videoData.fps();
    if (_expectedframe>worker()->videoData.frameCount()){
        throw std::ios_base::failure("Please select a time not greater than the video length");
    }

    //_let user set threshold?
    _treshold = 200.0f;
    /*if (parser.has("treshold")) {
        _treshold = parser.get<int>("treshold");
    }*/
}

void TimedSlideReplaceTask::execute(const cv::Mat & srcImage, cv::Mat & dstImage, uint32_t frameIndex, std::mutex &mutex, uint32_t passIndex) {
    if (passIndex==0)
    {
                
        if (frameIndex == _expectedframe)
        {
            _searchImage=srcImage;
            std::cout << "Image Found" << std::endl;
        }
    }
    else
    {        
        auto similarity = tools::imageSimilarity(srcImage, _searchImage);
        
        if (similarity<=_treshold) {            
            cv::Mat difference;
            cv::absdiff(srcImage, _searchImage, difference);            
            dstImage = cv::Mat::zeros(difference.rows, difference.cols, CV_8UC3);            
            tools::combineImages(srcImage, difference, _replacingImage, _treshold, dstImage);            
        }
        else {            
            dstImage = srcImage;            
        }
    }    
}
