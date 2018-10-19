#pragma once

#include "istream.hpp"
#include "ostream.hpp"

namespace hwstl {
    template <class t_io>
    class iostream : public istream<t_io>, public ostream<t_io> {
    public:
        iostream() { }
    };
} // namespace hwstls