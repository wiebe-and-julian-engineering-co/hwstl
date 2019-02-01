/*
 * signature.h
 *
 * Created: 4-12-2018 21:51:09
 *  Author: Wiebe
 */ 

#pragma once

#include <cstdint>
#include <type_traits>
#include <typeinfo>

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
    }
}
