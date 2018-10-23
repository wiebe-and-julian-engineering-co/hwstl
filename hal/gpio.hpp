#pragma once

#include <tuple>

namespace hwstl {
    template <hwstl::target::pin_index t_pin>
    class ipin {
    public:
        bool get() {
            return hwstl::target::pin::pin_impl<t_pin>::get();
        }
    };

    template <hwstl::target::pin_index t_pin>
    class opin {
    public:
        void set(bool v) {
            hwstl::target::pin::pin_impl<t_pin>::set(v);
        }
    };

    template <hwstl::target::pin_index t_pin>
    class iopin : public ipin<t_pin>, public opin<t_pin> { };

    template <hwstl::target::pin_index... vt_pins>
	auto make_opin(hwstl::target::pin::pin_impl<vt_pins>... opins) {
		hwstl::target::pin::Configure(std::make_index_sequence<sizeof...(opins), opins...>());

		return std::tuple(
			hwstl::opin<hwstl::target::pin::pin_impl<vt_pins>::pin>()...
		);
	}
} // namespace hwstl