# 配置 cmake 最小版本
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# 配置构建工程及语言
project(cicd LANGUAGES CXX)

# 配置资源文件列表
list(APPEND _source_cicd
  entity.cicd.cpp
  theapp.my_request.cpp
  main.cpp
)

add_executable(${PROJECT_NAME} ${_source_cicd})

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

# 追加库链接可执行文件
if (NOT WIN32)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
      pthread
  )
endif (NOT WIN32)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    utility
)