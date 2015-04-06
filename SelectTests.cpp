/*
 * =====================================================================================
 *
 *       Filename:  SelectTests.h
 *
 *    Description:  Select statement tests
 *
 *        Version:  1.0
 *        Created:  12/15/2014 05:22:07 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *
 * =====================================================================================
 */

#include "TestDefs.hpp"

namespace test_templatious {
    struct CompositeType {
        CompositeType() : _a(7), _b(7.77), _c('7') {}

        int _a;
        double _b;
        char _c;
    };

    typedef std::vector< CompositeType > CompVect;

    static const int CVEC_SIZE = 100;

    CompVect compTypeVect() {
        CompVect res;

        // TEMPLATIOUS_REPEAT could be used here
        // but these compiler warnings about unused vars...
        for (int i = 0; i < 100; ++i) {
            SA::add(res,CompositeType());
        }

        return std::move(res);
    }
}


TEST_CASE( "select_tests_simple", "[select_tests]" )
{
    INIT_BALLER;

    auto v = tt::compTypeVect();

    auto l = [](const tt::CompositeType& i) -> const int& { return i._a; };
    auto s = SF::select(v,l);
    auto sc = SF::selectC<int>(v,l);

    SM::forEach(sf,s);

    REQUIRE( sum == tt::CVEC_SIZE * 7 );
    REQUIRE( SM::areCollectionsEqual(s,sc) );
}

TEST_CASE( "select_tests_mutate", "[select_tests]" )
{
    auto v = tt::compTypeVect();

    auto l = [](tt::CompositeType& i) -> int& { return i._a; };
    auto s = SF::select(v,l);

    TEMPLATIOUS_FOREACH(auto& i,s) {
        i *= 2;
    }

    int sum = 0;
    TEMPLATIOUS_FOREACH(auto& i,v) {
        sum += i._a;
    }

    REQUIRE( sum == tt::CVEC_SIZE * 7 * 2 );
}

TEST_CASE( "select_tests_const", "[select_tests]" )
{
    auto v = tt::compTypeVect();

    const decltype(v)& cv(v);

    auto l = [](const tt::CompositeType& i) -> const double& { return i._b; };
    auto s = SF::select(cv,l);

    double sum = 0;
    auto sf = templatious::StaticFactory::storageFunctor<Sum>(sum);
    SM::forEach(sf,s);

    REQUIRE( fabs(sum - tt::CVEC_SIZE * 7.77) < 0.00000001 );
}

TEST_CASE( "select_tests_explicit", "[select_tests]" )
{
    auto v = tt::compTypeVect();

    auto l = [](const tt::CompositeType& i) { return i._c * i._c; };
    auto s = SF::select<short>(v,l);
    auto sc = SF::selectC<short>(v,l);

    int sum = 0;
    auto sf = templatious::StaticFactory::storageFunctor<Sum>(sum);
    SM::forEach(sf,s);

    bool sizeGood = true;
    TEMPLATIOUS_FOREACH(auto i,s) {
        sizeGood &= sizeof(i) == sizeof(short);
    }

    REQUIRE( sizeGood );
    REQUIRE( sum == tt::CVEC_SIZE * '7' * '7' );
    REQUIRE( SM::areCollectionsEqual(s,sc) );
}

TEST_CASE( "select_tests_move_semantics", "[select_tests]" )
{
    auto v = tt::compTypeVect();

    auto l = [](const tt::CompositeType& i) { return i._c * i._c; };
    auto s = SF::select<short>(std::move(v),l);

    REQUIRE( SA::size(v) == 0 );
    REQUIRE( SA::size(s) == tt::CVEC_SIZE );
}

