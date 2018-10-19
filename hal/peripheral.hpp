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