#include "target/target.hpp"


int main(void)
{
	SystemInit();

	// TODO: Disable watchdog timer.
	hwstl::target::uart_io::Configure();
	hwstl::wait_ms(1000);

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
	hwstl::cout << static_cast<unsigned short>(12345);
	hwstl::cout << hwstl::endl;

	hwstl::cout << "Floating:\n";
	hwstl::cout << static_cast<float>(234.123);
	hwstl::cout << hwstl::endl << hwstl::endl;
}
