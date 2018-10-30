#pragma once

namespace hwstl {
    class internal {
    public:
        internal() {};

        inline void watchdog_enable() const {
            hwstl::target::internal::watchdog_enable();
        }

        inline void watchdog_disable() const {
            hwstl::target::internal::watchdog_disable(); 
        }
    };
} // namespace hwstls