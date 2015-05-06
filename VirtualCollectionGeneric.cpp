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
 *         Author:  David Kazlauskas (dk), david@templatious.org
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

TEST_CASE( "vcollection_allow", "[virtual_collection]" )
{
    std::vector< int > v;

    auto h = SF::allow< tt::t::VCOL_ADD | tt::t::VCOL_TRAVERSE | tt::t::VCOL_SIZE >(v);

    SA::add(h,1,2,3);

    bool areEqual = SM::areCollectionsEqual(h,SF::seqI(1,3));
    REQUIRE( areEqual );
}

TEST_CASE( "vcollection_prevent", "[virtual_collection]" )
{
    std::vector< int > v;

    auto h = SF::prevent< tt::t::VCOL_CLEAR | tt::t::VCOL_ACCESS >(v);

    SA::add(h,1,2,3);

    bool areEqual = SM::areCollectionsEqual(h,SF::seqI(1,3));
    REQUIRE( areEqual );
}

TEST_CASE( "vcollection_no_decrement_iter", "[virtual_collection]" )
{
    std::forward_list<int> fwd;
    SA::add(fwd,SF::seqL(7));

    auto vcol = SF::vcollection(fwd);
    auto iter = SA::begin(vcol);

    bool caught = false;
    try {
        --iter;
    } catch (const tt::t::VirtualIteratorNoDecrementException& e) {
        caught = true;
    }
    REQUIRE( caught );
}

TEST_CASE( "vcollection_decrement_iter", "[virtual_collection]" )
{
    std::vector<int> v;
    SA::add(v,SF::seqL(7));

    auto vcol = SF::vcollection(v);
    auto iter = SA::begin(vcol);

    ++iter;
    REQUIRE( *iter == 1 );
    --iter;
    REQUIRE( *iter == 0 );
}

TEST_CASE( "vcollection_distinct_iterator_comparison", "[virtual_collection]" )
{
    std::vector<int> v;
    std::list<int> l;

    auto vv = SF::vcollection(v);
    auto vl = SF::vcollection(l);

    REQUIRE( SA::begin(vv) != SA::begin(vl) );
}

struct IntPod {
    IntPod(int i) : _i(i) {}
    int _i;
};

TEST_CASE( "vcollection_iter_move", "[virtual_collection]" )
{

    std::vector<IntPod> v;
    SA::add(v,7);
    auto vv = SF::vcollection(v);

    auto beg = SA::begin(vv);
    auto mov = std::move(beg);

    REQUIRE( mov->_i == 7 );
}

TEST_CASE( "vcollection_equality", "[virtual_collection]" )
{
    std::vector<int> v;
    std::list<int> l;

    auto vv1 = SF::vcollection(v);
    auto vv2 = SF::vcollection(v);
    auto vl = SF::vcollection(l);

    REQUIRE( vv1 == vv2 );
    REQUIRE( vv1 != vl );
}
