#include "target/target.hpp"

int main(void)
{
	hwstl::target::uart_io::Configure();
	hwstl::wait_ms(1000);

	hwstl::cout << "Hello world\n";
}
