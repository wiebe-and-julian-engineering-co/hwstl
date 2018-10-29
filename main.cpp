#include "hwstl.hpp"

void hwstl::main() {
    hwstl::target::hwconfig(hwstl::cout) << hwstl::target::uart_util::Enable;
}