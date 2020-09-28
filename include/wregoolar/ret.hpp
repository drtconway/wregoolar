#ifndef WREGOOLAR_RET_HPP
#define WREGOOLAR_RET_HPP

#include <type_traits>

namespace wregoolar
{
    namespace ret
    {
        //
        // Compile time set support
        //

        template <char... Cs> struct cset;

        template <char C, typename B> struct cons_cset;

        template <char C, char... Cs>
        struct cons_cset<C, cset<Cs...>>
        {
            using type = cset<C, Cs...>;
        };

        template <typename T>
        struct wrapper
        {
            static constexpr bool value = T::value;
        };

        template <>
        struct cset<>
        {
            using is_empty = std::true_type;

            template <char X>
            using contains = std::false_type;

            template <typename B>
            using is_contained_in = std::true_type;

            template <typename B>
            using intersection = cset<>;

            template <typename B>
            using union_ = B;
        };

        template <char C, char... Cs>
        struct cset<C, Cs...>
        {
            using rest = cset<Cs...>;

            using is_empty = std::false_type;

            template <char X>
            using contains = std::conditional<
                    X == C,
                    std::true_type,
                    typename rest::contains<X>
                >::type;

            template <typename B>
            using is_contained_in = std::conjunction<
                    typename B::contains<C>,
                    typename rest::is_contained_in<B>
                >::type;

            template <typename B>
            using intersection = std::conditional<
                    wrapper<typename B::contains<C>>::value,
                    typename cons_cset<C, typename rest::intersection<B>>::type,
                    typename rest::intersection<B>
                >::type;

            template <typename B>
            using union_ = std::conditional<
                    wrapper<typename B::contains<C>>::value,
                    typename rest::union_<B>,
                    typename cons_cset<C, typename rest::union_<B>>::type
                >::type;
        };

        template <typename A, typename B>
        struct equal_csets :
            std::conjunction<typename A::is_contained_in<B>, typename B::is_contained_in<A>>
        {};

        template <typename A, typename B>
        struct intersection
        {
            using type = typename A::intersection<B>;
        };

        template <typename A, typename B>
        struct union_
        {
            using type = typename A::union_<B>;
        };

        //
        // Regular Expressions
        //

        struct empty
        {
            using has_epsilon = std::false_type;

            template <char X>
            using derivative = empty;
        };

        struct epsilon
        {
            using has_epsilon = std::true_type;

            template <char X>
            using derivative = empty;
        };

        template <typename T, typename U> struct conj;
        template <typename T, typename U> struct disj;

        //
        // Construction / Simplification
        //

        template <template<typename,typename> typename C, typename T, typename U>
        struct make
        {
            using type = C<T,U>;
        };
        
        template <typename T>
        struct make<conj, empty, T>
        {
            using type = empty;
        };

        template <typename T>
        struct make<conj, T, empty>
        {
            using type = empty;
        };

        template <typename T>
        struct make<conj, epsilon, T>
        {
            using type = T;
        };

        template <typename T>
        struct make<conj, T, epsilon>
        {
            using type = T;
        };

        template <typename T>
        struct make<disj, empty, T>
        {
            using type = T;
        };

        template <typename T>
        struct make<disj, T, empty>
        {
            using type = T;
        };

        template <char C>
        struct atom
        {
            using has_epsilon = std::false_type;

            template <char X>
            using derivative = std::conditional<
                    X == C,
                    epsilon,
                    empty
                >::type;
        };

        template <typename T>
        struct star
        {
            using has_epsilon = std::true_type;

            template <char X>
            using derivative = make<conj, typename T::derivative<X>, star<T>>::type;
        };

        template <typename T, typename U>
        struct conj
        {
            using has_epsilon = std::conjunction<typename T::has_epsilon, typename U::has_epsilon>::type;

            template <char X>
            using derivative = std::conditional<
                    wrapper<typename T::has_epsilon>::value,
                    typename make<disj,
                         make<conj, typename T::derivative<X>, U>,
                         typename U::derivative<X>>::type,
                    typename make<conj, typename T::derivative<X>, U>::type
                >::type;
        };

        template <typename T, typename U>
        struct disj
        {
            using has_epsilon = std::disjunction<typename T::has_epsilon, typename U::has_epsilon>::type;

            template <char X>
            using derivative = make<disj, typename T::derivative<X>, typename U::derivative<X>>::type;
        };
    }
    // namespace ret
}
// namespace wregoolar

#endif // WREGOOLAR_RET_HPP
