/*
 * =====================================================================================
 *
 *       Filename:  StaticVector.h
 *
 *    Description:  Static vector test
 *
 *        Version:  1.0
 *        Created:  07/28/2014 04:49:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef STATICVECTOR_8L32QS9F
#define STATICVECTOR_8L32QS9F

#include "../TestAlgs.h"

BOOST_AUTO_TEST_CASE( static_vector_tests )
{

    const size_t SIZE = 256; 
    size_t arr[SIZE];
    auto v = tt::t::makeStaticVector(arr);

    BOOST_CHECK(tt::existantCollectionTest(v));
}

BOOST_AUTO_TEST_CASE( static_vector_tests_proxy )
{

    const size_t SIZE = 256;
    size_t arr[SIZE];
    auto v = tt::t::makeStaticVector(arr);

    BOOST_CHECK(tt::proxyTest(v));
}


#endif /* end of include guard: STATICVECTOR_8L32QS9F */
