/*
 * uart.h
 *
 * Created: 18-11-2018 15:11:23
 *  Author: Wiebe
 */ 


#ifndef UART_H_
#define UART_H_

template <class T>
class uart_io {
        private:
            static hwstl::ring_buffer<uint8_t, 64> uartReceiveBuffer;
            static hwstl::ring_buffer<uint8_t, 64> uartTransmitBuffer;

            static constexpr uint32_t MainClockFrequency = 84000000;

            static constexpr int32_t FromFrequencyToPrescalerSelector(uint32_t main_clock_frequency) {
                return T::FromFrequencyToPrescalerSelector(main_clock_frequency);
            }

            static constexpr int32_t FromPrescalerSelectorToFrequency(uint32_t prescaler_selection) {
               return T::FromPrescalerSelectorToFrequency(prescaler_selection); 
            }

            /**
             * @brief Disable baud generation for UART
             */
            static inline void DisableBaud() {
                //UART->UART_BRGR = 0;
                T::DisableBaud();
            }

        };


#endif /* UART_H_ */