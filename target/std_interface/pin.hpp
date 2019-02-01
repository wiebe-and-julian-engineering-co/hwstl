/*
 * pin.h
 *
 * Created: 18-11-2018 13:22:18
 *  Author: Wiebe
 */ 

#pragma once
#include "signature.hpp"
#include "pin_defs.hpp"

namespace hwstl {
    namespace std_interface {
        template <pin_index t_pin>
        class pin {
        public:
            constexpr pin() = delete;

            static inline void set(bool v) {
                static_assert(has_signature<decltype(hwstl::device::pin::set<t_pin>), void(bool)>::value, "void hwstl::device::pin::set<t_pin>(bool) is not implemented for target");
                hwstl::device::pin::set<t_pin>(v);
            }

            static inline bool get() {
                static_assert(has_signature<decltype(hwstl::device::pin::get<t_pin>), bool(void)>::value, "bool hwstl::device::pin::set<t_pin>(bool) is not implemented for target");
                return hwstl::device::pin::get<t_pin>();
            }

            static inline void enable_pullup() {
                static_assert(has_signature<decltype(hwstl::device::pin::enable_pullup<t_pin>), void(void)>::value, "void hwstl::device::pin::enable_pullup<t_pin>() is not implemented for target");
                hwstl::device::pin::enable_pullup<t_pin>(); 
            }

            static inline void disable_pullup() {
                static_assert(has_signature<decltype(hwstl::device::pin::disable_pullup<t_pin>), void(void)>::value, "void hwstl::device::pin::disable_pullup<t_pin>() is not implemented for target");
                hwstl::device::pin::disable_pullup<t_pin>();
            }
        };

        namespace pins {
            template <pin_index... vt_pins>
            static inline constexpr void configure_in(pin_sequence<vt_pins...> pins) {
                // Compiler throws an decltype error - this function is overloaded.
                //static_assert(has_signature<TestType<hwstl::device::pin::configure_in>, void(pin_sequence<vt_pins...>)>::value, "void hwstl::device::pins::configure_in<pin_sequence<vt_pins...> is not implemented for target");
                hwstl::device::pin::configure_in(pins);
            }

            template <pin_index... vt_pins>
            static inline constexpr void configure_out(pin_sequence<vt_pins...> pins) {
                // Compiler throws an decltype error - this function is overloaded.
                //static_assert(has_signature<decltype(hwstl::device::pin::configure_out(pins)), void(pin_sequence<vt_pins...>)>::value, "void hwstl::device::pins::configure_out<pin_sequence<vt_pins...> is not implemented for target");
                hwstl::device::pin::configure_out(pins);
            }

            template <pin_index... vt_pins>
            static inline constexpr void configure_in_out(pin_sequence<vt_pins...> pins) {
            // Compiler throws an decltype error - this function is overloaded.
                hwstl::device::pin::configure_in_out(pins);
            }
        };

    }
}