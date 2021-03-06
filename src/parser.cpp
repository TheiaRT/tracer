/* Uses Jsoncpp, reference here:
 * http://open-source-parsers.github.io/jsoncpp-docs/
 */
#include "parser.h"
#include <iostream>


Parser::Parser(std::string filename)
{
    init();

    std::ifstream contents(filename, std::ifstream::binary);

    /* root is the top-level handle on the json object we parse */
    Json::Reader reader;
    if (reader.parse(contents, root, false) == false) {
        std::cerr << "Error parsing scene" << std::endl;
    }
}

Parser::Parser(Json::Value root) : root(root)
{
    init();
}

void Parser::init()
{
    /* We map object types to their JSON converters. */
    converters.insert(
        std::make_pair("sphere", (json_converter)&Parser::json_to_sphere)
    );
    converters.insert(
        std::make_pair("box", (json_converter)&Parser::json_to_box)
    );
    converters.insert(
        std::make_pair("point_light",
                       (json_converter)&Parser::json_to_point_light)
    );
}

bool Parser::valid_object_type(std::string type)
{
    /* Is the type in the map? */
    return converters.find(type) != converters.end();
}

vector3_t Parser::json_to_vector3(Json::Value json_vector)
{
    return vector3_t(
        json_vector["x"].asDouble(),
        json_vector["y"].asDouble(),
        json_vector["z"].asDouble()
        );
}

color_t Parser::json_to_color(Json::Value json_color)
{
    return color_t(
        json_color["r"].asDouble(),
        json_color["g"].asDouble(),
        json_color["b"].asDouble(),
        json_color["a"].asDouble()
        );
}

material_t Parser::json_to_material(Json::Value json_material)
{
    material_t mat = material_t(
        json_to_color(json_material["ambient"]),
        json_to_color(json_material["diffuse"]),
        json_to_color(json_material["specular"]),
        json_to_color(json_material["emission"]),
        json_material["reflection"].asDouble()
    );

    /* Pptionally set refraction amount, and index of refraction.
       If unspecified default to 0 and 1 respectively. */
    mat.refraction =
        json_material.get("refraction", Json::Value(0.0)).asDouble();
    mat.refraction_index =
        json_material.get("refraction_index", Json::Value(1)).asDouble();
    mat.texture = json_material.get("texture", Json::Value(false)).asBool();


    return mat;
}

Sphere *Parser::json_to_sphere(Json::Value json_sphere)
{
    return new Sphere(
        json_to_vector3(json_sphere["loc"]),
        json_to_material(json_sphere["material"]),
        json_sphere["radius"].asDouble()
        );
}

Box *Parser::json_to_box(Json::Value json_box)
{
    return new Box(
        json_to_vector3(json_box["min"]),
        json_to_vector3(json_box["max"]),
        json_to_material(json_box["material"])
    );
}

PointLight *Parser::json_to_point_light(Json::Value json_point_light) {

    return new PointLight(
            json_to_vector3(json_point_light["loc"]),
            json_to_color(json_point_light["intensity"]));
}

scene_t Parser::parse()
{
    scene_t scene;

    /* The list of scene objects is referenced by key:
     * "scene_objects"
     */
    Json::Value scene_objs_json = root["scene_objects"];
    /* Create scene_obj from each item in JSON "scene_objects"[]
     * based on object_type; append object into scene_objs vector
     */
    for (Json::ValueIterator itr = scene_objs_json.begin();
         itr != scene_objs_json.end(); itr++) {

        /* We branch based on the type of object we encounter
         * in the scene file.
         */
        std::string type = (*itr)["object_type"].asString();
        if (valid_object_type(type)) {
            json_converter c = converters[type];
            scene.objects.push_back((this->*c)(*itr));
        }
        else {
            std::cerr << "UNKNOWN TYPE " << type << std::endl;
        }
    }
    Json::Value scene_lights_json = root["scene_lights"];
    for (Json::ValueIterator itr = scene_lights_json.begin();
         itr != scene_lights_json.end(); itr++) {

        /* We branch based on the type of object we encounter
         * in the scene file.
         */
        std::string type = (*itr)["object_type"].asString();
        if (valid_object_type(type)) {
            json_converter c = converters[type];
            scene.lights.push_back((this->*c)(*itr));
        }
        else {
            std::cerr << "UNKNOWN TYPE " << type << std::endl;
        }
    }

    return scene;
}
