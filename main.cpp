#include "hwstl.hpp"

void hwstl::main() {
	// hwstl::target::uart_io::Configure();

	auto [ d7, d0 ] = hwstl::make_opin(
		hwstl::target::pin::d7,
		hwstl::target::pin::d0
	);

	(void) d7;
	(void) d0;
}
