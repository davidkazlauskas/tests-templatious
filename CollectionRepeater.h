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

TEMPLATIOUS_CALLEACH_FCTOR_WSTOR( Sum, _c += i );
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

BOOST_AUTO_TEST_SUITE_END()

#endif /* end of include guard: COLLECTIONREPEATER_7ZC3F3ES */
