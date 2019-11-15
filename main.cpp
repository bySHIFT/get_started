#include "include/theapp/my_request.h"

#include <iostream>

int main() noexcept try {
  std::cout << ">> EXIT: " << zh::run_my_request()
    << std::endl;

  getchar();
  return 0;
} catch (...) {
  return -1;
}
