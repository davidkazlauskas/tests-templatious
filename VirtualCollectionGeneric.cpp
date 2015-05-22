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
    return res;
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
    auto vl1 = SF::vcollection(l);
    auto vl2 = SF::vcollection(l);

    REQUIRE( vv1 == vv2 );
    REQUIRE( vv1 != vl1 );

    vv1 = std::move(vl1);
    REQUIRE( vv1 == vl2 );
    REQUIRE( vl1 != vl2 );
    REQUIRE( vl2 != vl1 );
}

TEST_CASE( "vcollection_concurrent_wdtor", "[virtual_collection]" )
{
    std::vector<int>* v = new std::vector<int>();
    {
        auto vv = SF::vcollectionWDtor(*v,[&]() {
            delete v;
            v = nullptr;
        });

        SA::add(vv,1,2,3,4,5,6,7);

        REQUIRE( SA::size(vv) == 7 );
        REQUIRE( SA::getByIndex(vv,6) == 7 );

        typedef const templatious::VCollection<int> CVCol;
        CVCol& cvref(vv);

        long expSum = SM::sum<int>(1,2,3,4,5,6,7);
        typedef templatious::adapters::
            CollectionAdapter< CVCol > Ad;
        auto cbeg = Ad::cbegin(cvref);
        auto cend = Ad::cend(cvref);
        long sum = 0;
        for (; cbeg != cend; ++cbeg) {
            sum += *cbeg;
        }
        REQUIRE( sum == expSum );
    }
    REQUIRE( v == nullptr );
}

TEST_CASE( "vcollection_access", "[virtual_collection]" )
{
    std::vector<int> v;
    SA::add(v,SF::seqL(7));

    auto vc = SF::prevent<0>(v);

    REQUIRE( vc.first() == 0 );
    REQUIRE( vc.last() == 6 );
    REQUIRE( vc.cfirst() == 0 );
    REQUIRE( vc.clast() == 6 );
    REQUIRE( vc.getByIndex(1) == 1 );
    REQUIRE( vc.cgetByIndex(1) == 1 );

    auto vct = SF::vcollectionCustom<
        templatious::AP_THROW,
        templatious::CP_THROW,
        templatious::TP_ENABLED,
        templatious::ACP_THROW,
        templatious::SP_THROW
    >(v);

    long sum = 0;
    const decltype(vct)& vctRef(vct);
    for (auto i = vctRef.cbegin(); i != vctRef.cend(); ++i) {
        sum += *i;
    }
    REQUIRE( sum == 1 + 2 + 3 + 4 + 5 + 6 );
}

TEST_CASE( "vcollection_adddable_fake_canadd", "[virtual_collection]" )
{
    std::vector<int> v;

    auto vct = SF::vcollectionCustom<
        templatious::AP_FAKE,
        templatious::CP_THROW,
        templatious::TP_ENABLED,
        templatious::ACP_THROW,
        templatious::SP_ENABLED
    >(v);

    REQUIRE( !vct.canAdd() );
}

TEST_CASE( "vcollection_traversable_test", "[virtual_collection]" )
{
    std::vector<int> v;
    SA::add(v,SF::seqL(7));

    auto vctA = SF::vcollectionCustom<
        templatious::AP_FAKE,
        templatious::CP_THROW,
        templatious::TP_ENABLED,
        templatious::ACP_THROW,
        templatious::SP_ENABLED
    >(v);

    auto vctB = SF::vcollectionCustom<
        templatious::AP_FAKE,
        templatious::CP_THROW,
        templatious::TP_FAKE,
        templatious::ACP_THROW,
        templatious::SP_ENABLED
    >(v);

    REQUIRE( *vctA.iterAt(6) == 6 );
    REQUIRE( *vctA.citerAt(6) == 6 );

    REQUIRE( vctB.iterAt(6) == vctB.end() );
    REQUIRE( vctB.citerAt(6) == vctB.cend() );
}
