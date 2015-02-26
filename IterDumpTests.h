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

BOOST_AUTO_TEST_CASE( iter_dump_tests_mutate )
{

    auto v = std::vector<int>();
    SA::add(v,1,2,3);
    auto d = SF::iterDump(v);
    SM::set(7,d);

    auto sd = SM::sum<int>(d);
    auto sv = SM::sum<int>(v);

    BOOST_CHECK( 21 == sd );
    BOOST_CHECK( sd == sv );
}

BOOST_AUTO_TEST_CASE( iter_dump_tests_clearance )
{

    auto v = std::vector<int>();
    SA::add(v,1,2,3);
    auto d = SF::iterDump(v);

    // clearing iter dump shouldn't mutate original
    SA::clear(d);

    BOOST_CHECK( SA::size(d) == 0 );
    BOOST_CHECK( SA::size(v) == 3 );
    BOOST_CHECK( SM::sum<int>(v) == 6 );
}

template <class T,class U>
void sortStuff(T& t,U& u) {
    auto b = SA::begin(t);
    auto i = b;

    auto bit = SA::begin(u);

    if ((*bit) != i) {
        auto saved = std::move(*i);
        *i = *(*bit);
        while ((*bit).iter() != i) {
            auto dist = std::distance(b,(*bit).iter());
            auto oldBit = bit;
            bit = SA::iterAt(u,dist);
            if ((*bit).iter() != i) {
                *(*oldBit) = std::move(*(*bit));
            } else {
                *(*oldBit) = saved;
            }
        }
        //*(*bit) = saved;
    }
}

BOOST_AUTO_TEST_CASE( iter_dump_tests_sort )
{
    auto v = std::vector<int>();
    SA::add(v,3,2,1);
    auto d = SF::iterDump(v);

    std::sort(SA::begin(d),SA::end(d));

    auto b = SA::begin(d);

    //TEMPLATIOUS_FOREACH( auto i, v ) {
        //std::cout << i << std::endl;
    //}

    BOOST_CHECK( *b == 1 );
    ++b;
    BOOST_CHECK( *b == 2 );
    ++b;
    BOOST_CHECK( *b == 3 );

    sortStuff(v,d);

    auto b2 = SA::begin(v);
    BOOST_CHECK( *b2 == 1 );
    ++b2;
    BOOST_CHECK( *b2 == 2 );
    ++b2;
    BOOST_CHECK( *b2 == 3 );
}

BOOST_AUTO_TEST_CASE( iter_dump_tests_sort_hard )
{
    auto v = std::vector<int>();
    SA::add(v,6,3,4,5,2,7,1);
    auto d = SF::iterDump(v);

    std::sort(SA::begin(d),SA::end(d));

    auto b = SA::begin(d);

    BOOST_CHECK( *b == 1 );
    ++b;
    BOOST_CHECK( *b == 2 );
    ++b;
    BOOST_CHECK( *b == 3 );
    ++b;
    BOOST_CHECK( *b == 4 );
    ++b;
    BOOST_CHECK( *b == 5 );
    ++b;
    BOOST_CHECK( *b == 6 );
    ++b;
    BOOST_CHECK( *b == 7 );

    sortStuff(v,d);

    TEMPLATIOUS_FOREACH( auto i, v ) {
        std::cout << i << std::endl;
    }
}

BOOST_AUTO_TEST_SUITE_END();

#endif /* end of include guard: ITERDUMPTESTS_XJXLMIV5 */
