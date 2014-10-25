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

    int res0 = SM::distributeSpecial(
        [](int av,int& bv) { bv = av * 7; },
        SF::seqI(1,7),
        p
    );

    BOOST_CHECK( res0 == 7 );

    int sum = 0;
    int res1 = SM::callEach([&](int x) { sum += x; },p);

    BOOST_CHECK( res1 == 7 );
    BOOST_CHECK( sum == 196 );
}

BOOST_AUTO_TEST_CASE( distributor_tests_midbreak )
{
    TEMPLATIOUS_TRIPLET_STD;
    int a,b,c,d,e,f,g;
    auto p = SF::pack(a,b,c,d,e,f,g);

    SM::set(-1,p);
    int sum = 0;
    SM::callEach([&](int i) { sum += i; },p);

    BOOST_CHECK( sum == -7 );

    int count = 4;
    int res0 = SM::distributeSpecial(
        [&](int av,int& bv) {
            bv = av;
            return --count > 0;
        },
        SF::seqI(1,7),
        p
    );

    BOOST_CHECK( res0 == 4 );

    sum = 0;
    int res1 = SM::callEach([&](int i) { sum += i; },p);

    BOOST_CHECK( res1 == 7 );
    BOOST_CHECK( sum == 1 + 2 + 3 + 4 - 1 - 1 - 1 );
}

BOOST_AUTO_TEST_CASE( distributor_tests_ignorestop )
{
    TEMPLATIOUS_TRIPLET_STD;
    int a,b,c,d,e,f,g;
    auto p = SF::pack(a,b,c,d,e,f,g);

    SM::set(-1,p);
    int sum = 0;
    SM::callEach([&](int i) { sum += i; },p);

    BOOST_CHECK( sum == -7 );

    int count = 4;
    int res0 = SM::distributeSpecial<true>(
        [&](int av,int& bv) {
            bv = av;
            return --count > 0;
        },
        SF::seqI(1,7),
        p
    );

    BOOST_CHECK( res0 == 7 );

    sum = 0;
    int res1 = SM::callEach([&](int i) { sum += i; },p);

    BOOST_CHECK( res1 == 7 );
    BOOST_CHECK( sum == 1 + 2 + 3 + 4 + 5 + 6 + 7 );
}

BOOST_AUTO_TEST_CASE( distributor_tests_ignorestop_reverse )
{
    TEMPLATIOUS_TRIPLET_STD;
    int a,b,c,d,e,f,g;
    auto p = SF::pack(a,b,c,d,e,f,g);
    int res0 = SM::distribute(SF::seqI(1,7),p);

    BOOST_CHECK( res0 == 7 );

    int sum = 0;
    SM::callEach([&](int i) { sum += i; },p);

    BOOST_CHECK( sum == 1 + 2 + 3 + 4 + 5 + 6 + 7 );

    int arr[7];
    SM::set(-1,arr);

    sum = 0;
    int res1 = SM::forEach([&](int i) { sum += i; },arr);
    BOOST_CHECK( res1 == 7 );
    BOOST_CHECK( sum == -7 );

    int count = 4;
    int res2 = SM::distributeSpecial<true>(
            [&](int av,int& bv) {
                bv = av;
                return --count > 0;
            },
            p,arr);
    BOOST_CHECK( res2 == 7 );

    sum = 0;
    int res3 = SM::forEach([&](int i) { sum += i; },arr);

    BOOST_CHECK( res3 == 7 );
    BOOST_CHECK( sum == 1 + 2 + 3 + 4 + 5 + 6 + 7 );
}

BOOST_AUTO_TEST_CASE( distributor_tests_calleach_midbreak )
{
    TEMPLATIOUS_TRIPLET_STD;
    int a,b,c,d,e,f,g;
    auto p = SF::pack(a,b,c,d,e,f,g);
    int res0 = SM::distribute(SF::seqI(1,7),p);

    BOOST_CHECK( res0 == 7 );

    int count = 10;
    int res1 = SM::callEach([&](int& i) {
            i *= 7;
            return --count > 0;
        },p,p,p);

    BOOST_CHECK( res1 == 10 );

    int sum = 0;
    SM::callEach([&](int i) { sum += i; },p);

    int expectedSum = 49 + 98 + 147 +
        28 + 35 + 42 + 49;

    BOOST_CHECK( sum == expectedSum );
}

BOOST_AUTO_TEST_CASE( distributor_tests_calleach_midbreak_ignore )
{
    TEMPLATIOUS_TRIPLET_STD;
    int a,b,c,d,e,f,g;
    auto p = SF::pack(a,b,c,d,e,f,g);
    int res0 = SM::distribute(SF::seqI(1,7),p);

    BOOST_CHECK( res0 == 7 );

    int count = 10;
    int res1 = SM::callEach<true>([&](int& i) {
            i *= 7;
            return --count > 0;
        },p,p,p);

    BOOST_CHECK( res1 == 7 * 3 );

    int sum = 0;
    SM::callEach([&](int i) { sum += i; },p);

    int expectedSum = 343 + 686 + 1029 + 1372 + 1715 + 2058 + 2401;

    BOOST_CHECK( sum == expectedSum );
}

BOOST_AUTO_TEST_CASE( distributor_tests_foreach_midbreak )
{
    TEMPLATIOUS_TRIPLET_STD;
    int arr[7];
    int res0 = SM::distribute(SF::seqI(1,7),arr);

    BOOST_CHECK( res0 == 7 );

    int count = 10;
    int res1 = SM::forEach<true>([&](int& i) {
            i *= 7;
            return --count > 0;
        },arr,arr,arr);

    std::cout << res1 << std::endl;
    BOOST_CHECK( res1 == 7 * 3 );

    int sum = 0;
    SM::forEach([&](int i) { sum += i; },arr);

    int expectedSum = 343 + 686 + 1029 + 1372 + 1715 + 2058 + 2401;

    BOOST_CHECK( sum == expectedSum );
}


BOOST_AUTO_TEST_SUITE_END()

#endif /* end of include guard: DISTRIBUTORTESTS_JA9IEGAL */
