#include "wregoolar/ret.hpp"

#define BOOST_TEST_MODULE basic template operations
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( cset_cons_0 )
{
    using namespace wregoolar::ret;

    using R = cset<'a'>;
    using S = cset<>;
    using T = cons_cset<'a', S>::type;
    using U = std::is_same<R, T>;

    BOOST_CHECK_EQUAL(U::value, true);
}

BOOST_AUTO_TEST_CASE( cset_cons_1 )
{
    using namespace wregoolar::ret;

    using R = cset<'a', 'b', 'c'>;
    using S = cset<'b', 'c'>;
    using T = cons_cset<'a', S>::type;
    using U = std::is_same<R, T>;

    BOOST_CHECK_EQUAL(U::value, true);
}

BOOST_AUTO_TEST_CASE( cset_contains_0 )
{
    using namespace wregoolar::ret;

    using S = cset<>;
    BOOST_CHECK_EQUAL(S::contains<'a'>::value, false);
}

BOOST_AUTO_TEST_CASE( cset_contains_1 )
{
    using namespace wregoolar::ret;

    using S = cset<'a'>;
    BOOST_CHECK_EQUAL(S::contains<'a'>::value, true);
}

BOOST_AUTO_TEST_CASE( cset_contains_2 )
{
    using namespace wregoolar::ret;

    using S = cset<'b'>;
    BOOST_CHECK_EQUAL(S::contains<'a'>::value, false);
}

BOOST_AUTO_TEST_CASE( cset_contains_3 )
{
    using namespace wregoolar::ret;

    using S = cset<'a', 'b', 'c', 'd'>;
    BOOST_CHECK_EQUAL(S::contains<'a'>::value, true);
}

BOOST_AUTO_TEST_CASE( cset_contains_4 )
{
    using namespace wregoolar::ret;

    using S = cset<'a', 'b', 'c', 'd'>;
    BOOST_CHECK_EQUAL(S::contains<'c'>::value, true);
}


BOOST_AUTO_TEST_CASE( cset_is_contained_in_0 )
{
    using namespace wregoolar::ret;

    using S = cset<'a', 'b', 'c', 'd'>;
    using T = cset<'b', 'd'>;
    BOOST_CHECK_EQUAL(T::is_contained_in<S>::value, true);
    BOOST_CHECK_EQUAL(S::is_contained_in<T>::value, false);
}

BOOST_AUTO_TEST_CASE( equal_csets_0 )
{
    using namespace wregoolar::ret;

    using S = cset<'a', 'b', 'c', 'd'>;
    using T = cset<'b', 'd'>;
    using X = equal_csets<S,T>;
    BOOST_CHECK_EQUAL(X::value, false);
    using Y = equal_csets<T,S>;
    BOOST_CHECK_EQUAL(Y::value, false);
}

BOOST_AUTO_TEST_CASE( equal_csets_1 )
{
    using namespace wregoolar::ret;

    using S = cset<'a', 'b', 'c', 'd'>;
    using T = cset<'b', 'd', 'a', 'c'>;
    using X = equal_csets<S,T>;
    BOOST_CHECK_EQUAL(X::value, true);
    using Y = equal_csets<T,S>;
    BOOST_CHECK_EQUAL(Y::value, true);
}

BOOST_AUTO_TEST_CASE( intersection_0 )
{
    using namespace wregoolar::ret;

    using R = cset<'a', 'c'>;
    using S = cset<'a', 'b', 'c', 'd'>;
    using T = cset<'g', 'a', 'f', 'c'>;
    using X = intersection<S,T>::type;
    using Y = equal_csets<X,R>;
    static_assert(Y::value);
}

BOOST_AUTO_TEST_CASE( intersection_1 )
{
    using namespace wregoolar::ret;

    using R = cset<'a', 'c'>;
    using S = cset<'a', 'b', 'c', 'd'>;
    using T = cset<'g', 'a', 'f', 'c'>;
    using X = intersection<S,T>::type;
    using Y = equal_csets<X,R>;
    static_assert(Y::value);
}

BOOST_AUTO_TEST_CASE( union_0 )
{
    using namespace wregoolar::ret;

    using R = cset<'a', 'b', 'c', 'd', 'f', 'g'>;
    using S = cset<'a', 'b', 'c', 'd'>;
    using T = cset<'g', 'a', 'f', 'c'>;
    using X1 = union_<S,T>::type;
    using Y1 = equal_csets<X1,R>;
    static_assert(Y1::value);
    using X2 = union_<T,S>::type;
    using Y2 = equal_csets<X2,R>;
    static_assert(Y2::value);
}

BOOST_AUTO_TEST_CASE( has_epsilon_0 )
{
    using namespace wregoolar::ret;

    using R0 = empty;
    static_assert(R0::has_epsilon::value == false);

    using R1 = epsilon;
    static_assert(R1::has_epsilon::value == true);

    using R2 = atom<'a'>;
    static_assert(R2::has_epsilon::value == false);

    using R3 = star<atom<'a'>>;
    static_assert(R3::has_epsilon::value == true);

    using R4 = conj<atom<'a'>, atom<'b'>>;
    static_assert(R4::has_epsilon::value == false);

    using R5 = conj<star<atom<'a'>>, atom<'b'>>;
    static_assert(R5::has_epsilon::value == false);

    using R6 = conj<star<atom<'a'>>, star<atom<'b'>>>;
    static_assert(R6::has_epsilon::value == true);

    using R7 = disj<atom<'a'>, atom<'b'>>;
    static_assert(R7::has_epsilon::value == false);

    using R8 = disj<star<atom<'a'>>, atom<'b'>>;
    static_assert(R8::has_epsilon::value == true);

    using R9 = disj<star<atom<'a'>>, star<atom<'b'>>>;
    static_assert(R9::has_epsilon::value == true);
}

BOOST_AUTO_TEST_CASE( derivative_0 )
{
    using namespace wregoolar::ret;

    using R0 = atom<'a'>;

    using Sa = R0::derivative<'a'>;
    static_assert(std::is_same<Sa, epsilon>::value);

    using Sb = R0::derivative<'b'>;
    static_assert(std::is_same<Sb, empty>::value);
}

BOOST_AUTO_TEST_CASE( derivative_1 )
{
    using namespace wregoolar::ret;

    using R0 = conj<atom<'a'>, atom<'b'>>;

    using Sa = R0::derivative<'a'>;
    static_assert(std::is_same<Sa, atom<'b'>>::value);

    using Sb = R0::derivative<'b'>;
    static_assert(std::is_same<Sb, empty>::value);
}

BOOST_AUTO_TEST_CASE( derivative_2 )
{
    using namespace wregoolar::ret;

    using R0 = disj<atom<'a'>, atom<'b'>>;

    using Sa = R0::derivative<'a'>;
    static_assert(std::is_same<Sa, epsilon>::value);

    using Sb = R0::derivative<'b'>;
    static_assert(std::is_same<Sb, epsilon>::value);
}
