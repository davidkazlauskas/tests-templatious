/*
 * =====================================================================================
 *
 *       Filename:  StdList.hpp
 *
 *    Description:  List unit test
 *
 *        Version:  1.0
 *        Created:  07/27/2014 09:09:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Kazlauskas (dk), david@templatious.org
 *
 * =====================================================================================
 */

#include "../TestAlgs.hpp"

TEST_CASE( "std_list_tests_basic", "[std_list_tests]" )
{
    typedef std::list<int> V;
    V v;

    REQUIRE(tt::existantCollectionTest(v));
}


TEST_CASE( "std_list_tests_proxy", "[std_list_tests]" )
{
    typedef std::list<int> V;
    V v;

    REQUIRE(tt::proxyTest(v));
}

TEST_CASE( "std_list_tests_proxy_const", "[std_list_tests]" )
{
    typedef std::list<int> V;
    V v;

    REQUIRE(tt::constProxyTest(v));
}

TEST_CASE( "std_list_tests_proxy_clearance", "[std_list_tests]" )
{
    typedef std::list<int> V;
    V v;

    REQUIRE(tt::proxyDeletionTest(v));
}

TEST_CASE( "std_list_tests_virtual", "[std_list_tests]" )
{
    typedef std::list<int> V;
    V v;

    REQUIRE(tt::virtualTest(v));
}

TEST_CASE( "std_list_tests_destruction", "[std_list_tests]" )
{
    struct UniqueToken {};
    typedef tt::ConstructorCountCollection<UniqueToken> ValType;
    typedef std::list<ValType> V;
    V v;

    REQUIRE(tt::constructionCountCollectionTest<UniqueToken>(v));
    REQUIRE(ValType::count() == 0);
}

TEST_CASE( "std_list_element_move", "[std_list_tests]" )
{
    std::list< MovablePod > v;

    REQUIRE( moveTest(v) );
}

