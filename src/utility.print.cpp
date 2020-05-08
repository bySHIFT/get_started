#include "include/utility.print.h"
#if defined(_WIN32)
  #include <windows.h>
#endif

#include <string>

namespace zh { namespace utility { namespace print {
namespace details
{
  void print(zh::utility::string_view message) {
    fwrite(message.data(), 1, message.size(), stdout);
  }

  void print(const color c, zh::utility::string_view message)
  {
#if defined(_WIN32)
    const HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info{};
    GetConsoleScreenBufferInfo(console_handle, &console_screen_buffer_info);
    const auto original_color = console_screen_buffer_info.wAttributes;

    SetConsoleTextAttribute(console_handle
      , static_cast<WORD>(c) | (original_color & 0xF0));
    zh::utility::print::print2(message);
    SetConsoleTextAttribute(console_handle, original_color);
#else
    std::string message_with_color{};
    switch (c)
    {
      case color::red:
        message_with_color
          = zh::utility::strings::concat_or_view("\e[1;31m", message);
      break;
      case color::green:
        message_with_color
          = zh::utility::strings::concat_or_view("\e[1;32m", message);
      break;
      case color::blue:
        message_with_color
          = zh::utility::strings::concat_or_view("\e[1;34m", message);
      break;
      default:
      break;
    }

    message_with_color += "\e[0m";
    zh::utility::print::print2(message_with_color);
#endif
  }

} // end namespace details
} // end namespace print
} // end namespace utility
} // end namespace zh