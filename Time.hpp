/*
 * Time.h
 *
 * Created: 19-7-2018 10:28:38
 *  Author: Wiebe
 */ 


#ifndef TIME_H_
#define TIME_H_

#include <sam.h>
#include <cstddef>
#include <cstdint>

uint_fast64_t now_ticks();
uint64_t now_us();
void wait_us_busy( int_fast32_t n );
void wait_us( int_fast32_t n );
void wait_ms( int_fast32_t n );



#endif /* TIME_H_ */