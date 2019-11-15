#include "../../include/entity/cicd.h"

#include <iostream>

namespace zh
{

std::string cicd::job::get_stage_name(int step) const noexcept
{
  switch (step)
  {
    case status::CPPCHECK:
    return std::string { "cppcheck" };
    case status::BUILD:
    return std::string { "build" };
    case status::CI:
    return std::string { "ci" };
    case status::CD:
    return std::string { "cd" };
    case status::CLEAN:
    return std::string { "clean" };
  }

  return std::string {};
}

cicd::operator bool() const noexcept
{
  const auto over = jobs.stage - 1;
  return (over == steps);
}

#define RUN_NEXT_JOB(step)    \
  jobs = job { step };        \
  if (!(*this)) return *this; \
  return run();

cicd& cicd::cppcheck() { RUN_NEXT_JOB(status::CPPCHECK); }
cicd& cicd::build() { RUN_NEXT_JOB(status::BUILD); }
cicd& cicd::ci() { RUN_NEXT_JOB(status::CI); }
cicd& cicd::cd() { RUN_NEXT_JOB(status::CD); }
cicd& cicd::clean() { RUN_NEXT_JOB(status::CLEAN); }
#undef RUN_NEXT_JOB

cicd& cicd::run()
{
  // steps &= ~(1 << jobs.stage);
  steps |= jobs.stage;

  std::cout << sha1 << std::endl;
  std::cout << jobs;

  return *this;
}

} // end namespace zh