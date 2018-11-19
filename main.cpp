#include "hwstl.hpp"

void hwstl::main() {
    WDT->WDT_MR = WDT_MR_WDDIS;
    //hwstl::target::uart_io::Configure();

    auto [d13] = hwstl::make_opin(
        hwstl::pins::d13
    );
    
    auto [d7] = hwstl::make_ipin(
        hwstl::pins::d7
    );

    while (true) {
        (void) d13.set(true);
        (void) d7.get();
    }
}