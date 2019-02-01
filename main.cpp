#include "hwstl.hpp"

// void HandlePIOInterrupt(Pio* pio, uint32_t status) {
//     // Interrupts on PIOC are cleared while reading PIO_ISR.
//     // When an Interrupt is enabled on a �Level�, the interrupt
//     // is generated as long as the interrupt source is not cleared, even if some read accesses in PIO_ISR are performed.
//     if (status != 0) {
//         //hwstl::cout << "Interrupted!\n";
//         //hwstl::wait_ms(20);

//         uint8_t i = 0;

//         while (status != 0) {
//             if (status & 0x01u) {
//                 hwstl::cout << "P" << (unsigned int) i << hwstl::endl;
//             }

//             status >>= 1;
//             ++i;
//         }
//     }
// }

// extern void PIOC_Handler(void) {
//     uint32_t status = PIOC->PIO_ISR;
//     status &= PIOC->PIO_IMR;

//     HandlePIOInterrupt(PIOC, status);
// }


void hwstl::main() {
}