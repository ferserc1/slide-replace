
#include <slide-merger-task.hpp>

bool SlideMergerTask::setCommandLine(int argc, const char ** argv) {
    // TODO: Setup using command line
    path inputImagePath = "/Users/fernando/Downloads/video-data";
    setOriginalImagePath(inputImagePath);
    
    loadResources();
    
    return true;
}

void SlideMergerTask::setup() {
    std::cout << "Setup test" << std::endl;
}

void SlideMergerTask::execute(const cv::Mat & inputFrame, cv::Mat & outputFrame) {
    uint32_t currentImage = closestImageIndex(inputFrame, _originalImages, _currentImage);
    if (currentImage!=_currentImage) {
        
    }
    
    {
        std::lock_guard<std::mutex> lock(_imageMutex);
        _currentImage = currentImage;
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
    loadImages(_modifiedImagePath, _imagePrefix, _imageExtension, _modifiedImages);
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

double SlideMergerTask::imageSimilarity(const cv::Mat & img1, const cv::Mat & img2) {
    cv::Mat diff;
    absdiff(img1, img2, diff);
    bitwise_not(diff, diff);
    std::vector<cv::Mat> planes;
    split(diff, planes);
    
    int histSize = 256;
    float range[] = { 0.0f, 256.0f };
    const float * histRange = { range };
    cv::Mat bHist, gHist, rHist;
    cv::calcHist(&planes[0], 1, 0, cv::Mat(), bHist, 1, &histSize, &histRange, true, false);
    cv::calcHist(&planes[1], 1, 0, cv::Mat(), gHist, 1, &histSize, &histRange, true, false);
    cv::calcHist(&planes[2], 1, 0, cv::Mat(), rHist, 1, &histSize, &histRange, true, false);
    
    float value = 0.0f;
    for (int i = 0; i<histSize/3; ++i) {
        auto histColor = (bHist.at<float>(i) + gHist.at<float>(i) + bHist.at<float>(i)) / 3.0f;
        value += histColor;
    }
    return value;
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
        similarityL = imageSimilarity(source, *imL);
    }
    if (imC) {
        similarityC = imageSimilarity(source, *imC);
    }
    if (imR) {
        similarityR = imageSimilarity(source, *imR);
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

void SlideMergerTask::combineTranslatedVideo(const cv::Mat & videoFrame, const cv::Mat & difference, const cv::Mat & translatedImage, float threshold, cv::Mat & imgResult) {
    float dist;
    for (int j = 0; j<videoFrame.rows; ++j) {
        for (int i = 0; i<videoFrame.cols; ++i) {
            cv::Vec3b pix = difference.at<cv::Vec3b>(j,i);
            
            dist = sqrtf(pix[0] * pix[0] + pix[2] * pix[1] + pix[2] * pix[2]);
            if (dist>threshold) {
                imgResult.at<cv::Vec3b>(j,i) = videoFrame.at<cv::Vec3b>(j,i);
            }
            else {
                imgResult.at<cv::Vec3b>(j,i) = translatedImage.at<cv::Vec3b>(j,i);
            }
        }
    }
}

