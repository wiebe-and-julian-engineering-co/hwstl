#pragma once

namespace hwstl {
    enum class data_types { hex = 0, bin = 1, oct = 2, dec = 3 };

    static const auto hex = hwstl::data_types::hex;
    static const auto bin = hwstl::data_types::bin;
    static const auto oct = hwstl::data_types::oct;
    static const auto dec = hwstl::data_types::dec;

    class pure_data_type {
    public:
        hwstl::data_types m_type;
        uint8_t m_base;

        constexpr pure_data_type(hwstl::data_types type, uint8_t base) : m_type(type), m_base(base) { }
    };

    static constexpr pure_data_type pure_data_types[4] = {
        { hwstl::data_types::hex, 16 },
        { hwstl::data_types::bin, 2  },
        { hwstl::data_types::oct, 8  },
        { hwstl::data_types::dec, 10 }
    };

    __attribute__((always_inline))
    static inline constexpr pure_data_type get_pure_data_type(const hwstl::data_types data_type) {
        return pure_data_types[static_cast<unsigned int>(data_type)];
    }
} // namespace hwstl