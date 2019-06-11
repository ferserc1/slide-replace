
#include <worker.hpp>

class MyTask {
public:
    void operator()(const cv::Mat & inputFrame, cv::Mat & outputFrame) {
        for (int j = 0; j<inputFrame.rows; ++j) {
            for (int i = 0; i<inputFrame.cols; ++i) {
                cv::Vec3b pix = inputFrame.at<cv::Vec3b>(j,i);
                outputFrame.at<cv::Vec3b>(j,i) = inputFrame.at<cv::Vec3b>(j,i);
            }
        }
    }
};

int main(int argc, char ** argv) {
    std::string testVideo = "/Users/fernando/Downloads/video-data/presentacion.mp4";
    std::string outTestVideo = "/Users/fernando/Downloads/video-data/out.mp4";
    
    cv::VideoCapture cap(testVideo);
    cv::VideoWriter outputVideo;
    cv::Size size(static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH)), static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT)));
    int ex = static_cast<int>(cap.get(cv::CAP_PROP_FOURCC));
    outputVideo.open(outTestVideo, ex, cap.get(cv::CAP_PROP_FPS), size, true);
    
    Worker w(cap,outputVideo);
    
//    w.setTask([](const cv::Mat & inputFrame, cv::Mat & outputFrame) {
//        for (int j = 0; j<inputFrame.rows; ++j) {
//            for (int i = 0; i<inputFrame.cols; ++i) {
//                cv::Vec3b pix = inputFrame.at<cv::Vec3b>(j,i);
//                outputFrame.at<cv::Vec3b>(j,i) = inputFrame.at<cv::Vec3b>(j,i);
//            }
//        }
//    });
    
    w.setTask(MyTask());
    
    w.run();
    
    return 0;
}
