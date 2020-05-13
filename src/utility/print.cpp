#include "utility/print.h"

#include "utility/chrono.h"

#if defined(_WIN32)
  #include <windows.h>
#endif

#include <cstdio>
#include <string>

#if defined(_WIN32)
// https://docs.microsoft.com/en-us/windows/console/using-the-high-level-input-and-output-functions
// SetConsoleTextAttribute
class console_text final
{
public:
  explicit operator bool() const {
    return (INVALID_HANDLE_VALUE != m_hStdOutput);
  }

  void get_std_handle() const noexcept try {
    const auto console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (console_handle != INVALID_HANDLE_VALUE)
      m_hStdOutput = console_handle;
  } catch (...) {
  }

  void set_console_text(const zh::utility::print::color c) const noexcept try {
    if (!*this)
      return;

    // Save the current text colors. 
    CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info{};
    GetConsoleScreenBufferInfo(m_hStdOutput, &console_screen_buffer_info);
    m_wOriginalColor = console_screen_buffer_info.wAttributes;

    // Set the text attributes to draw color text on black background. 
    SetConsoleTextAttribute(m_hStdOutput
      , static_cast<WORD>(c) | FOREGROUND_INTENSITY);
  } catch (...) {
  }

  void restore_console_text() const noexcept try {
    if (!*this)
      return;

    // Restore the original text colors.
    SetConsoleTextAttribute(m_hStdOutput, m_wOriginalColor);
  } catch (...) {
  }

private:
  mutable HANDLE m_hStdOutput;
  mutable WORD m_wOriginalColor;
};

#endif

namespace zh { namespace utility { namespace print {
  void print(const std::string& message)
  {
    fwrite(message.data(), 1, message.size(), stdout);
  }

  void print(const color c, const std::string& message)
  {
#if defined(_WIN32)
    // Note: This static initializer instance will be constructed only once.
    static struct Initializer {
      Initializer() {
        text.get_std_handle();
      }

      console_text text;
    } gConsoleService;

    gConsoleService.text.set_console_text(c);
    zh::utility::print::print2(message);
    gConsoleService.text.restore_console_text();
#else
    auto _fn_get_color_message = [](const std::string& message
      , const char* foreground) {
      return zh::utility::strings::concat_or_view(foreground, message, "\e[0m");
    };

    std::string message_with_color{};
    switch (c)
    {
      case color::red:
        message_with_color = _fn_get_color_message(message, "\e[1;31m");
      break;
      case color::green:
        message_with_color = _fn_get_color_message(message, "\e[1;32m");
      break;
      case color::blue:
        message_with_color = _fn_get_color_message(message, "\e[1;34m");
      break;

      case color::yellow:
        message_with_color = _fn_get_color_message(message, "\e[1;33m");
      break;
      case color::magenta:
        message_with_color = _fn_get_color_message(message, "\e[1;35m");
      break;
      case color::cyan:
        message_with_color = _fn_get_color_message(message, "\e[1;36m");
      break;

      default:
        message_with_color = message;
      break;
    }

    zh::utility::print::print2(message_with_color);
#endif
  }

  void print_banner()
  {
    print::print2(print::color::green, std::string(60, '='), '\n');
    print::print2(print::color::magenta, "== ");
    print::print2(print::color::red, "放胆开怀, 尽情肆意\n");
    print::print2(print::color::magenta, "== ");
    print::print2(print::color::cyan, chrono::now("%F %T %z"), '\n');
  }
} // end namespace print
} // end namespace utility
} // end namespace zh