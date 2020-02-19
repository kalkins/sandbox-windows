#include <stdio.h>
#include <k4a/k4a.h>
#include "k4a_helper.hpp"

namespace sandbox {
	int k4a_open(k4a_device_t& device)
	{
		uint32_t device_count = k4a_device_get_installed_count();

		if (device_count == 0) {
			printf("No Kinect devices found.\n");
			return -1;
		}
		else if (device_count > 1) {
			printf("Multiple Kinects found. Using the first one.\n");
		}

		if (K4A_RESULT_SUCCEEDED != k4a_device_open(0, &device)) {
			printf("Failed to open Kinect\n");

			if (device != NULL) {
				k4a_device_close(device);
			}

			return -1;
		}

		return 0;
	}

	int k4a_start(k4a_device_t& device, k4a_device_configuration_t& config)
	{
		if (K4A_RESULT_SUCCEEDED != k4a_device_start_cameras(device, &config)) {
			printf("Failed to start device\n");
			if (device != NULL) {
				k4a_device_close(device);
			}

			return -1;
		}

		return 0;
	}

	int k4a_open_start(k4a_device_t& device, k4a_device_configuration_t& config)
	{
		if (k4a_open(device)) {
			return -1;
		}

		if (k4a_start(device, config)) {
			k4a_close(device);
			return -1;
		}

		return 0;
	}

	void k4a_close(k4a_device_t& device)
	{
		if (device != NULL) {
			k4a_device_close(device);
		}
	}
}