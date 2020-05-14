# 配置 cmake 最小版本
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# 配置构建工程及语言
project(utility LANGUAGES CXX)

# 配置资源文件列表
list(APPEND _source_utility
  chrono.cpp
  print.cpp
  random.cpp
  strings.cpp
)

# 构建工具库
add_library(${PROJECT_NAME}
  STATIC
    ${_source_utility}
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)