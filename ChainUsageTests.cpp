/*
 * =====================================================================================
 *
 *       Filename:  ChainUsageTests.hpp
 *
 *    Description:  This header contains various chained usages
 *                  of templatious classes
 *
 *        Version:  1.0
 *        Created:  12/19/2014 05:22:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Kazlauskas (dk), david@templatious.org
 *
 * =====================================================================================
 */

#include "TestDefs.hpp"

TEST_CASE( "chained_usage_tests_varying_addition", "[chained_usage_tests]" )
{
    INIT_BALLER;

    std::vector<int> v;

    { // pack, seq, vars
        SA::clear(v);
        int a,b,c,d,e,f,g;
        auto p = SF::pack(a,b,c,d,e,f,g);
        SM::distribute(SF::seqI(1,7),p);

        SA::add(v,p,SF::seqI(17,27),a,b,c,d,e,f,g);

        sum = 0;
        SM::forEach(sf,v);

        REQUIRE( sum == 28 + 242 + 28 );
    }

    { // pack, vars, seq
        SA::clear(v);
        int a,b,c,d,e,f,g;
        auto p = SF::pack(a,b,c,d,e,f,g);
        SM::distribute(SF::seqI(1,7),p);

        SA::add(v,p,a,b,c,d,e,f,g,SF::seqI(17,27));

        sum = 0;
        SM::forEach(sf,v);

        REQUIRE( sum == 28 + 242 + 28 );
    }


    { // vars, pack, seq
        SA::clear(v);
        int a,b,c,d,e,f,g;
        auto p = SF::pack(a,b,c,d,e,f,g);
        SM::distribute(SF::seqI(1,7),p);

        SA::add(v,a,b,c,d,e,f,g,p,SF::seqI(17,27));

        sum = 0;
        SM::forEach(sf,v);

        REQUIRE( sum == 28 + 242 + 28 );
    }

    { // vars, seq, pack
        SA::clear(v);
        int a,b,c,d,e,f,g;
        auto p = SF::pack(a,b,c,d,e,f,g);
        SM::distribute(SF::seqI(1,7),p);

        SA::add(v,a,b,c,d,e,f,g,SF::seqI(17,27),p);

        sum = 0;
        SM::forEach(sf,v);

        REQUIRE( sum == 28 + 242 + 28 );
    }


    { // seq, vars, pack
        SA::clear(v);
        int a,b,c,d,e,f,g;
        auto p = SF::pack(a,b,c,d,e,f,g);
        SM::distribute(SF::seqI(1,7),p);

        SA::add(v,SF::seqI(17,27),a,b,c,d,e,f,g,p);

        sum = 0;
        SM::forEach(sf,v);

        REQUIRE( sum == 28 + 242 + 28 );
    }

    { // seq, pack, vars
        SA::clear(v);
        int a,b,c,d,e,f,g;
        auto p = SF::pack(a,b,c,d,e,f,g);
        SM::distribute(SF::seqI(1,7),p);

        SA::add(v,SF::seqI(17,27),p,a,b,c,d,e,f,g);

        sum = 0;
        SM::forEach(sf,v);

        REQUIRE( sum == 28 + 242 + 28 );
    }


#define DAT_7_PACK SF::pack(a,b,c,d,e,f,g)
    { // RVALUE PACK, pack, seq, vars
        SA::clear(v);
        int a,b,c,d,e,f,g;
        SM::distribute(SF::seqI(1,7),DAT_7_PACK);

        SA::add(v,DAT_7_PACK,SF::seqI(17,27),a,b,c,d,e,f,g);

        sum = 0;
        SM::forEach(sf,v);

        REQUIRE( sum == 28 + 242 + 28 );
    }

}

static auto toDouble =
    SF::matchFunctor(
        SF::matchLoose<std::string>(
            [](const std::string& str) {
                return std::atof(str.c_str());
            }
        ),
        SF::matchLoose<const char*>(
            [](const char* str) {
                return std::atof(str);
            }
        ),
        SF::matchLoose<double>(
            [](double d) {
                return d;
            }
        )
    );

TEST_CASE( "chained_usage_tests_varying_addition_special_match", "[chained_usage_tests]" )
{
    auto p = SF::pack(7.6,"7.7",std::string("7.8"));

    std::vector<double> v;
    SA::addCustom(v,toDouble,p);

    REQUIRE( tt::almostEqual(SA::getByIndex(v,0),7.6) );
    REQUIRE( tt::almostEqual(SA::getByIndex(v,1),7.7) );
    REQUIRE( tt::almostEqual(SA::getByIndex(v,2),7.8) );
}

TEST_CASE( "chained_usage_quadro_proxies", "[chain_usage_tests]" )
{
    std::vector<long> v;
    SA::add(v,SF::seqL(100));

    auto flt = SF::filter(v,[](int i) { return i > 50; });
    auto skip = SF::skip(v,3);
    auto range = SF::range(v,10,20);

    long sum = 0;
    SM::quadro(
        [&](long i,long j,long k) {
            sum += i + j + k;
        },flt,skip,range);

    const int EXPECTED = 2315740;
    REQUIRE( EXPECTED == sum );
    REQUIRE( SM::areCollectionsEqual(v,SF::seqL(100)) );
}

TEST_CASE( "chained_usage_quadro_deep_nest", "[chain_usage_tests]" )
{
    std::vector<long> v;
    SA::add(v,SF::seqL(100));

    auto flt =
        SF::skip(
            SF::range(
                SF::filter(
                    v,
                    [](long i) { return i > 50; }
                ),
                17,37
            ),
            3
        );

    long sum = 0;
    SM::quadro([&](long i,long j) {
            sum += i + j;
        },flt,flt);

    const int EXPECTED = 539 * 7 + 539 * 7;
    REQUIRE( EXPECTED == sum );
}
