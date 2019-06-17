

#include <worker.hpp>
#include <iostream>
#include <vector>
#include <thread>


Worker::Worker(cv::VideoCapture & cap, cv::VideoWriter & writer, int threads)
    :_capture(cap)
    ,_writer(writer)
    ,_numberOfThreads(threads==0 ? std::thread::hardware_concurrency() : threads)
{
}

void Worker::run() {
    bool useLambdas = _taskFunction != nullptr;
    bool useTask = _taskInstance != nullptr && !useLambdas;
    
    if (!useLambdas && !useTask) {
        throw new std::invalid_argument("No task function specified");
    }
    
    if (!_capture.isOpened()) {
        throw new std::ios_base::failure("The video capture is not ready");
    }
    
    if (!_writer.isOpened()) {
        throw new std::ios_base::failure("The video writer is not ready");
    }
    
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
            _taskInstance->setup();
        }
        
        for (auto i = 0; i<frames.size(); ++i) {
            workers.push_back(std::thread([&](int index) {
                // TODO: do something with input frame
                const auto & frame = frames[index];
                cv::Mat newFrame = cv::Mat::zeros(frame.rows, frame.cols, CV_8UC3);
                
                if (useLambdas) {
                    _taskFunction(frame,newFrame);
                }
                else if (useTask) {
                    _taskInstance->execute(frame,newFrame);
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
}

