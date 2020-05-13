# 配置构建工程及语言
project(utility LANGUAGES CXX)

# 配置 include 路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include)

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
