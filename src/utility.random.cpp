#include "include/utility.random.h"

#include <cstdio>
#include <cstring>
#include <numeric>
#include <random>
#include <vector>

int zh::utility::random::get_uniform_distribution(int min, int max)
{
  std::random_device rd;  // 将用于为随机数引擎获得种子
  std::mt19937 gen(rd()); // 以播种标准 mersenne_twister_engine
  std::uniform_int_distribution<> dis(min, max);

  return dis(gen);
}

// For the X conversion letters ABCDEF are used.
std::string zh::utility::random::get_uuid(bool X)
{ // 744B6B77-EB60-43C7-951B-3C5AF951E4B0
  auto fn_r = []() { return get_uniform_distribution(0, 0X0F); };
  std::vector<int> parts{ 8, 4, 4, 4, 12 };
  std::vector<char> container;
  container.resize(
    std::accumulate(parts.begin(), parts.end(), 0) + parts.size() - 1
    , '-'
  );

  for (auto i = 0; i < parts.size(); ++i) {
    const auto offset = std::accumulate(parts.begin(), parts.begin() + i, 0) + i;
    for (auto j = 0; j < parts[i]; ++j) {
      const auto num = fn_r();
      if (num < 10) {
        container[j + offset] = static_cast<char>('0' + num);
      } else {
        container[j + offset] = static_cast<char>((X ? 'A' : 'a') + (num - 10));
      }
    }
  }

  std::string uuid{};
  uuid.assign(container.data(), container.size());

  return uuid;
}
