#include <iostream>
#include <sstream>

#include "json_util.h"

std::string json_to_string(Json::Value root)
{
    Json::StreamWriterBuilder builder;
    builder.settings_["indentation"] = "";
    std::string out = Json::writeString(builder, root);
    return out;
}
