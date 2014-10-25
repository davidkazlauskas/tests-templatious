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

BOOST_AUTO_TEST_CASE( distributor_tests_basic_mixed )
{
    TEMPLATIOUS_TRIPLET_STD;

    long a;
    long long b;
    int c;
    short d;
    char e;
    double f;
    float g;

    auto p = SF::pack(a,b,c,d,e,f,g);
    int res = SM::distribute(SF::seqI('1','7'),p);

    BOOST_CHECK( res == 7 );

    std::stringstream ss;
    auto func = SF::streamFunctor(ss);

    auto prnt = SF::packInsert(p,' ');
    int res2 = SM::callEach(func,prnt);

    BOOST_CHECK( res2 == 7 + 6 );

    BOOST_CHECK( ss.str() == "49 50 51 52 5 54 55" );
}

BOOST_AUTO_TEST_CASE( distributor_tests_special )
{
    TEMPLATIOUS_TRIPLET_STD;
    int a,b,c,d,e,f,g;

    auto p = SF::pack(a,b,c,d,e,f,g);

    SM::distributeSpecial(
        [](int av,int& bv) { bv = av * 7; },
        SF::seqI(1,7),
        p
    );

    int sum = 0;
    SM::callEach([&](int x) { sum += x; },p);

    BOOST_CHECK( sum == 196 );
}

BOOST_AUTO_TEST_SUITE_END()

#endif /* end of include guard: DISTRIBUTORTESTS_JA9IEGAL */
