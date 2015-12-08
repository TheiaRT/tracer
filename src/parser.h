#include "dist/json/json.h"
#include "dist/json/json-forwards.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

#include "util/vector/vector.h"
#include "scene_object.h"
#include "sphere.h"
#include "box.h"
#include "point_light.h"
#include "material.h"
#include "scene.h"


class Parser {
public:
    Parser(std::string filename);
    Parser(Json::Value root);
    void init();
    scene_t parse();

private:
    Json::Value root;

    typedef SceneObject *(Parser::*json_converter)(Json::Value);
    std::map<std::string, json_converter> converters;

    bool valid_object_type(std::string type);

    vector3_t   json_to_vector3(Json::Value json_vector);
    color_t     json_to_color(Json::Value json_color);
    material_t  json_to_material(Json::Value json_material);
    Sphere     *json_to_sphere(Json::Value json_sphere);
    Box        *json_to_box(Json::Value json_box);
    PointLight *json_to_point_light(Json::Value json_point_light);
};
