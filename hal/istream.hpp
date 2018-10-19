#pragma once

#include "streambuf.hpp"

namespace hwstl {
    template <class t_io>
    class istream {
        // http://www.cplusplus.com/reference/istream/istream/operator%3E%3E/
        // 
        // arithmetic types (1)	
        // istream& operator>> (bool& val);
        // istream& operator>> (short& val);
        // istream& operator>> (unsigned short& val);
        // istream& operator>> (int& val);
        // istream& operator>> (unsigned int& val);
        // istream& operator>> (long& val);
        // istream& operator>> (unsigned long& val);
        // istream& operator>> (long long& val);
        // istream& operator>> (unsigned long long& val);
        // istream& operator>> (float& val);
        // istream& operator>> (double& val);
        // istream& operator>> (long double& val);
        // istream& operator>> (void*& val);
        // stream buffers (2)	
        // istream& operator>> (streambuf* sb );
        // manipulators (3)	
        // istream& operator>> (istream& (*pf)(istream&));
        // istream& operator>> (ios& (*pf)(ios&));
        // istream& operator>> (ios_base& (*pf)(ios_base&));
    public:
        ///< Aruthmetic types (1)
        istream& operator>> (signed char& val) {
            val = t_io::getc();
            return *this;
        }

        istream& operator>> (unsigned char& val) {
            val = t_io::getc();
            return *this;
        }

        istream& operator>> (float& val) {
            // TODO: Unimplemented
            return *this;
        }

        istream& operator>> (double& val) {
            // TODO: Unimplemented
            return *this;
        }

        istream& operator>> (long double& val) {
            // TODO: Unimplemented
            return *this;
        }

        istream& operator>> (void*& val) {
            // TODO: Unimplemented
            return *this;
        }

        istream& operator>> (bool& val) {
            // TODO: Unimplemented
            return *this;
        }

        template <class t_type>
        istream& operator>> (t_type& val) {
            // TODO: Unimplemented
            return *this;
        }

        ///< Stream buffers (2)	
        istream& operator>> (streambuf* sb) {
            // TODO: Unimplemented
            return *this;
        }

        ///< Manipulators (3)
        istream& operator>> (istream& (*pf)(istream&)) {
            return pf(*this);
        }
    };
} // namespace hwstl