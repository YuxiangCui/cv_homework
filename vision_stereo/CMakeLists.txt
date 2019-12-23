project(vision_noise)
cmake_minimum_required (VERSION 2.8)

#opencv 
find_package(OpenCV 3.3.1 REQUIRED)


SET(CMAKE_BUILD_TYPE "RELEASE")


add_executable(vision_stereo main.cpp)
target_link_libraries (vision_stereo ${OpenCV_LIBS} )

