#ifndef INCLUDE_UTILITY_RANDOM_H__
#define INCLUDE_UTILITY_RANDOM_H__
#include <string>

namespace zh
{
namespace utility
{
namespace random
{
int get_uniform_distribution(int min = 0, int max = 0X0F);
std::string get_uuid();

} // end namespace random
} // end namespace utility
} // end namespace zh

#endif // INCLUDE_UTILITY_RANDOM_H__