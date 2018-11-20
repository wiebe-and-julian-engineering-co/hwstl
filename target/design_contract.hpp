/*
 * design_contract.h
 *
 * Created: 18-11-2018 20:58:19
 *  Author: Wiebe
 */ 

#pragma once

#include <type_traits>
#include <utility>

namespace confirm_design_contract
{
    /**template<class> struct sfinae_true : std::true_type{};

    template<class T, class METHOD, class... ARGS>
    static auto test(ARGS...) -> sfinae_true<decltype(std::declval<T>().METHOD(std::declval<ARGS...>()))>;

    template<class T, class METHOD, class... ARGS>
    static auto test(...) -> std::false_type{};

    //template <typename C>
    //static one test(decltype(&C::set(true)));

    //template <typename C>
    //static one test(decltype(&C::(*METHOD)));
    //template <typename C>
    //static two test(...);

    template<class T, class METHOD, class... ARGS>
    struct has_method : decltype(test<T, METHOD, ARGS...>()){};**/

    /**template <template <typename> class TypeChecker, typename Type>
    struct has_method_helper
    {
        // these structs are used to recognize which version
        // of the two functions was chosen during overload resolution
        struct supported {};
        struct not_supported {};

        // this overload of chk will be ignored by SFINAE principle
        // if TypeChecker<Type_> is invalid type
        template <typename Type_>
        static supported chk(typename std::decay<TypeChecker<Type_>>::type *);

        // ellipsis has the lowest conversion rank, so this overload will be
        // chosen during overload resolution only if the template overload above is ignored
        template <typename Type_>
        static not_supported chk(...);

        // if the template overload of chk is chosen during
        // overload resolution then the feature is supported
        // if the ellipses overload is chosen the the feature is not supported
        static constexpr bool value = std::is_same<decltype(chk<Type>(nullptr)),supported>::value;
    };**/

    #define resultof(f, ...) typeof(Param<__VA_ARGS__>::Func(f))

    template <class... Args>
    class Param
    {

        public:

        template <class R>
        static R Func (R (*) (Args...));

        template <class R, class C>
        static R Func (R (C::*) (Args...));

        template <class R, class C>
        static R Func (R (C::*) (Args...) const);

    };

    template <class T>
    class Proxy
    {

        //  using my resultof solution
        template <class... Args>
        resultof(T::f, Args...) f (Args... x)
        {
            return x.f(x...);
        }

        //  using another return type extraction solution
        template<typename... Args>
        decltype( std::declval<T&>().f(std::declval<Args>()...) )
        f(Args&&... args)
        {
            return x.f(std::forward<Args>(args)...);
        }

        T x;

    };

    //template <typename T>
    //using has_method = decltype(double(std::declval<T>().set(std::declval<bool>())));
   
};