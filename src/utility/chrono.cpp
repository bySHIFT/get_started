#include "utility/chrono.h"

#include <chrono>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <regex>
#include <vector>

// 2020-05-07 18:21:30 610075 +0800
// %F:  ISO 8601 日期格式
// %T:  ISO 8601 时间格式
// %z:  以 ISO 8601 格式（例如 -0430 ）写距 UTC 的偏移
//      或者倘若时区信息不可用则不写字符
// %us: 微秒
std::string zh::utility::chrono::now(const char* format) try {
  using std::chrono::duration_cast;
  using std::chrono::microseconds;

  enum { SIZE_BUFFER = 32 };
  constexpr std::chrono::seconds sec(1);
  constexpr auto microseconds_in_sec = (microseconds(sec).count());

  std::string full_datetime{ format };
  do {
    if (full_datetime.empty())
      break;

    const auto tp_now = std::chrono::system_clock::now();
    const auto tt_now = std::chrono::system_clock::to_time_t(tp_now);
    const auto us \
      = duration_cast<microseconds>(tp_now.time_since_epoch()).count()
        % microseconds_in_sec;

    struct tm* ptr_tm_now{ nullptr };
#ifdef _WIN32
    struct tm tm_now = { 0 };
    localtime_s(&tm_now, &tt_now);
    ptr_tm_now = &tm_now;
#else
    ptr_tm_now = std::localtime(&tt_now);
#endif

    for (const auto& type : std::vector<const char*>{ "%F", "%T", "%z"}) {
      char buffer[SIZE_BUFFER] = { 0 };

      std::strftime(buffer, sizeof buffer, type, ptr_tm_now);
      std::regex type_re(type);

      full_datetime = std::regex_replace(full_datetime, type_re, buffer);
    }

    full_datetime = std::regex_replace(full_datetime
      , std::regex{ "%us" }
      , std::to_string(us));
  } while (false);

  return full_datetime;
} catch (...) {
  return std::string{};
}
