/*
 * uart.cpp
 *
 * Created: 4-12-2018 21:30:57
 *  Author: Wiebe
 */ 

#include "uart.hpp"
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

extern void UART_Handler(void) {
hwstl::arduino_due::uart_io::IRQHandler();
}