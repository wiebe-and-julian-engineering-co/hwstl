#pragma once

#include <cstdint>

namespace hwstl {
    class pin_info {
        public:
        uint8_t m_port;
        uint8_t m_pin;

        constexpr pin_info(uint8_t port, uint8_t pin) : m_port(port), m_pin(pin) { }
    };
}