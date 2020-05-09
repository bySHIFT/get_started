#pragma once
#include "utility.strings.h"

namespace zh
{
namespace utility
{
namespace print
{
  enum class color
  {
    red = 4
    , green = 2
    , blue = 1

    , yellow = (red | green)
    , magenta = (blue | red)
    , cyan = (green | blue)
  };

  namespace details
  {
    void print(const std::string& message);
    void print(const color c, const std::string& message);
  } // end namespace details

  template<typename... Args>
  void print2(const color c, const Args&... args) {
    details::print(c, zh::utility::strings::concat_or_view(args...));
  }

  template<typename... Args>
  void print2(const Args&... args) {
    details::print(zh::utility::strings::concat_or_view(args...));
  }
} // end namespace print
} // end namespace utility
} // end namespace zh