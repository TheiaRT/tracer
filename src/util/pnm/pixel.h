#ifndef PIXEL_H
#define PIXEL_H

#include <iostream>

#include "../../dist/json/json.h"
#include "../../json_util.h"


struct pixel_t {
    long r, g, b;

    pixel_t()
    {
        r = g = b = 0;
    }

    pixel_t(long r, long g, long b) : r(r), g(g), b(b)
    {
    }

    pixel_t(Json::Value root) : r(root[0].asInt()),
                                g(root[1].asInt()),
                                b(root[2].asInt())
    {
    }

    Json::Value to_json_value()
    {
        Json::Value root;
        root.append((int)r);
        root.append((int)g);
        root.append((int)b);
        return root;
    }

    std::string to_json()
    {
        return json_to_string(to_json_value());
    }

    static bool from_json(std::string json, pixel_t &res)
    {
        Json::Value root;
        Json::Reader reader;
        if (reader.parse(json, root, false)) {
            res = pixel_t(root);
            return true;
        }

        return false;
    }

    static pixel_t **from_json_value(Json::Value json_pixels,
                                     int width,
                                     int height)
    {
        pixel_t **pixels = new pixel_t*[height];
        for (int y = 0; y < height; y++) {
            pixels[y] = new pixel_t[width];
            for (int x = 0; x < width; x++) {
                pixels[y][x] = pixel_t(json_pixels[y][x]);
            }
        }

        return pixels;
    }
};

#endif
