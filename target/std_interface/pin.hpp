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