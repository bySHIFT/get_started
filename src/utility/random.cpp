#include "include/utility/random.h"

#include <cstdio>
#include <cstring>
#include <random>

int zh::utility::random::get_uniform_distribution(int min, int max)
{
  std::random_device rd;  // 将用于为随机数引擎获得种子
  std::mt19937 gen(rd()); // 以播种标准 mersenne_twister_engine
  std::uniform_int_distribution<> dis(min, max);

  return dis(gen);
}

std::string zh::utility::random::get_uuid()
{ // 744B6B77-EB60-43C7-951B-3C5AF951E4B0
  enum { UUID_SIZE = 64 };
  char buffer[UUID_SIZE] { 0 };
  const char* fmt_uuid = "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X";

  auto fn_r = []() { return get_uniform_distribution(0, 0XFF); };

  std::memset(buffer, 0, sizeof buffer);
  std::snprintf(buffer
    , sizeof buffer
    , fmt_uuid
    , fn_r(), fn_r(), fn_r(), fn_r()
    , fn_r(), fn_r()
    , fn_r(), fn_r()
    , fn_r(), fn_r()
    , fn_r(), fn_r(), fn_r(), fn_r(), fn_r(), fn_r()
  );

  return buffer;
}
