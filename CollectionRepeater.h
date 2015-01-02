/*
 * =====================================================================================
 *
 *       Filename:  CollectionRepeater.h
 *
 *    Description:  Collection repeater tests
 *
 *        Version:  1.0
 *        Created:  11/06/2014 08:29:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef COLLECTIONREPEATER_7ZC3F3ES
#define COLLECTIONREPEATER_7ZC3F3ES

#include "TestDefs.h"

BOOST_AUTO_TEST_SUITE( collection_repeater )

BOOST_AUTO_TEST_CASE( collection_repeater_abs )
{
    TEMPLATIOUS_TRIPLET_STD;

    auto l = SF::seqI(1,7);

    auto r = SF::repA(17,l);

    int sum = 0;
    auto f = SF::storageFunctor<Sum>(sum);

    SM::forEach(f,r);

    BOOST_CHECK( sum == 62 );
}

BOOST_AUTO_TEST_CASE( collection_repeater_mul )
{
    TEMPLATIOUS_TRIPLET_STD;

    auto l = SF::seqI(1,7);

    auto r = SF::repM(7,l);

    int sum = 0;
    auto f = SF::storageFunctor<Sum>(sum);

    SM::forEach(f,r);

    BOOST_CHECK( sum == 28 * 7 );
}

BOOST_AUTO_TEST_CASE( collection_repeater_rval_epic_oneliner )
{
    TEMPLATIOUS_TRIPLET_STD;

    int sum = 0;

    SM::forEach(
        SF::storageFunctor<Sum>(sum),
        SF::repA(17,SF::seqI(1,7))
    );

    BOOST_CHECK( sum == 62 );
}

BOOST_AUTO_TEST_CASE( collection_repeater_rval_move )
{
    TEMPLATIOUS_TRIPLET_STD;

    std::vector<int> v;
    SA::add(v,SF::seqI(1,7));

    int sum = 0;
    auto f = SF::storageFunctor<Sum>(sum);
    SM::forEach(f,v);
    BOOST_CHECK( sum == 28 );

    auto r = SF::repA(17,std::move(v));
    sum = 0;
    SM::forEach(f,v);
    BOOST_CHECK( sum == 0 );

    sum = 0;
    SM::forEach(f,r);
    BOOST_CHECK( sum == 62 );
}

BOOST_AUTO_TEST_CASE( collection_repeater_item_repeater )
{
    TEMPLATIOUS_TRIPLET_STD;

    std::vector<int> v;
    SA::add(v,SF::repS(7,7));

    auto r = SF::repS(7,7);
    BOOST_CHECK( SM::sum(v) == 49 );
    BOOST_CHECK( SM::sum(r) == 49 );
    BOOST_CHECK( SM::sum(SF::repS(7,7)) == 49 );
}

BOOST_AUTO_TEST_CASE( collection_repeater_item_repeater_empty )
{
    auto r = SF::repS(0,7);

    BOOST_CHECK( SM::sum(r) == 0 );
}

BOOST_AUTO_TEST_SUITE_END()

#endif /* end of include guard: COLLECTIONREPEATER_7ZC3F3ES */
