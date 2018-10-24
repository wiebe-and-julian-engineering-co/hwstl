#pragma once

#include <tuple>

namespace hwstl {
    template <hwstl::pin_index t_pin>
    class ipin {
    public:
        bool get() {
            return hwstl::target::pin::pin_impl<t_pin>::get();
        }
    };

    template <hwstl::pin_index t_pin>
    class opin {
    public:
        void set(bool v) {
            hwstl::target::pin::pin_impl<t_pin>::set(v);
        }
    };

    template <hwstl::pin_index t_pin>
    class iopin : public ipin<t_pin>, public opin<t_pin> { };

    template <hwstl::pin_index... vt_pins>
	auto make_ipin(hwstl::target::pin::pin_impl<vt_pins>... opins) {
		// hwstl::target::pin::configure_in(hwstl::pin_sequence<vt_pins...>());

		return std::tuple(
			hwstl::ipin<vt_pins>()...
		);
	}

    template <hwstl::pin_index... vt_pins>
	auto make_opin(hwstl::target::pin::pin_impl<vt_pins>... opins) {
		hwstl::target::pin::configure_out(hwstl::pin_sequence<vt_pins...>());

		return std::tuple(
			hwstl::opin<vt_pins>()...
		);
	}

    template <hwstl::pin_index... vt_pins>
	auto make_iopin(hwstl::target::pin::pin_impl<vt_pins>... opins) {
		// hwstl::target::pin::configure_inout(hwstl::pin_sequence<vt_pins...>());

		return std::tuple(
			hwstl::iopin<vt_pins>()...
		);
	}
} // namespace hwstl