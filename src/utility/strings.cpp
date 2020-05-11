#include "utility/strings.h"

#include <cstdarg>

namespace zh { namespace utility {
namespace details
{
std::string format_internal(const char* fmtstr, ...)
{
    va_list args;
    va_start(args, fmtstr);

    const int sz = vsnprintf(nullptr, 0, fmtstr, args);

    std::string output(sz, '\0');

    va_start(args, fmtstr);
    vsnprintf(&output.at(0), output.size() + 1, fmtstr, args);

    va_end(args);

    return output;
}
} // end namesapce details
} // end namespace utility
} // end namespace zh