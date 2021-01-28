#include "utility/print.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <tuple>
#include <vector>

namespace commands {
using type_table = std::vector<uint8_t>;
enum {
  NEXT_AGE = 5

  , MAX_RED = 33
  , COUNT_RED = 6

  , MAX_BLUE = 16
};

uint8_t random(uint8_t max) {
  std::random_device rd{};

  std::seed_seq seed2{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
  std::mt19937 gen{ seed2 };
  std::uniform_int_distribution<> dis{ 0, max - 1 };

  return dis(gen);
}

void fill(type_table& table, uint8_t count) {
  table.reserve(count);
  std::generate_n(std::back_inserter(table)
    , count
    , [idx = 0]() mutable { return ++idx; });

  std::random_device rd{};
  std::mt19937 gen{ rd() };
  std::shuffle(table.begin(), table.end(), gen);
}

uint8_t pop(type_table& table) {
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

  type_table ONE;
  std::generate_n(std::back_inserter(ONE)
    , COUNT_RED
    , [&red_table] {return pop(red_table); });

  std::sort(ONE.begin(), ONE.end());
  ONE.emplace_back(pop(blue_table));

  return ONE;
}
} // end commands namespace

int main() try {
  zh::utility::print::print_banner();

  std::vector<commands::type_table> ONES;
  std::generate_n(std::back_inserter(ONES)
    , commands::NEXT_AGE
    , [] { return commands::bingo(); });

  std::sort(ONES.begin(), ONES.end()
    , [](const commands::type_table& lhs, const commands::type_table& rhs) {
      return (std::tie(lhs[0], lhs[1], lhs[2], lhs[3], lhs[4], lhs[5])
        < std::tie(rhs[0], rhs[1], rhs[2], rhs[3], rhs[4], rhs[5]));
    });

  for (uint8_t idx{ 0 }; idx < ONES.size(); ++idx) {
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

  std::cout << std::endl << std::string(40, '=');

  for (std::size_t idx{ 0 }; idx < ONES.size(); ++idx) {
    const auto& ONE = ONES[idx];
    if ((idx % 5) == 0)
      std::cout << std::endl << "a3362";

    std::cout << "#";
    for (const auto& ball : ONE)
      std::cout << std::setw(2) << std::setfill('0') << (int)ball;
  }

  std::cout << std::endl;

  return EXIT_SUCCESS;
} catch (...) {
  return EXIT_FAILURE;
}