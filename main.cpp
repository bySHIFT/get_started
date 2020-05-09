#include "theapp.my_request.h"
#include "utility.chrono.h"
#include "utility.print.h"

#if defined(_WIN32)
  #include <windows.h>
#endif

#include <cstdlib>
#include <iostream>
#include <string>

void print_banner() {
  using namespace zh::utility;

  print::print2(print::color::green, std::string(60, '='), '\n');
  print::print2(print::color::magenta, "== ");
  print::print2(print::color::red, "放胆开怀, 尽情肆意\n");
  print::print2(print::color::magenta, "== ");
  print::print2(print::color::cyan, chrono::now("%F %T %z"), '\n');
}

int main() noexcept try {
#if defined(_WIN32)
  const auto init_console_cp = GetConsoleCP();
  const auto init_console_output_cp = GetConsoleOutputCP();

  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);
#endif
  print_banner();

  const auto result_cicd = zh::run_my_request();
  std::cout << ">> EXIT: " << result_cicd
    << std::endl;

#if defined(_WIN32)
  SetConsoleCP(init_console_cp);
  SetConsoleOutputCP(init_console_output_cp);
#endif

  return result_cicd;
} catch (...) {
  return EXIT_FAILURE;
}
