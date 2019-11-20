#include "../../include/utility/chrono.h"

#include <chrono>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <mutex>

std::string zh::utility::chrono::now()
{
  enum { NOW_SIZE = 64 };
  static char buffer[NOW_SIZE] { 0 };
  static const char* fmt_now = "%F %T %z";

  std::memset(buffer, 0, sizeof buffer);
  const auto t_now = std::chrono::system_clock::to_time_t( \
    std::chrono::system_clock::now());
  std::strftime(buffer
    , sizeof buffer
    , fmt_now
    , std::localtime(&t_now)
  );

  return buffer;
}