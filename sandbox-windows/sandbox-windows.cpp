#include <iostream>
#include <string>
#include <vector>
#include <k4a/k4a.h>
#include "k4a_helper.hpp"
#include "pipe.hpp"

using namespace sandbox;

int main()
{
    std::cout << "Connecting to Kinect" << std::endl;
    k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
    config.color_format = K4A_IMAGE_FORMAT_COLOR_MJPG;
    config.color_resolution = K4A_COLOR_RESOLUTION_OFF;
    config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
    config.camera_fps = K4A_FRAMES_PER_SECOND_30;

    k4a_device_t device;

    if (k4a_open_start(device, config)) {
        std::cout << "Could not connect to kinect" << std::endl;
        return -1;
    }

    Pipe pipe("sandbox_pipe");

    char c;
    std::string msg;
    std::vector<uint16_t> data = { 1, 2, 3, 4 };

    while (true) {
        std::cout << "Waiting for message" << std::endl;
        c = pipe.read();

        if (c == 'r') {
            std::cout << "Sending response" << std::endl;
            pipe.write(std::to_string(data.size()));

            for (auto& d : data) {
                d++;
                pipe.write(std::to_string(d));
            }

            std::cout << "Message sent" << std::endl;
        }
        else if (msg == "q") {
            break;
        }
    }

    k4a_close(device);
    return 0;
}
