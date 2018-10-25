//    Copyright Julian van Doorn and Wiebe van Breukelen.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

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