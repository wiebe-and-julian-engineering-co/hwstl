#pragma once

#include "streambuf.hpp"
#include "../target/type_definition.hpp"

namespace hwstl {
    template <class t_io>
    class ostream {
        // http://www.cplusplus.com/reference/ostream/ostream/operator%3C%3C/
        // 
        // arithmetic types (1)
        // ostream& operator<< (bool val);
        // ostream& operator<< (short val);
        // ostream& operator<< (unsigned short val);
        // ostream& operator<< (int val);
        // ostream& operator<< (unsigned int val);
        // ostream& operator<< (long val);
        // ostream& operator<< (unsigned long val);
        // ostream& operator<< (long long val);
        // ostream& operator<< (unsigned long long val);
        // ostream& operator<< (float val);
        // ostream& operator<< (double val);
        // ostream& operator<< (long double val);
        // ostream& operator<< (void* val);
        // stream buffers (2)	
        // ostream& operator<< (streambuf* sb );
        // manipulators (3)	
        // ostream& operator<< (ostream& (*pf)(ostream&));
        // ostream& operator<< (ios& (*pf)(ios&));
        // ostream& operator<< (ios_base& (*pf)(ios_base&));

    public:

        ostream& operator<< (const char* ptr) {
            while (*ptr != '\0') {
                t_io::putc(*ptr++);
            }
            return *this;
        }
    
        ///< Arithmetic types (1)
        ostream& operator<< (char val) {
            t_io::putc(val);
            return *this;
        }

        ostream& operator<< (signed char val) {
            t_io::putc(val);
            return *this;
        }

        ostream& operator<< (unsigned char val) {
            t_io::putc(val);
            return *this;
        }

        ostream& operator<< (signed short val) {
            print_base(val);

            return *this;
        }

        ostream& operator<< (unsigned short val) {
            print_base(val);

            return *this;
        }

        ostream& operator<< (signed int val) {
            print_base(val);

            return *this;
        }

        ostream& operator<< (unsigned int val) {
            print_base(val);

            return *this;
        }

        ostream& operator<< (signed long val) {
            print_base(val);

            return *this;
        }

        ostream& operator<< (unsigned long val) {
            print_base(val);

            return *this;
        }

        ostream& operator<< (signed long long val) {
            print_base(val);

            return *this;
        }

        ostream& operator<< (unsigned long long val) {
            print_base(val);

            return *this;
        }

        ostream& operator<< (float val) {
            print_floating(val, 7);
            
            return *this;
        }

        ostream& operator<< (double val) {
            print_floating(val, 13);

            return *this;
        }

        ostream& operator<< (long double val) {
            print_floating(val, 13);

            return *this;
        }

        ostream& operator<< (void* val) {
            // TODO: Unimplemented
            return *this;
        }

        ostream& operator<< (bool val) {
            print_bool(val);

            return *this;
        }

        ///< Stream buffers (2)	
        ostream& operator<< (streambuf* sb) {
            // TODO: Unimplemented
            return *this;
        }

        ///< Manipulators (3)
        /**ostream& operator<< (ostream& (*pf)(ostream&)) {
            // TODO: Unimplemented
            return pf(*this);
        }**/

        ostream& operator<< (hwstl::data_types data_type) {
            base = get_pure_data_type(data_type);

            return *this;
        }

    private:    
        hwstl::pure_data_type base = hwstl::get_pure_data_type(hwstl::dec);

        template <class t>
        inline void print_base(t val) {
            reverse_stream rev;

            if (val < 0) {
                *this << '-';
                val *= -1;
            } else if (val == 0) {
                rev.add_digit(0, 'A');
                return;
            }
            
            while (val > 0) {
                rev.add_digit(val % base.m_base, 'A');

                val /= base.m_base;
            }

             rev.add_prefix(base.m_type);

            *this << rev.content;
        }

        inline void print_bool(const bool b) {
            if (b) {
                *this << "true";
            } else {
                *this << "false";
            }
        }

        template <class t>
        inline void print_floating(t val, uint8_t digits) {  
            // Handle negative numbers
            if (val < 0.0)
            {
                *this << '-';
                //rev.add_char('-');
                val = -val;
            }

            // Round correctly so that print(1.999, 2) prints as "2.00"
            double rounding = 0.5;
            for (uint8_t i=0; i < digits; ++i)
                rounding /= 10.0;
  
            val += rounding;

            // Extract the integer part of the number and print it
            unsigned long int_part = static_cast<unsigned long>(val);
            double remainder = val - static_cast<double>(int_part);

            print_base(int_part);

            // Print the decimal point, but only if there are digits beyond
            if (digits > 0) {
                *this << '.';

                // Extract digits from the remainder one at a time
                while (digits-- > 0)
                {
                    remainder *= 10.0;
                    unsigned int toPrint = (unsigned int)(remainder);
                    print_base(toPrint);
                    remainder -= toPrint;
                }
            }
        }


        struct reverse_stream {
            static constexpr uint_fast16_t length = 70; // May be changed using template metaprogramming.-
            char body[ length ];
            char *content;
            
            reverse_stream(){
                body[ length - 1 ] = '\0';
                content = & body[ length - 1 ];
            }

            
            void add_char( char c ){
                content--;
                *content = c;
            }
            
            void add_digit( char c, char hex_base ){
                if( c > 9 ){
                    c += ( hex_base - 10 );
                    } else {
                    c += '0';
                }
                add_char( c );
            }
            
            void add_prefix(hwstl::data_types data_type ){
                //if( s.show_base ){
                    switch( data_type ){
                        case hwstl::bin  : add_char( 'b' ); break;
                        case hwstl::oct  : add_char( 'o' ); break;
                        case hwstl::dec : return;
                        case hwstl::hex : add_char( 'x' ); break;
                        default : add_char( '?' ); break;
                    }
                    add_char( '0' );
                //}
            }
        };
    };
} // namespace hwstl