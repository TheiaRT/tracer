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


class Parser {
public:
    Parser(std::string filename);
    std::vector<SceneObject *> parse_file();

private:
    std::string filename;

    typedef SceneObject *(Parser::*json_converter)(Json::Value);
    std::map<std::string, json_converter> converters;

    bool valid_object_type(std::string type);

    template <int N>
    vector_t<N> json_to_vector(Json::Value json_vector);
    color_t     json_to_color(Json::Value json_color);
    material_t  json_to_material(Json::Value json_material);
    Sphere     *json_to_sphere(Json::Value json_sphere);
};
