#include "hwstl.hpp"

void hwstl::main() {
    hwstl::target::uart_io::Configure();

    auto [d13] = hwstl::make_opin(
        hwstl::target::pin::d13
    );
    
    auto [d7] = hwstl::make_ipin(
        hwstl::target::pin::d7
    );


    while (true) {
        hwstl::cout << "Enter a character:" << hwstl::endl;
        hwstl::wait_ms(50);
        signed char ch = 0;
        while (ch == 0) {
            hwstl::cin >> ch;
            hwstl::wait_ms(5);
        }

        hwstl::cout << "Result: " << ch << hwstl::endl;
        hwstl::wait_ms(1000);
    }
}
