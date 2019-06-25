# SlideCastFixer

placeholder description here

# Usage

```
placeholder
```

## Dependencies

Video Slide Merger depends on OpenCV. You also need CMake to generate the project files.

## Build

### Linux

Install OpenCV dependencies from sources or using a package manager (line apt-get). Build using CMake and make:

```
cd slide-replace
mkdir build
cd build
cmake ..
make
```

### macOS

Install OpenCV dependencies from sources, binaries or using homebrew.

You can build the project using make. You can also use Xcode to build and debug the project:

```
cd slide-replace
mkdir build
cd build
cmake .. -G Xcode
open slide-replace.xcodeproj
```

### Windows

Get the OpenCV binaries for your Visual Studio version and use CMake GUI to generate the Visual Studio project.

