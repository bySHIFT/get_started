#pragma once
#include "include/utility.strings.h"

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
  };

  namespace details
  {
    using zh::utility::string_view;

    void print(string_view message);
    void print(const color c, string_view message);
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