#include "include/utility/chrono.h"

#include <chrono>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <mutex>

std::string zh::utility::chrono::now()
{
  enum { NOW_SIZE = 64 };
  char buffer[NOW_SIZE] { 0 };
  const char* fmt_now = "%F %T %z";

  std::memset(buffer, 0, sizeof buffer);
  const auto t_now = std::chrono::system_clock::to_time_t( \
    std::chrono::system_clock::now());
  std::strftime(buffer
    , sizeof buffer
    , fmt_now
#pragma warning(disable: 4996)
    , std::localtime(&t_now)
#pragma warning(default: 4996)
  );

  return buffer;
}