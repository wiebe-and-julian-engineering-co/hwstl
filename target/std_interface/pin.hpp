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
        template <pin_index t_pin>
        class pin {
        public:
            constexpr pin() = delete;

            static inline void set(bool v) {
                static_assert(
                    !std::is_same<
                        decltype(hwstl::device::pin::set<t_pin>(v)),
                        interface_exception
                    >::value,
                    "void hwstl::device::pin::set<t_pin>(bool) is not implemented for target"
                );
                static_assert(
                    std::is_same<
                        decltype(hwstl::device::pin::set<t_pin>(v)),
                        void
                    >::value,
                    "void hwstl::device::pin::set<t_pin>(bool) is not implemented for target"
                );
                hwstl::device::pin::set<t_pin>(v);
            }

            static inline bool get() {
                static_assert(
                    !std::is_same<
                        decltype(hwstl::device::pin::get<t_pin>()),
                        interface_exception
                    >::value,
                    "bool hwstl::device::pin::get<t_pin>() is not implemented for target"
                );
                static_assert(
                    std::is_same<
                        decltype(hwstl::device::pin::get<t_pin>()),
                        bool
                    >::value,
                    "bool hwstl::device::pin::get<t_pin>() is not implemented for target"
                );
                return hwstl::device::pin::get<t_pin>();
            }

            static inline void enable_pullup() {
                
            }

            static inline void disable_pullup() {

            }
        };
    }
}