/*
 * =====================================================================================
 *
 *       Filename:  PackTests.h
 *
 *    Description:  test for packs
 *
 *        Version:  1.0
 *        Created:  10/13/2014 08:40:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef PACKTESTS_1UNIFFZN
#define PACKTESTS_1UNIFFZN

#include <sstream>

#include "TestDefs.h"

BOOST_AUTO_TEST_SUITE( pack_tests )

BOOST_AUTO_TEST_CASE( pack_basic_test )
{
    TEMPLATIOUS_TRIPLET_STD;

    std::stringstream ss;

    auto p =
        SF::pack(
            SF::packInsert(
                SF::pack(1,2,3,4),
                " "),
            "\n"
        );

    auto f = SF::streamFunctor(ss);

    SM::callEach(f,p);

    std::string expected = "1 2 3 4\n";
    BOOST_CHECK( ss.str() == expected );
}

TEMPLATIOUS_CALLEACH_FCTOR( Square, i *= i );

BOOST_AUTO_TEST_CASE( pack_mutation )
{
    TEMPLATIOUS_TRIPLET_STD;

    int a = 3;
    int b = 4;

    auto p = SF::pack(a,b);

    SM::callEach(Square(),p);

    BOOST_CHECK( a + b == 25 );
}

BOOST_AUTO_TEST_CASE( pack_match_fctor_diff )
{
    TEMPLATIOUS_TRIPLET_STD;

    int a = 7;
    char b = '7';
    long c = 77;
    short d = 777;
    double e = 7.7;
    float f = 0.77;

    auto p = SF::pack(a,b,c,d,e,f);

    int mul = 1;

    auto func =
            SF::matchFunctor(
                SF::matchLoose<int>([&](int i) { mul *= 2; }),
                SF::matchLoose<char>([&](char i) { mul *= 3; }),
                SF::matchLoose<long>([&](long i) { mul *= 5; }),
                SF::matchLoose<short>([&](short i) { mul *= 7; }),
                SF::matchLoose<double>([&](double i) { mul *= 11; }),
                SF::matchLoose<float>([&](float i) { mul *= 13; })
            );

    SM::callEach(func,p);

    BOOST_CHECK( mul == 30030 );
}

TEMPLATIOUS_CALLEACH_FCTOR_WSTOR( SumAll, _c += i );

BOOST_AUTO_TEST_CASE( pack_integrity )
{
    TEMPLATIOUS_TRIPLET_STD;

    int a = 7;
    char b = '7';
    long c = 77;
    short d = 777;
    double e = 7.7; // will be truncated to 7
    float f = 0.77; // will be truncated to 0

    auto p = SF::pack(a,b,c,d,e,f);

    int sum = 0;

    auto func = SumAll<int&>(sum);

    SM::callEach(func,p);

    BOOST_CHECK( sum == 923 );
}

BOOST_AUTO_TEST_CASE( pack_integrity_shared_stor )
{
    TEMPLATIOUS_TRIPLET_STD;

    int a = 7;
    char b = '7';
    long c = 77;
    short d = 777;
    double e = 7.7; // will be truncated to 7
    float f = 0.77; // will be truncated to 0

    auto p = SF::pack(a,b,c,d,e,f);

    int sum = 0;

    auto func =
            SF::matchFunctor(
                SF::matchLoose<int>(SF::storageFunctor<SumAll>(sum)),
                SF::matchLoose<char>(SF::storageFunctor<SumAll>(sum)),
                SF::matchLoose<long>(SF::storageFunctor<SumAll>(sum)),
                SF::matchLoose<short>(SF::storageFunctor<SumAll>(sum)),
                SF::matchLoose<double>(SF::storageFunctor<SumAll>(sum)),
                SF::matchLoose<float>(SF::storageFunctor<SumAll>(sum))
            );


    SM::callEach(func,p);

    BOOST_CHECK( sum == 923 );
}

BOOST_AUTO_TEST_CASE( pack_repeat_test_lval )
{
    TEMPLATIOUS_TRIPLET_STD;

    int a = 5;

    int sum = 0;

    auto func = SF::storageFunctor<SumAll>(sum);
    auto rpt = SF::packRepeat<5>(a);

    SM::callEach(func,rpt);

    BOOST_CHECK( sum == 25 );
}

BOOST_AUTO_TEST_CASE( pack_repeat_test_lval_mul )
{
    TEMPLATIOUS_TRIPLET_STD;

    int a = 5;
    short b = 7;

    int sum = 0;

    auto func = SF::storageFunctor<SumAll>(sum);
    auto rpt = SF::packRepeat<5>(a,b);

    SM::callEach(func,rpt);

    BOOST_CHECK( sum == 60 );
}

BOOST_AUTO_TEST_CASE( pack_repeat_test_rval_mul )
{
    TEMPLATIOUS_TRIPLET_STD;

    int sum = 0;

    auto func = SF::storageFunctor<SumAll>(sum);
    auto rpt = SF::packRepeat<5>(5,7);

    SM::callEach(func,rpt);

    BOOST_CHECK( sum == 60 );
}

TEMPLATIOUS_CALLEACH_FCTOR_WSTOR( Sum, _c += i );
BOOST_AUTO_TEST_CASE( pack_storage_semantics_move )
{
    TEMPLATIOUS_TRIPLET_STD;

    std::vector<int> i;
    SA::add(i,7,7,7,7,7,7,7);

    int sum = 0;
    auto fs = SF::storageFunctor<Sum>(sum);

    SM::forEach(fs,i);

    BOOST_CHECK( sum == 7 * 7 );

    auto p = SF::pack(std::move(i));

    sum = 0;
    SM::forEach(fs,i);

    BOOST_CHECK( sum == 0 );

    sum = 0;
    SM::forEach(fs,p.get<0>());

    BOOST_CHECK( sum == 7 * 7 );
    BOOST_CHECK( std::addressof(p.get<0>()) != std::addressof(i) );
}

BOOST_AUTO_TEST_CASE( pack_storage_semantics_address )
{
    TEMPLATIOUS_TRIPLET_STD;

    int a,b,c,d,e,f,g;
    auto p = SF::pack(a,b,c,d,e,f,g);

    BOOST_CHECK( std::addressof(p.get<0>()) == &a );
    BOOST_CHECK( std::addressof(p.get<1>()) == &b );
    BOOST_CHECK( std::addressof(p.get<2>()) == &c );
    BOOST_CHECK( std::addressof(p.get<3>()) == &d );
    BOOST_CHECK( std::addressof(p.get<4>()) == &e );
    BOOST_CHECK( std::addressof(p.get<5>()) == &f );
    BOOST_CHECK( std::addressof(p.get<6>()) == &g );
}

BOOST_AUTO_TEST_SUITE_END()

#endif /* end of include guard: PACKTESTS_1UNIFFZN */

