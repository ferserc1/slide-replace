#include <task.hpp>

TaskFactory * TaskFactory::s_singleton = nullptr;

TaskFactory * TaskFactory::Get() {
    if (!s_singleton) {
        s_singleton = new TaskFactory();
    }
    return s_singleton;
}

Task * TaskFactory::Instantiate(const std::string & task, const std::string & videoIn, const std::string & videoOut) {
    return TaskFactory::Get()->instantiate(task,videoIn,videoOut);
}

void TaskFactory::registerFactory(const std::string & name, FactoryLambda factory) {
    _factoryMap[name] = factory;
}

Task * TaskFactory::instantiate(const std::string & task, const std::string & videoIn, const std::string & videoOut) {
    if (_factoryMap.find(task) == _factoryMap.end()) {
        throw std::invalid_argument("No such factory with name " + task);
    }
    Task * result = _factoryMap[task]();
    result->setupPaths(videoIn,videoOut);
    return result;
}
