
class Worker {
    void get_work(Collector *c);
    // trace_scene
    // submit_work(Collector *c)
    long collectorID; // or something
    SceneObject *scene;

    bool hasWork();
    RayTracer rTracer();
    // are we working directly on the collector's image?

    //storing work locally & sending all back when done
    std::vector<pixel_t>
};

class Collector {

};

class WorkQueue{

};

struct work_t{
    std::vector<vector2_t> workVector;

    work_t(vector2_t topRightCoord, coor){

    }

    to_JSON() {

    }
    
};



{

public:
    Parser(std::string filename);
    std::vector<SceneObject *> parse_file();

private:
    std::string filename;

    typedef SceneObject *(Parser::*json_converter)(Json::Value);
    std::map<std::string, json_converter> converters;

    bool valid_object_type(std::string type);
    vector3_t json_to_vector3(Json::Value json_vector);
    color_t json_to_color(Json::Value json_color);
    material_t json_to_material(Json::Value json_material);
    Sphere *json_to_sphere(Json::Value json_sphere);
};