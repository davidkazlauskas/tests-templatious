/*
 * =====================================================================================
 *
 *       Filename:  IterDumpTests.h
 *
 *    Description:  Iterator dump
 *
 *        Version:  1.0
 *        Created:  2015.02.24 19:26:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef ITERDUMPTESTS_XJXLMIV5
#define ITERDUMPTESTS_XJXLMIV5

#include "TestDefs.h"

BOOST_AUTO_TEST_SUITE( iter_dump_tests );

BOOST_AUTO_TEST_CASE( iter_dump_tests_basic )
{

    auto v = std::vector<int>();
    SA::add(v,1,2,3);
    auto d = SF::iterDump(v);

    BOOST_CHECK( 6 == SM::sum<int>(d) );
}

BOOST_AUTO_TEST_SUITE_END();

#endif /* end of include guard: ITERDUMPTESTS_XJXLMIV5 */
