/*
 * =====================================================================================
 *
 *       Filename:  StdVector.hpp
 *
 *    Description:  deque unit test
 *
 *        Version:  1.0
 *        Created:  07/27/2014 07:27:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Kazlauskas (dk), david@templatious.org
 *
 * =====================================================================================
 */

#include "../TestAlgs.hpp"

TEST_CASE( "std_deque_tests_basic", "[std_deque_tests]" )
{

    typedef std::deque<int> V;
    V v;

    REQUIRE(tt::existantCollectionTest(v));
}

TEST_CASE( "std_deque_tests_proxy", "[std_deque_tests]" )
{
    typedef std::deque<int> V;
    V v;

    REQUIRE(tt::proxyTest(v));
}

TEST_CASE( "std_deque_tests_proxy_const", "[std_deque_tests]" )
{
    typedef std::deque<int> V;
    V v;

    REQUIRE(tt::constProxyTest(v));
}

TEST_CASE( "std_deque_tests_proxy_clearance", "[std_deque_tests]" )
{
    typedef std::deque<int> V;
    V v;

    REQUIRE(tt::proxyDeletionTest(v));
}

TEST_CASE( "std_deque_tests_virtual", "[std_deque_tests]" )
{
    typedef std::deque<int> V;
    V v;

    REQUIRE(tt::virtualTest(v));
}

TEST_CASE( "std_deque_tests_destruction", "[std_deque_tests]" )
{
    struct UniqueToken {};
    typedef tt::ConstructorCountCollection<UniqueToken> ValType;
    typedef std::deque<ValType> V;
    V v;

    REQUIRE(tt::constructionCountCollectionTest<UniqueToken>(v));
    REQUIRE(ValType::count() == 0);
}

TEST_CASE( "std_deque_element_move", "[std_deque_tests]" )
{
    std::deque< MovablePod > v;

    REQUIRE( moveTest(v) );
}

