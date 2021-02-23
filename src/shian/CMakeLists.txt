# 配置 cmake 最小版本
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# 配置构建工程及语言
project(ShiAN LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

# 配置资源文件列表
list(APPEND _source_ShiAN
  ShiAN.cpp
)

add_executable(${PROJECT_NAME} ${_source_ShiAN})
