/*
 * EmbeddedCPPLib.cpp
 *
 * Created: 19-8-2018 12:35:15
 * Author : Wiebe
 */ 


#include "sam.h"
#include "Time.hpp"
#include "MemoryPool.hpp"
#include "Vector.h"
//#include "lib/UART/HardwareUART.hpp"
//#include "lib/UART/UARTConnection.hpp"

MemoryPool<uint8_t, 500> pool;

struct Person {
	const char* name = "Henk";
	uint8_t age = 20;

	Person (const char* name, uint8_t age) : name(name), age(age) {};
	Person () {};

	void empty() {
		age = 0;
	}
};

bool test() {
	Vector<unsigned int, 500> vec(pool);

	vec.push_back(200);

	if (vec[0] != 200)
		return false;

	vec.erase();

	if (vec[0] != INT_MAX)
		return false;


	return true;
}

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();

	//WDT->WDT_MR = WDT_MR_WDDIS;
	WDT->WDT_MR = WDT_MR_WDDIS;

	//UARTLib::HardwareUART uart(115200, UARTLib::UARTController::ZERO)

	if (test()) {
		//uart << "Test success!";
	} else {
		//uart << "Test failed!";
	}

	// Initialize Serial controller
	//

	//Person* p1 = pool.allocate<Person>();


    /* Replace with your application code */
}
