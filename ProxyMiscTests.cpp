/*
 * =====================================================================================
 *
 *       Filename:  ProxyMiscTests.cpp
 *
 *    Description:  Proxy miscellanious tests
 *
 *        Version:  1.0
 *        Created:  11/30/2014 02:33:42 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Kazlauskas (dk), david@templatious.org
 *
 * =====================================================================================
 */

#include "TestDefs.hpp"

TEST_CASE( "proxy_range_iter_at_rand_access", "[proxy_tests]" )
{
    std::vector<int> v;
    SA::add(v,SF::seqL(100));

    auto r = SF::range(v,10,20);

    REQUIRE( *SA::iterAt(r,7) == 17 );

    auto beg = SA::iterAt(r,1);
    REQUIRE( *beg == 11 );
    --beg;
    REQUIRE( *beg == 10 );

    r.clear();
    REQUIRE( r.size() == 0 );
}

TEST_CASE( "proxy_range_iter_only_beg", "[proxy_tests]" )
{
    std::vector<int> v;
    SA::add(v,SF::seqL(100));

    auto r = SF::range(v,SA::iterAt(v,10));
    REQUIRE( *SA::iterAt(r,0) == 10 );
    REQUIRE( *SA::iterAt(r,89) == 99 );
}

