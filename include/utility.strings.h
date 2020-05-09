#pragma once
#include <string>

namespace zh { namespace utility {
namespace details
{
  template<class T>
  auto to_string(const T& t) -> decltype(t.to_string())
  {
      return t.to_string();
  }

  // first looks up to_string on `T` using ADL; then, if that isn't found,
  // uses the above definition which returns t.to_string()
  template<class T, class = std::enable_if_t<!std::is_arithmetic<T>::value>>
  auto to_printf_arg(const T& t) -> decltype(to_string(t))
  {
    return to_string(t);
  }

  inline const char* to_printf_arg(const std::string& s) { return s.c_str(); }

  inline const char* to_printf_arg(const char* s) { return s; }

  template<class T, class = std::enable_if_t<std::is_arithmetic<T>::value>>
  T to_printf_arg(T s)
  {
    return s;
  }

  std::string format_internal(const char* fmtstr, ...);

  inline void append_internal(std::string& into, char c) { into += c; }

  template<class T, class = decltype(std::to_string(std::declval<T>()))>
  inline void append_internal(std::string& into, T x)
  {
      into += std::to_string(x);
  }

  inline void append_internal(std::string& into, const char* v)
  { into.append(v); }
  inline void append_internal(std::string& into, const std::string& s)
  { into.append(s); }

  template<class T
    , class = decltype(std::declval<const T&>().to_string(
      std::declval<std::string&>()))>
  void append_internal(std::string& into, const T& t)
  {
    t.to_string(into);
  }

  template<class T, class = void
    , class = decltype(to_string(std::declval<std::string&>()
      , std::declval<const T&>()))>
  void append_internal(std::string& into, const T& t)
  {
    to_string(into, t);
  }
} // end namespace details

namespace strings
{
  template<typename Arg>
  std::string& append(std::string& into, const Arg& a)
  {
    details::append_internal(into, a);
    return into;
  }
  template<typename Arg, typename... Args>
  std::string& append(std::string& into, const Arg& a, const Args&... args)
  {
    append(into, a);
    return append(into, args...);
  }

  template<typename... Args>
  [[nodiscard]] std::string concat(const Args&... args)
  {
    std::string ret;
    append(ret, args...);
    return ret;
  }

  template<typename... Args, class = void>
  std::string concat_or_view(const Args&... args)
  {
    return strings::concat(args...);
  }

  // template<typename T
  //   , class = std::enable_if_t<std::is_convertible<T
  //     , zh::utility::string_view>::value>>
  // zh::utility::string_view concat_or_view(const T& v)
  // {
  //   return v;
  // }

} // end namespace strings
} // end namespace utility
} // end namespace zh
