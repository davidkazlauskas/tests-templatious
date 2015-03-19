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
    auto vc = SF::vcollection(rvalVect());

    int sum = 1 + 2 + 3 + 4 + 5 + 6 + 7;

    REQUIRE( SM::sum<int>(vc) == sum );
}
