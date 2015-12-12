#include <fstream>
#include <functional>

#include "collector.h"


Collector::Collector(std::string filename,
                     size_t width,
                     size_t height,
                     size_t num_splits)
    : pixmap(PnmImage(width, height))
{

    std::ifstream ifile(filename, std::ifstream::binary);

    Json::Reader reader;
    if (reader.parse(ifile, scene, false) == false) {
        std::cerr << "Error parsing scene" << std::endl;
    }

    /* Split the work (a render of the whole scene) into num_splits pieces. */
    work_t initial = work_t(0, 0, width, height, width, height, 255);
    queue.split(initial, num_splits);
    remaining_work = num_splits;
    finished.lock();

    /* Every request hits this lambda. */
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
        /* Add the pixels to the pixmap. */
        process_work(root["work"], root["pixels"]);
        Json::Value root;
        root["success"] = true;
        return json_to_string(root);
    }
    else if (root["status"] == "need_work") {
        /* We want to reduce network slowdown by only sending the scene when
           necessary. */
        return generate_work(root["have_scene"].asBool());
    }
    else {
        return generate_error("bad_request");
    }
}

std::string Collector::generate_work(bool has_scene)
{
    Json::Value root;

    /* No need to send scene if client already has it. */
    if (has_scene == false) {
        root["scene"] = scene;
        std::cout << "client needs scene" << std::endl;
    }

    /* Return work from the queue, but don't remove it just yet. */
    root["work"] = queue.get().to_json_value();
    return json_to_string(root);
}

/* Allows Collector process to finish when it acquires the lock. */
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

/* Parse work, store in pixmap. */
void Collector::process_work(Json::Value json_work, Json::Value json_pixels)
{
    work_t work = work_t(json_work);

    std::cerr << work.id << std::endl;
    queue_lock.lock();
    if (!queue.isdone(work.id)) {
        /* This comes first so we don't double-assign. */
        queue.remove(work.id);
        remaining_work--;
        queue_lock.unlock();
        pixel_t **pixels = pixel_t::from_json_value(json_pixels,
                                                    work.width,
                                                    work.height);
        pixmap.insert_chunk(pixels, work.x, work.y, work.width, work.height);
        std::cerr << "Insert!" << std::endl;
    }
    else {
        /* Need else case so we can a) unlock early in above case and b) unlock
           if above case does not happen. */
        queue_lock.unlock();
    }

    /* Finished rendering everything! */
    if (remaining_work == 0) {
        finished.unlock();
    }
}

void Collector::write_image(std::string filename)
{
    pixmap.write(filename);
}
