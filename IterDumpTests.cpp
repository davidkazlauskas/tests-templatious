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

#include "TestDefs.hpp"

TEST_CASE( "iter_dump_tests_basic", "[iter_dump_tests]" )
{

    auto v = std::vector<int>();
    SA::add(v,1,2,3);
    auto d = SM::iterDump(v);

    REQUIRE( 6 == SM::sum<int>(d) );
}

TEST_CASE( "iter_dump_tests_mutate", "[iter_dump_tests]" )
{

    auto v = std::vector<int>();
    SA::add(v,1,2,3);
    auto d = SM::iterDump(v);
    SM::set(7,d);

    auto sd = SM::sum<int>(d);
    auto sv = SM::sum<int>(v);

    REQUIRE( 21 == sd );
    REQUIRE( sd == sv );
}

TEST_CASE( "iter_dump_tests_clearance", "[iter_dump_tests]" )
{

    auto v = std::vector<int>();
    SA::add(v,1,2,3);
    auto d = SM::iterDump(v);

    // clearing iter dump shouldn't mutate original
    SA::clear(d);

    REQUIRE( SA::size(d) == 0 );
    REQUIRE( SA::size(v) == 3 );
    REQUIRE( SM::sum<int>(v) == 6 );
}

TEST_CASE( "iter_dump_tests_sort", "[iter_dump_tests]" )
{
    auto v = std::vector<int>();
    SA::add(v,3,2,1);
    auto d = SM::iterDump(v);

    std::sort(SA::begin(d),SA::end(d));

    auto b = SA::begin(d);

    REQUIRE( *b == 1 );
    ++b;
    REQUIRE( *b == 2 );
    ++b;
    REQUIRE( *b == 3 );

    templatious::detail::VectoratorAlgs
        ::vectoratorSort(v,d);

    auto b2 = SA::begin(v);
    REQUIRE( *b2 == 1 );
    ++b2;
    REQUIRE( *b2 == 2 );
    ++b2;
    REQUIRE( *b2 == 3 );
}

TEST_CASE( "iter_dump_tests_sort_hard", "[iter_dump_tests]" )
{
    auto v = std::vector<int>();
    SA::add(v,6,3,4,5,2,7,1);
    auto d = SM::iterDump(v);

    std::sort(SA::begin(d),SA::end(d));

    auto b = SA::begin(d);

    REQUIRE( *b == 1 );
    ++b;
    REQUIRE( *b == 2 );
    ++b;
    REQUIRE( *b == 3 );
    ++b;
    REQUIRE( *b == 4 );
    ++b;
    REQUIRE( *b == 5 );
    ++b;
    REQUIRE( *b == 6 );
    ++b;
    REQUIRE( *b == 7 );

    templatious::detail::VectoratorAlgs
        ::vectoratorSort(v,d);
}

TEST_CASE( "iter_dump_tests_sort_random", "[iter_dump_tests]" )
{
    auto v = std::vector<int>();
    SA::add(v,4,0,5,1,5,2,4,6,6,0);
    // 0,0,1,2,4,4,5,5,6,6
    auto d = SM::iterDump(v);

    std::sort(SA::begin(d),SA::end(d));

    auto b = SA::begin(d);

    REQUIRE( *b == 0 );
    ++b;
    REQUIRE( *b == 0 );
    ++b;
    REQUIRE( *b == 1 );
    ++b;
    REQUIRE( *b == 2 );
    ++b;
    REQUIRE( *b == 4 );
    ++b;
    REQUIRE( *b == 4 );
    ++b;
    REQUIRE( *b == 5 );
    ++b;
    REQUIRE( *b == 5 );
    ++b;
    REQUIRE( *b == 6 );
    ++b;
    REQUIRE( *b == 6 );

    *SA::end(v) = 7;
    templatious::detail::VectoratorAlgs
        ::vectoratorSort(v,d);
}

TEST_CASE( "iter_dump_tests_sort_stress", "[iter_dump_tests]" )
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
        REQUIRE( SM::areCollectionsEqual(v,v2) );
        REQUIRE( SM::areCollectionsEqual(v,v3) );

        std::sort(SA::begin(v2),SA::end(v2));

        auto d = SM::iterDump(v3);
        std::sort(SA::begin(d),SA::end(d));
        // good for debugging
        //*SA::end(v3) = 777;
        templatious::detail::VectoratorAlgs
            ::vectoratorSort(v3,d);
        REQUIRE( SM::areCollectionsEqual(v2,v3) );
        REQUIRE( SM::sum<int>(v2) == sum );
        REQUIRE( SM::sum<int>(v3) == sum );

    }
}

// sort non-random access iter
TEST_CASE( "iter_dump_tests_sort_list", "[iter_dump_tests]" ) {
    std::list<int> l;
    std::vector<int> vcpy;

    SA::add(l,SF::seqI(100,1));
    SA::add(vcpy,l);

    REQUIRE( SM::areCollectionsEqual(vcpy,l) );
    REQUIRE( !SM::isSorted(l) );

    int tot = SM::sum<int>(l);

    auto flt = SF::filter(l,[](int i) { return i % 5 == 0; });
    int sum = SM::sum<int>(flt);

    REQUIRE( !SM::isSorted(flt) );

    SM::sort(flt);

    REQUIRE( SM::isSorted(flt) );
    REQUIRE( !SM::isSorted(l) );
    REQUIRE( !SM::areCollectionsEqual(vcpy,l) );

    REQUIRE( sum == SM::sum<int>(flt) );
    REQUIRE( tot == SM::sum<int>(l) );
}

TEST_CASE( "iter_dump_tests_sort_raw", "[iter_dump_tests]" ) {
    std::vector<int> v;

    SA::add(v,SF::seqI(100,1));

    REQUIRE( !SM::isSorted(v) );

    SM::sort(v);

    REQUIRE( SM::isSorted(v) );
}

TEST_CASE( "iter_dump_tests_sort_manual", "[iter_dump_tests]" ) {
    std::list<int> l;

    SA::add(l,7,6,5,4,3,2,1);
    SM::sort(SF::skip(l,3));

    REQUIRE( SM::areCollectionsEqual(l,
        (int[]){1,6,5,4,3,2,7}) );

    SM::sort(SF::range(l,1,3));

    REQUIRE( SM::areCollectionsEqual(l,
        (int[]){1,5,6,4,3,2,7}) );

    SM::sort(
        SF::skip(
            SF::filter(l,[](int i) { return i > 2; }),
            2
        )
    );

    REQUIRE( SM::areCollectionsEqual(l,
        (int[]){1,4,6,5,3,2,7}) );
}

TEST_CASE( "iter_dump_tests_sort_empty", "[iter_dump_tests]" ) {
    // sort shouldn't crash with 0 or 1 elements.
    // and shouldn't alter collection
    std::list<int> l;

    REQUIRE( SA::size(l) == 0 );
    SM::sort(l);
    REQUIRE( SA::size(l) == 0 );

    SA::add(l,7);
    std::vector<int> v;
    SA::add(v,l);

    REQUIRE( SA::size(l) == 1 );
    SM::sort(l);
    REQUIRE( SM::areCollectionsEqual(v,l) );
    REQUIRE( SA::size(l) == 1 );
}

