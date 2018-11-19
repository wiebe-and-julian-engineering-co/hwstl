#pragma once

#include "pin_defs.hpp"

namespace hwstl {
    class interface_exception {
    public:
        constexpr interface_exception() { }
    };

    namespace interface_contracts {
        namespace pin {
            // If no target::pin::set(bool) was defined,
            // interface_contract::pin::set(bool) is used as fallback. Because
            // the fallback function has return type interface_exception,
            // decltype(<expression>) can be used to identify the errornous
            // function.
            // Using that information an error can be reported during
            // compilation.
            template <pin_index t_pin>
            constexpr interface_exception set(bool);

            template <pin_index t_pin>
            constexpr interface_exception get();
        }

        class uart_io {
        public:
            uart_io() = delete;

            constexpr interface_exception putc(bool);
            constexpr interface_exception getc();
        };
    }
};