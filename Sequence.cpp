/*
 * =====================================================================================
 *
 *       Filename:  Sequence.h
 *
 *    Description:  Sequence tests
 *
 *        Version:  1.0
 *        Created:  11/30/2014 02:33:42 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
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
