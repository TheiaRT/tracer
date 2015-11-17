/* Uses Jsoncpp, reference here:
 * http://open-source-parsers.github.io/jsoncpp-docs/ 
 */

#include "parser.h"

using namespace std;

Parser::Parser(string filename) {
    this->filename = filename;
}

/*
 * json_to_vector3
 * json_to_material
 * json_to_sphere
 */


vector3_t Parser::json_to_vector3(Json::Value json_vector) {
    return vector3_t(
            json_vector["x"].asDouble(),
            json_vector["y"].asDouble(),
            json_vector["z"].asDouble());
}

color_t Parser::json_to_color(Json::Value json_color) {
    return color_t(
            json_color["r"].asDouble(),
            json_color["g"].asDouble(),
            json_color["b"].asDouble(),
            json_color["a"].asDouble()
            );
}

material_t Parser::json_to_material(Json::Value json_material) {
    return { .ambient = json_to_color(json_material["ambient"]),
        .diffuse = json_to_color(json_material["diffuse"]),
        .specular = json_to_color(json_material["specular"]),
        .emission = json_to_color(json_material["emission"]),
        .shine = json_material["shine"].asDouble()};
}

Sphere *Parser::json_to_sphere(Json::Value json_sphere) {
    return new Sphere(
            json_to_vector3(json_sphere["loc"]),
            json_to_material(json_sphere["material"]),
            json_sphere["radius"].asDouble());

}

std::vector<SceneObject *> Parser::parse_file() {
    std::vector<SceneObject *> scene_objs;
    ifstream contents;
    contents.open(filename);
    Json::Value root;
    Json::Reader reader;
    bool json_parse_successful = reader.parse(contents, root);

    if (json_parse_successful) {
        cout << "Json Parsing Success" <<endl;
        Json::Value scene_objs_json = root["scene_objects"];
        for (Json::ValueIterator itr = scene_objs_json.begin();
                itr != scene_objs_json.end(); itr++) {
            if ((*itr)["object_type"] == "sphere") {
                cout << "SPHERE" << endl;
                scene_objs.push_back(json_to_sphere(*itr));
            }
        }
    }
    return scene_objs;
}





