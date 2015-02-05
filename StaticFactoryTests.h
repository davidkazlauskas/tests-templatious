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


BOOST_AUTO_TEST_SUITE_END();

#endif /* end of include guard: STATICFACTORYTESTS_ZJBL62XK */
