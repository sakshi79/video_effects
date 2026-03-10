cmake_minimum_required(VERSION 3.16)

project(realtime_effects)
find_package(OpenCV REQUIRED)

add_executable(realtime_effects
    vidDisplay.cpp
    filter.cpp
    faceDetect/faceDetect.cpp
)


target_include_directories(realtime_effects PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(realtime_effects ${OpenCV_LIBS})