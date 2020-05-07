#include "include/theapp.my_request.h"

#if defined(_WIN32)
#include <windows.h>
#endif

#include <cstdlib>
#include <iostream>
#include <string>

int main() noexcept try {
#if defined(_WIN32)
  const auto init_console_cp = GetConsoleCP();
  const auto init_console_output_cp = GetConsoleOutputCP();

  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);
#endif

  std::cout << std::string(60, '=') << std::endl
    << "== "
#ifndef _WIN32
       "\e[1;31m"
#endif
      "放胆开怀, 尽情肆意"
#ifndef _WIN32
       "\e[0m"
#endif
    << std::endl << std::endl;

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
