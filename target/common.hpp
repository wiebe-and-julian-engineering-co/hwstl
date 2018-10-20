#pragma once

#include "../hal/iostream.hpp"
#include "../hal/peripheral.hpp"

namespace hwstl {
    static auto cout = hwstl::ostream<hwstl::target::uart_io>();
    static auto cin = hwstl::istream<hwstl::target::uart_io>();
	static auto endl = '\n';
    static auto& cerr = cout;

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
} // namespace hwstl