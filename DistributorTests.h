/*
 * =====================================================================================
 *
 *       Filename:  DistributorTests.h
 *
 *    Description:  Distributor tests
 *
 *        Version:  1.0
 *        Created:  10/23/2014 06:58:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef DISTRIBUTORTESTS_JA9IEGAL
#define DISTRIBUTORTESTS_JA9IEGAL

#include "TestDefs.h"

BOOST_AUTO_TEST_SUITE( distributor_tests )

BOOST_AUTO_TEST_CASE( distributor_tests_basic )
{
    TEMPLATIOUS_TRIPLET_STD;

    int a,b,c,d,e,f,g;
    auto p = SF::pack(a,b,c,d,e,f,g);
    SM::distribute(SF::seqI(1,7),p);

    int sum = 0;
    SM::callEach([&](int i) { sum += i; },p);
    BOOST_CHECK( sum == 28 );
}

BOOST_AUTO_TEST_SUITE_END()

#endif /* end of include guard: DISTRIBUTORTESTS_JA9IEGAL */
