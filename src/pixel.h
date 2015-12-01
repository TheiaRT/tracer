#ifndef PIXEL_H
#define PIXEL_H

#include <iostream>
#include <sstream>

#include "dist/json/json.h"


struct pixel_t {
    long r, g, b;

    pixel_t()
    {
        r = g = b = 0;
    }

    pixel_t(long r, long g, long b) : r(r), g(g), b(b)
    {
    }

    std::string to_json()
    {
        std::ostringstream strs;
        Json::Value root;
        root["r"] = (int)r;
        root["g"] = (int)g;
        root["b"] = (int)b;
        strs << root;
        return strs.str();
    }

    static bool from_json(std::string json, pixel_t &res)
    {
        Json::Value root;
        Json::Reader reader;
        if (reader.parse(json, root)) {
            res = pixel_t(root["r"].asInt(),
                          root["g"].asInt(),
                          root["b"].asInt());
            return true;
        }

        return false;
    }
};

#endif
