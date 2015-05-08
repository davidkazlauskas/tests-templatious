/*
 * =====================================================================================
 *
 *       Filename:  StdForwardList.hpp
 *
 *    Description:  forward list unit test
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

TEST_CASE( "std_fw_list_tests_basic", "[std_fw_list_tests]" )
{
    typedef std::forward_list<int> V;
    V v;

    REQUIRE(tt::existantCollectionTest(v));
}

TEST_CASE( "std_fw_list_tests_proxy", "[std_fw_list_tests]" )
{
    typedef std::forward_list<int> V;
    V v;

    REQUIRE(tt::proxyTest(v));
}

TEST_CASE( "std_fw_list_tests_proxy_const", "[std_fw_list_tests]" )
{
    typedef std::forward_list<int> V;
    V v;

    REQUIRE(tt::constProxyTest(v));
}

TEST_CASE( "std_fw_list_tests_proxy_clearance", "[std_fw_list_tests]" )
{
    typedef std::forward_list<int> V;
    V v;

    REQUIRE(tt::proxyDeletionTest(v));
}

TEST_CASE( "std_fw_list_tests_virtual", "[std_fw_list_tests]" )
{
    typedef std::forward_list<int> V;
    V v;

    REQUIRE(tt::virtualTest(v));
}

TEST_CASE( "std_fw_list_tests_destruction", "[std_fw_list_tests]" )
{
    struct UniqueToken {};
    typedef tt::ConstructorCountCollection<UniqueToken> ValType;
    typedef std::forward_list<ValType> V;
    V v;

    REQUIRE(tt::constructionCountCollectionTest<UniqueToken>(v));
    REQUIRE(ValType::count() == 0);
}

TEST_CASE( "std_fw_list_element_move", "[std_fw_list_tests]" )
{
    std::forward_list< MovablePod > v;

    REQUIRE( moveTest(v) );
}

TEST_CASE( "std_fw_list_separate_iter_insert_throw", "[std_fw_list_tests]" )
{
    std::forward_list<int> a;
    std::forward_list<int> b;
    SA::add(b,7);

    bool caught = false;
    try {
        SA::insert(a,SA::begin(b),7);
    } catch (const tt::t::adapters::CollectionAdapterIteratorOutOfBoundsException& e) {
        caught = true;
    }
    REQUIRE( caught );

    caught = false;
    try {
        SA::erase(a,SA::begin(b));
    } catch (const tt::t::adapters::CollectionAdapterIteratorOutOfBoundsException& e) {
        caught = true;
    }
    REQUIRE( caught );

    caught = false;
    try {
        SA::erase(a,SA::begin(b),SA::end(b));
    } catch (const tt::t::adapters::CollectionAdapterIteratorOutOfBoundsException& e) {
        caught = true;
    }
    REQUIRE( caught );
}
