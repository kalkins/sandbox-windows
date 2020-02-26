#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>
#include <k4a/k4a.h>
#include "k4a_helper.hpp"
#include "pipe.hpp"

using namespace sandbox;

const int TIMEOUT = 1000;

void transfer_image(Pipe &pipe, k4a_device_t &device)
{
    std::string request;
    k4a_capture_t capture = NULL;
    k4a_image_t depth_image = NULL;

    while (true) {
        request = pipe.readLine();

        if (request == "r") {
            switch (k4a_device_get_capture(device, &capture, TIMEOUT)) {
            case K4A_WAIT_RESULT_SUCCEEDED:
                break;
            case K4A_WAIT_RESULT_TIMEOUT:
                std::cout << "K4A Request timed out" << std::endl;
                capture = NULL;
                break;
            case K4A_WAIT_RESULT_FAILED:
                std::cout << "K4A Failed to read a capture" << std::endl;
                break;
            }

            if (capture != NULL) {
                depth_image = k4a_capture_get_depth_image(capture);
                uint8_t *image_buffer = k4a_image_get_buffer(depth_image);
                size_t buffer_size = k4a_image_get_size(depth_image);
                uint16_t height = k4a_image_get_height_pixels(depth_image);
                uint16_t width = k4a_image_get_width_pixels(depth_image);

				pipe.write(height);
				pipe.write(width);
                pipe.write((uint32_t) buffer_size);
                pipe.write(image_buffer, buffer_size);

                k4a_image_release(depth_image);
            } else {
				pipe.write<uint16_t>(0);
				pipe.write<uint16_t>(0);
            }
        } else if (request == "q") {
            std::cout << "The receiver closed the connection" << std::endl;
            break;
        } else {
            std::cout << "Invalid request: " << request << std::endl;
        }
    }
}

int main()
{
    int status = 0;

    std::cout << "Connecting to Kinect" << std::endl;
    k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
    config.color_format = K4A_IMAGE_FORMAT_COLOR_MJPG;
    config.color_resolution = K4A_COLOR_RESOLUTION_OFF;
    config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
    config.camera_fps = K4A_FRAMES_PER_SECOND_30;

    k4a_device_t device;

    if (k4a_open_start(device, config)) {
        std::cout << "Could not connect to kinect" << std::endl;
        status = -1;
    } else {
        std::cout << "Creating pipe" << std::endl;
        Pipe pipe("sandbox_pipe");
        std::cout << "Connected" << std::endl;

        transfer_image(pipe, device);

        sandbox::k4a_close(device);
    }

    system("pause");
    return status;
}
