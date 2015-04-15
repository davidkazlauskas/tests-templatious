/*
 * =====================================================================================
 *
 *       Filename:  PackTests.hpp
 *
 *    Description:  test for virtual packs
 *
 *        Version:  1.0
 *        Created:  10/13/2014 08:40:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Kazlauskas (dk), david@templatious.org
 *
 * =====================================================================================
 */

#include "TestDefs.hpp"

TEST_CASE( "virtual_pack_impl", "[virtual_pack_tests]" )
{
    tt::t::VirtualPackImpl<int,int> impl(1,2);

    bool matches = impl.matchesSignature<int,int>();
    REQUIRE( matches );
    REQUIRE( impl.size() == 2 );
    matches = impl.matchesSignature<int,char>();
    REQUIRE( !matches );
    matches = impl.matchesSignature<char,int>();
    REQUIRE( !matches );
    matches = impl.matchesSignature<int,const int>();
    REQUIRE( matches );
    matches = impl.matchesSignature<int,int&>();
    REQUIRE( matches );
    matches = impl.matchesSignature<int,int*>();
    REQUIRE( !matches );
}

TEST_CASE( "virtual_pack_impl_const_correct", "[virtual_pack_tests]" )
{
    tt::t::VirtualPackImpl<int,const int> impl(1,2);
    bool matches = impl.matchesSignature<int,int>();
    REQUIRE( !matches );
    matches = impl.matchesSignature<int,const int>();
    REQUIRE( matches );
    // if element is non const but
    // we need const it's okay
    matches = impl.matchesSignature<const int,const int>();
    REQUIRE( matches );
}

TEST_CASE( "virtual_pack_impl_call_correct_fails", "[virtual_pack_tests]" )
{
    tt::t::VirtualPackImpl<int,const int> impl(1,2);

    int outA,outB;
    outA = outB = -7;
    auto testLambda =
        [&](const int& a,const int& b) {
            outA = a;
            outB = b;
        };
    bool tryCall = impl.tryCallFunction<int,int>(testLambda);
    REQUIRE( !tryCall );
    REQUIRE( outA == -7 );
    REQUIRE( outA == outB );

    bool caught = false;
    try {
        impl.callFunction<int,int>(testLambda);
    } catch (const templatious::
            VirtualPackWrongTypeSignatureException& e) {
        caught = true;
    }

    REQUIRE( caught );
    REQUIRE( outA == -7 );
    REQUIRE( outA == outB );

    caught = false;
    try {
        impl.callFunction<int>([](int i) {});
    } catch (const templatious::
            VirtualPackBadNumberOfArgumentsException& e) {
        caught = true;
    }
    REQUIRE( caught );
}

TEST_CASE( "virtual_pack_impl_call_success", "[virtual_pack_tests]" )
{
    tt::t::VirtualPackImpl<int,const int> impl(1,2);

    int outA,outB;
    outA = outB = -7;
    auto testLambda =
        [&](const int& a,const int& b) {
            outA = a;
            outB = b;
        };

    bool tryCall = impl.tryCallFunction<int,const int>(testLambda);
    REQUIRE( tryCall );
    REQUIRE( outA == 1 );
    REQUIRE( outB == 2 );

    outA = outB = -7;
    impl.callFunction<int,const int>(testLambda);
    REQUIRE( outA == 1 );
    REQUIRE( outB == 2 );
}

TEST_CASE( "virtual_pack_impl_call_mod_contents", "[virtual_pack_tests]" )
{
    tt::t::VirtualPackImpl<int,int> impl(1,2);

    int outA,outB;
    outA = outB = -7;
    auto testLambda =
        [&](int& a,int& b) {
            ++a;
            ++b;
            outA = a;
            outB = b;
        };

    bool tryCall = impl.tryCallFunction<int,int>(testLambda);
    REQUIRE( tryCall );
    REQUIRE( outA == 2 );
    REQUIRE( outB == 3 );

    outA = outB = -7;
    impl.callFunction<int,int>(testLambda);
    REQUIRE( outA == 3 );
    REQUIRE( outB == 4 );
}

TEST_CASE( "virtual_pack_impl_const_ref", "[virtual_pack_tests]" )
{
    typedef tt::t::VirtualPackImpl<int,int> TheImpl;
    TheImpl impl(1,2);

    const TheImpl& cref(impl);

    int outA,outB;
    outA = outB = -7;
    auto testLambda =
        [&](const int& a,const int& b) {
            outA = a;
            outB = b;
        };

    bool success = cref.tryCallFunction<
        const int,const int>(testLambda);

    REQUIRE( success );
    REQUIRE( outA == 1 );
    REQUIRE( outB == 2 );

    outA = outB = -7;

    success = cref.tryCallFunction<
        const int,const long>([&](int a,long b) {
                outA = a; outB = b;
        });

    REQUIRE( !success );
    REQUIRE( outA == -7 );
    REQUIRE( outA == outB );
}

TEST_CASE( "virtual_pack_impl_const_ref_notry", "[virtual_pack_tests]" )
{
    typedef tt::t::VirtualPackImpl<int,int> TheImpl;
    TheImpl impl(1,2);

    const TheImpl& cref(impl);

    int outA,outB;
    outA = outB = -7;
    auto testLambda =
        [&](const int& a,const int& b) {
            outA = a;
            outB = b;
        };

    cref.callFunction<
        const int,const int>(testLambda);

    REQUIRE( outA == 1 );
    REQUIRE( outB == 2 );

    outA = outB = -7;

    bool caught = false;
    try {
        cref.callFunction<
            const int,const long>([&](int a,long b) {
                    outA = a; outB = b;
            });
    } catch (const templatious::
            VirtualPackWrongTypeSignatureException& e) {
        caught = true;
    }

    REQUIRE( caught );
    REQUIRE( outA == -7 );
    REQUIRE( outA == outB );
}

TEST_CASE( "virtual_pack_match_test", "[virtual_pack_tests]" )
{
    int outA,outB;
    outA = outB = -7;
    auto l = [&](int& a,int& b) {
        outA = a;
        outB = a;
    };

    tt::t::VirtualMatch<
        decltype(l),
        templatious::util::DefaultStoragePolicy,
        int, int
    > m(l);
}
