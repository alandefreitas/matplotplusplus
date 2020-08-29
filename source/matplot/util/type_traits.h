//
// Created by Alan Freitas on 2020-07-05.
//

#ifndef MATPLOTPLUSPLUS_TYPE_TRAITS_H
#define MATPLOTPLUSPLUS_TYPE_TRAITS_H

#include <string>
#include <type_traits>
#include <vector>

namespace matplot {
    template <typename C> struct is_iterable {
        typedef long false_type;
        typedef char true_type;

        template <class T> static false_type check(...);
        template <class T>
        static true_type check(int, typename T::const_iterator = C().end());

        enum { value = sizeof(check<C>(0)) == sizeof(true_type) };
    };

    template <typename C> constexpr bool is_iterable_v = is_iterable<C>::value;

    template <typename C> struct has_value_type {
        typedef long false_type;
        typedef char true_type;

        template <class T> static false_type check(...);
        template <class T>
        static true_type check(int, typename T::value_type = *C().begin());

        enum { value = sizeof(check<C>(0)) == sizeof(true_type) };
    };

    template <typename C>
    constexpr bool has_value_type_v = has_value_type<C>::value;

    template <typename C> struct has_iterable_value_type {
        typedef long false_type;
        typedef char true_type;

        template <class T> static false_type check(...);
        template <class T>
        static true_type
        check(int,
              typename T::value_type::value_type = *(C().begin()->begin()));

        enum { value = sizeof(check<C>(0)) == sizeof(true_type) };
    };

    template <typename C>
    constexpr bool has_iterable_value_type_v =
        has_iterable_value_type<C>::value;

    // Something like std::vector<double>
    template <class C>
    struct is_iterable_value
        : public std::integral_constant<
              bool, is_iterable_v<C> && !has_iterable_value_type_v<C>> {};

    template <typename C>
    constexpr bool is_iterable_value_v = is_iterable_value<C>::value;

    // Something like std::vector<std::vector<double>>
    template <class C>
    struct is_iterable_iterable
        : public std::integral_constant<
              bool, is_iterable_v<C> && has_iterable_value_type_v<C>> {};

    template <typename C>
    constexpr bool is_iterable_iterable_v = is_iterable_iterable<C>::value;

    template <class T>
    using is_string =
        std::integral_constant<bool,
                               std::is_same_v<std::decay_t<T>, std::string> &&
                                   std::is_convertible_v<T, std::string>>;

    template <typename C> constexpr bool is_string_v = is_string<C>::value;

    template <typename> struct is_pair : std::false_type {};

    template <typename T, typename U>
    struct is_pair<std::pair<T, U>> : std::true_type {};

    template <typename T> constexpr bool is_pair_v = is_pair<T>::value;

    template <class C>
    struct is_iterable_pair
        : public std::integral_constant<
              bool, is_iterable_v<C> && !has_iterable_value_type_v<C> &&
                        is_pair_v<typename C::value_type>> {};

    template <typename C>
    constexpr bool is_iterable_pair_v = is_iterable_pair<C>::value;

    template <typename> struct is_vector : std::false_type {};

    template <typename T, typename U>
    struct is_vector<std::vector<T, U>> : std::true_type {};

    template <typename> struct is_initializer_list : std::false_type {};

    template <typename T>
    struct is_initializer_list<std::initializer_list<T>> : std::true_type {};

    template <typename T> struct first_type_if_pair { using type = T; };

    template <typename T, typename U>
    struct first_type_if_pair<std::pair<T, U>> {
        using type = T;
    };

    // also tried this, but it didn't help
    template <typename T, typename U>
    struct first_type_if_pair<std::pair<const T, U>> {
        using type = T;
    };

    template <typename T> struct second_type_if_pair { using type = T; };

    template <typename T, typename U>
    struct second_type_if_pair<std::pair<T, U>> {
        using type = U;
    };

    // also tried this, but it didn't help
    template <typename T, typename U>
    struct second_type_if_pair<std::pair<const T, U>> {
        using type = U;
    };

    template <typename ITERABLE> struct iterable_traits {
        using container_type = ITERABLE;
        using iterator_type = decltype(std::declval<container_type>().begin());
        using value_type = typename std::remove_reference<decltype(
            std::declval<iterator_type>().operator*())>::type;
        static constexpr bool is_map = is_pair<value_type>::value;
        using key_type =
            std::conditional_t<is_map, typename value_type::first_type,
                               value_type>;
        using mapped_type =
            std::conditional_t<is_map, typename value_type::second_type,
                               value_type>;
    };
} // namespace matplot

#endif // MATPLOTPLUSPLUS_TYPE_TRAITS_H
