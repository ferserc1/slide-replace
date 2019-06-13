
#include <path.hpp>

Path Path::pathAddingComponent(const std::string & component) const {
    Path result(_pathString);
    result.addComponent(component);
    return result;
}

Path Path::pathAddingExtension(const std::string & extension) const {
    Path result(_pathString);
    result.addExtension(extension);
    return result;
}

std::string Path::fileName() {
    std::string result;
    auto index = _pathString.find_last_of('/');
    if (index!=std::string::npos) {
        result = _pathString.substr(index + 1);
    }
    return result;
}

std::string Path::extension() {
    std::string result;
    auto index = _pathString.find_last_of('.');
    if (index!=std::string::npos) {
        result = _pathString.substr(index + 1);
    }
    return result;
}

Path & Path::addComponent(const std::string & comp) {
    std::string fixedComp = comp;
    std::replace(fixedComp.begin(), fixedComp.end(), '\\', '/');
    
    if (_pathString.length()==0) {
        _pathString = comp;
    }
    else if (_pathString.back()=='/' && comp.front()=='/') {
        _pathString = _pathString.substr(0, _pathString.size() - 1);
        _pathString += comp;
    }
    else if ((_pathString.back()=='/' && comp.front()!='/') ||
             (_pathString.back()!='/' && comp.front()=='/'))
    {
        _pathString += comp;
    }
    else {
        _pathString += '/' + comp;
    }
    fixSlashes();
    return *this;
}

Path & Path::addExtension(const std::string & ext) {
    if (_pathString.back()=='/') {
        _pathString = _pathString.substr(0, _pathString.size() - 1);
    }
    if (ext.front()=='.') {
        _pathString += ext;
    }
    else {
        _pathString += '.' + ext;
    }
    return *this;
}

Path & Path::removeLastComponent() {
    auto index = _pathString.find_last_of('/');
    if (index!=std::string::npos) {
        _pathString = _pathString.substr(0,index);
    }
    return *this;
}

Path & Path::removeExtension() {
    auto index = _pathString.find_last_of('.');
    if (index!=std::string::npos) {
        _pathString = _pathString.substr(0, index);
    }
    return *this;
}


