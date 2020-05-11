#include "cicd/theapp.my_request.h"
#include "cicd/entity.cicd.h"

#include "utility/print.h"
#include "utility/task.hxx"

#include <cstdlib>
#include <iostream>

namespace zh
{

int run_my_request()
{
  cicd once_cicd("ce42bf7a");
  task<cicd&(cicd&)> my_cicd{
    [](cicd& once_cicd) -> cicd& { return once_cicd; }
  };

  const auto result = my_cicd
    .then([](cicd& commit) -> cicd& { return commit.cppcheck(); })
    .then([](cicd& commit) -> cicd& { return commit.build(); })
    .then([](cicd& commit) -> cicd& { return commit.ci(); })
    .then([](cicd& commit) -> cicd& { return commit.cd(); })
    .then([](cicd& commit) -> cicd& { return commit.clean(); })
    .run(once_cicd);

  const auto steps = result.get_steps();
  zh::utility::print::print2("\n>> STATUS: ");
  if (steps == cicd::status::ALL) {
    zh::utility::print::print2(zh::utility::print::color::green, "DONE\n\n");
  } else {
    zh::utility::print::print2(zh::utility::print::color::red, "ERROR\n\n");
  }

  return (steps == cicd::status::ALL ? EXIT_SUCCESS : EXIT_FAILURE);
}

} // end namespace zh
