#include <iostream>
#include <sstream>

#include "json_util.h"


std::string json_to_string(Json::Value root)
{
    Json::StreamWriterBuilder builder;
    /* Strip whitespace from output to reduce size. */
    builder.settings_["indentation"] = "";
    std::string out = Json::writeString(builder, root);
    return out;
}
