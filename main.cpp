#include "hwstl.hpp"

void HandlePIOInterrupt(Pio* pio, uint32_t status) {
    // Interrupts on PIOC are cleared while reading PIO_ISR.
    // When an Interrupt is enabled on a “Level”, the interrupt
    // is generated as long as the interrupt source is not cleared, even if some read accesses in PIO_ISR are performed.
    if (status != 0) {
        //hwstl::cout << "Interrupted!\n";
        //hwstl::wait_ms(20);

        uint8_t i = 0;

        while (status != 0) {
            if (status & 0x01u) {
                hwstl::cout << "P" << (unsigned int) i << hwstl::endl;
            }

            status >>= 1;
            ++i;
        }
    }
}

extern void PIOC_Handler(void) {
    uint32_t status = PIOC->PIO_ISR;
    status &= PIOC->PIO_IMR;

    HandlePIOInterrupt(PIOC, status);
}


void hwstl::main() {
    WDT->WDT_MR = WDT_MR_WDDIS; 
    //hwstl::internals::disable_watchdog();

    hwstl::target::uart_io::Configure();

    auto [d13] = hwstl::make_opin(
        hwstl::target::pin::d13
    );

    auto [d7] = hwstl::make_ipin(
        hwstl::target::pin::d7
    );

    // All the interrupt sources are enabled by writing 32’hFFFF_FFFF in PIO_IER. -> Section 31.5.10.2
    // Then the Additional Interrupt Mode is enabled for line 0 to 7 by writing 32’h0000_00FF in PIO_AIMER.

    // Set ISR to zero
    PIOC->PIO_ISR;

    // Disable all interrupts.
	PIOC->PIO_IDR = 0xFFFFFFFF;

    NVIC_DisableIRQ(PIOC_IRQn);
    NVIC_ClearPendingIRQ(PIOC_IRQn);
    NVIC_SetPriority(PIOC_IRQn, 0);
    NVIC_EnableIRQ(PIOC_IRQn);

    // Extended interrupt mode used for edge detection.
    //PIOC->PIO_AIMER  = (0x01 << 23);
    
    PIOC->PIO_AIMDR = (0x01 << 23);
    //PIOC->PIO_LSR = (0x01 << 23);    
    // Enable interrupts on pin d7 -> 2, 23 

    PIOC->PIO_ISR;
    PIOC->PIO_IER = (0x01 << 23);
    

    

    while (true) {};

   /** while (true) {
        uint32_t status;
	    uint32_t i;

	    status = PIOC->PIO_ISR;
	    //status &= PIOC->PIO_IMR;

        if (status != 0) {
            hwstl::cout << "PIO interrupt!\n";
            hwstl::wait_ms(50);
            //hwstl::cout << status << hwstl::endl;
        }

        //hwstl::cout << d7.get() << hwstl::endl;
        //hwstl::wait_ms(10);
    }**/
    
    // Disable interrupts on pin d7
    PIOC->PIO_IDR = (0x01u << 23);

    hwstl::wait_ms(100);


}


