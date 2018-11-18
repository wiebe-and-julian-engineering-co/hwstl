//    Copyright Julian van Doorn and Wiebe van Breukelen.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "arduino-due.hpp" 
#include <sam3xa.h>

hwstl::ring_buffer<uint8_t, 64> hwstl::arduino_due::uart_io::uartReceiveBuffer;
hwstl::ring_buffer<uint8_t, 64> hwstl::arduino_due::uart_io::uartTransmitBuffer;



void hwstl::arduino_due::uart_io::IRQHandler() {
    uint32_t status = UART->UART_SR;

    if ((status & UART_SR_RXRDY) == UART_SR_RXRDY) {
        uartReceiveBuffer.enqueue(UART->UART_RHR);
    }
    
    if ((status & UART_SR_TXRDY) == UART_SR_TXRDY) {
        if (uartTransmitBuffer.size() > 0) {
            UART->UART_THR = uartTransmitBuffer.dequeue();
            } else {
            // Temp disable the interrupt, as we do not have to send anymore data.
            UART->UART_IDR = UART_IDR_TXRDY;
        }
        
    }

    if ((status & UART_SR_OVRE) == UART_SR_OVRE || (status & UART_SR_FRAME) == UART_SR_FRAME) {
        // Fault
        UART->UART_CR |= UART_CR_RSTSTA;
    }
}
 
void hwstl::arduino_due::init() {
    SystemInit();
}

uint_fast64_t hwstl::arduino_due::now_ticks(){
    static bool init_done = false;
    if(!init_done){
        
        // switch to the 84 MHz crystal/PLL clock
        //sam3xa::SystemInit();
        
        EFC0->EEFC_FMR = EEFC_FMR_FWS(4);
        EFC1->EEFC_FMR = EEFC_FMR_FWS(4);
        
        SysTick->CTRL = 0;         // stop the timer
        SysTick->LOAD = 0xFFFFFF;  // use its as a 24-bit timer
        SysTick->VAL = 0;         // clear the timer
        SysTick->CTRL = 5;         // start the timer, 1:1

        init_done = true;
    }
    
    static unsigned int last_low = 0;
    static unsigned long long int high = 0;

    // the timer ticks down, but we want an up counter
    unsigned int low = 0xFFFFFF - (SysTick->VAL & 0xFFFFFF);
    if(low < last_low){
        
        // the timer rolled over, so increment the high part
        high += 0x1ULL << 24;
    }
    last_low = low;

    // return the aggregated ticks value
    // the counter runs at 84 MHz
    return ( low | high );

}

uint64_t hwstl::arduino_due::now_us(){
    return now_ticks() / 84;
}

void hwstl::arduino_due::wait_us_busy( int_fast32_t n ) {
    auto end = now_us() + n;
    while(now_us() < end);
}

void hwstl::arduino_due::wait_us(int_fast32_t n) {
    wait_us_busy(n);
}

void hwstl::arduino_due::wait_ms(int_fast32_t n) {
    while(n > 0){
        wait_us(1'000);
        --n;
    }
}

extern void UART_Handler(void) {
    hwstl::arduino_due::uart_io::IRQHandler();
}


