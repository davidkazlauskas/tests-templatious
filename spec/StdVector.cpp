/*
 * =====================================================================================
 *
 *       Filename:  StdVector.hpp
 *
 *    Description:  vector unit test
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

TEST_CASE( "std::vecor_insert_foreign_throw", "[std_vector_tests]" )
{
    std::vector<int> a;
    std::vector<int> b;

    bool caught = false;
    try {
        SA::insert(a,SA::begin(b),7);
    } catch (const tt::t::adapters::CollectionAdapterIteratorOutOfBoundsException& e) {
        caught = true;
    }
    REQUIRE( caught );
}
