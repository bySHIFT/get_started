#include "include/utility.chrono.h"

#include <chrono>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <mutex>

// 2020-05-07 14:05:37 +0800
std::string zh::utility::chrono::now() try {
  enum { NOW_SIZE = 64 };
  char buffer[NOW_SIZE] { 0 };
  const char* fmt_now = "%F %T %z";

  std::memset(buffer, 0, sizeof buffer);
  const auto t_now = std::chrono::system_clock::to_time_t( \
    std::chrono::system_clock::now());

  struct tm* ptr_tm_now{ nullptr };
#ifdef _WIN32
  struct tm tm_now = { 0 };
  localtime_s(&tm_now, &t_now);
  ptr_tm_now = &tm_now;
#else
  ptr_tm_now = std::localtime(&t_now);
#endif

  std::strftime(buffer
    , sizeof buffer
    , fmt_now
    , ptr_tm_now
  );

  return buffer;
} catch (...) {
  return std::string{};
}
