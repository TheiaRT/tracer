#include "work.h"
#include "parser.h"
#include "worker.h"
#include "misc.h"

Worker::Worker(std::string host, int port)
    : collector_host(host), collector_port(port)
{
    tracer = NULL;
}

Worker::~Worker()
{
    delete tracer;
}

/* returns true if the server had work to send */
bool Worker::get_work()
{
    std::string work_request = generate_work_request();
    std::string json_message;
    if (send_and_receive(work_request, json_message) == false) {
        return false;
    }

    /* Set class member work to the work received. */
    if (parse_work_message(json_message, work, tracer) == false) {
        return false;
    }

    have_work = true;
    return true;
}

bool Worker::parse_work_message(std::string json_message,
                                work_t &work,
                                RayTracer *&tracer)
{
    Json::Value message;
    Json::Reader reader;
    if (reader.parse(json_message, message, false) == false) {
        std::cout << "could not parse message: " << json_message << std::endl;
        std::cout << reader.getFormattedErrorMessages() << std::endl;
        return false;
    }

    if (message["status"] == "no_work") {
        return false;
    }

    work = work_t(message["work"]);
    std::vector<SceneObject *> objects, lights;
    Parser p(message["scene"]);
    p.parse(objects, lights);

    tracer = new RayTracer(objects, lights);
    return true;
}

bool Worker::trace_and_send_work()
{
    if (!have_work) {
        return false;
    }

    std::string json_work = trace();
    std::string resp;
    /* send the json work to the server */
    if (send_and_receive(json_work, resp) == false) {
        return false;
    }

    have_work = false;
    return true;
}

/* render the pixels and return json */
std::string Worker::trace()
{
    pixel_t **pixels = tracer->render_pixel_chunk(work.x,
                                                  work.y,
                                                  work.width,
                                                  work.height,
                                                  work.image_width,
                                                  work.image_height,
                                                  work.denominator);
    Json::Value root;
    root["status"] = "have_work";
    root["work"] = work.to_json_value();
    for (int i = 0; i < work.height; i++) {
        Json::Value pixel_row;
        for (int j = 0; j < work.width; j++) {
            pixel_row.append(pixels[i][j].to_json_value());
        }
        delete [] pixels[i];
        root["pixels"].append(pixel_row);
    }
    delete [] pixels;

    return json_to_string(root);
}

bool Worker::send_and_receive(std::string req, std::string &resp)
{
    TCPClient client;
    if (client.connect(collector_host, collector_port) == false) {
        return false;
    }

    // std::cout << "sending data: " << req << std::endl;
    if (client.send_data(req) == false) {
        return false;
    }

    resp = client.receive();
    // std::cout << "got data: " << resp << std::endl;
    return true;
}

std::string Worker::generate_work_request()
{
    Json::Value root;
    root["status"] = "need_work";
    return json_to_string(root);
}
