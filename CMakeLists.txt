# 配置 cmake 最小版本
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# 配置构建工程及语言
project(making LANGUAGES CXX)

add_subdirectory(./src/utility)
add_subdirectory(./src/ccia)
add_subdirectory(./src/cicd)
