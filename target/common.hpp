//    Copyright Julian van Doorn and Wiebe van Breukelen.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "../hal/iostream.hpp"
#include "../hal/peripheral.hpp"
#include "../hal/gpio.hpp"
#include "../memory/ring_buffer.hpp"

namespace hwstl {
    static auto cout = hwstl::ostream<hwstl::std_interface::uart_io>();
    
    static auto cin = hwstl::istream<hwstl::std_interface::uart_io>();
    static const auto endl = '\n';
    static auto& cerr = cout;

    void main();
} // namespace hwstl

int main() {
    //hwstl::target::init();
    hwstl::device::init();
    hwstl::main();
}