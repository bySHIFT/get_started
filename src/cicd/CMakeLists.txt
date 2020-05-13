# 配置构建工程及语言
project(cicd LANGUAGES CXX)

# 配置 include 路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include)

# 配置资源文件列表
list(APPEND _source_cicd
  entity.cicd.cpp
  theapp.my_request.cpp
  main.cpp
)

add_executable(${PROJECT_NAME} ${_source_cicd})

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