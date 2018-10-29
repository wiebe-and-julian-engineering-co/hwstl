//    Copyright Julian van Doorn and Wiebe van Breukelen.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "../hal/iostream.hpp"
#include "../hal/peripheral.hpp"
#include "../hal/gpio.hpp"

namespace hwstl {
    static auto cout = hwstl::ostream<hwstl::target::uart_impl<hwstl::target::uart_peripheral::uart>>();
    static auto cin = hwstl::istream<hwstl::target::uart_impl<hwstl::target::uart_peripheral::uart>>();
    static const auto endl = '\n';
    static auto& cerr = cout;

    // using iopin = hwstl::internal::iopin<hwstl::target::pin>;
    // using ipin = hwstl::internal::ipin<hwstl::target::pin>;
    // using opin = hwstl::internal::opin<hwstl::target::pin>;

    inline uint_fast64_t now_ticks() {
        return target::now_ticks();
    }

    inline uint64_t now_us() {
        return target::now_us();
    }

    inline void wait_us_busy(int_fast32_t n) {
        target::wait_us_busy(n);
    }

    inline void wait_us(int_fast32_t n) {
        target::wait_us_busy(n);
    }

    inline void wait_ms(int_fast32_t n) {
        target::wait_ms(n);
    }

    void main();
} // namespace hwstl

int main() {
    hwstl::target::init();
    hwstl::main();
}