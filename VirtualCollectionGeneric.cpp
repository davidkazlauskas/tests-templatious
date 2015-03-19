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

    //SomePod s(7);
    //SA::add(vc,s);

    //REQUIRE( s._i == 7 );
    //REQUIRE( SA::getByIndex(vc,0)._i == 7 );
    //REQUIRE( std::addressof(SA::getByIndex(vc,0)) != std::addressof(s) );

    //SA::add(vc,std::move(s));
    //REQUIRE( s._i == -7 );
}
