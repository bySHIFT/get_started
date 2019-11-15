#ifndef INCLUDE_CICD_H__
#define INCLUDE_CICD_H__
#include <ostream>
#include <string>

namespace zh
{
class cicd
{
public:
  enum status
  {
    ERROR_S     = -1
    , OK_S      = 0

    , CPPCHECK  = 1
    , BUILD     = (1 << 1)
    , CI        = (1 << 2)
    , CD        = (1 << 3)
    , CLEAN     = (1 << 4)

    , ALL       = (CPPCHECK | BUILD | CI | CD | CLEAN)
  };

  struct job
  {
    status stage;

    std::string get_stage_name(int step) const noexcept;

    friend std::ostream& operator<<(std::ostream& out, const job& jobs) noexcept
    {
        out << ">> job name: " << jobs.get_stage_name(jobs.stage) << std::endl
            << "stage: " << jobs.stage << std::endl << std::endl;

        return out;
    }
  };

public:
  explicit cicd(const std::string& commit_id) : sha1(commit_id){}
  explicit operator bool() const noexcept;

  cicd& cppcheck();
  cicd& build();
  cicd& ci();
  cicd& cd();
  cicd& clean();

  int get_steps() const noexcept { return steps; }

private:
  const std::string sha1;
  job jobs;
  int steps { status::OK_S };

  cicd& run();
};

} // end namespace zh

#endif // INCLUDE_CICD_H__