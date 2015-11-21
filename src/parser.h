#include "dist/json/json.h"
#include "dist/json/json-forwards.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

#include "vector.h"
#include "scene_object.h"
#include "sphere.h"
#include "material.h"
#include "point_light.h"


class Parser {
public:
    Parser(std::string filename);
    void parse_file(
            std::vector<SceneObject *> &scene_objs,
            std::vector<SceneObject *> &scene_lights);

private:
    std::string filename;

    typedef SceneObject *(Parser::*json_converter)(Json::Value);
    std::map<std::string, json_converter> converters;

    bool valid_object_type(std::string type);
    vector3_t json_to_vector3(Json::Value json_vector);
    color_t json_to_color(Json::Value json_color);
    material_t json_to_material(Json::Value json_material);
    Sphere *json_to_sphere(Json::Value json_sphere);
    PointLight *json_to_point_light(Json::Value json_point_light);
};
