#pragma once



#if TARGET == arduino_due

#include "arduino-due/arduino-due.hpp"

namespace hwstl {
    //namespace device = hwstl::arduino_due;
    namespace target = hwstl::arduino_due;
    namespace sf = hwstl::subframework;
    
    //namespace target = hwstl::arduino_due;
    //namespace target::pin = hwstl::subframework::pin<hwstl::arduino_due>;
    
    //static auto target::pin = hwstl::subframework::pin<device>;
    
    
    namespace pins {
        #ifdef HWSTL_ONCE
        auto d0 = target::pin::pin_impl<0>();
        auto d1 = target::pin::pin_impl<1>();
        auto d2 = target::pin::pin_impl<2>();
        auto d7 = target::pin::pin_impl<7>();
        auto d13 = target::pin::pin_impl<13>();
        #endif
    }
}
#endif

#include "subframework.hpp"
#include "common.hpp"