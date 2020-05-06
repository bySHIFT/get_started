#include "include/theapp/my_request.h"

#include <cstdlib>
#include <iostream>

int main() noexcept try {
  const auto result_cicd = zh::run_my_request();
  std::cout << ">> EXIT: " << result_cicd
    << std::endl;

  return result_cicd;
} catch (...) {
  return EXIT_FAILURE;
}
