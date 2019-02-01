#pragma once

#include <cstdint>
#include <utility>
#include <algorithm>

namespace hwstl {
    using pin_index = size_t;

    template <pin_index... vt_pins>
    using pin_sequence = std::integer_sequence<pin_index, vt_pins...>;

    template <pin_index t_pin>
    class pin_definition {
    public:
        constexpr pin_definition() { }

        constexpr operator pin_index() const {
            return t_pin;
        }
    };
}