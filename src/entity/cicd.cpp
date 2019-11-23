#include "../../include/entity/cicd.h"
#include "../../include/utility/chrono.h"
#include "../../include/utility/random.h"

#include <future>
#include <iostream>
#include <thread>

namespace zh
{
namespace internal
{
namespace cicd
{
namespace
{

int my_dummy_work()
{
  enum { ok = 0, error };

  { // 输出本次操作的id
    const auto uuid = utility::random::get_uuid();
    std::cout << "  tag: " << uuid << std::endl;
  }

  { // 输出本次操作的时间戳
    const auto now = utility::chrono::now();
    std::cout << "  START: " << now << std::endl;
  }

  { // 处理本次操作
    const auto dummy_elapsed = utility::random::get_uniform_distribution(3, 10);
    auto fut_work = std::async(std::launch::async
      , [&dummy_elapsed]() ->int {
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(dummy_elapsed + 1));

        return ok;
      }
    );

    while (fut_work.wait_for(std::chrono::seconds(1)) \
     == std::future_status::timeout) {
      std::cout << "." << std::flush;
    }

    const auto result = fut_work.get();
    std::cout << std::endl;

    return result;
  }

  return error;
}

} // end namespace unnamed
} // end namespace cicd
} // end namespace internal

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

#pragma warning(disable: 26812)
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
#pragma warning(default: 26812)

cicd& cicd::run()
{
  // steps &= ~(1 << jobs.stage);
  steps |= jobs.stage;

  std::cout << ">> commit sha1: " << sha1 << std::endl
    << jobs;
  if (internal::cicd::my_dummy_work()) {
    steps &= ~jobs.stage;
  }

  return *this;
}

} // end namespace zh