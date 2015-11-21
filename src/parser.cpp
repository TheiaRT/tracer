/* Uses Jsoncpp, reference here:
 * http://open-source-parsers.github.io/jsoncpp-docs/
 */
#include "parser.h"


Parser::Parser(std::string filename)
{
    this->filename = filename;
    converters.insert(
        std::make_pair("sphere", (json_converter)&Parser::json_to_sphere)
    );
}

bool Parser::valid_object_type(std::string type)
{
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
    return {
        .ambient = json_to_color(json_material["ambient"]),
        .diffuse = json_to_color(json_material["diffuse"]),
        .specular = json_to_color(json_material["specular"]),
        .emission = json_to_color(json_material["emission"]),
        .shine = json_material["shine"].asDouble()
    };
}

Sphere *Parser::json_to_sphere(Json::Value json_sphere)
{
    return new Sphere(
        json_to_vector3(json_sphere["loc"]),
        json_to_material(json_sphere["material"]),
        json_sphere["radius"].asDouble()
    );
}

std::vector<SceneObject *> Parser::parse_file()
{
    std::vector<SceneObject *> scene_objs;
    std::ifstream contents;
    contents.open(filename);
    /* root is the top-level handle on the json object we parse */
    Json::Value root;
    Json::Reader reader;

    bool json_parse_successful = reader.parse(contents, root);
    if (json_parse_successful) {
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
                scene_objs.push_back((this->*c)(*itr));
            }
            else {
                std::cerr << "UNKNOWN TYPE " << type << std::endl;
            }
        }
    }
    return scene_objs;
}
