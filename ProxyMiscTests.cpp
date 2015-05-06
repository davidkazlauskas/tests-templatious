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

TEST_CASE( "proxy_range_iter_no_rand", "[proxy_tests]" )
{
    std::list<int> v;
    SA::add(v,SF::seqL(100));

    auto r = SF::range(v,10,20);
    REQUIRE( *SA::iterAt(r,7) == 17 );
}

TEST_CASE( "proxy_filter_past_end_iter", "[proxy_tests]" )
{
    std::vector<int> v;
    SA::add(v,SF::seqL(100));

    auto flt = SF::filter(v,[](int i) { return i % 2 != 0; });
    auto iter = SA::iterAt(flt,50);

    bool caught = false;
    try {
        ++iter;
    } catch (const tt::t::FilterPastEndIterationException& e) {
        caught = true;
    }
    REQUIRE( caught );

    SA::clear(flt);
    REQUIRE( SA::size(flt) == 0 );
}

TEST_CASE( "proxy_skipper_diff_parent_assignment", "[proxy_tests]" )
{
    std::vector<int> v1;
    std::vector<int> v2;
    SA::add(v1,SF::seqL(100));
    SA::add(v2,v1);

    auto s1 = SF::skip(v1,2);
    auto s2 = SF::skip(v2,2);

    auto b1 = SA::begin(s1);
    auto b2 = SA::begin(s2);

    bool caught = false;
    try {
        b1 = b2;
    } catch (const tt::t::SkipperInvalidAssignmentException& e) {
        caught = true;
    }

    REQUIRE( caught );

    SA::clear(s1);
    REQUIRE( SA::size(s1) == 0 );
}

struct IntPod {
    IntPod(int i) : _i(i) {}

    int _i;
};

TEST_CASE( "proxy_ptr_dereference", "[proxy_tests]" )
{
    std::vector< IntPod > v;
    SA::add(v,SF::seqL(100));

    auto r = SF::range(v,7,17);
    auto f = SF::filter(v,[](const IntPod& p) { return p._i % 2 != 0; });
    auto s = SF::skip(v,2);

    REQUIRE( SA::begin(r)->_i == 7 );
    REQUIRE( SA::iterAt(f,2)->_i == 5 );
    REQUIRE( SA::iterAt(s,3)->_i == 6 );
}
