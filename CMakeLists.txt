cmake_minimum_required(VERSION 3.27)
project(MyStudySpace)

set(CMAKE_CXX_STANDARD 17)

find_package(CURL REQUIRED)

add_executable(MyStudySpace main.cpp
        json.h
        curlUtils.h
)

target_link_libraries(MyStudySpace PRIVATE ${CURL_LIBRARIES})
target_include_directories(MyStudySpace PRIVATE ${CURL_INCLUDE_DIRS})
