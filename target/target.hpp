#pragma once

#if TARGET == arduino_due
#include "arduino-due.hpp"
namespace hwstl {
    namespace target = hwstl::arduino_due;
}
#endif

#include "common.hpp"