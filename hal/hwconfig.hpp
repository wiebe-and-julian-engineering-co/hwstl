#pragma once

#include <utility>

namespace hwstl {
    using uint32_t = long unsigned int;

    /**
     * @brief Compile time map pair
     * 
     * @tparam t_value_type 
     * @tparam t_key 
     * @tparam t_value 
     */
    template <class t_value_type, uint32_t t_key, t_value_type t_value>
    class constant_pair {
    public:
        static constexpr uint32_t key = t_key;
        static constexpr t_value_type value = t_value;
    };

    /**
     * @brief Boolean alias for constant_pair
     * 
     * @tparam t_key 
     * @tparam t_value 
     */
    template <uint32_t t_key, bool t_value>
    using bool_pair = constant_pair<bool, t_key, t_value>;

    template <
        class t_peripheral_type,
        t_peripheral_type t_peripheral,
        template <class, t_peripheral_type, class...> class t_applicator,
        class... vt_pairs
    >
    class _hwconfig;

    /**
     * @brief Internal _hwconfig class with config pairs
     * 
     * @details
     * Internal _hwconfig class. This class provides get<{int}> for looking
     * up a pair during compile time.
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
        uint32_t t_key,
        bool t_value,
        class... vt_pairs
    >
    class _hwconfig<
        t_peripheral_type,
        t_peripheral,
        t_applicator,
        bool_pair<t_key, t_value>,
        vt_pairs...
    > {
        template <uint32_t t_lookup_key>
        class get {
        public:
            static constexpr bool value = (t_lookup_key == t_key) ? t_value : _hwconfig<
                t_peripheral_type,
                t_peripheral,
                t_applicator,
                vt_pairs...
            >::template get<t_lookup_key>::value;
        };

    public:
        bool destruct = true;

        constexpr _hwconfig() { }

        ~_hwconfig() {
            if (destruct) {
                t_applicator<t_peripheral_type, t_peripheral, bool_pair<t_key, t_value>, vt_pairs...> applicator;
                applicator.apply();
            }
        }
    };

    /**
     * @brief Internal _hwconfig class
     * 
     * @deteails
     * Internal _hwconfig class. This class provides the default get<{int}>
     * by defining a specialization for for a _hwconfig without config
     * pairs. 
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
        template <uint32_t t_lookup_key>
        class get {
        public:
            static constexpr bool value = false;
        };

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
        class... vt_pairs
    >
    constexpr auto operator<< (
        t_config_os<
            t_peripheral_type,
            t_peripheral,
            t_applicator,
            vt_pairs...
        >&& config_os,
        const std::integral_constant<uint32_t, t_applied_setting> conf
    ) {
        // Prevent multiple register writes by unsetting the destruct flag
        config_os.destruct = false;
        return _hwconfig<t_peripheral_type, t_peripheral, t_applicator, bool_pair<t_applied_setting, true>>();
    }
}