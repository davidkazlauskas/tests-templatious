/*
 * =====================================================================================
 *
 *       Filename:  Sequence.h
 *
 *    Description:  Sequence tests
 *
 *        Version:  1.0
 *        Created:  11/30/2014 02:33:42 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#include "TestDefs.h"

BOOST_AUTO_TEST_SUITE( sequence_tests )

BOOST_AUTO_TEST_CASE( sequence_simple )
{
    TEMPLATIOUS_TRIPLET_STD;

    SUM_VAR(sum,sf);

    auto s = SF::seqI(10);
    SM::forEach(sf,s);

    BOOST_CHECK( sum == 55 );
}

BOOST_AUTO_TEST_SUITE_END()
