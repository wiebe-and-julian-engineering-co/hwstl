#include "hwstl.hpp"

void hwstl::main() {
	hwstl::target::uart_io::Configure();

	// hwstl::target::pin_io::Configure(
	// 	hwstl::target::pin::d0,
	// 	hwstl::target::pin::d1,
	// 	hwstl::target::pin::d2
	// );

	// hwstl::wait_ms(1000);

	// hwstl::cout << "Hello world!\n";


	hwstl::cout << "Hello world!1\n";

	auto [ d7 ] = hwstl::make_opin(
		hwstl::target::pin::d7
	);

	// hwstl::opin<hwstl::target::pin::d1> d13 = {};

	// d0.set(0);

	hwstl::cout << "Hello world!2\n";

	while (true) {
		d7.set(0);
		hwstl::wait_ms(1000);
		d7.set(1);
		hwstl::wait_ms(1000);
		
	hwstl::cout << "Hello world!3\n";
	}
}
