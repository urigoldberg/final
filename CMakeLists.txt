cmake_minimum_required(VERSION 3.7)
project(final_copy_2)

set(CMAKE_CXX_STANDARD 11)
find_package( OpenCV REQUIRED xfeatures2d features2d highgui imgcodecs imgproc core)

set(SOURCE_FILES
        unit_tests/kd_array_unit_test.c
        unit_tests/sp_logger_unit_test.c
        unit_tests/tempTest.c
        unit_tests/unit_test_util.h
        AllocateManager.c
        AllocateManager.h
        kdTree.c
        kdTree.h
        main.cpp
        SPBPriorityQueue.c
        SPBPriorityQueue.h
        SPConfig.c
        SPConfig.h
        SPImageProc.cpp
        SPImageProc.h
        SPLogger.c
        SPLogger.h
        SPPoint.c
        SPPoint.h)
include_directories(${OpenCV_INCLUDE_DIRS})
#
include_directories(unit_tests)

add_executable(final_copy_2 ${SOURCE_FILES})