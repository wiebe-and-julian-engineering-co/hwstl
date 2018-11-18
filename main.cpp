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
    }
    //hwstl::wait_ms(200);
    /**hwstl::cout << "Hello!" << hwstl::endl;

    auto &stream = hwstl::cin;

    size_t available = 0;
    uint32_t lastTime = hwstl::time_ms();

    while (true) {
        if (stream.available()) {
            while (stream.available() > 0) {
                hwstl::cout << stream.getc();
            }
        }
       
        //lastTime = hwstl::time_ms();
        hwstl::wait_ms(5000);
    }**/
}
