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

void UART_Handler(void);

namespace hwstl {
    namespace arduino_due {
        using namespace hwstl::interface_contracts;

        namespace internal {

        };

        void init();

        class pin_info {
        public:
            uint8_t m_port;
            uint8_t m_pin;

            constexpr pin_info(uint8_t port, uint8_t pin) : m_port(port), m_pin(pin) { }
        };

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

            static constexpr pin_info pin_info_array[21] = {
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

            template <pin_index... vt_pins>
            static inline constexpr void configure_in(pin_sequence<vt_pins...> pins) {

            }
            
            template <pin_index... vt_pins>
            static inline constexpr void configure_out(pin_sequence<vt_pins...> pins) {

            }
            
            template <pin_index... vt_pins>
            static inline constexpr void configure_in_out(pin_sequence<vt_pins...> pins) {

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

            // template <pin_index t_pin>
            // class pin_impl {
            //     public:
            //     static constexpr pin_index pin = t_pin;

            //     constexpr pin_impl() { }

            //     static inline void set(bool v) {
            //         Pio* port = GetPortByPin<t_pin>();
            //         uint32_t mask = GetPinMask<t_pin>();

            //         (v ? port->PIO_SODR : port->PIO_CODR) = mask;
            //     }

            //     static inline bool get() {
            //         Pio* port = GetPortByPin<t_pin>();
            //         uint32_t mask = pin::GetPinMask<t_pin>();

            //         return (port->PIO_PDSR & mask) != 0;
            //     }

            //     static inline void enable_pullup() {
            //         Pio* port = GetPortByPin<t_pin>();
            //         uint32_t mask = pin::GetPinMask<t_pin>();
                    
            //         port->PIO_PUER = mask;
            //     }

            //     static inline void disable_pullup() {
            //         Pio* port = GetPortByPin<t_pin>();
            //         uint32_t mask = pin::GetPinMask<t_pin>();
                    
            //         port->PIO_PUDR = mask;
            //     }
            // };
        }

        /**
         * @brief Primary UART controller
         * 
         * @details
         * Primary UART controller routines. All debug logs should by default
         * be sent using the commands in this namespace.
         */
        class uart_io {
        private:
            static hwstl::ring_buffer<uint8_t, 64> uartReceiveBuffer;
            static hwstl::ring_buffer<uint8_t, 64> uartTransmitBuffer;

            static constexpr uint32_t MainClockFrequency = 84000000;

            static constexpr int32_t FromFrequencyToPrescalerSelector(uint32_t main_clock_frequency) {
                // 0 CLK Selected clock
                // 1 CLK_2 Selected clock divided by 2
                // 2 CLK_4 Selected clock divided by 4
                // 3 CLK_8 Selected clock divided by 8
                // 4 CLK_16 Selected clock divided by 16
                // 5 CLK_32 Selected clock divided by 32
                // 6 CLK_64 Selected clock divided by 64
                // 7 CLK_3 Selected clock divided by 3

                if (main_clock_frequency == MainClockFrequency) {
                    return 0;
                } else if (main_clock_frequency == MainClockFrequency / 2) {
                    return 1;
                } else if (main_clock_frequency == MainClockFrequency / 4) {
                    return 2;
                } else if (main_clock_frequency == MainClockFrequency / 8) {
                    return 3;
                } else if (main_clock_frequency == MainClockFrequency / 16) {
                    return 4;
                } else if (main_clock_frequency == MainClockFrequency / 32) {
                    return 5;
                } else if (main_clock_frequency == MainClockFrequency / 64) {
                    return 6;
                } else if (main_clock_frequency == MainClockFrequency / 3) {
                    return 7;
                } else {
                    return -1; // Invalid clock frequency
                }
            }

            static constexpr int32_t FromPrescalerSelectorToFrequency(uint32_t prescaler_selection) {
                if (prescaler_selection == 0) {
                    return MainClockFrequency;
                } else if (prescaler_selection == 1) {
                    return MainClockFrequency / 2;
                } else if (prescaler_selection == 2) {
                    return MainClockFrequency / 4;
                } else if (prescaler_selection == 3) {
                    return MainClockFrequency / 8;
                } else if (prescaler_selection == 4) {
                    return MainClockFrequency / 16;
                } else if (prescaler_selection == 5) {
                    return MainClockFrequency / 32;
                } else if (prescaler_selection == 6) {
                    return MainClockFrequency / 64;
                } else if (prescaler_selection == 7) {
                    return MainClockFrequency / 3;
                } else {
                    return -1; // Invalid prescaler
                }
            }

            /**
             * @brief Disable baud generation for UART
             */
            static inline void DisableBaud() {
                UART->UART_BRGR = 0;
            }

            /**
             * @brief Enables baud generation with the given clock
             * frequency and baudrate
             * 
             * @details
             * Uses the formula CD = (MCK / BAUD) / 16 to select the right
             * clock divider. That formula is derived from the formula
             * provided by the SAM3X datasheet: BAUD = MCK / (CD * 16)
             * 
             * @tparam t_master_clock_frequency 
             * @tparam t_baudrate 
             */
            template <uint32_t t_master_clock_frequency, uint32_t t_baudrate>
            static inline void EnableBaud() {
                static_assert(FromFrequencyToPrescalerSelector(t_master_clock_frequency) != -1, "Invalid master clock frequency");
                UART->UART_BRGR = (t_master_clock_frequency / t_baudrate) / 16;
            }

            /**
             * @brief Enables baud generation for the given baud
             * 
             * @details
             * This function is designed to change baudrate during runtime.
             * t_master_clock_frequency is provided during compilation since
             * the CPU clock speed generally doesn't change during
             * operation.
             * 
             * @tparam t_master_clock_frequency 
             * @param baudrate 
             */
            template <uint32_t t_master_clock_frequency>
            static inline void EnableBaud(uint32_t baudrate) {
                static_assert(FromFrequencyToPrescalerSelector(t_master_clock_frequency) != -1, "Invalid master clock frequency");
                UART->UART_BRGR = (t_master_clock_frequency / baudrate) / 16;
            }

            static inline void ResetTRX() {
                UART->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
            }

            static inline void EnableTRX() {
                UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN; 
            }

        public:
            static inline void IRQHandler();

            static inline void Enable() {
                // enable the clock to port A
                //PMC->PMC_PCER0 = 1 << ID_PIOA;

                // enable the clock to the UART
                PMC->PMC_PCER0 = 0x01 << ID_UART;

                // disable PIO Control on PA9 and set up for Peripheral A
                PIOA->PIO_PDR = PIO_PA8; 
                PIOA->PIO_ABSR &= ~PIO_PA8;
                PIOA->PIO_PDR = PIO_PA9; 
                PIOA->PIO_ABSR &= ~PIO_PA9;

                  // Disable PDC channel
                UART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

                ResetTRX();

                // Disable all interrupts.
                UART->UART_IDR = 0xFFFFFFFF;
                                
                // Enable receive interrupts.
                NVIC_DisableIRQ(UART_IRQn);
                NVIC_ClearPendingIRQ(UART_IRQn);
                NVIC_SetPriority(UART_IRQn, 0);
                

                UART->UART_IER = UART_IER_RXRDY; // Enable RXRDY interrupts, see section 34.6.3



                NVIC_EnableIRQ(UART_IRQn);
                
                EnableBaud<MainClockFrequency, 115200>();

                // No parity, normal channel mode.
                UART->UART_MR = UART_MR_PAR_NO;


                EnableTRX();
            }

            static inline void Disable() {
                // Disable PIO control on PA8 and PA9
                // Leaves peripheral mode undefined
                PIOA->PIO_PDR = PIO_PA8;
                PIOA->PIO_PDR = PIO_PA9;

                // Disable all interrupts.
                UART->UART_IDR = 0xFFFFFFFF;

                // Disable clock to the UART
                PMC->PMC_PCER0 = 0x01 << ID_UART;

                // Disable the IQR for the UART
                NVIC_DisableIRQ(UART_IRQn);
            }

            /**
             * @brief Places char c on the UART Tx line
             * 
             * @param c 
             */
            static void putc(const unsigned char c) {
                //while((UART->UART_SR & 2) == 0) { }
                //UART->UART_THR = c;

                // If the UART controller is not busy, we send directly

                if ((UART->UART_SR & UART_SR_TXRDY) == UART_SR_TXRDY) {
                   UART->UART_THR = c;
                } else {
                    // We are busy, add to the queue.
                    uartTransmitBuffer.enqueue(c);

                    // Enable TXRDY interrupt.
                    UART->UART_IER = UART_IER_TXRDY;
                }
            }

            /**
             * @brief Reads char from the UART Rx line
             * 
             * @return unsigned char 
             */
            static inline unsigned char getc() {
                if (uartReceiveBuffer.size() == 0) {
                    return 0;
                }

                return uartReceiveBuffer.dequeue();
            }

            static inline size_t available() {
                return uartReceiveBuffer.size();
            }

            /**
             * @brief Reads char from the UART Rx line with a timeout
             * 
             * @details
             * Returns -1 when a timeout occurred. Safe to cast to unsigned
             * char if getc did not timeout.
             * 
             * @return int32_t
             */
            template <uint32_t t_timeout>
            static inline int32_t getc() {
                //while((UART->UART_SR & 1) == 0) { } // TODO: Check for timeout and return -1
               // return UART->UART_RHR;

               return 0;
            }

            inline static void Configure()  { 
                Enable();
            }
        };

        uint_fast64_t now_ticks();

        uint64_t now_us();

        void wait_us_busy(int_fast32_t n);

        void wait_us(int_fast32_t n);

        void wait_ms(int_fast32_t n);
    } // namespace arduino_due
} // namespace hwstl
