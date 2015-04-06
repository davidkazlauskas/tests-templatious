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
 *
 * =====================================================================================
 */

#include "../TestAlgs.hpp"

TEST_CASE( "std_vector_tests_basic", "[std_vector_tests]" )
{

    typedef std::vector<int> V;
    V v;

    REQUIRE(tt::existantCollectionTest(v));
}

TEST_CASE( "std_vector_tests_proxy", "[std_vector_tests]" )
{
    typedef std::vector<int> V;
    V v;

    REQUIRE(tt::proxyTest(v));
}

TEST_CASE( "std_vector_tests_proxy_const", "[std_vector_tests]" )
{
    typedef std::vector<int> V;
    V v;

    REQUIRE(tt::constProxyTest(v));
}

TEST_CASE( "std_vector_tests_proxy_clearance", "[std_vector_tests]" )
{
    typedef std::vector<int> V;
    V v;

    REQUIRE(tt::proxyDeletionTest(v));
}

TEST_CASE( "std_vector_tests_virtual", "[std_vector_tests]" )
{
    typedef std::vector<int> V;
    V v;

    REQUIRE(tt::virtualTest(v));
}

TEST_CASE( "std_vector_tests_destruction", "[std_vector_tests]" )
{
    struct UniqueToken {};
    typedef tt::ConstructorCountCollection<UniqueToken> ValType;
    typedef std::vector<ValType> V;
    V v;

    REQUIRE(tt::constructionCountCollectionTest<UniqueToken>(v));
    REQUIRE(ValType::count() == 0);
}

TEST_CASE( "std_vector_element_move", "[std_vector_tests]" )
{
    std::vector< MovablePod > v;

    REQUIRE( moveTest(v) );
}

