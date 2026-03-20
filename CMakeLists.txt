cmake_minimum_required(VERSION 3.10)
project(GKD-Software-2025-Test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找 OpenCV 库
set(OpenCV_DIR "F:\\tool\\opencv\\build\\x64\\MinGW\\install\\x64\\mingw\\lib")
find_package(OpenCV REQUIRED)

# 包含源代码目录
include_directories(src)

# 收集所有源文件
set(SOURCE_FILES
    src/main.cpp
    src/part1.cpp
    src/part2.cpp
    src/part6.cpp
)

# 添加可执行文件
add_executable(main ${SOURCE_FILES})

# 链接 OpenCV 库
target_link_libraries(main ${OpenCV_LIBS})
