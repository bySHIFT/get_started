#include "../../include/theapp/my_request.h"

#include "../../include/entity/cicd.h"
#include "../../include/task.hxx"

#include <iostream>

namespace zh
{

int run_my_request()
{
  task <cicd&(cicd&)> my_cicd = \
    [](cicd& once_cicd) -> cicd& { return once_cicd; };
  cicd once_cicd("ce42bf7a");

  auto result = my_cicd
    .then([](cicd& commit) -> cicd& { return commit.cppcheck(); })
    .then([](cicd& commit) -> cicd& { return commit.build(); })
    .then([](cicd& commit) -> cicd& { return commit.ci(); })
    .then([](cicd& commit) -> cicd& { return commit.cd(); })
    .then([](cicd& commit) -> cicd& { return commit.clean(); })
    .run(once_cicd);

  const auto steps = result.get_steps();
  std::cout << std::endl
    << ">> STATUS: " << (steps == cicd::status::ALL ? "DONE" : "ERROR")
    << std::endl << std::endl;

  return (steps == cicd::status::ALL ? 0 : 1);
}

} // end namespace zh