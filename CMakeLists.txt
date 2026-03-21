cmake_minimum_required(VERSION 3.10)
project(GKD-Software-2025-Test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找 OpenCV 库
set(OpenCV_DIR "F:\\tool\\opencv\\build\\x64\\MinGW\\install\\x64\\mingw\\lib")
find_package(OpenCV REQUIRED)

# 包含源代码目录
include_directories(src)

# 收集源文件
set(MAIN_SOURCE_FILES
    src/main.cpp
)

set(APPLY_SOURCE_FILES
    src/apply.cpp
)

# 添加可执行文件
add_executable(main ${MAIN_SOURCE_FILES})
add_executable(apply ${APPLY_SOURCE_FILES})

# 链接库
target_link_libraries(main ${OpenCV_LIBS} ws2_32)
target_link_libraries(apply ${OpenCV_LIBS} ws2_32)
