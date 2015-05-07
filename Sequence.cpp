/*
 * =====================================================================================
 *
 *       Filename:  Sequence.hpp
 *
 *    Description:  Sequence tests
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

TEST_CASE( "sequence_simple", "[sequence_tests]" )
{
    SUM_VAR(sum,sf);

    auto s = SF::seqI(10);
    SM::forEach(sf,s);

    REQUIRE( sum == 55 );
}

TEST_CASE( "sequence_beg_end", "[sequence_tests]" )
{
    INIT_BALLER;

    auto s = SF::seqI(7,77);
    SM::forEach(sf,s);

    REQUIRE( 2982 == sum );
}

TEST_CASE( "sequence_beg_end_step", "[sequence_tests]" )
{
    INIT_BALLER;

    auto s = SF::seqL(7,77,3);
    SM::forEach(sf,s);

    REQUIRE( 996 == sum );
}

TEST_CASE( "sequence_beg_end_char", "[sequence_tests]" )
{
    std::string s = "";

    auto append = [&](char c) { s += c; };
    auto sq = SF::seqI('a','z');
    SM::forEach(append,sq);

    REQUIRE( "abcdefghijklmnopqrstuvwxyz" == s );
}

TEST_CASE( "sequence_throw_include", "[sequence_tests]" )
{
    bool caught = false;

    try {
        SF::seqI(7,77,3);
    } catch (templatious::IncorrectBoundsException e) {
        caught = true;
    }

    REQUIRE( caught );
}

TEST_CASE( "sequence_invert_manual", "[sequence_tests]" )
{
    INIT_BALLER;

    auto s = SF::seqI(77,7);
    int prev = 78;
    TEMPLATIOUS_FOREACH(auto i,s) {
        auto prm1 = prev - 1;
        REQUIRE( prm1 == i );
        prev = i;
    }

    SM::forEach(sf,s);

    REQUIRE( 2982 == sum );
}

TEST_CASE( "sequence_invert_auto", "[sequence_tests]" )
{
    INIT_BALLER;

    auto s = SF::seqI(7,77).rev();
    int prev = 78;
    TEMPLATIOUS_FOREACH(auto i,s) {
        auto prm1 = prev - 1;
        REQUIRE( prm1 == i );
        prev = i;
    }

    SM::forEach(sf,s);

    REQUIRE( 2982 == sum );
}

TEST_CASE( "sequence_invert_double", "[sequence_tests]" )
{
    INIT_BALLER;
    auto s = SF::seqI(7,77).rev().rev();
    int prev = 6;

    TEMPLATIOUS_FOREACH(auto i,s) {
        REQUIRE( prev == i - 1 );
        prev = i;
    }

    SM::forEach(sf,s);

    REQUIRE( 2982 == sum );
}

TEST_CASE( "sequence_unsigned_backwards", "[sequence_tests]" )
{
    bool caught = false;
    try {
        SF::seqI<unsigned int>(77,7);
    } catch (templatious::UnsignedSequenceException e) {
        caught = true;
    }

    REQUIRE( caught );
}

TEST_CASE( "sequence_neg_step", "[sequence_tests]")
{
    bool caught = false;
    try {
        SF::seqL<int>(77,7,-7);
    } catch (templatious::NegativeStepException e) {
        caught = true;
    }

    REQUIRE( caught );
}

TEST_CASE( "sequence_const_traversal", "[sequence_tests]" )
{
    auto s = SF::seqL(7);

    typedef decltype(s) SeqT;

    const SeqT& ref(s);

    typedef templatious::adapters::CollectionAdapter< const SeqT > Ad;

    auto b = Ad::cbegin(ref);
    auto e = Ad::cend(ref);

    long sum = 0;
    for (; b != e; ++b) {
        sum += *b;
    }

    long expSum = SM::sum<long>(s);

    REQUIRE( sum == expSum );
    REQUIRE( *Ad::iterAt(ref,6) == 6 );
    REQUIRE( *Ad::citerAt(ref,6) == 6 );
    REQUIRE( *SA::iterAt(s,6) == 6 );
    REQUIRE( *SA::citerAt(s,6) == 6 );
    REQUIRE( Ad::size(ref) == 7 );
    REQUIRE( SA::size(s) == 7 );

    bool caught = false;
    try {
        SA::iterAt(s,8);
    } catch (const tt::t::IteratorPastEndException& e) {
        caught = true;
    }
    REQUIRE( caught );

    caught = false;
    try {
        SA::citerAt(s,8);
    } catch (const tt::t::IteratorPastEndException& e) {
        caught = true;
    }
    REQUIRE( caught );
}

TEST_CASE( "sequence_raw_qualities", "[sequence_tests]" )
{
    bool caught = false;
    try {
        tt::t::SeqL<int,false> s(7,9,-1);
    } catch (const tt::t::NegativeStepException& e) {
        caught = true;
    }

    REQUIRE( caught );
}
