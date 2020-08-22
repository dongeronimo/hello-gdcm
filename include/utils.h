#ifndef __utils
#define __utils
#include <sstream>
namespace Utils{
    const char* StringstreamToCstr(std::stringstream &stream);
    const char* StringToCopiedCstr(std::string &str);
}
#endif
