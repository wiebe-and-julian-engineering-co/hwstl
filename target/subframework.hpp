/*
 * subframework.h
 *
 * Created: 18-11-2018 14:13:38
 *  Author: Wiebe
 */ 


#ifndef SUBFRAMEWORK_H_
#define SUBFRAMEWORK_H_


#include "pin.hpp"
#include "uart.hpp"

namespace hwstl::subframework {
    //inline uint_fast64_t time_ticks() {
        //return target::now_ticks();
    //}
    inline uint_fast64_t time_ticks();
    inline uint64_t time_us();
    inline uint32_t time_ms();
    inline void wait_us_busy(int_fast32_t n);
    inline void wait_us(int_fast32_t n);
    inline void wait_ms(int_fast32_t n);

    //inline uint64_t time_us() {
        //return target::now_us();
    //}

    //inline uint32_t time_ms() {
        //return (target::now_us() / 1000);
    //}

    //inline void wait_us_busy(int_fast32_t n) {
        //target::wait_us_busy(n);
    //}

    //inline void wait_us(int_fast32_t n) {
        //target::wait_us_busy(n);
    //}

    //inline void wait_ms(int_fast32_t n) {
        //target::wait_ms(n);
   // }
}





#endif /* SUBFRAMEWORK_H_ */