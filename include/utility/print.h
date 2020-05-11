#pragma once
#include "utility/strings.h"

namespace zh { namespace utility {
namespace print
{
  enum class color
  {
    red     = 4
    , green = 2
    , blue  = 1

    , yellow = (red | green)
    , magenta = (red | blue)
    , cyan = (green | blue)
  };

  void print(const std::string& message);
  void print(const color c, const std::string& message);

  template<typename... Args>
  void print2(const color c, const Args&... args) {
    print(c, zh::utility::strings::concat_or_view(args...));
  }

  template<typename... Args>
  void print2(const Args&... args) {
    print(zh::utility::strings::concat_or_view(args...));
  }

  void print_banner();
} // end namespace print
} // end namespace utility
} // end namespace zh