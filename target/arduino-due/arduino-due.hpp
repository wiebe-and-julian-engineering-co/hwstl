//    Copyright Julian van Doorn and Wiebe van Breukelen.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once


#include <system_sam3xa.h>
#include <sam3x8e.h>
#include <tuple>
#include <utility>
#include <algorithm>
#include "../../memory/ring_buffer.hpp" // Should not be placed here

#include "../std_interface/interface_contracts.hpp"

#ifdef HWSTL_ENABLE_UART
#include "uart.hpp"
#endif

namespace hwstl {
    namespace arduino_due {
        using namespace hwstl::interface_contracts;

        namespace internal {

        };

        void init();

        namespace pins {
            static constexpr auto d0 = pin_definition<0>();
            static constexpr auto d1 = pin_definition<1>();
            static constexpr auto d2 = pin_definition<2>();
            static constexpr auto d3 = pin_definition<3>();
            static constexpr auto d4 = pin_definition<4>();
            static constexpr auto d5 = pin_definition<5>();
            static constexpr auto d6 = pin_definition<6>();
            static constexpr auto d7 = pin_definition<7>();
            static constexpr auto d8 = pin_definition<8>();
            static constexpr auto d9 = pin_definition<9>();
            static constexpr auto d10 = pin_definition<10>();
            static constexpr auto d11 = pin_definition<11>();
            static constexpr auto d12 = pin_definition<12>();
            static constexpr auto d13 = pin_definition<13>();
            static constexpr auto d14 = pin_definition<14>();
            static constexpr auto d15 = pin_definition<15>();
            static constexpr auto d16 = pin_definition<16>();
            static constexpr auto d17 = pin_definition<17>();
            static constexpr auto d18 = pin_definition<18>();
            static constexpr auto d19 = pin_definition<19>();
        };
        
        namespace pin {
            using namespace hwstl::interface_contracts::pin;

            static constexpr hwstl::pin_info pin_info_array[21] = {
                { 0,  8 },  // d0
                { 0,  9 },  // d1
                { 1, 25 },  // d2
                { 2, 28 },  // d3
                { 2, 26 },  // d4
                { 2, 25 },  // d5
                { 2, 24 },  // d6
                { 2, 23 },  // d7
                { 2, 22 },  // d8
                { 2, 21 },  // d9
                { 2, 29 },  // d10

                { 3,  7 },  // d11
                { 3,  8 },  // d12
                { 1, 27 },  // d13
                { 3,  4 },  // d14
                { 3,  5 },  // d15
                { 0, 13 },  // d16
                { 0, 12 },  // d17
                { 0, 11 },  // d18
                { 0, 10 },  // d19
                { 1, 12 },  // d20
            };

            template <pin_index t_pin>
            static inline constexpr Pio* get_port_from_pin() {
                uint8_t port = pin_info_array[t_pin].m_port;

                if (port == 0) {
                    return PIOA;
                } else if (port == 1) {
                    return PIOB;
                } else if (port == 2) {
                    return PIOC;
                } else if (port == 3) {
                    return PIOD;
                } else {
                    return nullptr;
                }
            }

            template <pin_index t_pin>
            static inline constexpr uint32_t get_pin_in_port() {
                return pin_info_array[t_pin].m_pin;
            }

            template <pin_index t_pin>
            static inline constexpr uint32_t get_pin_mask() {
                return 1 << get_pin_in_port<t_pin>();
            }

            template <pin_index t_pin>
            static inline int PinEnable() {
                Pio* port = get_port_from_pin<t_pin>();
                uint32_t mask = get_pin_mask<t_pin>();

                port->PIO_PER = mask;
                port->PIO_OER = mask;

                return 1;
            }

            template <pin_index t_pin>
            static inline constexpr void ProcessPinEntry(uint32_t masks[4]) {
                uint8_t port = pin_info_array[t_pin].m_port;

                if (port == 0) {
                    masks[0] |= get_pin_mask<t_pin>();
                } else if (port == 1) {
                    masks[1] |= get_pin_mask<t_pin>();
                } else if (port == 2) {
                    masks[2] |= get_pin_mask<t_pin>();
                } else if (port == 3) {
                    masks[3] |= get_pin_mask<t_pin>();
                }
            }

            template <pin_index... vt_pins>
            static inline void PinSequenceEnable(pin_sequence<vt_pins...> pins) {
                uint32_t masks[4]   = {};
                uint32_t pmc0_enable = 0;

                (ProcessPinEntry<vt_pins>(masks), ...);

                if (masks[0]) {
                    pmc0_enable   |= (1 << 11);

                    PIOA->PIO_PER = masks[0];
                    PIOA->PIO_OER = masks[0];
                }

                if (masks[1]) {
                    pmc0_enable   |= (1 << 12);

                    PIOB->PIO_PER = masks[1];
                    PIOB->PIO_OER = masks[1];
                }
                
                if (masks[2]) {
                    pmc0_enable   |= (1 << 13);

                    PIOC->PIO_PER = masks[2];
                    PIOC->PIO_OER = masks[2];
                }

                if (masks[3]) {
                    pmc0_enable   |= (1 << 14);

                    PIOD->PIO_PER = masks[3];
                    PIOD->PIO_OER = masks[3];
                }

                PMC->PMC_PCER0 = pmc0_enable;
            }

            template <pin_index... vt_pins>
            static inline constexpr void configure_in(pin_sequence<vt_pins...> pins) {
                PinSequenceEnable(pins);
            }
            
            template <pin_index... vt_pins>
            static inline constexpr void configure_out(pin_sequence<vt_pins...> pins) {
                PinSequenceEnable(pins);
            }
            
            template <pin_index... vt_pins>
            static inline constexpr void configure_in_out(pin_sequence<vt_pins...> pins) {
                PinSequenceEnable(pins);
            }
            
            template <pin_index t_pin>
            static inline void set(bool v) {
                Pio* port = get_port_from_pin<t_pin>();
                uint32_t mask = get_pin_mask<t_pin>();

                (v ? port->PIO_SODR : port->PIO_CODR) = mask;
            }

            template <pin_index t_pin>
            static inline bool get() {
                Pio* port = get_port_from_pin<t_pin>();
                uint32_t mask = get_pin_mask<t_pin>();

                return (port->PIO_PDSR & mask) != 0;
            }

            template <pin_index t_pin>
            static inline void enable_pullup() {
                Pio* port = get_port_from_pin<t_pin>();
                uint32_t mask = get_pin_mask<t_pin>();
                
                port->PIO_PUER = mask;
            }

            template <pin_index t_pin>
            static inline void disable_pullup() {
                Pio* port = get_port_from_pin<t_pin>();
                uint32_t mask = get_pin_mask<t_pin>();
                
                port->PIO_PUDR = mask;
            }
        }


        uint_fast64_t now_ticks();

        uint64_t now_us();

        void wait_us_busy(int_fast32_t n);

        void wait_us(int_fast32_t n);

        void wait_ms(int_fast32_t n);
    } // namespace arduino_due
} // namespace hwstl
