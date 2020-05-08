#include "include/theapp.my_request.h"
#include "include/utility.print.h"

#include <cstdlib>
#include <iostream>
#include <string>

void print_banner() {
  using namespace zh::utility::print;

  print2(color::green, std::string(60, '='), '\n');
  print2(color::magenta, "== ");
  print2(color::red, "放胆开怀, 尽情肆意\n");
}

int main() noexcept try {
  print_banner();

  const auto result_cicd = zh::run_my_request();
  std::cout << ">> EXIT: " << result_cicd
    << std::endl;

  return result_cicd;
} catch (...) {
  return EXIT_FAILURE;
}
