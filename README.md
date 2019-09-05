# SlideCastFixer

The idea behind the slidecastfixer (SCF) is to conglomerate a set of tools that help us to fix in postproduction the tipical inconveniences that we find in our slidecast.

Each of this tools is called processor and each processor has its own way of work, but every processor has in common the input and output that will be a slidecast video.

# Usage

```
  slide-replace [params] input  
```
  
The processor launcher has 3 optative params

-o, --output: the path of the processed video 
-p, --processor: the selected processor (slideMerger|slideReplace)
-?,-h, --help,--usage: a flag to show the help

The default value for -p is slideMerger and that is the processor is the one that will be called if we leave the -p param blank.

If we want to see the specific help for a processor we need to set the -p param and add the -h param to the call like this

```
slide-replace -p=slideReplace -h
```
#### slideReplace 

The slideReplace procesor as the name suggest is a video processor that replaces one slide into another in the slidecast. This tool is handy in those situations where the caster has a misprint or a copyrighted image in an slide and we don't want to remake the cast.

```
slide-replace [params] -p=slideReplace input
```

This processor has 4 optional params

-?,-h, --help,--usage: a flag to show the processor help\
-s, --searchImage: the image that we want to find and replace in the slidecast\
-r, --replacingImage: the new image that we will use as substitute\
-t, --treshold: a numeric treshold that will define how much the searched image should resemble the image found in the video (default value 200)\

so a tipical call would look like this 

```
slide-replace -s=original.png -r=new.png -p=slideReplace video-input.mp4
```

#### timedSlideReplace

Our team thought that another way to select the slide that we wanted to replace is using a timestamp where this slide is used. 
The procesor uses the image at the timestamp in the original input.mp4 and uses that as reference afterward search that image in the entire video and replaces it with the replacing image.

```
slide-replace [params] -p=timedSlideReplace
```

This procesor has 2 params

-t, --timestamp: the moment where appears the slide that we want to replace (in seconds)\
-r, --replacingImage: the new image that we will use as substitute

so a tipical call would look like this

```
slide-replace -t=20 -r=new.png -p=timedSlideReplace video-input.mp4
```

#### slideMerger

This is a reimplementation of https://github.com/polimediaupv/video-slide-merger and as the original project replaces the frames of a Power Point presentation video with an alternative version, specified by a sequence of images. It also combine the pointer cursor with the new images.

The processor will also expect two sets of images in the same folder as the video with this format.

A sequence of images, corresponding to the ones in the original video, named as frame_0.jpg, frame_1.jpg... frame_n.jpg
A sequence with the alternative images that will be used as replacement, named as frame_alt_0.jpg, frame_alt_1.jpg... frame_alt_n.jpg

```
slide-replace [params] -p=slideMerger input.mp4
```

This processor has 7  optional params


-?,-h, --help,--usage: a flag to show the processor help\
--ext, --imageExtension: the expected extension of the slide images (default value jpg)\
-i, --startindex: the defalt starting index of the images [frame_0.jpg frame_1.jpg....] by default 0 but in case the set of images is generated with a tool that starts with 1 we can change it here.\
-op, --originalImagePath: in case the original set of images is in a different location than the input video we can set it here, if left blank we will use the input path\
--opre, --originalPrefix: in the example the images are called frame_0 frame_1 in this case the prefix is the part of the name that precedes the index (by default frame_)\
--rp, --replacingImagePath: like the original set of images we can change the location of the new images and like the original images if this parameter is left blank we will search them in the same path as the video\
--rpre, --replacingPrefix: like the original set of images we can change the expected image prefix by default frame_alt_

## Encoding and framerate

Right now the tool ask OpenCV to keep the same encoding and framerate as the original video in the resulting video. 

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

