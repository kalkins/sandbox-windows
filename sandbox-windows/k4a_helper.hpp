#pragma once

#ifndef K4A_HELPER_H
#define K4A_HELPER_H

#include <k4a/k4a.h>

namespace sandbox {
    int k4a_open(k4a_device_t& device);

    int k4a_start(k4a_device_t& device, k4a_device_configuration_t& config);

    int k4a_open_start(k4a_device_t& device, k4a_device_configuration_t& config);

    void k4a_close(k4a_device_t& device);
}

#endif