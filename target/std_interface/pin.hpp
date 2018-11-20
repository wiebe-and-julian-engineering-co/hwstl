/*
 * pin.h
 *
 * Created: 18-11-2018 13:22:18
 *  Author: Wiebe
 */ 

#pragma once

#include <cstdint>
#include <type_traits>
#include <typeinfo>

#include "pin_defs.hpp"

namespace hwstl {
    namespace std_interface {
        template <class, class>
        struct has_signature;

        template <class, class, bool>
        struct has_signature_helper;

        template <class t_lhs, class t_rhs>
        struct has_signature_helper<t_lhs, t_rhs, false> {
            static constexpr bool value = false;
        };

        template <class t_tested_return, class... vt_tested_args, class t_required_return, class... vt_required_args>
        struct has_signature_helper<t_tested_return(vt_tested_args...), t_required_return(vt_required_args...), true> {
            static constexpr bool value = (std::is_same<t_tested_return, t_required_return>::value && ... && std::is_same<vt_tested_args, vt_required_args>::value);
        };

        template <class t_tested_return, class... vt_tested_args, class t_required_return, class... vt_required_args>
        struct has_signature<t_tested_return(vt_tested_args...), t_required_return(vt_required_args...)> {
            static constexpr bool value = has_signature_helper<
                t_tested_return(vt_tested_args...),
                t_required_return(vt_required_args...),
                sizeof...(vt_tested_args) == sizeof...(vt_required_args)
            >::value;
        };

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