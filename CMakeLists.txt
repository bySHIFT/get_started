# 配置 cmake 最小版本
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# 配置构建工程及语言
project(making LANGUAGES CXX)

# 配置 include 路径
include_directories(./include)

# 配置资源文件列表
list(APPEND _source_utility
  ./include/utility/chrono.h
  ./include/utility/print.h
  ./include/utility/random.h
  ./include/utility/strings.h
  ./include/utility/task.hxx

  ./src/utility/chrono.cpp
  ./src/utility/print.cpp
  ./src/utility/random.cpp
  ./src/utility/strings.cpp
)

list(APPEND _source_cicd
  ./include/cicd/entity.cicd.h
  ./include/cicd/theapp.my_request.h

  ./src/cicd/entity.cicd.cpp
  ./src/cicd/theapp.my_request.cpp
  ./src/cicd/main.cpp
)

list(APPEND _source_ccia
  ./src/ccia/listing_c.atm.hpp
  ./src/ccia/listing_c.bank_machine.hpp
  ./src/ccia/listing_c.dispatcher.hpp
  ./src/ccia/listing_c.entity.hpp
  ./src/ccia/listing_c.interface_machine.hpp
  ./src/ccia/listing_c.queue.hpp
  ./src/ccia/listing_c.receiver.hpp
  ./src/ccia/listing_c.sender.hpp
  ./src/ccia/listing_c.template_dispatcher.hpp
  ./src/ccia/main.cpp
)

# 构建工具库
add_library(mutility
  STATIC
    ${_source_utility}
)

if (NOT WIN32)
  # 设置编译选项
  # 允许O3优化、多线程
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pthread")
  message(STATUS "C++ compiler flags: ${CMAKE_CXX_FLAGS}")
endif (NOT WIN32)

add_executable(making ${_source_cicd})
add_executable(ccia ${_source_ccia})

# 追加库链接可执行文件
target_link_libraries(making
  mutility
)