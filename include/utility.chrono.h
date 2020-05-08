#pragma once
#include <string>

namespace zh
{
namespace utility
{
namespace chrono
{
// 2020-05-07 18:21:30 610075 +0800
// %F:  ISO 8601 日期格式
// %T:  ISO 8601 时间格式
// %z:  以 ISO 8601 格式（例如 -0430 ）写距 UTC 的偏移
//      或者倘若时区信息不可用则不写字符
// %us: 微秒
std::string now(const char* format = "%F %T %us %z");

} // end namespace chrono
} // end namespace utility
} // end namespace zh
