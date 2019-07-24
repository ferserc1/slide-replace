

#include <worker.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>


void Worker::VideoData::setVideoData(const cv::VideoCapture &cap) {
    if (cap.isOpened()) {
        _fps = static_cast<uint32_t>(cap.get(cv::CAP_PROP_FPS));
        _frameCount = static_cast<uint32_t>(cap.get(cv::CAP_PROP_FRAME_COUNT));
        _width = static_cast<uint32_t>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
        _height = static_cast<uint32_t>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    }
    else {
        _fps = 0;
        _frameCount = 0;
        _width = 0;
        _height = 0;
    }
}

Worker::Worker(const path & src, const path & dst, int threads)
    :_src(src)
    ,_dst(dst)
    ,_numberOfThreads(threads==0 ? std::thread::hardware_concurrency() : threads)
{
}

void Worker::run(int argc, const char ** argv) {
	auto t0 = std::chrono::high_resolution_clock::now();

    bool useLambdas = _taskFunction != nullptr;
    bool useTask = _taskInstance != nullptr && !useLambdas;
    
	if (_taskInstance) {
		_taskInstance->setCommandLine(argc, argv);
	}

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
    
    for (int currentPass = 0; currentPass < _taskInstance->numberOfPasses(); ++currentPass) {
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
					mutex.lock();
                    const auto & frame = frames[index];
					auto thisFrameIndex = curFrame + index;
					mutex.unlock();
                    cv::Mat newFrame = cv::Mat::zeros(frame.rows, frame.cols, CV_8UC3);
                    
                    if (useLambdas) {
                        _taskFunction(frame,newFrame);
                    }
                    else if (useTask) {
                        _taskInstance->execute(frame,newFrame,thisFrameIndex,mutex,currentPass);
                    }
                    
					{
						std::lock_guard<std::mutex> l(mutex);
						results[index] = newFrame;
						if (thisFrameIndex % 20 == 0) {
							std::cout << "Frame " << thisFrameIndex << std::endl;
						}
					}
                }, i));
            }

            for (auto & w : workers) {
                w.join();
            }

			curFrame += _numberOfThreads;
            
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


	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::high_resolution_clock::now() - t0);
	std::cout << "Completed in " << diff.count() / 1000 << " seconds" << std::endl;
}

void Worker::openVideos() {
    if (_capture.isOpened()) {
        _capture.release();
    }
    
    if (_writer.isOpened()) {
        _writer.release();
    }
    
    _capture.open(_src.toString());
    if (!_capture.isOpened()) {
        throw std::ios::failure("Error opening source video.");
    }
    videoData.setVideoData(_capture);
    cv::Size size(
        static_cast<int>(_capture.get(cv::CAP_PROP_FRAME_WIDTH)),
        static_cast<int>(_capture.get(cv::CAP_PROP_FRAME_HEIGHT))
    );
    int ex = static_cast<int>(_capture.get(cv::CAP_PROP_FOURCC));
    _writer.open(_dst.toString(), ex, _capture.get(cv::CAP_PROP_FPS), size, true);
    if (!_writer.isOpened()) {
        throw std::ios::failure("Error opening destination video.");
    }
}

