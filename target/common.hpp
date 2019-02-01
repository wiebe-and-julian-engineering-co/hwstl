//    Copyright Julian van Doorn and Wiebe van Breukelen.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "../hwstl_config.hpp"
#include "../hal/iostream.hpp"
#include "../hal/peripheral.hpp"
#include "../hal/gpio.hpp"
#include "../memory/ring_buffer.hpp"
#include "../hal/hwconfig.hpp"
#include "../hal/internal.hpp"
#include "../hal/interrupt.hpp"
#include "type_definition.hpp"

namespace hwstl {
    
#ifdef HWSTL_ENABLE_UART
    static auto cout = hwstl::ostream<hwstl::std_interface::uart_io>();
    static auto cin = hwstl::istream<hwstl::std_interface::uart_io>();
    static const auto endl = '\n';
    static auto& cerr = cout;
#endif

    /**
     * @brief Generates a config ostream allowing for configuring streams
     * 
     * @details
     * Constructs a config ostream class which allows for compile time
     * optimized configuring for stream based peripherals.
     * 
     * @addtogroup Broken
     * 
     * @param ios Stream to configure
     * @return _hwconfig
     */
    template <
        class t_peripheral_type,
        t_peripheral_type t_peripheral,
        template < t_peripheral_type > class t_impl,
        template < class > class t_ios
    >
    auto hwconfig(t_ios<t_impl<t_peripheral>>& ios) {
        // return _hwconfig<t_peripheral_type, t_peripheral, hwstl::target::masks>();
    }

    void main();
} // namespace hwstl

int main() {
    //hwstl::target::init();
    hwstl::device::init();
    hwstl::main();
}