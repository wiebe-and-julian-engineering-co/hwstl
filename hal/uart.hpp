#pragma once

#include "../target/target.hpp"

namespace hwstl {
    template <class t_setup, class t_unset, class t_putc, class t_getc>
    class UART {
    public:
        UART() {
            t_setup();
        }

        ~UART() {
            t_unset();
        }

        inline void putc(unsigned char c) {
            putc(c);
        }

        inline unsigned char getc() {
            return getc();
        }
    };
} // namespace hwstl