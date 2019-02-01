#pragma once

#include "signature.hpp"

namespace hwstl {
    namespace std_interface {
        class uart_io {
        public:
            static inline void enable() {
                static_assert(has_signature<decltype(hwstl::device::uart_io::enable), void(void)>::value, "void hwstl::device::uart_io::enable is not implemented for target");
            }
            static inline void disable() {
                static_assert(has_signature<decltype(hwstl::device::uart_io::disable), void(void)>::value, "void hwstl::device::uart_io::disable is not implemented for target");
            }
            static inline void putc(const unsigned char c) {
                static_assert(has_signature<decltype(hwstl::device::uart_io::putc), void(const unsigned char)>::value, "void hwstl::device::uart_io::putc is not implemented for target");
                hwstl::device::uart_io::putc(c);
            }
            static inline unsigned char getc() {
                static_assert(has_signature<decltype(hwstl::device::uart_io::getc), unsigned char(void)>::value, "void hwstl::device::uart_io::getc is not implemented for target");
                return hwstl::device::uart_io::getc();
            }
        };
    }
}