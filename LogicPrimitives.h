/*
 * =====================================================================================
 *
 *       Filename:  LogicPrimitives.h
 *
 *    Description:  Logic primitives tests
 *
 *        Version:  1.0
 *        Created:  07/30/2014 05:25:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef LOGICPRIMITIVES_UPNAHJK9
#define LOGICPRIMITIVES_UPNAHJK9

#include "TestDefs.h"

BOOST_AUTO_TEST_CASE( logic_primitives_or ) {

    namespace t = tt::t;

    auto tr = []() { return true; };
    auto fa = []() { return false; };

    auto a = t::mOr(fa,fa);
    auto b = t::mOr(fa,tr);
    auto c = t::mOr(tr,fa);
    auto d = t::mOr(tr,tr);

    BOOST_CHECK( a() == false );
    BOOST_CHECK( b() == true );
    BOOST_CHECK( c() == true );
    BOOST_CHECK( d() == true );

}

BOOST_AUTO_TEST_CASE( logic_primitives_and ) {

    namespace t = tt::t;

    auto tr = []() { return true; };
    auto fa = []() { return false; };

    auto a = t::mAnd(fa,fa);
    auto b = t::mAnd(fa,tr);
    auto c = t::mAnd(tr,fa);
    auto d = t::mAnd(tr,tr);

    BOOST_CHECK( a() == false );
    BOOST_CHECK( b() == false );
    BOOST_CHECK( c() == false );
    BOOST_CHECK( d() == true );

}

BOOST_AUTO_TEST_CASE( logic_primitives_comp ) {

    namespace t = tt::t;

    auto tr = []() { return true; };
    auto fa = []() { return false; };

    auto a = t::mAnd(fa,fa);
    auto b = t::mOr(a,tr);
    auto c = t::mAnd(b,tr);
    auto d = t::mAnd(c,fa);
    auto e = t::mOr(d,tr);

    BOOST_CHECK( a() == false );
    BOOST_CHECK( b() == true );
    BOOST_CHECK( c() == true );
    BOOST_CHECK( d() == false );
    BOOST_CHECK( e() == true );

}


#endif /* end of include guard: LOGICPRIMITIVES_UPNAHJK9 */
