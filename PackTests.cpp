/*
 * =====================================================================================
 *
 *       Filename:  PackTests.hpp
 *
 *    Description:  test for packs
 *
 *        Version:  1.0
 *        Created:  10/13/2014 08:40:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Kazlauskas (dk), david@templatious.org
 *
 * =====================================================================================
 */

#include <sstream>

#include "TestDefs.hpp"

TEST_CASE( "pack_basic_test", "[pack_tests]" )
{
    std::stringstream ss;

    auto p =
        SF::pack(
            SF::packInsert(
                SF::pack(1,2,3,4),
                " "),
            "\n"
        );

    auto f = SF::streamOutFunctor(ss);

    SM::callEach(f,p);

    std::string expected = "1 2 3 4\n";
    REQUIRE( ss.str() == expected );
}

TEMPLATIOUS_CALLEACH_FCTOR( Square, i *= i );

TEST_CASE( "pack_mutation", "[pack_tests]" )
{
    int a = 3;
    int b = 4;

    auto p = SF::pack(a,b);

    SM::callEach(Square(),p);

    auto apb = a + b;
    REQUIRE( apb == 25 );
}

TEST_CASE( "pack_match_fctor_diff", "[pack_tests]" )
{
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

    REQUIRE( mul == 30030 );
}

TEST_CASE( "pack_integrity", "[pack_tests]" )
{
    int a = 7;
    char b = '7';
    long c = 77;
    short d = 777;
    double e = 7.7; // will be truncated to 7
    float f = 0.77; // will be truncated to 0

    auto p = SF::pack(a,b,c,d,e,f);

    int sum = 0;

    auto func = Sum<int&>(sum);

    SM::callEach(func,p);

    REQUIRE( sum == 923 );
}

TEST_CASE( "pack_integrity_shared_stor", "[pack_tests]" )
{
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
                SF::matchLoose<int>(SF::storageFunctor<Sum>(sum)),
                SF::matchLoose<char>(SF::storageFunctor<Sum>(sum)),
                SF::matchLoose<long>(SF::storageFunctor<Sum>(sum)),
                SF::matchLoose<short>(SF::storageFunctor<Sum>(sum)),
                SF::matchLoose<double>(SF::storageFunctor<Sum>(sum)),
                SF::matchLoose<float>(SF::storageFunctor<Sum>(sum))
            );


    SM::callEach(func,p);

    REQUIRE( sum == 923 );
}

TEST_CASE( "pack_repeat_test_lval", "[pack_tests]" )
{
    int a = 5;

    int sum = 0;

    auto func = SF::storageFunctor<Sum>(sum);
    auto rpt = SF::packRepeat<5>(a);

    SM::callEach(func,rpt);

    REQUIRE( sum == 25 );
}

TEST_CASE( "pack_repeat_test_lval_mul", "[pack_tests]" )
{
    int a = 5;
    short b = 7;

    int sum = 0;

    auto func = SF::storageFunctor<Sum>(sum);
    auto rpt = SF::packRepeat<5>(a,b);

    SM::callEach(func,rpt);

    REQUIRE( sum == 60 );
}

TEST_CASE( "pack_repeat_test_rval_mul", "[pack_tests]" )
{
    int sum = 0;

    auto func = SF::storageFunctor<Sum>(sum);
    auto rpt = SF::packRepeat<5>(5,7);

    SM::callEach(func,rpt);

    REQUIRE( sum == 60 );
}

TEST_CASE( "pack_storage_semantics_move", "[pack_tests]" )
{
    std::vector<int> i;
    SA::add(i,7,7,7,7,7,7,7);

    int sum = 0;
    auto fs = SF::storageFunctor<Sum>(sum);

    SM::forEach(fs,i);

    REQUIRE( sum == 7 * 7 );

    auto p = SF::pack(std::move(i));

    sum = 0;
    SM::forEach(fs,i);

    REQUIRE( sum == 0 );

    sum = 0;
    SM::forEach(fs,p.get<0>());

    REQUIRE( sum == 7 * 7 );
    REQUIRE( std::addressof(p.get<0>()) != std::addressof(i) );
}

TEST_CASE( "pack_storage_semantics_address", "[pack_tests]" )
{
    int a,b,c,d,e,f,g;
    auto p = SF::pack(a,b,c,d,e,f,g);

    REQUIRE( std::addressof(p.get<0>()) == &a );
    REQUIRE( std::addressof(p.get<1>()) == &b );
    REQUIRE( std::addressof(p.get<2>()) == &c );
    REQUIRE( std::addressof(p.get<3>()) == &d );
    REQUIRE( std::addressof(p.get<4>()) == &e );
    REQUIRE( std::addressof(p.get<5>()) == &f );
    REQUIRE( std::addressof(p.get<6>()) == &g );
}

TEST_CASE( "pack_storage_semantics_const_address", "[pack_tests]" )
{
    const int a = 7;
    const int b = 7;
    const int c = 7;
    const int d = 7;
    const int e = 7;
    const int f = 7;
    const int g = 7;

    auto p = SF::pack(a,b,c,d,e,f,g);

    REQUIRE( std::addressof(p.get<0>()) == &a );
    REQUIRE( std::addressof(p.get<1>()) == &b );
    REQUIRE( std::addressof(p.get<2>()) == &c );
    REQUIRE( std::addressof(p.get<3>()) == &d );
    REQUIRE( std::addressof(p.get<4>()) == &e );
    REQUIRE( std::addressof(p.get<5>()) == &f );
    REQUIRE( std::addressof(p.get<6>()) == &g );
}

TEST_CASE( "pack_function_mutation", "[pack_tests]" )
{
    int a,b,c,d,e,f,g;
    auto p = SF::pack(a,b,c,d,e,f,g);

    auto rq = [](int aa,int bb,int cc,int dd,int ee,int ff,int gg) {
        return aa + 2 * bb + 3 * cc + 4 * dd + 5 * ee + 6 * ff + 7 * gg;
    };

    auto fnFct = SF::packFunctor(rq,p);

    SM::set(0,a,b,c,d,e,f,g); // set without pack to be sure

    REQUIRE( fnFct() == 0 * 7 );
    a = 7;
    REQUIRE( fnFct() == 1 * 7 );
    b = 7;
    REQUIRE( fnFct() == 3 * 7 );
    c = 7;
    REQUIRE( fnFct() == 6 * 7 );
    d = 7;
    REQUIRE( fnFct() == 10 * 7 );
    e = 7;
    REQUIRE( fnFct() == 15 * 7 );
    f = 7;
    REQUIRE( fnFct() == 21 * 7 );
    g = 7;
    REQUIRE( fnFct() == 28 * 7 );
}

TEST_CASE( "pack_transform", "[pack_tests]" )
{
    int a,b,c,d,e,f,g;
    auto p = SF::pack(a,b,c,d,e,f,g);
    SM::set(7,p);

    auto trFunc = [](int i) { return SF::pack(int(i),int(i)); };
    auto trP = SF::packTransformWithin(trFunc,p);

    REQUIRE( SM::sum(p) == 7 * 7 );
    REQUIRE( SM::sum(trP) == 7 * 7 * 2 );
}

TEST_CASE( "pack_transform_moar", "[pack_tests]" )
{
    int a,b,c,d,e,f,g;
    auto p = SF::pack(a,b,c,d,e,f,g);
    SM::set(7,p);

    auto trFunc = [](int i) {
        std::stringstream s;
        s << ">";
        s << i;
        s << "<";

        std::string res = s.str();
        return res;
    };
    auto trP = SF::packTransformWithin(trFunc,p);

    std::stringstream ss;
    auto sf = SF::streamOutFunctor(ss);
    SM::callEach(sf,trP);

    auto res = ss.str();
    REQUIRE( res == ">7<>7<>7<>7<>7<>7<>7<" );
}

