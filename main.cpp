#include "include/theapp.my_request.h"
#include "include/utility.print.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

void print_banner() {
  std::stringstream ss;
  ss << std::string(60, '=') << std::endl
    << "== "
      "放胆开怀, 尽情肆意"
    << std::endl << std::endl;

    // std::cout << ss.str();
    zh::utility::print::print2(
      zh::utility::print::color::green
      , ss.str()
    );
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
