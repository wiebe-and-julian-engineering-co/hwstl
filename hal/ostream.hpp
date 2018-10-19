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

        ostream& operator<< (float val) {
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
            // TODO: Unimplemented
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
    };
} // namespace hwstl