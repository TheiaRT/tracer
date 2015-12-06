#ifndef WORK_H
#define WORK_H

#include "dist/json/json.h"


struct work_t {
    static const int INVALID_ID = -1;
    static const int NEW_ID = -2;

    int id;
    int x, y;
    int width, height;
    int image_width, image_height;
    int denominator;
    bool done;

    work_t() : id(INVALID_ID)
    {
    }

    work_t(int x, int y, int width, int height,
           int image_width, int image_height, int denominator)
        : id(NEW_ID), x(x), y(y),
          width(width), height(height),
          image_width(image_width), image_height(image_height),
          denominator(denominator),
          done(false)
    {
    }

    work_t(Json::Value json_work)
        : id(NEW_ID), x(json_work["x"].asInt()), y(json_work["y"].asInt()),
          width(json_work["width"].asInt()), height(json_work["height"].asInt()),
          image_width(json_work["image_width"].asInt()),
          image_height(json_work["image_height"].asInt()),
          denominator(json_work["denominator"].asInt()),
          done(false)
    {
    }

    static work_t invalid()
    {
        return work_t();
    }

    bool validp()
    {
        return id != INVALID_ID;
    }

    bool newp()
    {
        return id == NEW_ID;
    }

    bool donep()
    {
        return done;
    }

    Json::Value to_json_value()
    {
        Json::Value root;
        root["id"] = id;
        root["x"] = x;
        root["y"] = y;
        root["width"] = width;
        root["height"] = height;
        root["image_width"] = image_width;
        root["image_height"] = image_height;
        root["denominator"] = denominator;
        root["done"] = done;
        return root;
    }

    work_t *split(size_t chunks_count)
    {
        work_t *chunks = new work_t[chunks_count];
        int width = this->width / chunks_count;

        for (size_t i = 0; i < chunks_count; i++) {
            chunks[i].y = this->y;
            chunks[i].height = this->height;
            chunks[i].x = i * width;
            chunks[i].width = width;
            chunks[i].image_width = this->image_width;
            chunks[i].image_height = this->image_height;
            chunks[i].denominator = this->denominator;
            chunks[i].done = false;
            chunks[i].id = NEW_ID;
        }
        return chunks;
    }
};

#endif
