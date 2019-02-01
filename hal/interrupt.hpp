//    Copyright Julian van Doorn and Wiebe van Breukelen.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once


//#include "gpio.hpp"

namespace hwstl {
    namespace interrupt {
        /**
         * @addtogroup Broken 
         */
        // template <hwstl::pin_index... vt_pins>
        // auto make_iopin_interrupt(hwstl::target::interrupt::mode _mode, hwstl::target::pin::pin_impl<vt_pins>... pins) {
        //     hwstl::target::pin::configure_interrupt(hwstl::pin_sequence<vt_pins...>(), _mode);
        // }

        /**
         * @addtogroup Broken 
         */
        // template <hwstl::pin_index... vt_pins>
        // void disable(hwstl::target::pin::pin_impl<vt_pins>... pins) {
        //     hwstl::target::pin::configure_interrupt(hwstl::pin_sequence<vt_pins...>());
        // }
    }
    /**using PInterruptRoutine = void (*)();

    class CInterruptBindings {
    public:
        virtual inline void Call() const __attribute__((always_inline)) = 0;
    };

    template <class... vt_routines>
    class CInterruptBinder : public CInterruptBindings {
    public:
        using type = CInterruptBinder;
        using value_type = PInterruptRoutine;
        static constexpr std::size_t size() noexcept { return sizeof...(vt_routines); }

    private:
        // static const constexpr std::array<std::function<void(void)>, size()> sequence = {{ vt_routines... }};

    public:
        template <class... other_vt_routines>
        static auto Merge(CInterruptBinder<other_vt_routines...> other) {
            return CInterruptBinder<vt_routines..., other_vt_routines...>();
        }

        inline void Call() const override __attribute__((always_inline)) {
            if constexpr (sizeof...(vt_routines) > 0) {
                // for (PInterruptRoutine routine : sequence) {
                //     routine();
                // }
            }
        }
    };

    extern const CInterruptBindings& g_interrupt_bindings;**/
} // namespace hwstl