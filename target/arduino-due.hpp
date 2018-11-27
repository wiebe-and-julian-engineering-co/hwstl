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
#include <type_traits>

/************************************************************************/
/* Interrupt handlers                                                   */
/************************************************************************/

/**void HandlePIOInterrupt(Pio* pio, uint32_t status) {
    // Interrupts on PIOC are cleared while reading PIO_ISR.
    // When an Interrupt is enabled on a “Level”, the interrupt
    // is generated as long as the interrupt source is not cleared, even if some read accesses in PIO_ISR are performed.
    if (status != 0) {
        //hwstl::cout << "Interrupted!\n";
        //hwstl::wait_ms(20);

        uint8_t i = 0;

        while (status != 0) {
            if (status & 0x01u) {
                hwstl::cout << "P" << (unsigned int) i;
            }

            status >>= 1;
            ++i;
        }
    }
}

extern void PIOA_Handler(void) {
    uint32_t status = PIOA->PIO_ISR;
    status &= PIOA->PIO_IMR;

    HandlePIOInterrupt(PIOA, status);
}

extern void PIOB_Handler(void) {
    uint32_t status = PIOB->PIO_ISR;
    status &= PIOB->PIO_IMR;

    HandlePIOInterrupt(PIOB, status);
}

extern void PIOC_Handler(void) {
    uint32_t status = PIOC->PIO_ISR;
    status &= PIOC->PIO_IMR;

    HandlePIOInterrupt(PIOC, status);
}

extern void PIOD_Handler(void) {
    uint32_t status = PIOD->PIO_ISR;
    status &= PIOD->PIO_IMR;

    HandlePIOInterrupt(PIOD, status);
}
}**/

namespace hwstl {
    using pin_index = uint32_t;

    template <pin_index... vt_pins>
    using pin_sequence = std::integer_sequence<pin_index, vt_pins...>;

    namespace arduino_due {
#ifdef DEBUG
        static inline void debug_assert(bool assertion_success, const char* error) {
            if (!assertion_success) {
            // block execution, throw error with debug info
            }
        }
#else
        static inline void debug_assert(bool assertion_success, const char* error) {
            // Do nothing during execution, code should be designed so that
            // this function is never called. It's advised to compile with
            // debug to test these assertions.
        }
#endif

        namespace internal {
            __attribute__((always_inline))
            static inline void watchdog_enable() {
                WDT->WDT_MR &= ~(0x0u << 15);
            }
            
            __attribute__((always_inline))
            static inline void watchdog_disable() {
                WDT->WDT_MR = WDT_MR_WDDIS;
            }
        };

        namespace interrupt {
            enum class mode { RISING, FALLING };
        };

        void init();

        namespace pin {
            class pin_info {
            public:
                uint8_t m_port;
                uint8_t m_pin;

                constexpr pin_info(uint8_t port, uint8_t pin) : m_port(port), m_pin(pin) { }
            };

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
            __attribute__((always_inline))
            static inline constexpr Pio* GetPortByPin() {
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
            __attribute__((always_inline))
            static inline constexpr uint32_t GetPinInPort() {
                return pin_info_array[t_pin].m_pin;
            }

            template <pin_index t_pin>
            __attribute__((always_inline))
            static inline constexpr uint32_t GetPinMask() {
                return 1 << GetPinInPort<t_pin>();
            }

            template <pin_index t_pin>
            __attribute__((always_inline))
            static inline int PinEnable() {
                Pio* port = GetPortByPin<t_pin>();
                uint32_t mask = 1 << GetPinInPort<t_pin>();

                port->PIO_PER = mask;
                port->PIO_OER = mask;

                return 1;
            }

            template <pin_index t_pin>
            __attribute__((always_inline))
            static inline constexpr void ProcessPinEntry(uint32_t masks[4]) {
                uint8_t port = pin_info_array[t_pin].m_port;

                if (port == 0) {
                    masks[0] |= GetPinMask<t_pin>(); 
                } else if (port == 1) {
                    masks[1] |= GetPinMask<t_pin>(); 
                } else if (port == 2) {
                    masks[2] |= GetPinMask<t_pin>();
                } else if (port == 3) {
                    masks[3] |= GetPinMask<t_pin>();
                }
            }

            template <pin_index... vt_pins>
            __attribute__((always_inline))
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
            __attribute__((always_inline))
            static inline void PinInterruptConfigure(pin_sequence<vt_pins...> pins, const interrupt::mode mode) {
                 uint32_t masks[4] = {};
                 uint32_t enable_mask = 0;
                 IRQn_Type IRQn;
                 //uint32_t nvic_enable_mask = 0x00u;
                 //uint32_t nvic_disable_mask = 0x00u;

                (ProcessPinEntry<vt_pins>(masks), ...);
                
                if (masks[0]) {
                    enable_mask   |= (1 << 11);
                    PIOA->PIO_ISR;
                    PIOA->PIO_IDR = 0xFFFFFFFF;
                    
                    NVIC_DisableIRQ(PIOA_IRQn);
                    NVIC_ClearPendingIRQ(PIOA_IRQn);
                    NVIC_SetPriority(PIOA_IRQn, 0);
                    NVIC_EnableIRQ(PIOA_IRQn);
                } 
                if (masks[1]) {
                    enable_mask   |= (1 << 12);
                    PIOB->PIO_ISR;
                    PIOB->PIO_IDR = 0xFFFFFFFF;

                    NVIC_DisableIRQ(PIOB_IRQn);
                    NVIC_ClearPendingIRQ(PIOB_IRQn);
                    NVIC_SetPriority(PIOB_IRQn, 0);
                    NVIC_EnableIRQ(PIOB_IRQn);
                } 
                if (masks[2]) {
                    enable_mask   |= (1 << 13);
                    PIOC->PIO_ISR;
                    PIOC->PIO_IDR = 0xFFFFFFFF;
                    NVIC_DisableIRQ(PIOC_IRQn);
                    NVIC_ClearPendingIRQ(PIOC_IRQn);
                    NVIC_SetPriority(PIOC_IRQn, 0);
                    NVIC_EnableIRQ(PIOC_IRQn);
                } 
                if (masks[3]) {
                    enable_mask   |= (1 << 14);
                    PIOD->PIO_ISR;
                    PIOD->PIO_IDR = 0xFFFFFFFF;

                    NVIC_DisableIRQ(PIOD_IRQn);
                    NVIC_ClearPendingIRQ(PIOD_IRQn);
                    NVIC_SetPriority(PIOD_IRQn, 0);
                    NVIC_EnableIRQ(PIOD_IRQn);
                }

                PMC->PMC_PCER0 = enable_mask;

            }

            template <pin_index... vt_pins>
            __attribute__((always_inline))
            static inline void configure_in(pin_sequence<vt_pins...> pins) {
                PinSequenceEnable(pins);
            }

            template <pin_index... vt_pins>
            __attribute__((always_inline))
            static inline void configure_out(pin_sequence<vt_pins...> pins) {
                PinSequenceEnable(pins);
            }

            template <pin_index... vt_pins>
            __attribute__((always_inline))
            static inline void configure_inout(pin_sequence<vt_pins...> pins) {
                PinSequenceEnable(pins);
            }

            template <pin_index... vt_pins>
            __attribute__((always_inline))
            static inline void configure_interrupt(pin_sequence<vt_pins...> pins, const interrupt::mode mode) {
                PinSequenceEnable(pins);
                PinInterruptConfigure(pins, mode);
            }

            template <pin_index t_pin>
            class pin_impl {
            public:
                static constexpr pin_index pin = t_pin;

                constexpr pin_impl() { }

                static inline void set(bool v) {
                    Pio* port = GetPortByPin<t_pin>();
                    uint32_t mask = GetPinMask<t_pin>();

                    (v ? port->PIO_SODR : port->PIO_CODR) = mask;
                }

                static inline bool get() {
                    Pio* port = GetPortByPin<t_pin>();
                    uint32_t mask = GetPinMask<t_pin>();

                    return (port->PIO_PDSR & mask) != 0;
                }

                static inline void enable_pullup() {
                    Pio* port = GetPortByPin<t_pin>();
                    uint32_t mask = GetPinMask<t_pin>();
                    
                    port->PIO_PUER = mask;	
                }

                static inline void disable_pullup() {
                    Pio* port = GetPortByPin<t_pin>();
                    uint32_t mask = GetPinMask<t_pin>();
                    
                    port->PIO_PUDR = mask;
                }
            };

#ifdef HWSTL_ONCE
            auto d0 = pin_impl<0>();
            auto d1 = pin_impl<1>();
            auto d2 = pin_impl<2>();
            auto d7 = pin_impl<7>();
            auto d13 = pin_impl<13>();
#endif
        };

        class uart_port {
        public:
            enum class peripheral {
                uart,
                usart0,
                usart1,
                usart2
            };

            enum class type {
                uart,
                usart
            };

            class peripheral_info {
            public:
                type m_type;
                peripheral m_peripheral;
            };

            static constexpr peripheral_info uart { type::uart, peripheral::uart };
            static constexpr peripheral_info usart0 { type::usart, peripheral::usart0 };
            static constexpr peripheral_info usart1 { type::usart, peripheral::usart1 };
            static constexpr peripheral_info usart2 { type::usart, peripheral::usart2 };

            constexpr type get_type_of_peripheral(peripheral p) {
                if (p == peripheral::uart) {
                    return type::uart;
                } else if (
                    p == peripheral::usart0 ||
                    p == peripheral::usart1 ||
                    p == peripheral::usart2
                ) {
                    return type::usart;
                }
            }
        };

        static constexpr uint32_t MainClockFrequency = 84000000;

        enum class USARTMode {
            Normal = 0,
            RS485,
            HardwareHandshaking,
            ISO7816_T_0 = 4,
            ISO7816_T_1 = 6,
            IrDA = 8,
            LinMaster = 0xA,
            LinSlave,
            SPIMaster = 0xE,
            SPISlave
        };

        enum class ClockSelection {
            MasterClock = 0,
            Divided,
            SerialClock = 3
        };

        class CharacterLength {
        public:
            unsigned charLength : 3;

            constexpr CharacterLength(uint8_t charLength = 5) : charLength(charLength) { }
        };

        static constexpr int32_t GetCharacterLength(uint32_t bits) {
            if (bits == 5) {
                return 0;
            } else if (bits == 6) {
                return 1;
            } else if (bits == 7) {
                return 2;
            } else if (bits == 8) {
                return 3;
            } else if (bits == 9) {
                return 4; // THIS CASE SHOULD BE HANDLED DIFFERENTLY, SEE DATASHEET FOR USART "MODE9"
            }

            return -1;
        }

        enum class SynchronousMode {
            Async = 0,
            Sync = 1
        };

        enum class SPIClockPhase {
            // Needs more elaboration, and perhals a more descriptive enum
            LeadingEdgeChanged_FollowingEdgeCaptured = 0,
            LeadingEdgeCaptured_FollowingEdgeChanged = 1
        };

        enum class Parity : uint32_t {
            Even = 0,
            Odd = 1 << 9,
            Space = 2 << 9,
            Mark = 3 << 9,
            No = 4 << 9,
            Multidrop = 6 << 9
        };

        enum class StopBits {
            OneBit = 0,
            OneAndHalfBits,
            TwoBits
        };

        enum class Channel : uint32_t {
            Normal = 0,
            Automatic = 1 << 13,
            LocalLoopback = 2 << 13,
            RemoteLoopback = 3 << 13
        };

        enum class BitOrder {
            LeastSignificantFirst = 0,
            MostSignificantFirst = 1
        };

        enum class SPIClockPolarity {
            ActiveHigh = 0,
            ActiveLow = 1
        };

        enum class ClockOutput {
            NotDriven = 0,
            Driven = 1
        };

        enum class OversamplingMode {
            x16 = 0,
            x8 = 1
        };

        enum class InhibitNonAcknowledge {
            Generated = 0,
            NotGenerated = 1
        };

        enum class SuccessiveNACK {
            Enabled = 0,
            Disable = 1
        };

        enum class InvertedData {
            ActiveHigh = 0,
            ActiveLow = 1
        };

        enum class VariableSynchronization {
            UserDefined = 0,
            OnReceived = 1
        };

        class MaxIterations {
        public:
            unsigned maxIterations : 3;

            constexpr MaxIterations(uint8_t maxIterations = 0) : maxIterations(maxIterations) { }
        };

        enum class InfraredRxFilter {
            None = 0,
            ThreeSampleFilter = 1
        };

        enum class ManchesterCodecEnabled {
            Disabled = 0,
            Enabled = 1
        };

        enum class ManchesterSynchronizationMode {
            LowToHighTransition = 0,
            HighToLowTransition = 1
        };

        enum class StartFrameDelimiter {
            CommandOrDataSync = 0,
            OneBit = 1
        };

        union GenericMode {
        public:
            uint32_t generic;
            USARTMode mode;
            ClockSelection clockSelection;
            CharacterLength characterLength;
            SynchronousMode synchronousMode;
            SPIClockPhase spiClockPhase;
            Parity parity;
            StopBits stopBits;
            Channel channel;
            BitOrder bitOrder;
            SPIClockPolarity spiClockPolarity;
            ClockOutput clockOutput;
            OversamplingMode oversamplingMode;
            InhibitNonAcknowledge inhibitNonAcknowledge;
            SuccessiveNACK successiveNACK;
            InvertedData invertedData;
            VariableSynchronization variableSynchronization;
            MaxIterations maxIterations;
            InfraredRxFilter infraredRxFilter;
            ManchesterCodecEnabled manchesterCodecEnabled;
            ManchesterSynchronizationMode manchesterSynchronizationMode;
            StartFrameDelimiter startFrameDelimiter;

            template <class t_enum>
            constexpr GenericMode(t_enum mode) : generic(static_cast<uint32_t>(mode)) { }

            template <class T>
            constexpr operator T() {
                return static_cast<T>(generic);
            }
        };

        template <uart_port::type t_type>
        class Mode;

        /**
         * @brief Mode descriptor for UART peripherals 
         */
        template <>
        class Mode<uart_port::type::uart> {
        public:
            Parity parity;
            Channel channel;

            constexpr Mode(
                GenericMode parity = Parity::Even,
                GenericMode channel = Channel::Normal
            ) : 
                parity(parity),
                channel(channel)
            { }

            template <uart_port::peripheral t_uart>
            inline void Apply() {
                static_assert(t_uart == uart_port::peripheral::uart, "Provided t_uart is not an existing peripheral");
                debug_assert(parity != Parity::Multidrop, "UART does not support Parity::Multidrop");
                UART->UART_MR = (uint32_t) parity | (uint32_t) channel;
            }
        };

        /**
         * @brief Mode descriptor for USART peripherals
         */
        template <>
        class Mode<uart_port::type::usart> {
        public:
            USARTMode mode;
            ClockSelection clockSelection;
            CharacterLength characterLength;
            SynchronousMode synchronousMode;
            SPIClockPhase spiClockPhase;
            Parity parity;
            StopBits stopBits;
            Channel channel;
            BitOrder bitOrder;
            SPIClockPolarity spiClockPolarity;
            ClockOutput clockOutput;
            OversamplingMode oversamplingMode;
            InhibitNonAcknowledge inhibitNonAcknowledge;
            SuccessiveNACK successiveNACK;
            InvertedData invertedData;
            VariableSynchronization variableSynchronization;
            MaxIterations maxIterations;
            InfraredRxFilter infraredRxFilter;
            ManchesterCodecEnabled manchesterCodecEnabled;
            ManchesterSynchronizationMode manchesterSynchronizationMode;
            StartFrameDelimiter startFrameDelimiter;

            constexpr Mode(
                USARTMode mode = USARTMode::Normal,
                ClockSelection clockSelection = ClockSelection::MasterClock,
                CharacterLength characterLength = CharacterLength(),
                SynchronousMode synchronousMode = SynchronousMode::Async,
                SPIClockPhase spiClockPhase = SPIClockPhase::LeadingEdgeChanged_FollowingEdgeCaptured,
                Parity parity = Parity::Even,
                StopBits stopBits = StopBits::OneBit,
                Channel channel = Channel::Normal,
                BitOrder bitOrder = BitOrder::LeastSignificantFirst,
                SPIClockPolarity spiClockPolarity = SPIClockPolarity::ActiveHigh,
                ClockOutput clockOutput = ClockOutput::NotDriven,
                OversamplingMode oversamplingMode = OversamplingMode::x16,
                InhibitNonAcknowledge inhibitNonAcknowledge = InhibitNonAcknowledge::Generated,
                SuccessiveNACK successiveNACK = SuccessiveNACK::Enabled,
                InvertedData invertedData = InvertedData::ActiveHigh,
                VariableSynchronization variableSynchronization = VariableSynchronization::UserDefined,
                MaxIterations maxIterations = MaxIterations(),
                InfraredRxFilter infraredRxFilter = InfraredRxFilter::None,
                ManchesterCodecEnabled manchesterCodecEnabled = ManchesterCodecEnabled::Disabled,
                ManchesterSynchronizationMode manchesterSynchronizationMode = ManchesterSynchronizationMode::LowToHighTransition,
                StartFrameDelimiter startFrameDelimiter = StartFrameDelimiter::CommandOrDataSync
            ) :
                mode(mode),
                clockSelection(clockSelection),
                characterLength(characterLength),
                synchronousMode(synchronousMode),
                spiClockPhase(spiClockPhase),
                parity(parity),
                stopBits(stopBits),
                channel(channel),
                bitOrder(bitOrder),
                spiClockPolarity(spiClockPolarity),
                clockOutput(clockOutput),
                oversamplingMode(oversamplingMode),
                inhibitNonAcknowledge(inhibitNonAcknowledge),
                successiveNACK(successiveNACK),
                invertedData(invertedData),
                variableSynchronization(variableSynchronization),
                maxIterations(maxIterations),
                infraredRxFilter(infraredRxFilter),
                manchesterCodecEnabled(manchesterCodecEnabled),
                manchesterSynchronizationMode(manchesterSynchronizationMode),
                startFrameDelimiter(startFrameDelimiter)
            { }
        };

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
         * @brief Calculates the divider for UART_BRGR
         * 
         * @details
         * Uses the formula CD = (MCK / BAUD) / 16 to select the right
         * clock divider. That formula is derived from the formula
         * provided by the SAM3X datasheet: BAUD = MCK / (CD * 16)
         * 
         * @param master_clock_frequency 
         * @param baudrate 
         */
        static constexpr uint32_t CalculateDivider(uint32_t master_clock_frequency, uint32_t baudrate) {
            return (master_clock_frequency / baudrate) / 16;
        }

        /**
         * @brief Checks if the baudrate can be exactly generated
         * 
         * @details
         * According to the SAM3X datasheet, the clock divider cannot be
         * more than 16 bits wide.
         * 
         * @param baudrate 
         * @return true Baudrate can be generated by hardware
         * @return false Invalid baudrate
         */
        static constexpr bool IsValidBaudrate(uint32_t master_clock_frequency, uint32_t baudrate) {
            return CalculateDivider(master_clock_frequency, baudrate) <= 0xFFFF;
        }

        /**
         * @brief Enables baud generation with the given clock
         * frequency and baudrate
         * 
         * @tparam t_master_clock_frequency 
         * @tparam t_baudrate 
         */
        template <uint32_t t_master_clock_frequency, uint32_t t_baudrate, class t_type>
        static inline void EnableBaud(t_type uart) {
            static_assert(FromFrequencyToPrescalerSelector(t_master_clock_frequency) != -1, "Invalid master clock frequency");
            static_assert(IsValidBaudrate(t_master_clock_frequency, t_baudrate), "Invalid baudrate");
            uart->UART_BRGR = CalculateDivider(t_master_clock_frequency, t_baudrate);
        }

        /**
         * @brief Enables baud generation for the given baud
         * 
         * @details
         * This function is designed to change baudrate during runtime.
         * t_master_clock_frequency is provided during compilation since
         * the CPU clock speed generally doesn't change during
         * operation. Else EnableBaud(master_clock_frequency, baudrate)
         * should be used.
         * 
         * @tparam t_master_clock_frequency 
         * @param baudrate 
         */
        template <uint32_t t_master_clock_frequency, class t_type, t_type t_uart>
        static inline void EnableBaud(t_type uart, uint32_t baudrate) {
            static_assert(FromFrequencyToPrescalerSelector(t_master_clock_frequency) != -1, "Invalid master clock frequency");
            debug_assert(IsValidBaudrate(t_master_clock_frequency, baudrate), "Invalid baudrate");
            t_uart->UART_BRGR = CalculateDivider(t_master_clock_frequency, baudrate);
        }

        /**
         * @brief Enables baud generation for the given baud
         * 
         * @details
         * This function is designed to change the baudrate during runtime
         * without a predetermined master clock frequency. That means when
         * using this function the MCK must be looked up or calculated.
         * 
         * @param master_clock_frequency 
         * @param baudrate 
         */
        template <class t_type>
        static inline void EnableBaud(t_type uart, uint32_t master_clock_frequency, uint32_t baudrate) {
            debug_assert(FromFrequencyToPrescalerSelector(master_clock_frequency) != -1, "Invalid master clock frequency");
            debug_assert(IsValidBaudrate(master_clock_frequency, baudrate), "Invalid baudrate");
            uart->UART_BRGR = CalculateDivider(master_clock_frequency, baudrate);
        }

        /**
         * @brief Disable baud generation for UART
         */
        template <class t_type>
        static inline void DisableBaud(t_type uart) {
            // Zero for disabling
            uart->UART_BRGR = 0;
        }

        template <class t_type>
        static inline void ResetTRX(t_type uart);

        /**
         * @brief Resets the UART Tx and Rx
         */
        template <>
        void ResetTRX<Uart*>(Uart* uart) {
            uart->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
        }

        /**
         * @brief Resets the USART Tx and Rx
         */
        template <>
        void ResetTRX<Usart*>(Usart* usart) {
            usart->US_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
        }

        template <class t_type>
        static inline void EnableTRX(t_type uart);

        /**
         * @brief Enables the UART Tx and Rx
         */
        template <>
        void EnableTRX<Uart*>(Uart* uart) {
            uart->UART_CR = UART_CR_RXEN | UART_CR_TXEN; 
        }

        /**
         * @brief Enables the USART Tx and Rx
         */
        template <>
        void EnableTRX<Usart*>(Usart* usart) {
            usart->US_CR = UART_CR_RXEN | UART_CR_TXEN;
        }

        #define MAKE_CONFIG_STREAM_MODIFIER(name) static constexpr std::integral_constant<uint32_t, __COUNTER__> name = { }

        template <class t_peripheral_type, t_peripheral_type t_peripheral, class... vt_pairs>
        class masks;

        // Power control
        MAKE_CONFIG_STREAM_MODIFIER(Enable);
        MAKE_CONFIG_STREAM_MODIFIER(Disable);

        // Interrupts
        MAKE_CONFIG_STREAM_MODIFIER(DisableInterrupts);
        MAKE_CONFIG_STREAM_MODIFIER(EnableRxReady);
        MAKE_CONFIG_STREAM_MODIFIER(EnableTxReady);
        MAKE_CONFIG_STREAM_MODIFIER(EnableEndOfReceiveTransfer);
        MAKE_CONFIG_STREAM_MODIFIER(EnableEndOfTransmit);
        MAKE_CONFIG_STREAM_MODIFIER(EnableOverrunError);
        MAKE_CONFIG_STREAM_MODIFIER(EnableFramingError);
        MAKE_CONFIG_STREAM_MODIFIER(EnableParityError);
        MAKE_CONFIG_STREAM_MODIFIER(EnableTxEmpty);
        MAKE_CONFIG_STREAM_MODIFIER(EnableBufferEmpty);
        MAKE_CONFIG_STREAM_MODIFIER(EnableBufferFull);

        // Parity
        MAKE_CONFIG_STREAM_MODIFIER(EvenParity);
        MAKE_CONFIG_STREAM_MODIFIER(OddParity);
        MAKE_CONFIG_STREAM_MODIFIER(SpaceParity);
        MAKE_CONFIG_STREAM_MODIFIER(MarkParity);
        MAKE_CONFIG_STREAM_MODIFIER(NoParity);

        // Channels
        MAKE_CONFIG_STREAM_MODIFIER(ChannelNormal);
        MAKE_CONFIG_STREAM_MODIFIER(ChannelAutomatic);
        MAKE_CONFIG_STREAM_MODIFIER(ChannelLocalLoopback);
        MAKE_CONFIG_STREAM_MODIFIER(ChannelRemoteLoopback);

        #undef MAKE_CONFIG_STREAM_MODIFIER

        template <class... vt_pairs>
        class masks<uart_port::peripheral, uart_port::peripheral::uart, vt_pairs...> {
        public:
            template <uint32_t t_key, bool t_value>
            void setup() {
                if constexpr (t_key == Enable) {
                    // enable the clock to port A
                    PMC->PMC_PCER0 = 1 << ID_PIOA;

                    // disable PIO Control on PA9 and set up for Peripheral A
                    PIOA->PIO_PDR = PIO_PA8; 
                    PIOA->PIO_ABSR &= ~PIO_PA8;
                    PIOA->PIO_PDR = PIO_PA9; 
                    PIOA->PIO_ABSR &= ~PIO_PA9;

                    // enable the clock to the UART
                    PMC->PMC_PCER0 = 0x01 << ID_UART;
                    ResetTRX(UART);
                    EnableBaud<MainClockFrequency, 115200>(UART);
                    
                    EnableTRX(UART);
                } else if constexpr (t_key == Disable) {
                    // Disable PIO control on PA8 and PA9
                    // Leaves peripheral mode undefined
                    PIOA->PIO_PDR = PIO_PA8;
                    PIOA->PIO_PDR = PIO_PA9;

                    // Disable clock to the UART
                    PMC->PMC_PCER0 = 0x01 << ID_UART;
                } else if constexpr (t_key == EvenParity) {
                    UART->UART_MR = UART_MR_PAR_EVEN;
                } else if constexpr (t_key == OddParity) {
                    UART->UART_MR = UART_MR_PAR_ODD;
                } else if constexpr (t_key == SpaceParity) {
                    UART->UART_MR = UART_MR_PAR_SPACE;
                } else if constexpr (t_key == MarkParity) {
                    UART->UART_MR = UART_MR_PAR_MARK;
                } else if constexpr (t_key == NoParity) {
                    UART->UART_MR = UART_MR_PAR_NO;
                } else if constexpr (t_key == DisableInterrupts) {  
                    UART->UART_IDR = 0xFFFFFFFF;
                } else if constexpr (t_key == EnableRxReady) {
                    // Unimplemented
                } else if constexpr (t_key = EnableTxReady) {
                    // Unimplemented
                } else if constexpr (EnableEndOfReceiveTransfer) {
                    // Unimplemented
                } else if constexpr (EnableEndOfTransmit) {
                    // Unimplemented
                } else if constexpr (EnableOverrunError) {
                    // Unimplemented
                } else if constexpr (EnableFramingError) {
                    // Unimplemented
                } else if constexpr (EnableParityError) {
                    // Unimplemented
                } else if constexpr (EnableTxEmpty) {
                    // Unimplemented
                } else if constexpr (EnableBufferEmpty) {
                    // Unimplemented
                } else if constexpr (EnableBufferFull) {
                    // Unimplemented
                }
            }

            void apply() {
                (setup<vt_pairs::key, vt_pairs::value>(), ...);
            }
        };

        template <uart_port::peripheral t_uart>
        class uart_impl;

        template <>
        class uart_impl<uart_port::peripheral::uart> {
        public:
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
        };

        template <uart_port::peripheral t_peripheral>
        class uart_impl {
        public:
            /**
             * @brief Places char c on the UART Tx line
             * 
             * @param c 
             */
            static inline void putc(unsigned char c) {
                if constexpr (t_peripheral == uart_port::peripheral::usart0) {
                    while ((USART0->US_CSR & 2) == 0) { }
                    USART0->US_THR = c;
                } else if constexpr (t_peripheral == uart_port::peripheral::usart1) {
                    while ((USART1->US_CSR & 2) == 0) { }
                    USART1->US_THR = c;
                } else if constexpr (t_peripheral == uart_port::peripheral::usart2) {
                    while ((USART2->US_CSR & 2) == 0) { }
                    USART2->US_THR = c;
                }
            }

            /**
             * @brief Reads char from the UART Rx line
             * 
             * @return unsigned char 
             */
            static inline unsigned char getc() {
                if constexpr (t_peripheral == uart_port::peripheral::usart0) {
                    while ((USART0->US_CSR & 1) == 1) { }
                    return USART0->US_RHR; 
                } else if constexpr (t_peripheral == uart_port::peripheral::usart1) {
                    while ((USART1->US_CSR & 1) == 1) { }
                    return USART1->US_RHR; 
                } else if constexpr (t_peripheral == uart_port::peripheral::usart2) {
                    while ((USART2->US_CSR & 1) == 1) { }
                    return USART2->US_RHR; 
                }
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
                if constexpr (t_peripheral == uart_port::peripheral::usart0) {
                    while ((USART0->US_CSR & 1) == 1) { } // TODO: Check for timeout and return -1
                    return USART0->US_RHR;
                } else if constexpr (t_peripheral == uart_port::peripheral::usart1) {
                    while ((USART1->US_CSR & 1) == 1) { } // TODO: Check for timeout and return -1
                    return USART1->US_RHR;
                } else if constexpr (t_peripheral == uart_port::peripheral::usart2) {
                    while ((USART2->US_CSR & 1) == 1) { } // TODO: Check for timeout and return -1
                    return USART2->US_RHR;
                }
            }
        };

        uint_fast64_t now_ticks();

        uint64_t now_us();

        void wait_us_busy(int_fast32_t n);

        void wait_us(int_fast32_t n);

        void wait_ms(int_fast32_t n);
    } // namespace arduino_due
} // namespace hwstl