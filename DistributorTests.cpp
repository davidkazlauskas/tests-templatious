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

#include "TestDefs.h"

TEST_CASE( "distributor_tests_basic", "[distributor_tests]" )
{
    TEMPLATIOUS_TRIPLET_STD;

    int a,b,c,d,e,f,g;
    auto p = SF::pack(a,b,c,d,e,f,g);
    SM::distribute(SF::seqI(1,7),p);

    int sum = 0;
    SM::callEach([&](int i) { sum += i; },p);
    REQUIRE( sum == 28 );
}

TEST_CASE( "distributor_tests_basic_mixed", "[distributor_tests]" )
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

    REQUIRE( res == 7 );

    std::stringstream ss;
    auto func = SF::streamFunctor(ss);

    auto prnt = SF::packInsert(p,' ');
    int res2 = SM::callEach(func,prnt);

    REQUIRE( res2 == 7 + 6 );

    REQUIRE( ss.str() == "49 50 51 52 5 54 55" );
}

TEST_CASE( "distributor_tests_special", "[distributor_tests]" )
{
    TEMPLATIOUS_TRIPLET_STD;
    int a,b,c,d,e,f,g;

    auto p = SF::pack(a,b,c,d,e,f,g);

    int res0 = SM::distributeSpecial(
        [](int av,int& bv) { bv = av * 7; },
        SF::seqI(1,7),
        p
    );

    REQUIRE( res0 == 7 );

    int sum = 0;
    int res1 = SM::callEach([&](int x) { sum += x; },p);

    REQUIRE( res1 == 7 );
    REQUIRE( sum == 196 );
}

TEST_CASE( "distributor_tests_midbreak", "[distributor_tests]" )
{
    TEMPLATIOUS_TRIPLET_STD;
    int a,b,c,d,e,f,g;
    auto p = SF::pack(a,b,c,d,e,f,g);

    SM::set(-1,p);
    int sum = 0;
    SM::callEach([&](int i) { sum += i; },p);

    REQUIRE( sum == -7 );

    int count = 4;
    int res0 = SM::distributeSpecial(
        [&](int av,int& bv) {
            bv = av;
            return --count > 0;
        },
        SF::seqI(1,7),
        p
    );

    REQUIRE( res0 == 4 );

    sum = 0;
    int res1 = SM::callEach([&](int i) { sum += i; },p);

    REQUIRE( res1 == 7 );
    REQUIRE( sum == 1 + 2 + 3 + 4 - 1 - 1 - 1 );
}

TEST_CASE( "distributor_tests_ignorestop", "[distributor_tests]" )
{
    TEMPLATIOUS_TRIPLET_STD;
    int a,b,c,d,e,f,g;
    auto p = SF::pack(a,b,c,d,e,f,g);

    SM::set(-1,p);
    int sum = 0;
    SM::callEach([&](int i) { sum += i; },p);

    REQUIRE( sum == -7 );

    int count = 4;
    int res0 = SM::distributeSpecial<true>(
        [&](int av,int& bv) {
            bv = av;
            return --count > 0;
        },
        SF::seqI(1,7),
        p
    );

    REQUIRE( res0 == 7 );

    sum = 0;
    int res1 = SM::callEach([&](int i) { sum += i; },p);

    REQUIRE( res1 == 7 );
    REQUIRE( sum == 1 + 2 + 3 + 4 + 5 + 6 + 7 );
}

TEST_CASE( "distributor_tests_ignorestop_reverse", "[distributor_tests]" )
{
    TEMPLATIOUS_TRIPLET_STD;
    int a,b,c,d,e,f,g;
    auto p = SF::pack(a,b,c,d,e,f,g);
    int res0 = SM::distribute(SF::seqI(1,7),p);

    REQUIRE( res0 == 7 );

    int sum = 0;
    SM::callEach([&](int i) { sum += i; },p);

    REQUIRE( sum == 1 + 2 + 3 + 4 + 5 + 6 + 7 );

    int arr[7];
    SM::set(-1,arr);

    sum = 0;
    int res1 = SM::forEach([&](int i) { sum += i; },arr);
    REQUIRE( res1 == 7 );
    REQUIRE( sum == -7 );

    int count = 4;
    int res2 = SM::distributeSpecial<true>(
            [&](int av,int& bv) {
                bv = av;
                return --count > 0;
            },
            p,arr);
    REQUIRE( res2 == 7 );

    sum = 0;
    int res3 = SM::forEach([&](int i) { sum += i; },arr);

    REQUIRE( res3 == 7 );
    REQUIRE( sum == 1 + 2 + 3 + 4 + 5 + 6 + 7 );
}

TEST_CASE( "distributor_tests_calleach_midbreak", "[distributor_tests]" )
{
    TEMPLATIOUS_TRIPLET_STD;
    int a,b,c,d,e,f,g;
    auto p = SF::pack(a,b,c,d,e,f,g);
    int res0 = SM::distribute(SF::seqI(1,7),p);

    REQUIRE( res0 == 7 );

    int count = 10;
    int res1 = SM::callEach([&](int& i) {
            i *= 7;
            return --count > 0;
        },p,p,p);

    REQUIRE( res1 == 10 );

    int sum = 0;
    SM::callEach([&](int i) { sum += i; },p);

    int expectedSum = 49 + 98 + 147 +
        28 + 35 + 42 + 49;

    REQUIRE( sum == expectedSum );
}

TEST_CASE( "distributor_tests_calleach_midbreak_ignore", "[distributor_tests]" )
{
    TEMPLATIOUS_TRIPLET_STD;
    int a,b,c,d,e,f,g;
    auto p = SF::pack(a,b,c,d,e,f,g);
    int res0 = SM::distribute(SF::seqI(1,7),p);

    REQUIRE( res0 == 7 );

    int count = 10;
    int res1 = SM::callEach<true>([&](int& i) {
            i *= 7;
            return --count > 0;
        },p,p,p);

    REQUIRE( res1 == 7 * 3 );

    int sum = 0;
    SM::callEach([&](int i) { sum += i; },p);

    int expectedSum = 343 + 686 + 1029 + 1372 + 1715 + 2058 + 2401;

    REQUIRE( sum == expectedSum );
}

TEST_CASE( "distributor_tests_foreach_midbreak", "[distributor_tests]" )
{
    TEMPLATIOUS_TRIPLET_STD;
    int arr[7];
    int res0 = SM::distribute(SF::seqI(1,7),arr);

    REQUIRE( res0 == 7 );

    int count = 10;
    int res1 = SM::forEach([&](int& i) {
            i *= 7;
            return --count > 0;
        },arr,arr,arr);

    REQUIRE( res1 == 10 );

    int sum = 0;
    SM::forEach([&](int i) { sum += i; },arr);

    int expectedSum = 49 + 98 + 147 +
        28 + 35 + 42 + 49;

    REQUIRE( sum == expectedSum );
}

TEST_CASE( "distributor_tests_foreach_midbreak_ignore", "[distributor_tests]" )
{
    TEMPLATIOUS_TRIPLET_STD;
    int arr[7];
    int res0 = SM::distribute(SF::seqI(1,7),arr);

    REQUIRE( res0 == 7 );

    int count = 10;
    int res1 = SM::forEach<true>([&](int& i) {
            i *= 7;
            return --count > 0;
        },arr,arr,arr);

    REQUIRE( res1 == 7 * 3 );

    int sum = 0;
    SM::forEach([&](int i) { sum += i; },arr);

    int expectedSum = 343 + 686 + 1029 + 1372 + 1715 + 2058 + 2401;

    REQUIRE( sum == expectedSum );
}

TEST_CASE( "distributor_tests_dummy_var_very_functional", "[distributor_tests]" )
{
    TEMPLATIOUS_TRIPLET_STD;

    int a,b,c,d,e,f,g;

    auto r = SM::distribute(
        SF::seqI(1,77),
        SF::pack(
            a,
            SF::dummyVar(),
            b,
            SF::dummyVar<2>(),
            c,
            SF::dummyVar<3>(),
            d,
            SF::dummyVar<4>(),
            e,
            SF::dummyVar<5>(),
            f,
            SF::dummyVar<6>(),
            g
        )
    );

    REQUIRE( r == 7 + 1 + 2 + 3 + 4 + 5 + 6 );

    int sum = 0;
    auto fs = SF::storageFunctor<Sum>(sum);

    SM::callEach(fs,a,b,c,d,e,f,g);

    REQUIRE( sum == 84 );
}

