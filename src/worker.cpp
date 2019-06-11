

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
    if (!_capture.isOpened()) {
        throw new std::ios_base::failure("The video capture is not ready");
    }
    
    if (!_writer.isOpened()) {
        throw new std::ios_base::failure("The video writer is not ready");
    }
    
    if (!_taskFunction) {
        _taskFunction = [](const cv::Mat & inputFrame, cv::Mat & outputFrame) {
            for (int j = 0; j<inputFrame.rows; ++j) {
                for (int i = 0; i<inputFrame.cols; ++i) {
                    cv::Vec3b pix = inputFrame.at<cv::Vec3b>(j,i);
                    outputFrame.at<cv::Vec3b>(j,i) = inputFrame.at<cv::Vec3b>(j,i);
                }
            }
        };
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
        
        for (auto i = 0; i<frames.size(); ++i) {
            workers.push_back(std::thread([&](int index) {
                // TODO: do something with input frame
                const auto & frame = frames[index];
                cv::Mat newFrame = cv::Mat::zeros(frame.rows, frame.cols, CV_8UC3);
                _taskFunction(frame,newFrame);
                
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

