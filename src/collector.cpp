#include <fstream>
#include <functional>

#include "collector.h"


Collector::Collector(std::string filename,
                     size_t width,
                     size_t height,
                     size_t splits)
    : vp_width(width), vp_height(height), pixmap(PnmImage(width, height))
{

    std::ifstream ifile(filename, std::ifstream::binary);

    Json::Reader reader;
    if (reader.parse(ifile, scene, false) == false) {
        std::cerr << "Error parsing scene" << std::endl;
    }

    work_t initial = work_t(0, 0, width, height, width, height, 255);
    queue.split(initial, splits);
    remaining_work = splits;
    finished.lock();

    server = new TCPServer([=](std::string req) {
        return this->serve_request(req);
    });
}

Collector::~Collector()
{
    delete server;
}

bool Collector::start(std::string host, int port)
{
    std::cout << "Listening on " << host << ":" << port << "...\n";
    return server->start(host, port);
}

void Collector::stop()
{
    server->stop();
}

std::string Collector::serve_request(std::string req)
{
    Json::Value root;
    Json::Reader reader;
    if (reader.parse(req, root, false) == false) {
        return generate_error("bad_json");
    }

    if (root["status"] == "have_work") {
        std::cout << "work: " << root["work"] << std::endl;
        process_work(root["work"], root["pixels"]);
        Json::Value root;
        root["success"] = true;
        return json_to_string(root);
    }
    else if (root["status"] == "need_work") {
        return generate_work();
    }
    else {
        return generate_error("bad_request");
    }
}

/* TODO: remove sample */
std::string Collector::generate_work()
{
    //work_t example(0, 0, vp_width, vp_height, vp_width, vp_height, 255);
    Json::Value root;
    root["scene"] = scene;
    root["work"] = queue.get().to_json_value();
    return json_to_string(root);
}

bool Collector::finish()
{
    finished.lock();
    finished.unlock();
    return true;
}

std::string Collector::generate_error(std::string type)
{
    Json::Value error;
    error["error"] = true;
    error["type"] = type;
    return json_to_string(error);
}

/* parse work, store in pixmap */
/* TODO: remove sample */
void Collector::process_work(Json::Value json_work, Json::Value json_pixels)
{
    work_t work = work_t(json_work);
    pixel_t **pixels = pixel_t::from_json_value(json_pixels,
                                                work.width,
                                                work.height);

    std::cerr << work.id << std::endl;
    if (!queue.isdone(work.id)) {
        pixmap.insert_chunk(pixels, work.x, work.y, work.width, work.height);
        remaining_work--;
        queue.remove(work.id);
        std::cerr << "Insert!" << std::endl;
    }
    if (remaining_work == 0) {
        finished.unlock();
    }
}

void Collector::write_image(std::string filename)
{
    pixmap.write(filename);
}
