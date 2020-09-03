//
// Created by Alan Freitas on 2020-07-03.
//

#ifndef MATPLOTPLUSPLUS_CONCEPTS_H
#define MATPLOTPLUSPLUS_CONCEPTS_H

#include <matplot/util/handle_types.h>
#include <matplot/util/type_traits.h>
#include <string>

namespace matplot {
    // https://github.com/Neargye/yacppl/blob/master/include/concepts.hpp
    // Fallback to type T if Enable is not void
    template <typename T, typename Enable = void> using type_concept = T;

    // type_concept where Enable is void if C is false
    template <typename T, bool C>
    using TypeConcept = type_concept<T, typename std::enable_if_t<C>>;

    // TypeConcept where C is true only if T is RValue
    template <typename T>
    using RValue = TypeConcept<
        T, std::is_rvalue_reference<T>::value &&
               !std::is_const<typename std::remove_reference<T>::type>::value>
        &&;

    // TypeConcept where C is true only if T is Const
    template <typename T>
    using Const = TypeConcept<
        T, std::is_const<typename std::remove_reference<T>::type>::value>;

    // TypeConcept where C is true only if T is NotConst
    template <typename T>
    using NotConst = TypeConcept<
        T, !std::is_const<typename std::remove_reference<T>::type>::value>;

    // TypeConcept where C is true only if T is Arithmetic
    template <typename T>
    using Arithmetic = TypeConcept<
        T, std::is_arithmetic<typename std::remove_reference<T>::type>::value>;

    // TypeConcept where C is true only if T is Integral
    template <typename T>
    using Integral = TypeConcept<
        T, std::is_integral<typename std::remove_reference<T>::type>::value>;

    // TypeConcept where C is true only if T is Pointer
    template <typename T>
    using Pointer = TypeConcept<
        T, std::is_pointer<typename std::remove_reference<T>::type>::value>;

    // TypeConcept where C is true only if T is Trivial
    template <typename T>
    using Trivial = TypeConcept<
        T, std::is_trivial<typename std::remove_reference<T>::type>::value>;

    // TypeConcept where C is true only if T is TriviallyCopyable
    template <typename T>
    using TriviallyCopyable =
        TypeConcept<T, std::is_trivially_copyable<
                           typename std::remove_reference<T>::type>::value>;

    // TypeConcept where C is true only if T is String
    template <typename T>
    using String = TypeConcept<T, std::is_same_v<std::decay_t<T>, std::string>>;

    // TypeConcept where C is true only if T is NotString
    template <typename T>
    using NotString =
        TypeConcept<T, !std::is_same_v<std::decay_t<T>, std::string>>;

    // TypeConcept where C is true only if T is NotString
    template <typename T>
    using NotStringConvertible =
        TypeConcept<T, !std::is_convertible_v<std::decay_t<T>, std::string>>;

    class figure_type;
    // TypeConcept where C is true only if T is FigureHandle
    template <typename T>
    using FigureHandle = TypeConcept<
        T, std::is_same_v<std::decay_t<T>, std::shared_ptr<class figure_type>>>;

    // TypeConcept where C is true only if T is NotFigureHandle
    template <typename T>
    using NotFigureHandle =
        TypeConcept<T, !std::is_same_v<std::decay_t<T>,
                                       std::shared_ptr<class figure_type>>>;

    // TypeConcept where C is true only if T is Iterable
    template <typename T>
    using Iterable = TypeConcept<T, is_iterable_v<std::decay_t<T>>>;

    // TypeConcept where C is true only if T is IterableValues
    template <typename T>
    using IterableValues = TypeConcept<T, is_iterable_value_v<std::decay_t<T>>>;

    // TypeConcept where C is true only if T is IterablePair
    template <typename T>
    using IterablePairs = TypeConcept<T, is_iterable_pair_v<std::decay_t<T>>>;

    // TypeConcept where C is true only if T is IterableIterables
    template <typename T>
    using IterableIterables =
        TypeConcept<T, is_iterable_iterable_v<std::decay_t<T>>>;

    // TypeConcept where C is true only if T is IterableValues
    template <typename T>
    using NotAxesHandle =
        TypeConcept<T, !std::is_same_v<std::decay_t<T>, axes_handle>>;

    // TypeConcept where C is true only if T is an initializer list
    template <typename T>
    using InitializerList = TypeConcept<T, is_initializer_list<T>::value>;

    // TypeConcept where C is true only if T is not an initializer list
    template <typename T>
    using NotInitializerList = TypeConcept<T, !is_initializer_list<T>::value>;

    // TypeConcept where C is true only if T is a vector
    template <typename T> using Vector = TypeConcept<T, is_vector<T>::value>;

    // TypeConcept where C is true only if T is not a vector
    template <typename T> using NotVector = TypeConcept<T, is_vector<T>::value>;

    // TypeConcept where C is true only if T is a pair
    template <typename T> using Pair = TypeConcept<T, is_pair<T>::value>;

    // TypeConcept where C is true only if T is not a pair
    template <typename T> using NotPair = TypeConcept<T, is_pair<T>::value>;

} // namespace matplot

#endif // MATPLOTPLUSPLUS_CONCEPTS_H
