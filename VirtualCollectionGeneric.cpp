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

#include "TestDefs.hpp"

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

    {
        auto vc = SF::vcollection(v);

        REQUIRE( SM::sum<int>(v) == sum );
        REQUIRE( SM::sum<int>(vc) == sum );
    }

    { // THE MOVE
        auto vc = SF::vcollection(std::move(v));

        REQUIRE( SM::sum<int>(v) == 0 );
        REQUIRE( SM::sum<int>(vc) == sum );
    }
}
