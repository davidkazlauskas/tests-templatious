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
 *
 * =====================================================================================
 */

#include "TestDefs.hpp"

TEST_CASE( "static_factory_match_functor_basic", "[static_factory_tests]" )
{
    int a;
    char b;

    auto f = SF::matchFunctor(
        SF::matchLoose<int>([](int i) { return 7;}),
        SF::matchLoose<char>([](char i) { return 77;})
    );

    REQUIRE( f(a) == 7 );
    REQUIRE( f(b) == 77 );
}

TEST_CASE( "static_factory_match_functor_string_to_double", "[static_factory_tests]" )
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

    REQUIRE( f(str) == 7.77 );
    REQUIRE( f(stStr) == 7.77 );
    REQUIRE( f('7') == 55 );
}

TEST_CASE( "static_factory_match_functor_mutate", "[static_factory_tests]" )
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
    REQUIRE( f(s) == 14 );
    REQUIRE( f(i) == 77 );
    REQUIRE( f(s) == 14 );
    REQUIRE( f(i) == 77 );
    REQUIRE( f(s) == 14 );

    REQUIRE( statFctor._cnt == 2 );
}

TEST_CASE( "static_factory_match_functor_tight_n_loose", "[static_factory_tests]" )
{
    auto f = SF::matchFunctor(
        SF::matchTight<int&>([](int& i) { ++i; return i; }),
        SF::matchLoose<int>([](int i) { return 2*i; })
    );

    int i = 0;
    const int& cref = i;

    REQUIRE( f(i) == 1 );
    REQUIRE( f(i) == 2 );
    REQUIRE( f(i) == 3 );
    REQUIRE( f(7) == 14 );
    REQUIRE( f(8) == 16 );
    REQUIRE( f(i) == 4 );
    REQUIRE( f(i) == 5 );
    REQUIRE( f(cref) == 10 );
    REQUIRE( f(i) == 6 );
    REQUIRE( f(cref) == 12 );
    REQUIRE( f(i) == 7 );

    REQUIRE( std::addressof(i) == std::addressof(cref) );
}

struct AnyFctor {
    template <class... T>
    int operator()(T&&... t) { return 77; }
};

TEST_CASE( "static_factory_match_functor_composition", "[static_factory_tests]" )
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

    REQUIRE( f12(i) == 1 );
    REQUIRE( f12(c) == 2 );
    REQUIRE( f12(f) == 3 );
    REQUIRE( f12(s) == 5 );
    REQUIRE( f12(l) == 6 );

    REQUIRE( f21(i) == 4 );
    REQUIRE( f21(c) == 2 );
    REQUIRE( f21(f) == 3 );
    REQUIRE( f21(s) == 5 );
    REQUIRE( f21(l) == 6 );

    auto fAny = SF::matchFunctor(
        f12,
        SF::matchAny(AnyFctor())
    );

    REQUIRE( fAny(i) == 1 );
    REQUIRE( fAny(c) == 2 );
    REQUIRE( fAny(f) == 3 );
    REQUIRE( fAny(s) == 5 );
    REQUIRE( fAny(l) == 6 );

    struct Banana {};

    REQUIRE( fAny("string") == 77 );
    REQUIRE( fAny(std::string("string")) == 77 );
    REQUIRE( fAny(Banana()) == 77 );
}

template <class A,int pos>
struct DeciderA {
    typedef typename std::decay<A>::type Dec;

    static const bool isInt = std::is_same<Dec,int>::value;
    static const bool isDouble = std::is_same<Dec,double>::value;
    static const bool isChar = std::is_same<Dec,char>::value;

    static const bool firstArg = pos == 1 && isInt;
    static const bool secondArg = (pos == 2) && (isDouble || isChar);

    static const bool does_match =
        firstArg || secondArg;

    static const int num_args = 2;
};

template <class A,int pos>
struct DeciderB {
    typedef typename std::decay<A>::type Dec;

    static const bool isInt = std::is_same<Dec,int>::value;
    static const bool isStr = std::is_same<Dec,const char*>::value;

    static const bool firstArg = pos == 1 && isInt;
    static const bool secondArg = pos == 2 && isStr;
    static const bool tailArg = pos > 2 && isInt;

    static const bool does_match =
        firstArg || secondArg || tailArg;

    static const int num_args = 0;
};

struct DummyB {
    template <class... Args>
    int operator()(Args&&... args) const {
        return 2;
    }
};

TEST_CASE( "static_factory_match_functor_custom_function", "[static_factory_tests]" )
{
    auto mf = SF::matchFunctor(
        SF::matchSpecial<DeciderA>([](int i,double d) { return 1; }),
        SF::matchSpecial<DeciderB>(DummyB()),
        SF::matchAny(AnyFctor())
    );

    REQUIRE( mf(1,7.7) == 1 );
    REQUIRE( mf(1,7) == 77 );
    REQUIRE( mf(1,'7') == 1 );
    REQUIRE( mf(1,"7") == 2 );
    REQUIRE( mf(1,"7",7) == 2 );
    REQUIRE( mf(1,"7",7,7) == 2 );
}

// Match only when second argument is double
template <class TypeList>
struct DeciderC {
    typedef typename std::decay<
        typename TypeList::template ByIndex<1>::type >::type TDec;
    static const bool secondDouble =
        std::is_same< TDec, double >::value;

    static const bool does_match = secondDouble;
};

struct AnyFctorCust {
    template <class... T>
    int operator()(T&&... t) { return 1; }
};

TEST_CASE( "static_factory_match_functor_custom_function_ext", "[static_factory_tests]" )
{
    auto mf = SF::matchFunctor(
        SF::matchSpecialExt<DeciderC>(AnyFctorCust()),
        SF::matchAny(AnyFctor())
    );

    // 2nd arg should be double to match
    REQUIRE( mf(1,7.7) == 1 );
    REQUIRE( mf(1,7.7,"s") == 1 );
    REQUIRE( mf(1,7.7f,"s") == 77 );
    REQUIRE( mf('a',7.7,"s") == 1 );
    REQUIRE( mf("go ninja go ninja go",7.7,"s",1,2,3,4,5,6,7) == 1 );
}

TEST_CASE( "static_factory_match_functor_loose_first", "[static_factory_tests]" )
{
    // for loose comparison first matches should be enough
    auto mf = SF::matchFunctor(
        SF::matchLoose<int>([](int i,double d) { return 1; }),
        SF::matchLoose<char>([](int i,double d) { return 2; }),
        SF::matchAny(AnyFctor())
    );

    REQUIRE( mf(1,7.7) == 1 );
    REQUIRE( mf('1',7.7) == 2 );
    REQUIRE( mf("moo",7.7) == 77 );
}

TEST_CASE( "static_factory_match_functor_loose_base", "[static_factory_tests]" )
{
    struct A {};
    struct B : public A {};

    auto mf = SF::matchFunctor(
        SF::matchLoose<A>([](A a) { return 1; }),
        SF::matchLoose<B>([](B a) { return 2; }),
        SF::matchAny(AnyFctor())
    );

    REQUIRE( mf(B()) == 1 );
}

