/*
 * =====================================================================================
 *
 *       Filename:  VirtualCollectionGeneric.cpp
 *
 *    Description:  Generic applicable to all
 *
 *        Version:  1.0
 *        Created:  2015.03.19 17:43:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#include "TestAlgs.hpp"

std::vector<int> rvalVect() {
    std::vector<int> res;
    SA::add(res,1,2,3,4,5,6,7);
    return std::move(res);
}

TEST_CASE( "virtual_collection_move_semantics", "[virtual_collection]" ) {
    int sum = 1 + 2 + 3 + 4 + 5 + 6 + 7;

    auto vc = SF::vcollection(rvalVect());
    REQUIRE( SM::sum<int>(vc) == sum );
}

TEST_CASE( "virtual_collection_move_semantics_existing", "[virtual_collection]" ) {
    int sum = 1 + 2 + 3 + 4 + 5 + 6 + 7;

    std::vector<int> v;
    SA::add(v,1,2,3,4,5,6,7);

    REQUIRE( SM::sum<int>(v) == sum );

    { // BASIC
        auto vc = SF::vcollection(v);

        REQUIRE( SM::sum<int>(v) == sum );
        REQUIRE( SM::sum<int>(vc) == sum );
    }

    { // MUTATION, REFERENCE SAME
        auto vc = SF::vcollection(v);

        SA::erase(v,SA::begin(v));

        REQUIRE( SM::sum<int>(vc) == sum - 1 );

        SA::insert(vc,SA::begin(vc),1);
        REQUIRE( SM::sum<int>(v) == sum );
        REQUIRE( SM::areCollectionsEqual(v,SF::seqI(1,7)) );
    }

    { // THE MOVE
        auto vc = SF::vcollection(std::move(v));

        REQUIRE( SM::sum<int>(v) == 0 );
        REQUIRE( SM::sum<int>(vc) == sum );
    }
}

TEST_CASE( "virtual_collection_move_add", "[virtual_collection]" ) {
    std::vector<MovablePod> v;

    auto vc = SF::vcollection(v);

    REQUIRE( moveTest(vc) );
}

TEST_CASE( "virtual_collection_custom_destructor_rval", "[virtual_collection]" ) {
    std::vector< int > v;
    SA::add(v,1,2,3,4,5,6,7);

    {
        auto h = SF::vcollectionCustomWDtor(
            v,
            [&]() {
                SA::clear(v);
            }
        );
    }

    REQUIRE( SA::size(v) == 0 );
}

TEST_CASE( "virtual_collection_custom_destructor_lval", "[virtual_collection]" ) {
    std::vector< int > v;
    SA::add(v,1,2,3,4,5,6,7);

    auto lval =
        [&]() {
            SA::clear(v);
        };
    {
        auto h = SF::vcollectionCustomWDtor(
            v,
            lval
        );
    }

    REQUIRE( SA::size(v) == 0 );
}

TEST_CASE( "virtual_collection_custom_destructor_cval", "[virtual_collection]" ) {
    std::vector< int > v;
    SA::add(v,1,2,3,4,5,6,7);

    auto lval =
        [&]() {
            SA::clear(v);
        };
    {
        auto h = SF::vcollectionCustomWDtor(
            v,
            static_cast<const decltype(lval)&>(lval)
        );
    }

    REQUIRE( SA::size(v) == 0 );
}

TEST_CASE( "virtual_collection_destructor_rval", "[virtual_collection]" ) {
    std::vector< int > v;
    SA::add(v,1,2,3,4,5,6,7);

    {
        auto h = SF::vcollectionWDtor(
            v,
            [&]() {
                SA::clear(v);
            }
        );
    }

    REQUIRE( SA::size(v) == 0 );
}

TEST_CASE( "virtual_collection_destructor_lval", "[virtual_collection]" ) {
    std::vector< int > v;
    SA::add(v,1,2,3,4,5,6,7);

    auto lval =
        [&]() {
            SA::clear(v);
        };
    {
        auto h = SF::vcollectionWDtor(
            v,
            lval
        );
    }

    REQUIRE( SA::size(v) == 0 );
}

TEST_CASE( "virtual_collection_destructor_cval", "[virtual_collection]" ) {
    std::vector< int > v;
    SA::add(v,1,2,3,4,5,6,7);

    auto lval =
        [&]() {
            SA::clear(v);
        };
    {
        auto h = SF::vcollectionWDtor(
            v,
            static_cast<const decltype(lval)&>(lval)
        );
    }

    REQUIRE( SA::size(v) == 0 );
}
