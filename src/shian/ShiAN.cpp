﻿#include <atomic>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace internal {
namespace arithmetic {
enum class type_operation : int { // ＋ － × ÷ ﹦
  to_add = 0
  , to_substract
  , to_multiply
  , to_divide
};

std::string get_operation_string(type_operation ot) {
  switch (ot) {
  case type_operation::to_add:
    return { "＋" };
  case type_operation::to_substract:
    return { "－" };
  case type_operation::to_multiply:
    return { "×" };
  case type_operation::to_divide:
    return { "÷" };
  }

  return {};
}

class random {
public:
  random(uint8_t min, uint8_t max)
    : MIN{ min }, MAX{ max } {
  }

  uint8_t next() const {
    std::random_device rd{};

    std::seed_seq seed2{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
    std::mt19937 gen{ seed2 };
    std::uniform_int_distribution<> dis{ MIN, MAX };

    return dis(gen);
  }

private:
  const uint8_t MIN;
  const uint8_t MAX;
};

class four_operations {
public:
  four_operations(type_operation first, type_operation second, int limit)
    : ot_first(first)
    , ot_second(second)
    , operation_limit(limit) {
  }

  void get_lazy() const {
    if (!flag_init.exchange(true))
      calc();
  }

  std::string to_string() const {
    enum { NUMBER_SIZE = 3 };
    get_lazy();

    const auto magic_number = random{ 1, 4 }.next();
    auto magic([magic_number](int v, int length, int idx) {
      std::string str(length, magic_number != idx ? ' ' : '_');
      if (magic_number != idx)
        str.append(std::to_string(v));

      str = str.substr(str.length() - length);
      return str;
    });

    std::stringstream ss;
    ss << magic(operation_first, NUMBER_SIZE, 1)
       << " " << get_operation_string(ot_first) << " "
       << magic(operation_second, NUMBER_SIZE, 2)
       << " " << get_operation_string(ot_second) << " "
       << magic(operation_third, NUMBER_SIZE, 3)
       << " ﹦ "
       << magic(operation_result, NUMBER_SIZE * 2, 4)
       << "";

    return ss.str();
  }

private:
  using OP = int(*)(int, int);

  static
  OP OPS(type_operation ot) {
    switch (ot) {
    case type_operation::to_add:
      return [](int l, int r) { return l + r; };
    case type_operation::to_substract:
      return [](int l, int r) { return l - r; };
    case type_operation::to_multiply:
      return [](int l, int r) { return l * r; };
    case type_operation::to_divide:
      return [](int l, int r) { return l / r; };
    }

    return [](int, int) { return 0; };
  }

  static
  bool DIV_CHECK(int x, int y) {
    if (y && !std::div(x, y).rem)
      return true;

    return false;
  }

  inline
  static
  bool is_divide(type_operation ot) {
    return type_operation::to_divide == ot;
  }

  void calc() const {
    enum { __MAX = 99
    
      , ADD_SUB = 0
      , MUL_DIV = 1
    };

    // A OP1 B OP2 C = D
    // OP[1, 2]: ＋－ = 0, ×÷ = 1
    // OP1OP2: 00, 01, 10, 11
    const auto OP1 = (int)ot_first & 0X02;
    const auto OP2 = (int)ot_second & 0X02;

    uint8_t MAX = __MAX;
    if (operation_limit >= 20 && operation_limit <= 100)
      MAX = operation_limit - 1;

    random r{ 1, MAX };
    while (true) {
      operation_first = r.next();
      operation_second = r.next();
      operation_third = r.next();

      if (ADD_SUB == OP1 && MUL_DIV == OP2) {
        if (is_divide(ot_second) && !DIV_CHECK(operation_second, operation_third))
            continue;

        const auto temp = OPS(ot_second)(operation_second, operation_third);

        operation_result = OPS(ot_first)(operation_first, temp);
      } else { // 都可以按顺序执行计算
        if (is_divide(ot_first) && !DIV_CHECK(operation_first, operation_second))
          continue;

        const auto temp = OPS(ot_first)(operation_first, operation_second);
        if (is_divide(ot_second) && !DIV_CHECK(temp, operation_third))
          continue;

        operation_result = OPS(ot_second)(temp, operation_third);
      }

      if (operation_result < 0)
        continue;
      else
        break;
    }
  }

  const type_operation ot_first;
  const type_operation ot_second;
  const int operation_limit;

  mutable int operation_first{ 0 };
  mutable int operation_second{ 0 };
  mutable int operation_third{ 0 };
  mutable int operation_result{ 0 };

  mutable std::atomic_bool flag_init{ false };
};
} // end arithmetic namespace

namespace commands {
std::string get_date() {
  enum { date_string_size_max = 64 };
  const auto nowtt = std::time(nullptr);
#if _WIN32
  struct tm temp_nowtm { 0 };
  localtime_s(&temp_nowtm, &nowtt);
  auto* nowtm = &temp_nowtm;
#else
  const auto nowtm = std::localtime(&nowtt);
#endif

  char date_string[date_string_size_max] = { 0 };
  std::strftime(date_string, sizeof date_string, "%F", nowtm);

  return { date_string };
}

// OPERATION: [1: 加减, 2: 加减乘, 3: 加减乘除]
// LIMIT: [20, 100]
std::string get_four_operations(int OPERATION, int LIMIT) {
  auto tempMax = (uint8_t)arithmetic::type_operation::to_substract;
  if (2 == OPERATION)
    tempMax = (uint8_t)arithmetic::type_operation::to_multiply;
  else if (3 == OPERATION)
    tempMax = (uint8_t)arithmetic::type_operation::to_divide;

  const auto min = (uint8_t)arithmetic::type_operation::to_add;
  const auto max = tempMax;
  const arithmetic::random r{ min, max };

  return arithmetic::four_operations{
    (arithmetic::type_operation)r.next()
    , (arithmetic::type_operation)r.next()
    , LIMIT
    }.to_string();
}
} // end commands namespace

void print_usage() noexcept {
  std::cout << R"(
用法: ShiAN.exe [-name:<这里输入学生的信息>] [-count:<题目输出总数>] [-operation:<操作类型>] [-limit:<最大操作数>]
  -name: 学生信息, 如: 浦江一小 一(1)班 张芮安;
  -count: 输出题目的总数, 输入范围[10, 200];
  -operation: 四则运算的操作符, 输入范围[1, 3], 其中1: 加减, 2: 加减乘, 3: 加减乘除;
  -limit: 四则运算的操作数的最大值, 输入范围[20, 100];
  -help: 输出本条信息, 并退出;

例: ShiAN.exe -name:"浦江一小 一(1)班 张芮安" -count:"20" -operation:"1" -limit:"30"
)"
    << std::endl;
}

void print_title(const char* name) noexcept {
  enum { SIZE = 40 };
  std::string label(SIZE, '_');
  if (name) {
    label = name;
    if (label.size() > SIZE)
      label = label.substr(0, SIZE);
  }

  std::cout << "日期: " << commands::get_date()
    << "    " << label << std::endl;
}

const char*
get_value(int argc, char* argv[], const char* name) {
  const size_t nameLen = std::strlen(name);
  for (int i = 0; i < argc; i++) {
    if (std::strncmp(argv[i] + 1, name, nameLen) == 0) {
      return argv[i] + 1 + nameLen + 1;
    }
  }
  return nullptr;
}
} // end internal namespace

int main(int argc, char **argv)
{
  enum {
    BANNER_SIZE = 80
    , ROW_SIZE = 2
    , SEPERATOR_SIZE = 10
    , PREFIX_SIZE = 2
    , __TOTAL = 30
    , __OPERATION = 1 // 1: 加减, 2: 加减乘, 3: 加减乘除
    , __LIMIT = 100
  };

  if (internal::get_value(argc, argv, "help") || internal::get_value(argc, argv, "?")) {
    internal::print_usage();
    exit(EXIT_FAILURE);
  }

  // -name:"浦江一小 一(1)班 张芮安" -count:"20" -operation:"1" -limit:"100"
  { // -name:"浦江一小 一(1)班 张芮安"
    auto argName = internal::get_value(argc, argv, "name");
    internal::print_title(argName);
    std::cout << std::string(BANNER_SIZE, '=') << std::endl << std::endl;
  }

  int TOTAL = __TOTAL;
  if (auto argCount = internal::get_value(argc, argv, "count")) {
    const auto c = std::atoi(argCount);
    if (c >= 10 && c <= 200)
      TOTAL = c;
  }

  int OPERATION = __OPERATION;
  if (auto argOperation = internal::get_value(argc, argv, "operation")) {
    const auto opt = std::atoi(argOperation);
    if (opt >= 1 && opt <= 3)
      OPERATION = opt;
  }

  int LIMIT = __LIMIT;
  if (auto argLimit = internal::get_value(argc, argv, "limit")) {
    const auto opt = std::atoi(argLimit);
    if (opt >= 20 && opt <= 100)
      LIMIT = opt;
  }

  const auto SEQ_SIZE = std::to_string(TOTAL).length();
  int SEQ_currentNO = 0;
  auto print([OPERATION
    , LIMIT
    , SEQ_SIZE, &SEQ_currentNO](int size) mutable {
    auto fold([](std::string a, const std::string& b) {
      return std::move(a) + std::string(SEPERATOR_SIZE, ' ') + b;
    });

    if (size <= 0)
      return;

    std::vector<std::string> ROW(size);
    for (int idx = 0; idx < size; ++idx) {
      std::stringstream ssSequenceNo;
      ssSequenceNo << "("
        << std::setw(SEQ_SIZE) << std::setfill('0')
        << std::to_string(++SEQ_currentNO)
        << ")";

      ROW[idx] = ssSequenceNo.str()
        + internal::commands::get_four_operations(OPERATION, LIMIT);
    }

    std::string s = std::accumulate(std::next(ROW.begin())
      , ROW.end()
      , ROW.front()
      , fold);
    std::cout << std::string(PREFIX_SIZE, ' ') << s << std::endl << std::endl;
  });

  const auto INFO = std::div(TOTAL, ROW_SIZE);
  for (int idx = 0; idx < INFO.quot; ++idx)
    print(ROW_SIZE);

  print(INFO.rem);

#if _WIN32
  std::cout << std::endl << std::endl << "按任意键关闭此窗口. . ." << std::endl;
  getchar();
#endif
  return 0;
}
