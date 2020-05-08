#include "include/utility.string_view.h"

namespace zh
{
namespace utility
{

string_view::string_view(const std::string& s)
  : m_ptr(s.data()), m_size(s.size()) {}

std::string string_view::to_string() const {
  return std::string(m_ptr, m_size);
}

void string_view::to_string(std::string& s) const {
  s.append(m_ptr, m_size);
}

} // end namespace utility
} // end namespace zh