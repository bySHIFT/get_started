#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
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
  four_operations(type_operation first, type_operation second)
    : ot_first(first)
    , ot_second(second)
  {
  }

  void get_lazy() const {
    std::call_once(flag_init, [&] { return calc(); });
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
    enum { MIN = 1
      , MAX = 99
    
      , ADD_SUB = 0
      , MUL_DIV = 1
    };

    // A OP1 B OP2 C = D
    // OP[1, 2]: ＋－ = 0, ×÷ = 1
    // OP1OP2: 00, 01, 10, 11
    const auto OP1 = (int)ot_first & 0X02;
    const auto OP2 = (int)ot_second & 0X02;

    random r{ MIN, MAX };
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

  mutable int operation_first{ 0 };
  mutable int operation_second{ 0 };
  mutable int operation_third{ 0 };
  mutable int operation_result{ 0 };

  mutable std::once_flag flag_init;
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

std::string get_four_operations() {
  const auto min = (uint8_t)arithmetic::type_operation::to_add;
  const auto max = (uint8_t)arithmetic::type_operation::to_substract;
  const arithmetic::random r{ min, max };

  return arithmetic::four_operations{
    (arithmetic::type_operation)r.next()
    , (arithmetic::type_operation)r.next() }
  .to_string();
}
} // end commands namespace


void print_title() noexcept {
  std::cout << "日期: " << commands::get_date()
    << "    " << "浦江一小 一(1) 张芮安" << std::endl;
}

} // end internal namespace

int main()
{
  enum {
    BANNER_SIZE = 80
    , ROW_SIZE = 2
    , SEPERATOR_SIZE = 10
    , PREFIX_SIZE = 2
    , TOTAL = 30
  };

  const auto SEQ_SIZE = std::to_string(TOTAL).length();
  int SEQ_currentNO = 0;
  auto print([SEQ_SIZE, &SEQ_currentNO](int size) mutable {
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

      ROW[idx] = ssSequenceNo.str() + internal::commands::get_four_operations();
    }

    std::string s = std::accumulate(std::next(ROW.begin())
      , ROW.end()
      , ROW.front()
      , fold);
    std::cout << std::string(PREFIX_SIZE, ' ') << s << std::endl << std::endl;
  });

  internal::print_title();
  std::cout << std::string(BANNER_SIZE, '=') << std::endl << std::endl;

  
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
