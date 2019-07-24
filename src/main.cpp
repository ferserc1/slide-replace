
#include <worker.hpp>
#include <path.hpp>
#include <bw-filter-task.hpp>
#include <slide-merger-task.hpp>

#include <chrono>

int main(int argc, char ** argv) {
    const cv::String keys =
    "{help h usage ?     |             | print this message                 }"
    "{@input             |             | video to process                   }"
    "{output o           |             | processed video output path        }"
    "{processor p        |slideMerger  | processor to use                   }"
    "{jobs j             |0            | number of threads, 0=number of CPUs}"
    ;
    
    cv::CommandLineParser parser(argc, argv, keys);
    
    std::string inputVideo = parser.get<cv::String>(0);
    std::string processor = parser.get<cv::String>("processor");
    int jobs = parser.get<int>("jobs");
    
    if ((inputVideo.empty() || parser.has("help")) && !parser.has("processor")) {
        parser.printMessage();
        
        return 0;
    }
    std::cout << parser.get<cv::String>(0) << std::endl << processor << std::endl;

    path inputVideoPath = inputVideo;
    path outVideoPath = "";
    if (parser.has("output")) {
        outVideoPath = parser.get<cv::String>("output");
    }
    
    Worker w(inputVideoPath,outVideoPath,jobs);
    
    try {
        std::unique_ptr<Task> task(TaskFactory::Instantiate(processor,inputVideoPath.toString(),outVideoPath.toString()));
        w.setTask(task.get());
       
		w.run(argc, argv);
    }
    catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
