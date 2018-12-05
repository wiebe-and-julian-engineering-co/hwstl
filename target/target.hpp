#pragma once

#include "../hwstl_config.hpp"

#if TARGET == arduino_due
#include "arduino-due/arduino-due.hpp"
namespace hwstl {
    namespace device = hwstl::arduino_due;
    namespace pins = device::pins;
}
#include "std_interface/std_interface.hpp"
#endif

#include "common.hpp"