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
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef SELECTTESTS_90V9R4K9
#define SELECTTESTS_90V9R4K9

#include "TestDefs.h"

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

BOOST_AUTO_TEST_SUITE( select_tests );

BOOST_AUTO_TEST_CASE( select_tests_simple )
{
    INIT_BALLER;

    auto v = tt::compTypeVect();

    auto l = [](tt::CompositeType& i) -> int& { return i._a; };
    auto s = SF::select(v,l);

    SM::forEach(sf,s);

    BOOST_CHECK( sum == tt::CVEC_SIZE * 7 );
}

BOOST_AUTO_TEST_CASE( select_tests_mutate )
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

    BOOST_CHECK( sum == tt::CVEC_SIZE * 7 * 2 );
}

BOOST_AUTO_TEST_CASE( select_tests_const )
{
    auto v = tt::compTypeVect();

    const decltype(v)& cv(v);

    auto l = [](const tt::CompositeType& i) -> const double& { return i._b; };
    auto s = SF::select(cv,l);

    double sum = 0;
    auto sf = templatious::StaticFactory::storageFunctor<Sum>(sum);
    SM::forEach(sf,s);

    BOOST_CHECK( fabs(sum - tt::CVEC_SIZE * 7.77) < 0.00000001 );
}

BOOST_AUTO_TEST_CASE( select_tests_explicit )
{
    auto v = tt::compTypeVect();

    auto l = [](const tt::CompositeType& i) { return i._c * i._c; };
    auto s = SF::select<short>(v,l);

    int sum = 0;
    auto sf = templatious::StaticFactory::storageFunctor<Sum>(sum);
    SM::forEach(sf,s);

    bool sizeGood = true;
    TEMPLATIOUS_FOREACH(auto i,s) {
        sizeGood &= sizeof(i) == sizeof(short);
    }

    BOOST_CHECK( sizeGood );
    BOOST_CHECK( sum == tt::CVEC_SIZE * '7' * '7' );
}

BOOST_AUTO_TEST_CASE( select_tests_move_semantics )
{
    auto v = tt::compTypeVect();

    auto l = [](const tt::CompositeType& i) { return i._c * i._c; };
    auto s = SF::select<short>(std::move(v),l);

    BOOST_CHECK( SA::size(v) == 0 );
    BOOST_CHECK( SA::size(s) == tt::CVEC_SIZE );
}

BOOST_AUTO_TEST_SUITE_END();

#endif /* end of include guard: SELECTTESTS_90V9R4K9 */
