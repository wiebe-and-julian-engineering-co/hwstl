#include "hwstl.hpp"

void hwstl::main() {
    hwstl::target::uart_io::Configure();

    auto [d13] = hwstl::make_opin(
        hwstl::target::pin::d13
    );
    
    auto [d7] = hwstl::make_ipin(
        hwstl::target::pin::d7
    );

    d7.disable_pullup();

    hwstl::cout << "Hello World!\n";
    hwstl::cout << hwstl::endl;

    hwstl::cout << "Int:\n";
    hwstl::cout << static_cast<unsigned int>(2147483647);
    hwstl::cout << hwstl::endl;

    hwstl::cout << "Long long:\n";
    hwstl::cout << 999999999999LL;
    hwstl::cout << hwstl::endl;

    hwstl::cout << "Signed long:\n";
    hwstl::cout << static_cast<signed long>(-2147483647);
    hwstl::cout << hwstl::endl;

    hwstl::cout << "Unsigned long:\n";
    hwstl::cout << static_cast<unsigned long>(4047483647);
    hwstl::cout << hwstl::endl;

    hwstl::cout << "Short:\n";
    hwstl::cout << static_cast<signed short>(-12345);
    hwstl::cout << hwstl::endl;

    hwstl::cout << "Floating:\n";
    hwstl::cout << static_cast<float>(234.123);
    hwstl::cout << hwstl::endl << hwstl::endl;

    hwstl::wait_ms(100);

    while (true) {
        d13.set(0);
        hwstl::cout << "ON" << hwstl::endl;
        hwstl::wait_ms(1000);
        d13.set(1);
        hwstl::cout << "OFF" << hwstl::endl;
        hwstl::wait_ms(1000);

        // if (!d7.get()) {
        // 	hwstl::cout << "PRESSED!" << hwstl::endl;
        // 	hwstl::wait_ms(50);
        // }
    }
}
