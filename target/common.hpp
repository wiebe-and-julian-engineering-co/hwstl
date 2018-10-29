//    Copyright Julian van Doorn and Wiebe van Breukelen.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "../hal/iostream.hpp"
#include "../hal/peripheral.hpp"
#include "../hal/gpio.hpp"
#include "../hal/hwconfig.hpp"

namespace hwstl {
    static auto cout = hwstl::ostream<hwstl::target::uart_impl<hwstl::target::uart_port::peripheral::uart>>();
    static auto cin = hwstl::istream<hwstl::target::uart_impl<hwstl::target::uart_port::peripheral::uart>>();
    static const auto endl = '\n';
    static auto& cerr = cout;

    /**
     * @brief Generates a config ostream allowing for configuring streams
     * 
     * @details
     * Constructs a config ostream class which allows for compile time
     * optimized configuring for stream based peripherals.
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
        return _hwconfig<t_peripheral_type, t_peripheral, hwstl::target::masks>();
    }

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