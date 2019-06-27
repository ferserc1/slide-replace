

#include <worker.hpp>
#include <iostream>
#include <vector>
#include <thread>


Worker::Worker(const path & src, const path & dst, int threads)
    :_src(src)
    ,_dst(dst)
    ,_numberOfThreads(threads==0 ? std::thread::hardware_concurrency() : threads)
{
}

void Worker::run() {
    bool useLambdas = _taskFunction != nullptr;
    bool useTask = _taskInstance != nullptr && !useLambdas;
    
    if (!useLambdas && !useTask) {
        throw new std::invalid_argument("No task function specified");
    }
    
    openVideos();
    if (!_capture.isOpened()) {
        throw new std::ios_base::failure("The video capture is not ready");
    }
    
    if (!_writer.isOpened()) {
        throw new std::ios_base::failure("The video writer is not ready");
    }
    
    for (uint32_t currentPass = 0; currentPass < _taskInstance->numberOfPasses(); ++currentPass) {
        std::cout << "Starting pass " << (currentPass + 1) << " of " << _taskInstance->numberOfPasses() << std::endl;

        std::vector<cv::Mat> frames;
        std::vector<cv::Mat> results;
        std::vector<std::thread> workers;
        std::mutex mutex;
        bool done = false;
        int curFrame = 0;
        while (!done) {
            frames.clear();
            results.clear();
            workers.clear();
            for (auto i = 0; i<_numberOfThreads; ++i) {
                cv::Mat videoFrame;
                _capture >> videoFrame;
                if (videoFrame.empty()) {
                    done = true;
                }
                else {
                    frames.push_back(videoFrame);
                    results.push_back(cv::Mat::zeros(videoFrame.rows, videoFrame.cols, CV_8UC3));
                }
            }
            
            if (useLambdas && _setupFunction) {
                _setupFunction();
            }
            else if (useTask) {
                _taskInstance->setup(frames,currentPass);
            }
            
            for (auto i = 0; i<frames.size(); ++i) {
                workers.push_back(std::thread([&](int index) {
                    const auto & frame = frames[index];
                    cv::Mat newFrame = cv::Mat::zeros(frame.rows, frame.cols, CV_8UC3);
                    
                    if (useLambdas) {
                        _taskFunction(frame,newFrame);
                    }
                    else if (useTask) {
                        _taskInstance->execute(frame,newFrame,mutex,currentPass);
                    }
                    
                    {
                        std::lock_guard<std::mutex> l(mutex);
                        results[index] = newFrame;
                        ++curFrame;
                        if (curFrame%20==0) {
                            std::cout << "Frame " << curFrame << std::endl;
                        }
                    }
                }, i));
            }
            
            
            for (auto & w : workers) {
                w.join();
            }
            
            // Write output
            for (auto i = 0; i<frames.size(); ++i) {
                _writer << results[i];
            }
        }
        std::cout << "Pass " << (currentPass + 1) << " done" << std::endl << std::endl;
        
        // Open videos againt to go to the starting position (seek to frame 0 is not working)
        if (currentPass<_taskInstance->numberOfPasses() - 1) {
            openVideos();
        }
    }
}

void Worker::openVideos() {
    if (_capture.isOpened()) {
        _capture.release();
    }
    
    if (_writer.isOpened()) {
        _writer.release();
    }
    
    _capture.open(_src.toString());
    cv::Size size(
        static_cast<int>(_capture.get(cv::CAP_PROP_FRAME_WIDTH)),
        static_cast<int>(_capture.get(cv::CAP_PROP_FRAME_HEIGHT))
    );
    int ex = static_cast<int>(_capture.get(cv::CAP_PROP_FOURCC));
    _writer.open(_dst.toString(), ex, _capture.get(cv::CAP_PROP_FPS), size, true);
}

