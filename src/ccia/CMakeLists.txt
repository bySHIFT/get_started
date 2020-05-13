# 配置构建工程及语言
project(ccia LANGUAGES CXX)

# 配置 include 路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include)

# 配置资源文件列表
list(APPEND _source_ccia
  listing_c.atm.hpp
  listing_c.bank_machine.hpp
  listing_c.dispatcher.hpp
  listing_c.entity.hpp
  listing_c.interface_machine.hpp
  listing_c.queue.hpp
  listing_c.receiver.hpp
  listing_c.sender.hpp
  listing_c.template_dispatcher.hpp
  main.cpp
)

add_executable(${PROJECT_NAME} ${_source_ccia})

# 追加库链接可执行文件
if (NOT WIN32)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
      pthread
  )
endif (NOT WIN32)
