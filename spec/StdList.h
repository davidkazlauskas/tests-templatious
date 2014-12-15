/*
 * =====================================================================================
 *
 *       Filename:  StdList.h
 *
 *    Description:  List unit test
 *
 *        Version:  1.0
 *        Created:  07/27/2014 09:09:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef STDLIST_UDCMQSLF
#define STDLIST_UDCMQSLF

#include "../TestAlgs.h"

BOOST_AUTO_TEST_SUITE( std_list_tests );

BOOST_AUTO_TEST_CASE( std_list_tests_basic )
{
    typedef std::list<int> V;
    V v;

    BOOST_CHECK(tt::existantCollectionTest(v));
}

BOOST_AUTO_TEST_CASE( std_list_tests_proxy )
{
    typedef std::list<int> V;
    V v;

    BOOST_CHECK(tt::proxyTest(v));
}

BOOST_AUTO_TEST_CASE( std_list_tests_proxy_const )
{
    typedef std::list<int> V;
    V v;

    BOOST_CHECK(tt::constProxyTest(v));
}

BOOST_AUTO_TEST_CASE( std_list_tests_proxy_clearance )
{
    typedef std::list<int> V;
    V v;

    BOOST_CHECK(tt::proxyDeletionTest(v));
}

BOOST_AUTO_TEST_CASE( std_list_tests_virtual )
{
    typedef std::list<int> V;
    V v;

    BOOST_CHECK(tt::virtualTest(v));
}

BOOST_AUTO_TEST_CASE( std_list_tests_destruction )
{
    struct UniqueToken {};
    typedef tt::ConstructorCountCollection<UniqueToken> ValType;
    typedef std::list<ValType> V;
    V v;

    BOOST_CHECK(tt::constructionCountCollectionTest<UniqueToken>(v));
    BOOST_CHECK(ValType::count() == 0);
}

BOOST_AUTO_TEST_SUITE_END();

#endif /* end of include guard: STDLIST_UDCMQSLF */
