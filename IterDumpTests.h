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
    auto d = SM::iterDump(v);

    BOOST_CHECK( 6 == SM::sum<int>(d) );
}

BOOST_AUTO_TEST_CASE( iter_dump_tests_mutate )
{

    auto v = std::vector<int>();
    SA::add(v,1,2,3);
    auto d = SM::iterDump(v);
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
    auto d = SM::iterDump(v);

    // clearing iter dump shouldn't mutate original
    SA::clear(d);

    BOOST_CHECK( SA::size(d) == 0 );
    BOOST_CHECK( SA::size(v) == 3 );
    BOOST_CHECK( SM::sum<int>(v) == 6 );
}

BOOST_AUTO_TEST_CASE( iter_dump_tests_sort )
{
    auto v = std::vector<int>();
    SA::add(v,3,2,1);
    auto d = SM::iterDump(v);

    std::sort(SA::begin(d),SA::end(d));

    auto b = SA::begin(d);

    BOOST_CHECK( *b == 1 );
    ++b;
    BOOST_CHECK( *b == 2 );
    ++b;
    BOOST_CHECK( *b == 3 );

    templatious::detail::VectoratorAlgs
        ::vectoratorSort(v,d);

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
    auto d = SM::iterDump(v);

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

    templatious::detail::VectoratorAlgs
        ::vectoratorSort(v,d);
}

BOOST_AUTO_TEST_CASE( iter_dump_tests_sort_random )
{
    auto v = std::vector<int>();
    SA::add(v,4,0,5,1,5,2,4,6,6,0);
    // 0,0,1,2,4,4,5,5,6,6
    auto d = SM::iterDump(v);

    std::sort(SA::begin(d),SA::end(d));

    auto b = SA::begin(d);

    BOOST_CHECK( *b == 0 );
    ++b;
    BOOST_CHECK( *b == 0 );
    ++b;
    BOOST_CHECK( *b == 1 );
    ++b;
    BOOST_CHECK( *b == 2 );
    ++b;
    BOOST_CHECK( *b == 4 );
    ++b;
    BOOST_CHECK( *b == 4 );
    ++b;
    BOOST_CHECK( *b == 5 );
    ++b;
    BOOST_CHECK( *b == 5 );
    ++b;
    BOOST_CHECK( *b == 6 );
    ++b;
    BOOST_CHECK( *b == 6 );

    *SA::end(v) = 7;
    templatious::detail::VectoratorAlgs
        ::vectoratorSort(v,d);
}

BOOST_AUTO_TEST_CASE( iter_dump_tests_sort_stress )
{
    const int TOTAL = 150;
    const int LOWER_BOUND = 1;
    //const int VARIATION = 17;
    const int VARIATION = 100;
    const int ROUNDS = 77;

    srand(777); // deterministic seed

    TEMPLATIOUS_REPEAT(ROUNDS) {

        auto v = std::vector<int>();
        TEMPLATIOUS_REPEAT(TOTAL) {
            int rnd = rand() % VARIATION + LOWER_BOUND;
            SA::add(v,rnd);
        }

        auto sum = SM::sum<int>(v);

        auto v2 = std::vector<int>();
        auto v3 = std::vector<int>();
        SA::add(v2,v);
        SA::add(v3,v);
        BOOST_CHECK( SM::areCollectionsEqual(v,v2) );
        BOOST_CHECK( SM::areCollectionsEqual(v,v3) );

        std::sort(SA::begin(v2),SA::end(v2));

        auto d = SM::iterDump(v3);
        std::sort(SA::begin(d),SA::end(d));
        // good for debugging
        //*SA::end(v3) = 777;
        templatious::detail::VectoratorAlgs
            ::vectoratorSort(v3,d);
        BOOST_CHECK( SM::areCollectionsEqual(v2,v3) );
        BOOST_CHECK( SM::sum<int>(v2) == sum );
        BOOST_CHECK( SM::sum<int>(v3) == sum );

    }
}

// sort non-random access iter
BOOST_AUTO_TEST_CASE( iter_dump_tests_sort_raw ) {
    std::list<int> l;
    std::vector<int> vcpy;

    SA::add(l,SF::seqI(100,1));
    SA::add(vcpy,l);

    BOOST_CHECK( SM::areCollectionsEqual(vcpy,l) );
    BOOST_CHECK( !SM::isSorted(l) );

    int tot = SM::sum<int>(l);

    auto flt = SF::filter(l,[](int i) { return i % 5 == 0; });
    int sum = SM::sum<int>(flt);

    BOOST_CHECK( !SM::isSorted(flt) );

    SM::sort(flt);

    BOOST_CHECK( SM::isSorted(flt) );
    BOOST_CHECK( !SM::isSorted(l) );
    BOOST_CHECK( !SM::areCollectionsEqual(vcpy,l) );

    BOOST_CHECK( sum == SM::sum<int>(flt) );
    BOOST_CHECK( tot == SM::sum<int>(l) );
}

BOOST_AUTO_TEST_SUITE_END();

#endif /* end of include guard: ITERDUMPTESTS_XJXLMIV5 */
