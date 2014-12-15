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
    INIT_BALLER;

    auto v = tt::compTypeVect();

    auto l = [](tt::CompositeType& i) -> int& { return i._a; };
    auto s = SF::select(v,l);

    TEMPLATIOUS_FOREACH(auto& i,s) {
        i *= 2;
    }

    SM::forEach(sf,s);

    BOOST_CHECK( sum == tt::CVEC_SIZE * 7 );
}


BOOST_AUTO_TEST_SUITE_END();

#endif /* end of include guard: SELECTTESTS_90V9R4K9 */
