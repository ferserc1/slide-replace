cmake_minimum_required(VERSION 2.8)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/worker.cpp"
)
set(HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)


project(slide-replace)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${HEADERS})
add_executable(slide-replace ${SOURCE})
target_link_libraries(slide-replace ${OpenCV_LIBS})
