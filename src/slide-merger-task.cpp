
#include <slide-merger-task.hpp>
#include <tools.hpp>

TaskRegistrar<SlideMergerTask> task("slideMerger");

void SlideMergerTask::setCommandLine(int argc, const char ** argv) {
    const cv::String keys =
    "{help h                      |                 | show this message }"
    "{originalImagePath op        |                 | path with the original images        }"
    "{replacingImagePath rp       |                 | path with the replacing images       }"
    "{startIndex i                | 0               | value used as starting index         }"
    "{originalPrefix opre         |frame_           | prefix used in the original images   }"
    "{replacingPrefix rpre        |frame_alt_       | prefix used in the replacing images  }"
    "{imageExtension ext          |jpg              | image file type extension            }"
    ;
    
    cv::CommandLineParser parser(argc,argv,keys);
    
    if (parser.has("help")) {
        parser.printMessage();
        throw std::runtime_error("Execution aborted");
    }
    
    path originalImagePath = videoIn().pathRemovingLastComponent();
    if (parser.has("originalImagePath")) {
        originalImagePath = parser.get<cv::String>("originalImagePath");
    }
    
    path replacingImagePath = originalImagePath;
    if (parser.has("replacingImagePath")) {
        replacingImagePath = parser.get<cv::String>("replacingImagePath");
    }
    
    setOriginalImagePath(originalImagePath);
    setModifiedImagePath(replacingImagePath);
    
    auto startIndex = parser.get<int>("startIndex");
    std::string originalPrefix = parser.get<cv::String>("originalPrefix");
    std::string replacingPrefix = parser.get<cv::String>("replacingPrefix");
    std::string imageExtension = parser.get<cv::String>("imageExtension");
    
    setStartIndex(startIndex);
    setImagePrefix(originalPrefix);
    setModifiedImagePrefix(replacingPrefix);
    setImageExtension(imageExtension);
    
    loadResources();
}

void SlideMergerTask::setup(const std::vector<cv::Mat> & inputFrames, uint32_t passIndex) {
    //std::cout << "Setup test" << std::endl;
    _slideImage = &inputFrames[0];
}

void SlideMergerTask::execute(const cv::Mat & inputFrame, cv::Mat & outputFrame, uint32_t frameIndex, std::mutex & mutex, uint32_t passIndex) {
    uint32_t currentImage = static_cast<uint32_t>(closestImageIndex(inputFrame, _originalImages, _currentImage));
    if (currentImage!=_currentImage) {
        _slideImage = &inputFrame;
    }

    
    
    cv::Mat difference;
    cv::absdiff(inputFrame, *_slideImage, difference);
    
    cv::Mat translatedImage = _modifiedImages[currentImage];
    outputFrame = cv::Mat::zeros(difference.rows, difference.cols, CV_8UC3);
    
    
    tools::combineImages(inputFrame, difference, translatedImage, _treshold, outputFrame);
    
    {
        std::lock_guard<std::mutex> lock(mutex);
        _currentImage = _currentImage>currentImage ? _currentImage : currentImage;
    }
}

void SlideMergerTask::loadResources() {
    if (_originalImagePath.empty()) {
        throw std::invalid_argument("Invalid original image path");
    }
    if (_modifiedImagePath.empty()) {
        _modifiedImagePath = _originalImagePath;
    }
    
    _originalImages.clear();
    _modifiedImages.clear();
    loadImages(_originalImagePath, _imagePrefix, _imageExtension, _originalImages);
    loadImages(_modifiedImagePath, _modifiedImagePrefix, _imageExtension, _modifiedImages);
}

void SlideMergerTask::loadImages(const path & basePath, const std::string & imgPrefix, const std::string & extension, std::vector<cv::Mat> & result) {
    for (int i = _startIndex ; ; ++i) {
        path imgPath = basePath.pathAddingComponent(imgPrefix + std::to_string(i))
                .addExtension(extension);
        std::cout << "Read image file: " << imgPath.toString() << std::endl;
        cv::Mat image = cv::imread(imgPath.toString(), cv::IMREAD_COLOR);
        if (!image.empty()) {
            result.push_back(image);
        }
        else {
            break;
        }
    }
}

size_t SlideMergerTask::closestImageIndex(const cv::Mat & source, std::vector<cv::Mat> & images, size_t lastIndex) {
    auto indexL = lastIndex - 1;
    auto indexC = lastIndex;
    auto indexR = lastIndex + 1;
    cv::Mat * imL = indexL>=0 && indexL<images.size() ? &images[indexL] : nullptr;
    cv::Mat * imC = indexC>=0 && indexC<images.size() ? &images[indexC] : nullptr;
    cv::Mat * imR = indexR>=0 && indexR<images.size() ? &images[indexR] : nullptr;
    double similarityL = std::numeric_limits<double>::max();
    double similarityR = similarityL;
    double similarityC = similarityC;
    
    if (imL) {
        similarityL = tools::imageSimilarity(source, *imL);
    }
    if (imC) {
        similarityC = tools::imageSimilarity(source, *imC);
    }
    if (imR) {
        similarityR = tools::imageSimilarity(source, *imR);
    }
    
    if (similarityL<=similarityC && similarityC<=similarityR) {
        return indexL;
    }
    else if (similarityC<=similarityL && similarityC<=similarityR) {
        return indexC;
    }
    else {
        return indexR;
    }
}

