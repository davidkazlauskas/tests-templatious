/*
 * =====================================================================================
 *
 *       Filename:  StaticFactoryTests.h
 *
 *    Description:  Static factory tests
 *
 *        Version:  1.0
 *        Created:  2015.02.05 19:35:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef STATICFACTORYTESTS_ZJBL62XK
#define STATICFACTORYTESTS_ZJBL62XK

#include "TestDefs.h"

BOOST_AUTO_TEST_SUITE( static_factory_tests );

BOOST_AUTO_TEST_CASE( static_factory_match_functor_basic )
{
    int a;
    char b;

    auto f = SF::matchFunctor(
        SF::matchLoose<int>([](int i) { return 7;}),
        SF::matchLoose<char>([](char i) { return 77;})
    );

    BOOST_CHECK( f(a) == 7 );
    BOOST_CHECK( f(b) == 77 );
}

BOOST_AUTO_TEST_CASE( static_factory_match_functor_string_to_double )
{
    auto f = SF::matchFunctor(
        SF::matchLoose<const char*>(
            [](const char* c) -> double {
                return std::atof(c);
            }
        ),
        SF::matchLoose<std::string>(
            [](const std::string& c) -> double {
                return std::atof(c.c_str());
            }
        ),
        SF::matchAnyForward<double>()
    );

    const char* str = "7.77";
    std::string stStr = str;

    BOOST_CHECK( f(str) == 7.77 );
    BOOST_CHECK( f(stStr) == 7.77 );
    BOOST_CHECK( f('7') == 55 );
}

BOOST_AUTO_TEST_CASE( static_factory_match_functor_mutate )
{
    struct Fctor {
        Fctor() : _cnt(0) {}
        double operator()(int op) { ++_cnt; return op; }
        int _cnt;
    };

    Fctor statFctor;

    auto f = SF::matchFunctor(
        SF::matchLoose<short>([](short i) -> double { return 2*i; }),
        SF::matchLoose<int>(statFctor)
    );

    short s = 7;
    int i = 77;
    BOOST_CHECK( f(s) == 14 );
    BOOST_CHECK( f(i) == 77 );
    BOOST_CHECK( f(s) == 14 );
    BOOST_CHECK( f(i) == 77 );
    BOOST_CHECK( f(s) == 14 );

    BOOST_CHECK( statFctor._cnt == 2 );
}

BOOST_AUTO_TEST_CASE( static_factory_match_functor_tight_n_loose )
{
    auto f = SF::matchFunctor(
        SF::matchTight<int&>([](int& i) { ++i; return i; }),
        SF::matchLoose<int>([](int i) { return 2*i; })
    );

    int i = 0;
    const int& cref = i;

    BOOST_CHECK( f(i) == 1 );
    BOOST_CHECK( f(i) == 2 );
    BOOST_CHECK( f(i) == 3 );
    BOOST_CHECK( f(7) == 14 );
    BOOST_CHECK( f(8) == 16 );
    BOOST_CHECK( f(i) == 4 );
    BOOST_CHECK( f(i) == 5 );
    BOOST_CHECK( f(cref) == 10 );
    BOOST_CHECK( f(i) == 6 );
    BOOST_CHECK( f(cref) == 12 );
    BOOST_CHECK( f(i) == 7 );

    BOOST_CHECK( std::addressof(i) == std::addressof(cref) );
}

struct AnyFctor {
    template <class... T>
    int operator()(T&&... t) { return 77; }
};

BOOST_AUTO_TEST_CASE( static_factory_match_functor_composition )
{
    auto f1 = SF::matchFunctor(
        SF::matchLoose<int>([](int i) { return 1; }),
        SF::matchLoose<char>([](char i) { return 2; }),
        SF::matchLoose<float>([](char i) { return 3; })
    );

    auto f2 = SF::matchFunctor(
        SF::matchLoose<int>([](int i) { return 4; }),
        SF::matchLoose<short>([](short i) { return 5; }),
        SF::matchLoose<long>([](long l) { return 6; })
    );

    auto f12 = SF::matchFunctor(f1,f2);
    auto f21 = SF::matchFunctor(f2,f1);

    int i;
    char c;
    float f;
    short s;
    long l;

    BOOST_CHECK( f12(i) == 1 );
    BOOST_CHECK( f12(c) == 2 );
    BOOST_CHECK( f12(f) == 3 );
    BOOST_CHECK( f12(s) == 5 );
    BOOST_CHECK( f12(l) == 6 );

    BOOST_CHECK( f21(i) == 4 );
    BOOST_CHECK( f21(c) == 2 );
    BOOST_CHECK( f21(f) == 3 );
    BOOST_CHECK( f21(s) == 5 );
    BOOST_CHECK( f21(l) == 6 );

    auto fAny = SF::matchFunctor(
        f12,
        SF::matchAny(AnyFctor())
    );

    BOOST_CHECK( fAny(i) == 1 );
    BOOST_CHECK( fAny(c) == 2 );
    BOOST_CHECK( fAny(f) == 3 );
    BOOST_CHECK( fAny(s) == 5 );
    BOOST_CHECK( fAny(l) == 6 );

    struct Banana {};

    BOOST_CHECK( fAny("string") == 77 );
    BOOST_CHECK( fAny(std::string("string")) == 77 );
    BOOST_CHECK( fAny(Banana()) == 77 );
}

template <class A,int pos>
struct DeciderA {
    typedef typename std::decay<A>::type Dec;

    static const bool isInt = std::is_same<Dec,int>::value;
    static const bool isDouble = std::is_same<Dec,double>::value;

    static const bool firstArg = pos == 1 && isInt;
    static const bool secondArg = pos == 2 && isDouble;

    static const bool does_match =
        firstArg && secondArg;

    static const int num_args = 2;
};

BOOST_AUTO_TEST_CASE( static_factory_match_functor_custom_function )
{
    auto mf = SF::matchFunctor(
        SF::matchSpecial<DeciderA>([](int i,double d) { return 1; }),
        SF::matchAny(AnyFctor())
    );

    BOOST_CHECK( mf(1,7.7) == 1 );
}

BOOST_AUTO_TEST_SUITE_END();

#endif /* end of include guard: STATICFACTORYTESTS_ZJBL62XK */
