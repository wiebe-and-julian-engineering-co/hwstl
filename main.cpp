#include "hwstl.hpp"

void hwstl::main() {
    hwstl::target::oconfig(hwstl::cout) << hwstl::target::uart_util::Enable;
}