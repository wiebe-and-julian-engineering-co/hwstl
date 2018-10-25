//    Copyright Julian van Doorn and Wiebe van Breukelen.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace hwstl {
    template <class t_enable, class t_disable>
    class Peripheral {
        t_enable enable;
        t_disable disable;

    public:
        Peripheral(t_enable enable, t_disable disable) : enable(enable), disable(disable) { }

        void Enable() {
            enable();
        }

        void Disable() {
            disable();
        }
    };
}