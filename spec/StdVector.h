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

#include "../TestAlgs.h"


BOOST_AUTO_TEST_CASE( std_vector_tests )
{

    typedef std::vector<int> V;
    V v;

    BOOST_CHECK(tt::existantCollectionTest(v));
}

BOOST_AUTO_TEST_CASE( std_vector_tests_proxy )
{
    typedef std::vector<int> V;
    V v;

    BOOST_CHECK(tt::proxyTest(v));
}

BOOST_AUTO_TEST_CASE( std_vector_tests_proxy_const )
{
    typedef std::vector<int> V;
    V v;

    BOOST_CHECK(tt::constProxyTest(v));
}

BOOST_AUTO_TEST_CASE( std_vector_tests_proxy_clearance )
{
    typedef std::vector<int> V;
    V v;

    BOOST_CHECK(tt::proxyDeletionTest(v));
}

BOOST_AUTO_TEST_CASE( std_vector_tests_virtual )
{
    typedef std::vector<int> V;
    V v;

    BOOST_CHECK(tt::virtualTest(v));
}

#endif /* end of include guard: STDVECTOR_97AM89C */

