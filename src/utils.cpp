#include "utils.h"

const char* Utils::StringToCopiedCstr(std::string &str){
    char * cstr = new char[str.size() + 1];
    std::copy(str.begin(), str.end(), cstr);
    cstr[str.size()] = '\0';
    return cstr;
}
const char* Utils::StringstreamToCstr(std::stringstream &stream){
    auto sStr = stream.str();
    return StringToCopiedCstr(sStr);
}
