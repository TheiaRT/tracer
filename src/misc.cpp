#include <iostream>
#include <sstream>

#include "misc.h"

std::string json_to_string(Json::Value root)
{
    std::ostringstream strs;
    strs << root;
    return strs.str();
}
