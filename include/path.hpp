
#ifndef _path_HPP_
#define _path_HPP_

#include <string>
#include <algorithm>

class path {
public:
    path() {}
    path(const std::string & p) :_pathString(p) { fixSlashes(); }
    path(const char * p) :_pathString(p) { fixSlashes(); }
    path(const path & p) :_pathString(p.toString()) { fixSlashes(); }

    inline const std::string & toString() const { return _pathString; }

    path pathAddingComponent(const std::string & component) const;
    path pathAddingExtension(const std::string & extension) const;
    path pathRemovingLastComponent() const;
    path pathRemovingExtension() const;

    path & addComponent(const std::string & comp);
    path & addExtension(const std::string & ext);
    path & removeLastComponent();
    path & removeExtension();
    
    std::string fileName();
    std::string extension();
    
    inline bool empty() const { return _pathString.empty(); }
    
    inline void operator=(const path & p) {
        _pathString = p._pathString;
    }
    
    inline void operator=(const std::string & p) {
        _pathString = p;
        fixSlashes();
    }
    
    inline void operator=(const char * p) {
        _pathString = p;
        fixSlashes();
    }

protected:
    std::string _pathString;
    
    inline void fixSlashes() { std::replace(_pathString.begin(), _pathString.end(), '\\', '/'); }
};

#endif
