#pragma once
#include <string>

namespace zh
{
namespace utility
{
namespace random
{
int get_uniform_distribution(int min = 0, int max = 0X0F);

// For the X conversion letters ABCDEF are used.
std::string get_uuid(bool X = true);

} // end namespace random
} // end namespace utility
} // end namespace zh
