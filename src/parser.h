#include "dist/json/json.h"
#include "dist/json/json-forwards.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
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
    vector3_t json_to_vector3(Json::Value json_vector);
    color_t json_to_color(Json::Value json_color);
    material_t json_to_material(Json::Value json_material);
    Sphere *json_to_sphere(Json::Value json_sphere);

};



