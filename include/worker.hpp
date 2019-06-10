#ifndef _WORKER_
#define _WORKER_

#include <functional>

class Worker {
public:
    typedef std::function<void()> TaskFunction;
    
    Worker(int threads = 0);
    
    inline void setPreAction(TaskFunction f) { _preAction = f; }
    inline void setParallelAction(TaskFunction f) { _parallelAction = f; }
    
    void run();

protected:
    TaskFunction _preAction;
    TaskFunction _parallelAction;
    int _numberOfThreads;
};

#endif
