//    Copyright Julian van Doorn and Wiebe van Breukelen.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tuple>

#include "../target/std_interface/pin_defs.hpp"

namespace hwstl {
    template <hwstl::pin_index t_pin>
    class ipin {
    public:
        bool get() {
            return hwstl::std_interface::pin<t_pin>::get();
        }

        void enable_pullup() {
            hwstl::std_interface::pin<t_pin>::enable_pullup();
        }

        void disable_pullup() {
            hwstl::std_interface::pin<t_pin>::disable_pullup();
        }
    };

    template <hwstl::pin_index t_pin>
    class opin {
    public:
        void set(bool v) {
            hwstl::std_interface::pin<t_pin>::set(v);
        }

        void enable_pullup() {
            hwstl::std_interface::pin<t_pin>::enable_pullup();
        }

        void disable_pullup() {
            hwstl::std_interface::pin<t_pin>::disable_pullup();
        }
    };

    template <hwstl::pin_index t_pin>
    class iopin : public ipin<t_pin>, public opin<t_pin> { };

    template <hwstl::pin_index... vt_pins>
    auto make_ipin(hwstl::pin_definition<vt_pins>... opins) {
        hwstl::device::pin::configure_in(hwstl::pin_sequence<vt_pins...>());

        return std::tuple(
            hwstl::ipin<vt_pins>()...
        );
    }

    template <hwstl::pin_index... vt_pins>
    auto make_opin(hwstl::pin_definition<vt_pins>... opins) {
        hwstl::device::pin::configure_out(hwstl::pin_sequence<vt_pins...>());

        return std::tuple(
            hwstl::opin<vt_pins>()...
        );
    }

    template <hwstl::pin_index... vt_pins>
    auto make_iopin(hwstl::pin_definition<vt_pins>... opins) {
        // hwstl::std_interface::pin::configure_in_out(hwstl::pin_sequence<vt_pins...>());

        return std::tuple(
            hwstl::iopin<vt_pins>()...
        );
    }
} // namespace hwstl