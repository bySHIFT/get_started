# 配置 cmake 最小版本
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# 配置构建工程及语言
project(ShiAN LANGUAGES CXX VERSION 21.02.01)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

# 配置资源文件列表
list(APPEND _source_ShiAN
  ShiAN.cpp
)

if(MSVC)
  set(MY_VERSIONINFO_RC "${CMAKE_BINARY_DIR}/ShiAN.rc")
  configure_file("${PROJECT_SOURCE_DIR}/ShiAN.rc.in"
  "${MY_VERSIONINFO_RC}")
endif()

add_executable(${PROJECT_NAME} ${_source_ShiAN} ${MY_VERSIONINFO_RC})

# clear && rm -rvf ./build_msvc && cmake -H. -Bbuild_msvc && cmake --build ./build_msvc --config release && ./build_msvc/src/shian/Release/ShiAN.exe 
