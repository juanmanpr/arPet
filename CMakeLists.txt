cmake_minimum_required(VERSION 2.8)
project( arPet )
find_package( OpenCV REQUIRED )
add_library(PointCloud src/PointCloud.cpp)
add_executable( arPet src/main.cpp )
target_link_libraries( arPet ${OpenCV_LIBS} PointCloud)