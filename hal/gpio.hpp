//    Copyright Julian van Doorn and Wiebe van Breukelen.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tuple>

namespace hwstl {
    template <hwstl::pin_index t_pin>
    class ipin {
    public:
        bool get() {
            return hwstl::target::pin::pin_impl<t_pin>::get();
        }

        void enable_pullup() {
            hwstl::target::pin::pin_impl<t_pin>::enable_pullup();
        }

        void disable_pullup() {
            hwstl::target::pin::pin_impl<t_pin>::disable_pullup();
        }

        void set_interrupt(const hwstl::target::interrupt::mode mode) {
            hwstl::target::pin::pin_impl<t_pin>::set_interrupt(mode);
        }
    };

    template <hwstl::pin_index t_pin>
    class opin {
    public:
        void set(bool v) {
            hwstl::target::pin::pin_impl<t_pin>::set(v);
        }
    };

    template <hwstl::pin_index t_pin>
    class iopin : public ipin<t_pin>, public opin<t_pin> { };

    template <hwstl::pin_index... vt_pins>
    auto make_ipin(hwstl::target::pin::pin_impl<vt_pins>... opins) {
        hwstl::target::pin::configure_in(hwstl::pin_sequence<vt_pins...>());

        return std::tuple(
            hwstl::ipin<vt_pins>()...
        );
    }

    template <hwstl::pin_index... vt_pins>
    auto make_opin(hwstl::target::pin::pin_impl<vt_pins>... opins) {
        hwstl::target::pin::configure_out(hwstl::pin_sequence<vt_pins...>());

        return std::tuple(
            hwstl::opin<vt_pins>()...
        );
    }

    template <hwstl::pin_index... vt_pins>
    auto make_iopin(hwstl::target::pin::pin_impl<vt_pins>... opins) {
        // hwstl::target::pin::configure_inout(hwstl::pin_sequence<vt_pins...>());

        return std::tuple(
            hwstl::iopin<vt_pins>()...
        );
    }
} // namespace hwstl