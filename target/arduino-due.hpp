#pragma once

#include <sam.h>

using size_t = uint32_t;

namespace hwstl {
    namespace arduino_due {
        /**
         * @brief Primary UART controller
         * 
         * @details
         * Primary UART controller routines. All debug logs should by default
         * be sent using the commands in this namespace.
         */
        class uart_io {
        private:
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
            static inline void Enable() {
                // enable the clock to port A
                PMC->PMC_PCER0 = 1 << ID_PIOA;

                // disable PIO Control on PA9 and set up for Peripheral A
                PIOA->PIO_PDR = PIO_PA8; 
                PIOA->PIO_ABSR &= ~PIO_PA8;
                PIOA->PIO_PDR = PIO_PA9; 
                PIOA->PIO_ABSR &= ~PIO_PA9;

                // enable the clock to the UART
                PMC->PMC_PCER0 = 0x01 << ID_UART;

                // Reset and disable receiver and transmitter.
                // UART->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
                ResetTRX();
                EnableBaud<MainClockFrequency, 115200>();

                // No parity, normal channel mode.
                UART->UART_MR = UART_MR_PAR_NO;

                // Disable all interrupts.	  
                UART->UART_IDR = 0xFFFFFFFF;   

                EnableTRX();
            }

            static inline void Disable() {
                // Disable PIO control on PA8 and PA9
                // Leaves peripheral mode undefined
                PIOA->PIO_PDR = PIO_PA8;
                PIOA->PIO_PDR = PIO_PA9;

                // Disable clock to the UART
                PMC->PMC_PCER0 = 0x01 << ID_UART;
            }

            /**
             * @brief Places char c on the UART Tx line
             * 
             * @param c 
             */
            static inline void putc(unsigned char c) {
                while((UART->UART_SR & 2) == 0) { }
                UART->UART_THR = c;
            }

            /**
             * @brief Reads char from the UART Rx line
             * 
             * @return unsigned char 
             */
            static inline unsigned char getc() {
                while((UART->UART_SR & 1) == 1) { }
                return UART->UART_RHR; 
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
                while(UART->UART_SR & 1 == 1) { } // TODO: Check for timeout and return -1
                return UART->UART_RHR;
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