
#include <worker.hpp>
#include <path.hpp>
#include <bw-filter-task.hpp>
#include <slide-merger-task.hpp>

int main(int argc, char ** argv) {
    path testVideo = "/Users/fernando/Downloads/video-data/presentacion.mp4";
    path outTestVideo = "/Users/fernando/Downloads/video-data/out.mp4";
    
    cv::VideoCapture cap(testVideo.toString());
    cv::VideoWriter outputVideo;
    cv::Size size(static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH)), static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT)));
    int ex = static_cast<int>(cap.get(cv::CAP_PROP_FOURCC));
    outputVideo.open(outTestVideo.toString(), ex, cap.get(cv::CAP_PROP_FPS), size, true);

    
    Worker w(cap,outputVideo);
    
    std::unique_ptr<SlideMergerTask> slideMerger(new SlideMergerTask());
    
    slideMerger->setCommandLine(argc, argv);
    
    w.setTask(slideMerger.get());
    //w.setSetup(slideMerger);
    //w.setTask(slideMerger);
    
    w.run();
    
    return 0;
}
