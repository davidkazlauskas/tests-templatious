/*
 * =====================================================================================
 *
 *       Filename:  OnceTraversable.h
 *
 *    Description:  Once traversable tests
 *
 *        Version:  1.0
 *        Created:  11/02/2014 05:03:51 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Kazlauskas (dk), david@templatious.org
 *
 * =====================================================================================
 */

#include <vector>

#include "TestDefs.hpp"

TEST_CASE( "once_traversable_basic", "[once_traversable]" )
{
    int a,b,c;
    auto p = SF::pack(a,b,c);
    auto ot = SF::onceTraversable(SF::seqI(1,7));

    int sz = -1;
    int sum = 0;
    do {
        sz = SM::distribute(ot,p);
        sum += a;
        sum += 7 * b;
        sum += 17 * c;
        // 1 + 2 * 7 + 3 * 17 + 4 + 5 * 7 + 6 * 17 + 7 + 5 * 7 + 6 * 17
    } while ( sz == p.size );

    REQUIRE( sum == 351 );
}

TEST_CASE( "once_traversable_move_semantics", "[once_traversable]" )
{
    typedef std::vector<int> Vect;

    Vect v;
    SA::add(v,SF::seqI(1,7));

    int sum = 0;
    auto sf = SF::storageFunctor<Sum>(sum);

    SM::forEach(sf,v);

    REQUIRE( sum == 28 );

    auto ot = SF::onceTraversable(std::move(v));

    sum = 0;
    SM::forEach(sf,ot);

    REQUIRE( sum == 28 );

    sum = 0;
    SM::forEach(sf,v);

    REQUIRE( sum == 0 );
}

