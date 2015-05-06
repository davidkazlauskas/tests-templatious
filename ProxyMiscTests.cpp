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
}

