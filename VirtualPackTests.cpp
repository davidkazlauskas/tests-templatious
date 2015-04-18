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

TEST_CASE( "virtual_pack_match_test_const_fail", "[virtual_pack_tests]" )
{
    int outA,outB;
    outA = outB = -7;
    auto l = [&](int& a,int& b) {
        outA = a;
        outB = b;
    };

    tt::t::VirtualMatch<
        decltype(l),
        false,
        templatious::util::DefaultStoragePolicy,
        int, int
    > m(l);

    tt::t::VirtualPackImpl<int,const int> impl(1,2);

    bool isGood = m(impl);
    REQUIRE( !isGood );
    REQUIRE( outA == -7 );
    REQUIRE( outA == outB );
}

TEST_CASE( "virtual_pack_match_test_const_pass", "[virtual_pack_tests]" )
{
    int outA,outB;
    outA = outB = -7;
    auto l = [&](int& a,const int& b) {
        outA = a;
        outB = b;
    };

    tt::t::VirtualMatch<
        decltype(l),
        false,
        templatious::util::DefaultStoragePolicy,
        int, const int
    > m(l);

    tt::t::VirtualPackImpl<int,const int> impl(1,2);

    bool isGood = m(impl);
    REQUIRE( isGood );
    REQUIRE( outA == 1 );
    REQUIRE( outB == 2 );
}

TEST_CASE( "virtual_pack_fget", "[virtual_pack_tests]" )
{
    tt::t::VirtualPackImpl<int,const int,double> impl(1,2,7.7);

    REQUIRE( impl.fGet<0>() == 1 );
    REQUIRE( impl.fGet<1>() == 2 );
    REQUIRE( std::fabs( impl.fGet<2>() - 7.7 ) < 0.00000001 );
}

TEST_CASE( "virtual_pack_match_functor", "[virtual_pack_tests]" )
{
    auto pack = SF::vpack<long,long>(1,2);

    int outResult = -7;
    auto func =
        SF::virtualMatchFunctor(
            SF::virtualMatch<long,int>(
                [&](long l,int i) {
                    outResult = 1;
                }
            )
        );

    bool matched = func.tryMatch(pack);
    REQUIRE( !matched );
    REQUIRE( outResult == -7 );

    auto func2 =
        SF::virtualMatchFunctor(
            SF::virtualMatch<long,int>(
                [&](long l,int i) {
                    outResult = 1;
                }
            ),
            SF::virtualMatch<long,long>(
                [&](long l,long i) {
                    outResult = 2;
                }
            )
        );

    matched = func2.tryMatch(pack);
    REQUIRE( matched );
    REQUIRE( outResult == 2 );
}

TEST_CASE( "virtual_pack_match_functor_composition", "[virtual_pack_tests]" )
{
    auto pack = SF::vpack<long,long>(1,2);

    int outResult = -7;
    auto func =
        SF::virtualMatchFunctor(
            SF::virtualMatch<long,long>(
                [&](long l,long i) {
                    outResult = 1;
                }
            )
        );

    auto func2 =
        SF::virtualMatchFunctor(
            SF::virtualMatch<long,int>(
                [&](long l,long i) {
                    outResult = 2;
                }
            ),
            func
        );

    bool matched = func2.tryMatch(pack);

    REQUIRE( matched );
    REQUIRE( outResult == 1 );

    outResult = -7;

    auto func3 =
        SF::virtualMatchFunctor(
            func,
            SF::virtualMatch<long,int>(
                [&](long l,long i) {
                    outResult = 3;
                }
            )
        );

    matched = func3.tryMatch(pack);
    REQUIRE( matched );
    REQUIRE( outResult == 1 );
}

TEST_CASE( "virtual_pack_match_functor_priorities", "[virtual_pack_tests]" )
{
    auto pack = SF::vpack<long,long>(1,2);

    int outResult = -7;
    auto func =
        SF::virtualMatchFunctor(
            SF::virtualMatch<long,long>(
                [&](long l,long i) {
                    outResult = 1;
                }
            ),
            SF::virtualMatch<long,long>(
                [&](long l,long i) {
                    outResult = 2;
                }
            )
        );

    bool matched = func.tryMatch(pack);
    REQUIRE( matched );
    REQUIRE( outResult == 1 );
}

TEST_CASE( "virtual_pack_match_functor_throw", "[virtual_pack_tests]" )
{
    auto pack = SF::vpack<long,long>(1,2);

    int outResult = -7;

    auto func =
        SF::virtualMatchFunctorPtr(
            SF::virtualMatch<long,long>(
                [&](long l,long i) {
                    outResult = 1;
                }
            )
        );

    (*func)(pack);

    REQUIRE( outResult == 1 );
    outResult = -7;

    auto func2 =
        SF::virtualMatchFunctorPtr(
            SF::virtualMatch<long,int>(
                [&](long l,int i) {
                    outResult = 2;
                }
            )
        );

    bool caught = false;
    try {
        (*func2)(pack);
    } catch (const templatious::
        VirtualPackMatcherNoMatchException& e) {
        caught = true;
    }

    REQUIRE( caught );
}

TEST_CASE( "virtual_pack_match_functor_transparency", "[virtual_pack_tests]" )
{
    auto pack = SF::vpack<long,long>(1,2);

    int outResultA = -7;
    int outResultB = -7;

    auto func =
        SF::virtualMatchFunctor(
            SF::virtualTransparentMatch<long,long>(
                [&](long l,long i) {
                    outResultA = 1;
                }
            ),
            SF::virtualTransparentMatch<long,long>(
                [&](long l,long i) {
                    outResultB = 2;
                }
            )
        );

    bool matched = func.tryMatch(pack);

    REQUIRE( !matched );
    REQUIRE( outResultA == 1 );
    REQUIRE( outResultB == 2 );
}

TEST_CASE( "virtual_pack_match_functor_vptr", "[virtual_pack_tests]" )
{
    auto pack = SF::vpack<long,long>(1,2);
    int outResultA = -7;

    auto func =
        SF::virtualMatchFunctorPtr(
            SF::virtualMatch<long,long>(
                [&](long l,long i) {
                    outResultA = 1;
                }
            )
        );

    bool matched = func->tryMatch(pack);

    REQUIRE( matched );
    REQUIRE( outResultA == 1 );
}

TEST_CASE( "virtual_pack_use_count", "[virtual_pack_tests]" )
{
    auto pack = SF::vpack<long,long>(1,2);

    auto func =
        SF::virtualMatchFunctor(
            SF::virtualTransparentMatch<long,long>(
                [&](long l,long i) {}
            ),
            SF::virtualMatch<long,long>(
                [&](long l,long i) {}
            )
        );

    bool matched = func.tryMatch(pack);

    REQUIRE( matched );
    REQUIRE( pack.useCount() == 2 );
}

TEST_CASE( "virtual_pack_vptr", "[virtual_pack_tests]" )
{
    auto vpack = SF::vpackPtr<long,long>(1,2);

    long outResultA = -7;
    long outResultB = -7;
    REQUIRE( vpack->useCount() == 0 );
    bool matched = vpack->tryCallFunction<long,long>(
        [&](long a,long b) {
            outResultA = a;
            outResultB = b;
        }
    );

    REQUIRE( matched );
    REQUIRE( vpack->useCount() == 1 );
}

TEST_CASE( "virtual_pack_dynamic_match_functor", "[virtual_pack_tests]" )
{
    templatious::DynamicVMatchFunctor dvmf;

    auto pack = SF::vpack<long,long>(1,2);
    int outResultA = -7;

    auto func =
        SF::virtualMatchFunctorPtr(
            SF::virtualMatch<long,long>(
                [&](long l,long i) {
                    outResultA = 1;
                }
            )
        );

    auto preAddr = func.get();

    bool matched = dvmf.tryMatch(pack);
    REQUIRE( !matched );
    REQUIRE( outResultA == -7 );

    int id = dvmf.attach(std::move(func));
    matched = dvmf.tryMatch(pack);
    REQUIRE( matched );

    auto backPtr = dvmf.detach(id);
    auto postAddr = backPtr.get();
    REQUIRE( preAddr == postAddr );

    matched = dvmf.tryMatch(pack);
    REQUIRE( !matched );
}

TEST_CASE( "virtual_pack_dynamic_match_functor_priority", "[virtual_pack_tests]" )
{
    templatious::DynamicVMatchFunctor dvmf;

    auto pack = SF::vpack<long,long>(1,2);
    int outResultA = -7;

    auto func =
        SF::virtualMatchFunctorPtr(
            SF::virtualMatch<long,long>(
                [&](long l,long i) {
                    outResultA = 1;
                }
            )
        );

    auto func2 =
        SF::virtualMatchFunctorPtr(
            SF::virtualMatch<long,long>(
                [&](long l,long i) {
                    outResultA = 2;
                }
            )
        );

    int idA = dvmf.attach(std::move(func));
    int idB = dvmf.attach(std::move(func2),129);

    bool matched = dvmf.tryMatch(pack);

    REQUIRE( matched );
    REQUIRE( outResultA == 2 );

    func2 = dvmf.detach(idB);
    dvmf.attach(std::move(func2),128);

    matched = dvmf.tryMatch(pack);
    REQUIRE( matched );
    REQUIRE( outResultA == 1 );

    func = dvmf.detach(idA);
    dvmf.attach(std::move(func),127);

    matched = dvmf.tryMatch(pack);
    REQUIRE( matched );
    REQUIRE( outResultA == 2 );
}
