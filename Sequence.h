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
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#include "TestDefs.h"

BOOST_AUTO_TEST_SUITE( sequence_tests )

BOOST_AUTO_TEST_CASE( sequence_simple )
{
    TEMPLATIOUS_TRIPLET_STD;

    SUM_VAR(sum,sf);

    auto s = SF::seqI(10);
    SM::forEach(sf,s);

    BOOST_CHECK( sum == 55 );
}

BOOST_AUTO_TEST_CASE( sequence_beg_end )
{
    INIT_BALLER;

    auto s = SF::seqI(7,77);
    SM::forEach(sf,s);

    BOOST_CHECK( 2982 == sum );
}

BOOST_AUTO_TEST_CASE( sequence_beg_end_step )
{
    INIT_BALLER;

    auto s = SF::seqL(7,77,3);
    SM::forEach(sf,s);

    BOOST_CHECK( 996 == sum );
}

BOOST_AUTO_TEST_CASE( sequence_beg_end_char )
{
    TEMPLATIOUS_TRIPLET_STD;
    std::string s = "";

    auto append = [&](char c) { s += c; };
    auto sq = SF::seqI('a','z');
    SM::forEach(append,sq);

    BOOST_CHECK( "abcdefghijklmnopqrstuvwxyz" == s );
}

BOOST_AUTO_TEST_CASE( sequence_throw_include )
{
    TEMPLATIOUS_TRIPLET_STD;

    bool caught = false;

    try {
        SF::seqI(7,77,3);
    } catch (templatious::IncorrectBoundsException e) {
        caught = true;
    }

    BOOST_CHECK( caught );
}

BOOST_AUTO_TEST_CASE( sequence_invert_manual )
{
    INIT_BALLER;

    auto s = SF::seqI(77,7);
    int prev = 78;
    TEMPLATIOUS_FOREACH(auto i,s) {
        BOOST_CHECK( prev - 1 == i );
        prev = i;
    }

    SM::forEach(sf,s);

    BOOST_CHECK( 2982 == sum );
}

BOOST_AUTO_TEST_CASE( sequence_invert_auto )
{
    INIT_BALLER;

    auto s = SF::seqI(7,77).rev();
    int prev = 78;
    TEMPLATIOUS_FOREACH(auto i,s) {
        BOOST_CHECK( prev - 1 == i );
        prev = i;
    }

    SM::forEach(sf,s);

    BOOST_CHECK( 2982 == sum );
}

BOOST_AUTO_TEST_CASE( sequence_invert_double )
{
    INIT_BALLER;
    auto s = SF::seqI(7,77).rev().rev();
    int prev = 6;

    TEMPLATIOUS_FOREACH(auto i,s) {
        BOOST_CHECK( prev == i - 1 );
        prev = i;
    }

    SM::forEach(sf,s);

    BOOST_CHECK( 2982 == sum );
}

BOOST_AUTO_TEST_CASE( sequence_unsigned_backwards )
{
    bool caught = false;
    try {
        SF::seqI<unsigned int>(77,7);
    } catch (templatious::UnsignedSequenceException e) {
        caught = true;
    }

    BOOST_CHECK( caught );
}

BOOST_AUTO_TEST_SUITE_END()
