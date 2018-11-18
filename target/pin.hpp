/*
 * pin.h
 *
 * Created: 18-11-2018 13:22:18
 *  Author: Wiebe
 */ 

#pragma once

#include <cstdint>

// https://stackoverflow.com/questions/34222703/how-to-override-static-method-of-template-class-in-derived-class

namespace hwstl {

    using pin_index = uint32_t;
    template <pin_index... vt_pins>
    using pin_sequence = std::integer_sequence<pin_index, vt_pins...>;
    
    
    namespace subframework {
        class pin {
        public:
            //extern static constexpr pin_info pin_info_array;

            class pin_info {
                public:
                uint8_t m_port;
                uint8_t m_pin;

                constexpr pin_info(uint8_t port, uint8_t pin) : m_port(port), m_pin(pin) { }
            };

            template <pin_index t_pin>
            static inline constexpr uint32_t GetPinInPort() {
                return hwstl::device::pin::pin_info_array[t_pin].m_pin;
            }

            template <pin_index t_pin>
            static inline constexpr uint32_t GetPinMask() {
                return 1 << GetPinInPort<t_pin>();
            }

            template <pin_index t_pin, class PORT_SUPER>
            static inline constexpr PORT_SUPER* GetPortByPin() {
                return hwstl::device::pin::GetPortByPin();
            }

            template <pin_index t_pin>
            static inline int PinEnable() {
                return hwstl::device::pin::PinEnable();
            }

            template <pin_index t_pin>
            static inline constexpr void ProcessPinEntry(uint32_t masks[4]) {
                hwstl::device::pin::ProcessPinEntry(masks);
            }

            template <pin_index... vt_pins>
            static inline void PinSequenceEnable(pin_sequence<vt_pins...> pins) {
                hwstl::device::pin::PinSequenceEnable(pins);
            }

            template <pin_index... vt_pins>
            static inline void configure_in(pin_sequence<vt_pins...> pins) {
                hwstl::device::pin::PinSequenceEnable(pins);
            }

            template <pin_index... vt_pins>
            static inline void configure_out(pin_sequence<vt_pins...> pins) {
                hwstl::device::pin::PinSequenceEnable(pins);
            }

            template <pin_index... vt_pins>
            static inline void configure_inout(pin_sequence<vt_pins...> pins) {
                hwstl::device::pin::PinSequenceEnable(pins);
            }

            template <pin_index t_pin>
            class pin_impl {
            public:
                static constexpr pin_index pin = t_pin;

                constexpr pin_impl() { }

                static inline void set(bool v) {
                    hwstl::device::set<t_pin>(v);
                }

                static inline bool get() {
                    //return T::pin_impl<t_pin>::get();
                    //return T::get<t_pin>();
                    return true;
                }

                static inline void enable_pullup() {
                    
                }

                static inline void disable_pullup() {

                }
            };
        };

        

    };

    }