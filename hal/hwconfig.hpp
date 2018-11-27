#pragma once

#include <utility>

namespace hwstl {
    // TODO: Include header with uint32_t
    using uint32_t = long unsigned int;

    /**
     * @brief setting_param
     * 
     * @details
     * Compile time constant settings parameter passed into hwconfig
     * 
     * @tparam t_key 
     * @tparam t_value 
     */
    template <uint32_t t_key, bool t_value>
    class setting_params {
    public:
        static constexpr uint32_t key = t_key;
        static constexpr bool value = t_value;
    };

    /**
     * @brief Internal _hwconfig class with config pairs
     * 
     * @details
     * Internal _hwconfig class. The first pair is internally known as t_key
     * and t_value. The rest of the pairs are put in vt_pairs.
     * 
     * If this class is destructed, the config pairs are passed into a
     * setup routine. This routine should evaluate the pairs into
     * the minimum required register IO to configure the underlying
     * peripheral. 
     * 
     * @tparam t_ios iostream to configure
     * @tparam t_peripheral Peripheral to configure
     * @tparam t_key Key of the next item in the map
     * @tparam t_value Value of the next item in the map
     * @tparam vt_pairs Tailing config pairs in the map 
     */
    template <
        class t_peripheral_type,
        t_peripheral_type t_peripheral,
        template <class, t_peripheral_type, class...> class t_applicator,
        class... vt_pairs
    >
    class _hwconfig {
    public:
        bool destruct = true;

        constexpr _hwconfig() { }

        ~_hwconfig() {
            if (destruct) {
                t_applicator<t_peripheral_type, t_peripheral, vt_pairs...> applicator;
                applicator.apply();
            }
        }
    };

    /**
     * @brief Internal _hwconfig class
     * 
     * @deteails
     * Internal _hwconfig class. This specialization is for a _hwconfig without
     * config pairs. 
     * 
     * @tparam t_ios iostream to configure
     * @tparam t_peripheral Peripheral to configure
     */
    template <
        class t_peripheral_type,
        t_peripheral_type t_peripheral,
        template <class, t_peripheral_type, class...> class t_applicator
    >
    class _hwconfig<
        t_peripheral_type,
        t_peripheral,
        t_applicator
    > {
    public:
        bool destruct = true;

        constexpr _hwconfig() { }
    };

    /**
     * @brief Compile time << operator
     * 
     * @details
     * Applies the manipulator to the underlying hardware peripheral of the
     * iostream wrapped by hwconfig. Register IO is combined as much as
     * possible in order to improve run-time efficiency and code size.
     * 
     * @param config_os Configuring ostream
     * @param conf Config item <<'ed into hwconfig
     * @return hwconfig
     */
    template <
        class t_peripheral_type,
        t_peripheral_type t_peripheral,
        template <class, t_peripheral_type, class...> class t_applicator,
        template <class, t_peripheral_type, template <class, t_peripheral_type, class...> class, class...> class t_config_os,
        uint32_t t_applied_setting,
        class... vt_pairs,
        class... vt_params
    >
    constexpr auto operator<< (
        t_config_os<
            t_peripheral_type,
            t_peripheral,
            t_applicator,
            vt_pairs...
        >&& config_os,
        const hwstl::target::constant_stream_manipulator<t_applied_setting, vt_params...> conf
    ) {
        // Prevent multiple register writes by unsetting the destruct flag
        config_os.destruct = false;
        return _hwconfig<t_peripheral_type, t_peripheral, t_applicator, setting_params<t_applied_setting, true>>();
    }
}