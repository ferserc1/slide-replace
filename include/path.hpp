
#ifndef _PATH_HPP_
#define _PATH_HPP_

#include <string>

class Path {
public:
    Path() {}
    Path(const std::string & p) :_pathString(p) { fixSlashes(); }
    Path(const Path & p) :_pathString(p.toString()) { fixSlashes(); }

    inline const std::string & toString() const { return _pathString; }

    Path pathAddingComponent(const std::string & component) const;
    Path pathAddingExtension(const std::string & extension) const;

    Path & addComponent(const std::string & comp);
    Path & addExtension(const std::string & ext);
    Path & removeLastComponent();
    Path & removeExtension();
    
    std::string fileName();
    std::string extension();
    
    inline void operator=(const Path & p) {
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
