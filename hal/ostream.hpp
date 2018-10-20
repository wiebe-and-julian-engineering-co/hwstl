#pragma once

#include "streambuf.hpp"

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
			print_dec(val);

			return *this;
		}

		ostream& operator<< (unsigned short val) {
			print_dec(val);

			return *this;
		}

		ostream& operator<< (signed int val) {
			print_dec(val);

			return *this;
		}

		ostream& operator<< (unsigned int val) {
			print_dec(val);

			return *this;
		}

		ostream& operator<< (signed long val) {
			print_dec(val);

			return *this;
		}

		ostream& operator<< (unsigned long val) {
			print_dec(val);

			return *this;
		}

		ostream& operator<< (signed long long val) {
			print_dec(val);

			return *this;
		}

		ostream& operator<< (unsigned long long val) {
			print_dec(val);

			return *this;
		}

        ostream& operator<< (float val) {
			print_floating(val);
			
            // TODO: Unimplemented
            return *this;
        }

        ostream& operator<< (double val) {
            // TODO: Unimplemented
            return *this;
        }

        ostream& operator<< (long double val) {
            // TODO: Unimplemented
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

        // template <class t_type>
        // ostream& operator<< (const t_type& val) {
        //     // TODO: Unimplemented
        //     return *this;
        // }

        ///< Stream buffers (2)	
        ostream& operator<< (streambuf* sb) {
            // TODO: Unimplemented
            return *this;
        }

        ///< Manipulators (3)
        ostream& operator<< (ostream& (*pf)(ostream&)) {
            // TODO: Unimplemented
            return pf(*this);
        }

	private:
		template <class t>
		inline void print_dec(t val) {
			if (val < 0) {
				*this << '-';
				val *= -1;
			} else if (val == 0) {
				*this << '0';
				return;
			}

			reverse_stream rev;
			
			while (val) {
				//*this << static_cast<unsigned char>((val % 10) + '0');

				rev.add_digit(val % 10, 'A');

				val /= 10;
			}

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
		inline void print_floating(t val) {
			t floored = 0;
			floor(val, floored);
			val = static_cast<t>(val - floored);

			// Explicit signed or unsigned notation is favoured, needs further testing...
			print_dec(static_cast<long>(floored));
			*this << '.';
			*this << "FRACTIONAL"; // PRECISION IS CURRENTLY UNSUPPORTED!
			//print_dec(static_cast<long>(val));
		}

		template <class t, class t_ret>
		inline void floor(const t val, t_ret &ret_val) {

			if(val>0) {
				ret_val = static_cast<t_ret>(val);
			} else {
				ret_val = static_cast<t_ret>(val - static_cast<t>(0.9999999999999999));
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
			
			void add_prefix( const char base,const ostream & s ){
				/**if( s.show_base ){
					switch( s.numerical_radix ){
						case 2  : add_char( 'b' ); break;
						case 8  : add_char( 'o' ); break;
						case 10 : return;
						case 16 : add_char( 'x' ); break;
						default : add_char( '?' ); break;
					}
					add_char( '0' );
				}**/
			}
		};
    };
} // namespace hwstl