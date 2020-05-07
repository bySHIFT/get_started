#include "include/utility.chrono.h"

#include <chrono>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <mutex>

std::string zh::utility::chrono::now() try {
  enum { NOW_SIZE = 64 };
  char buffer[NOW_SIZE] { 0 };
  const char* fmt_now = "%F %T %z";

  std::memset(buffer, 0, sizeof buffer);
  const auto t_now = std::chrono::system_clock::to_time_t( \
    std::chrono::system_clock::now());

  struct tm tm_now = { 0 };
#ifdef _WIN32
  localtime_s(&tm_now, &t_now);
#else
  tm_now = *std::localtime(&t_now);
#endif

  std::strftime(buffer
    , sizeof buffer
    , fmt_now
    , &tm_now
  );

  return buffer;
} catch (...) {
  return std::string{};
}
