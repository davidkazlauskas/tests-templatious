/*
 * =====================================================================================
 *
 *       Filename:  CollectionRepeater.hpp
 *
 *    Description:  Collection repeater tests
 *
 *        Version:  1.0
 *        Created:  11/06/2014 08:29:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Kazlauskas (dk), david@templatious.org
 *
 * =====================================================================================
 */

#include "TestDefs.hpp"

TEST_CASE( "collection_repeater_abs", "[collection_repeater]" )
{
    auto l = SF::seqI(1,7);

    auto r = SF::repA(17,l);

    int sum = 0;
    auto f = SF::storageFunctor<Sum>(sum);

    SM::forEach(f,r);

    REQUIRE( sum == 62 );
}

TEST_CASE( "collection_repeater_mul", "[collection_repeater]" )
{
    auto l = SF::seqI(1,7);

    auto r = SF::repM(7,l);

    int sum = 0;
    auto f = SF::storageFunctor<Sum>(sum);

    SM::forEach(f,r);

    REQUIRE( sum == 28 * 7 );
}

TEST_CASE( "collection_repeater_rval_epic_oneliner", "[collection_repeater]" )
{
    int sum = 0;

    SM::forEach(
        SF::storageFunctor<Sum>(sum),
        SF::repA(17,SF::seqI(1,7))
    );

    REQUIRE( sum == 62 );
}

TEST_CASE( "collection_repeater_rval_move", "[collection_repeater]" )
{
    std::vector<int> v;
    SA::add(v,SF::seqI(1,7));

    int sum = 0;
    auto f = SF::storageFunctor<Sum>(sum);
    SM::forEach(f,v);
    REQUIRE( sum == 28 );

    auto r = SF::repA(17,std::move(v));
    sum = 0;
    SM::forEach(f,v);
    REQUIRE( sum == 0 );

    sum = 0;
    SM::forEach(f,r);
    REQUIRE( sum == 62 );
}

TEST_CASE( "collection_repeater_item_repeater", "[collection_repeater]" )
{
    std::vector<int> v;
    SA::add(v,SF::repS(7,7));

    int sev = 7;
    auto r = SF::repS(7,sev);
    REQUIRE( SM::sum(v) == 49 );
    REQUIRE( SM::sum(r) == 49 );
    REQUIRE( SM::sum(SF::repS(7,7)) == 49 );
    REQUIRE( SA::size(r) == 7 );
}

TEST_CASE( "collection_repeater_item_repeater_empty", "[collection_repeater]" )
{
    int sev = 7;
    auto r = SF::repS(0,sev);

    REQUIRE( SM::sum(r) == 0 );
    REQUIRE( SM::sum(SF::repS(0,7)) == 0 );
}

TEST_CASE( "collection_repeater_item_repeater_exceptions", "[collection_repeater]" )
{
    auto r = SF::repS(0,7);

    {
        bool caught = false;
        try {
            SA::getByIndex(r,7);
        } catch (const templatious::detail::
            ItemRepeaterOutOfRangeException& e) {
            caught = true;
        }
        REQUIRE( caught );
    }

    {
        bool caught = false;
        try {
            SA::iterAt(r,0);
        } catch (const templatious::detail::
            ItemRepeaterOutOfRangeException& e) {
            caught = true;
        }
        REQUIRE( caught );
    }

    {
        bool caught = false;
        try {
            r.theItem();
        } catch (const templatious::detail::
            ItemRepeaterEmptyException& e) {
            caught = true;
        }
        REQUIRE( caught );
    }

    {
        bool caught = false;
        try {
            SF::repS(-1,0.7);
        } catch (const templatious::detail::
            ItemRepeaterNegativeSizeException& e) {
            caught = true;
        }
        REQUIRE( caught );
    }
}

TEST_CASE( "collection_repeater_copyable_iterator", "[collection_repeater]" )
{
    auto r = SF::repS(2,7);

    int sum = 0;
    SM::quadro([&](int a,int b) {
        sum += a;
        sum += b;
    },r,r);

    REQUIRE( sum == 7 * 2 * 4 );
}

TEST_CASE( "collection_repeater_iter_at", "[collection_repeater]" )
{
    auto r = SF::repS(7,2);

    REQUIRE( *SA::iterAt(r,3) == 2 );
}
