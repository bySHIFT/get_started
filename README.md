# get_started
 less is more


## 快速开始
需求:
- Windows 10 或 Ubuntu18.04
- Visual Studio 2015 Update 3 或更新的版本 (Windows 中)
- Git
- CMake 3.10

如何开始:
```SHELL
> git clone https://github.com/byshift/get_started.git
> cd get_started
> cmake -H. -Bbuild && cmake --build ./build
> ./build/src/cicd/cicd
```

## 运行
```
============================================================
== 放胆开怀, 尽情肆意
== 2020-05-13 14:42:42 +0800
>> commit sha1: ce42bf7a
  job name: cppcheck
  stage: 1
  tag: 258A411E-C91C-F4F4-4B0C-ADAA3C5DE11C
  START: 2020-05-13 14:42:42 882656 +0800

...
>> commit sha1: ce42bf7a
  job name: build
  stage: 2
  tag: D8B60BEE-744E-5C94-1B4C-90B1C533FA60
  START: 2020-05-13 14:42:46 884026 +0800

........
>> commit sha1: ce42bf7a
  job name: ci
  stage: 4
  tag: A2A4A47E-166D-EA33-3180-0ADBA0A5D056
  START: 2020-05-13 14:42:55 885565 +0800

.....
  job name: cd
  stage: 8
  tag: 456A7DD5-982B-4797-C4D1-2AD3A9FD785C
  START: 2020-05-13 14:43:01 886523 +0800

.....
>> commit sha1: ce42bf7a
  job name: clean
  stage: 16
  tag: D96FC3A2-E9F4-C369-C150-C7E3DAF9202F
  START: 2020-05-13 14:43:07 887881 +0800

.......

>> STATUS: DONE

>> EXIT: 0
```

```cpp
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <tuple>
#include <vector>

namespace commands {
using type_table = std::vector<uint8_t>;
enum {
  NEXT_AGE = 10

  , MAX_RED = 33
  , COUNT_RED = 6

  , MAX_BLUE = 16
};

uint8_t random(uint8_t max) {
  std::random_device rd{};
  std::mt19937 gen{ rd() };
  std::uniform_int_distribution<> dis{ 0, max - 1 };

  return dis(gen);
}

void fill(type_table& table, uint8_t count) {
  table.reserve(count);
  for (uint8_t idx{ 0 }; idx < count; ++idx) {
    table.push_back(idx + 1);
  }

  std::random_device rd{};
  std::mt19937 gen{ rd() };
  std::shuffle(table.begin(), table.end(), gen);
}

uint8_t get(type_table& table) {
  const auto idx = random((uint8_t)table.size());
  const auto rst = table[idx];

  table[idx] = table.back();
  table.pop_back();

  return rst;
}

type_table bingo() {
  type_table red_table{};
  type_table blue_table{};
  fill(red_table, MAX_RED);
  fill(blue_table, MAX_BLUE);

  type_table ONE(COUNT_RED);
  for (uint8_t idx{ 0 }; idx < COUNT_RED; ++idx) {
    ONE[idx] = get(red_table);
  }

  std::sort(ONE.begin(), ONE.end());
  ONE.emplace_back(get(blue_table));

  return ONE;
}
} // end commands namespace

int main() try {
  std::vector<commands::type_table> ONES(commands::NEXT_AGE);
  const auto count_ONES = ONES.size();
  for (uint8_t idx{ 0 }; idx < count_ONES; ++idx)
    ONES[idx] = commands::bingo();

  std::sort(ONES.begin(), ONES.end()
    , [](const commands::type_table& lhs, const commands::type_table& rhs) {
      return (std::tie(lhs[0], lhs[1], lhs[2], lhs[3], lhs[4], lhs[5])
        < std::tie(rhs[0], rhs[1], rhs[2], rhs[3], rhs[4], rhs[5]));
    });

  for (uint8_t idx{ 0 }; idx < count_ONES; ++idx) {
    auto ONE = ONES[idx];
    const int BLUE = ONE.back();
    ONE.pop_back();

    std::cout << "("
      << std::setw(2) << std::setfill('0') << (int)(idx + 1)
      << ")";

    for (const auto& item : ONE)
      std::cout << " "
      << std::setw(2) << std::setfill('0') << (int)item;

    std::cout << ", "
      << std::setw(2) << std::setfill('0') << BLUE << std::endl;
  }

  return EXIT_SUCCESS;
} catch (...) {
  return EXIT_FAILURE;
}

```
