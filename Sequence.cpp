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

TEST_CASE( "sequence_null_step", "[sequence_tests]" )
{
    bool caught = false;
    try {
        SF::seqL(1,2,0);
    } catch (const tt::t::NullStepException& e) {
        caught = true;
    }
    REQUIRE( caught );
}

TEST_CASE( "sequence_decr_iter_neg_step", "[sequence_tests]" )
{
    {
        auto s = SF::seqL(3,7);
        auto iter = SA::iterAt(s,1);
        --iter;
        REQUIRE( *iter == 3 );
    }

    {
        auto s = SF::seqL(7,3);
        auto iter = SA::iterAt(s,1);
        --iter;
        REQUIRE( *iter == 7 );
    }

    auto s = SF::seqL<unsigned int>(3,7).rev();
    {
        auto iter = SA::iterAt(s,1);
        --iter;
        REQUIRE( *iter == 7 );
    }

    {
        auto iter = SA::citerAt(s,1);
        --iter;
        REQUIRE( *iter == 7 );
    }

    {
        bool caught = false;
        try {
            SA::iterAt(s,6);
        } catch (const tt::t::IteratorPastEndException& e) {
            caught = true;
        }
        REQUIRE( caught );
    }

    {
        bool caught = false;
        try {
            SA::citerAt(s,6);
        } catch (const tt::t::IteratorPastEndException& e) {
            caught = true;
        }
        REQUIRE( caught );
    }
}

TEST_CASE( "sequence_include_neg_test", "[sequence_tests]" )
{
    auto s = SF::seqI(-3);

    REQUIRE( *SA::iterAt(s,0) == 0 );
    REQUIRE( *SA::iterAt(s,1) == -1 );
    REQUIRE( *SA::iterAt(s,2) == -2 );
    REQUIRE( *SA::iterAt(s,3) == -3 );
}

TEST_CASE( "sequence_include_neg_test_beg_end", "[sequence_tests]" )
{
    {
    auto s = SF::seqI(-7,-3,1);

    REQUIRE( *SA::iterAt(s,0) == -7 );
    REQUIRE( *SA::iterAt(s,1) == -6 );
    REQUIRE( *SA::iterAt(s,2) == -5 );
    REQUIRE( *SA::iterAt(s,3) == -4 );
    REQUIRE( *SA::iterAt(s,4) == -3 );
    }

    {
    auto s = SF::seqI(-3,-7,1);

    REQUIRE( *SA::iterAt(s,0) == -3 );
    REQUIRE( *SA::iterAt(s,1) == -4 );
    REQUIRE( *SA::iterAt(s,2) == -5 );
    REQUIRE( *SA::iterAt(s,3) == -6 );
    REQUIRE( *SA::iterAt(s,4) == -7 );
    }
}

TEST_CASE( "sequence_operators", "[sequence_tests]" )
{
    auto s = SF::seqI(1,10);

    auto b = SA::iterAt(s,2);
    auto e = SA::iterAt(s,4);

    REQUIRE( b < e );
    REQUIRE( e > b );
    REQUIRE( b <= e );
    REQUIRE( e >= b );
    REQUIRE( b == b );
    REQUIRE( e == e );
    REQUIRE( b <= b );
    REQUIRE( e <= e );
}

TEST_CASE( "sugar_foreach", "[sequence_tests]" )
{
    int sum = 0;
    TEMPLATIOUS_FOREACH(auto i,SF::seqL(8,10)) {
        sum += i;
    }

    int expSum = 8 + 9;
    REQUIRE( sum == expSum );
}

TEST_CASE( "sugar_0_to_n", "[sequence_tests]" )
{
    int sum = 0;
    TEMPLATIOUS_0_TO_N(i,4) {
        sum += i;
    }

    int expSum = 0 + 1 + 2 + 3;
    REQUIRE( sum == expSum );
}

TEST_CASE( "sugar_repeat", "[sequence_tests]" )
{
    int count = 0;
    TEMPLATIOUS_REPEAT( 10 ) {
        ++count;
    }

    REQUIRE( count == 10 );
}
