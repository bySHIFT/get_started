#pragma once
#include <string>

namespace zh
{
namespace utility
{
struct string_view
{
  constexpr string_view() = default;
  string_view(const std::string& s); // Implicit by design

  // NOTE: we do this instead of the delegating constructor
  //       since delegating ctors are a perf footgun
  template<size_t Sz>
  constexpr string_view(const char (&arr)[Sz]) : m_ptr(arr), m_size(Sz - 1)
  {
  }

  constexpr string_view(const char* ptr, size_t size)
    : m_ptr(ptr)
    , m_size(size)
  {
  }

  constexpr string_view(const char* b, const char* e)
    : m_ptr(b)
    , m_size(static_cast<size_t>(e - b))
  {
  }

  constexpr const char* begin() const { return m_ptr; }
  constexpr const char* end() const { return m_ptr + m_size; }

  constexpr const char* data() const { return m_ptr; }
  constexpr size_t size() const { return m_size; }

  std::string to_string() const;
  void to_string(std::string& out) const;

private:
  const char* m_ptr = 0;
  size_t m_size = 0;
};
}
}