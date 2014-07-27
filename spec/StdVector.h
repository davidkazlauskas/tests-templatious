/*
 * =====================================================================================
 *
 *       Filename:  StdVector.h
 *
 *    Description:  vector unit test
 *
 *        Version:  1.0
 *        Created:  07/27/2014 07:27:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef STDVECTOR_97AM89C
#define STDVECTOR_97AM89C

#include "../CollectionAdapter.h"


BOOST_AUTO_TEST_CASE( std_vector_tests )
{

    typedef std::vector<int> V;
    V v;

    BOOST_CHECK(tt::existantCollectionTest(v));
}

#endif /* end of include guard: STDVECTOR_97AM89C */

