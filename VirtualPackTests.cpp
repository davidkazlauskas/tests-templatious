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

#include <chrono>
#include <thread>

#include "TestDefs.hpp"

static const int DefaultCoreSettings = tt::t::VPACK_DEF_MASK;

TEST_CASE( "virtual_pack_impl", "[virtual_pack_tests]" )
{
    tt::t::VirtualPackImpl<DefaultCoreSettings,int,int>
        impl(tt::t::ExpVpackConInvoke(),1,2);

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
    tt::t::VirtualPackImpl<DefaultCoreSettings,int,const int>
        impl(tt::t::ExpVpackConInvoke(),1,2);
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
    tt::t::VirtualPackImpl<DefaultCoreSettings,int,const int>
        impl(tt::t::ExpVpackConInvoke(),1,2);

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
    tt::t::VirtualPackImpl<DefaultCoreSettings,int,const int>
        impl(tt::t::ExpVpackConInvoke(),1,2);

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
    tt::t::VirtualPackImpl<DefaultCoreSettings,int,int> impl(
        tt::t::ExpVpackConInvoke(),
        1,2);

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
    typedef tt::t::VirtualPackImpl<DefaultCoreSettings,int,int> TheImpl;
    TheImpl impl(tt::t::ExpVpackConInvoke(),1,2);

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
    typedef tt::t::VirtualPackImpl<DefaultCoreSettings,int,int> TheImpl;
    TheImpl impl(tt::t::ExpVpackConInvoke(),1,2);

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

    tt::t::VirtualPackImpl<DefaultCoreSettings,int,const int>
        impl(tt::t::ExpVpackConInvoke(),1,2);

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

    tt::t::VirtualPackImpl<DefaultCoreSettings,int,const int>
        impl(tt::t::ExpVpackConInvoke(),1,2);

    bool isGood = m(impl);
    REQUIRE( isGood );
    REQUIRE( outA == 1 );
    REQUIRE( outB == 2 );
}

TEST_CASE( "virtual_pack_fget", "[virtual_pack_tests]" )
{
    tt::t::VirtualPackImpl<DefaultCoreSettings,int,const int,double>
        impl(tt::t::ExpVpackConInvoke(),1,2,7.7);

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
        SF::virtualMatchFunctorPtr(
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
            std::move(func)
        );

    bool matched = func2.tryMatch(pack);

    REQUIRE( matched );
    REQUIRE( outResult == 1 );

    outResult = -7;

    auto func3 =
        SF::virtualMatchFunctor(
            std::move(func2),
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

TEST_CASE( "virtual_pack_with_callback", "[virtual_pack_tests]" )
{
    volatile int sum = 0;
    auto pack = SF::vpackPtrWCallback<int,int>(
        [&](const TEMPLATIOUS_VPCORE<int,int>& vcore) {
            sum += vcore.fGet<0>();
            sum += vcore.fGet<1>();
        },
        1,2
    );

    auto mf = SF::virtualMatchFunctor(
        SF::virtualMatch<int,int>(
            [](int& a,int& b) {
                a *= 2;
                b *= 2;
            }
        )
    );

    bool matched = mf.tryMatch(*pack);

    REQUIRE( matched );
    REQUIRE( sum == 6 );
}

TEST_CASE( "virtual_pack_custom_wait", "[virtual_pack_tests]" )
{
    auto pack = SF::vpackPtrCustom<
        tt::t::VPACK_WAIT,
        int,int
    >(1,2);

    auto handle = std::async(std::launch::async,
        [=]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            pack->tryCallFunction<int,int>(
                [](int& a,int& b) {
                    a *= 2;
                    b *= 2;
                }
            );
        });

    pack->wait();
    int outA = pack->fGet<0>();
    int outB = pack->fGet<1>();

    REQUIRE( outA == 2 );
    REQUIRE( outB == 4 );
}

TEST_CASE( "virtual_pack_custom_nowait", "[virtual_pack_tests]" )
{
    auto pack = SF::vpackPtrCustom<
        0,
        int,int
    >(1,2);

    auto handle = std::async(std::launch::async,
        [=]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            pack->tryCallFunction<int,int>(
                [](int& a,int& b) {
                    a *= 2;
                    b *= 2;
                }
            );
        });
    pack->wait();
    int outA = pack->fGet<0>();
    int outB = pack->fGet<1>();

    REQUIRE( outA == 1 );
    REQUIRE( outB == 2 );
}

TEST_CASE( "virtual_pack_custom_synchronized", "[virtual_pack_tests]" )
{
    auto pack = SF::vpackPtrCustom<
        tt::t::VPACK_SYNCED,
        int,int
    >(1,2);

    const int ROUNDS = 100000;

    auto handleA = std::async(std::launch::async,
        [=]() {
            TEMPLATIOUS_REPEAT( ROUNDS ) {
                pack->tryCallFunction<int,int>(
                    [](int& a,int& b) {
                        ++a;
                        ++b;
                    }
                );
            }
        });

    auto handleB = std::async(std::launch::async,
        [=]() {
            TEMPLATIOUS_REPEAT( ROUNDS ) {
                pack->tryCallFunction<int,int>(
                    [](int& a,int& b) {
                        ++a;
                        ++b;
                    }
                );
            }
        });

    handleA.wait();
    handleB.wait();

    int outA = pack->fGet<0>();
    int outB = pack->fGet<1>();

    int expA = 1 + ROUNDS * 2;
    int expB = 2 + ROUNDS * 2;

    REQUIRE( outA == expA );
    REQUIRE( outB == expB );
}

TEST_CASE( "virtual_pack_custom_unsynchronized", "[virtual_pack_tests]" )
{
    auto pack = SF::vpackPtrCustom<
        0,
        int,int
    >(1,2);

    const int ROUNDS = 1000000;

    auto handleA = std::async(std::launch::async,
        [=]() {
            TEMPLATIOUS_REPEAT( ROUNDS ) {
                pack->tryCallFunction<int,int>(
                    [](int& a,int& b) {
                        ++a;
                        ++b;
                    }
                );
            }
        });

    auto handleB = std::async(std::launch::async,
        [=]() {
            TEMPLATIOUS_REPEAT( ROUNDS ) {
                pack->tryCallFunction<int,int>(
                    [](int& a,int& b) {
                        ++a;
                        ++b;
                    }
                );
            }
        });

    handleA.wait();
    handleB.wait();

    int outA = pack->fGet<0>();
    int outB = pack->fGet<1>();

    int expA = 1 + ROUNDS * 2;
    int expB = 2 + ROUNDS * 2;

    REQUIRE( outA != expA );
    REQUIRE( outB != expB );
}

TEST_CASE( "virtual_pack_counted", "[virtual_pack_tests]" )
{
    auto packA = SF::vpackPtrCustom<
        tt::t::VPACK_COUNT,
        int,int
    >(1,2);

    auto packB = SF::vpackPtrCustom<
        0,
        int,int
    >(1,2);

    REQUIRE( packA->useCount() == 0 );
    REQUIRE( packB->useCount() == -1 );

    packA->tryCallFunction<int,int>(
        [](int a,int b) {}
    );
    packB->tryCallFunction<int,int>(
        [](int a,int b) {}
    );

    REQUIRE( packA->useCount() == 1 );
    REQUIRE( packB->useCount() == -1 );
}

TEST_CASE( "virtual_pack_custom_synchronized_with_callback", "[virtual_pack_tests]" )
{
    // increment this from threads
    auto iPtr = std::make_shared< int >( 0 );

    auto pack = SF::vpackPtrCustomWCallback<
        tt::t::VPACK_SYNCED,
        int,int
    >([=](const TEMPLATIOUS_VPCORE<int,int>& vp) {
        *iPtr += vp.fGet<0>();
        *iPtr += vp.fGet<1>();
    },1,2);

    const int ROUNDS = 100000;

    // callbacks must also be synchronized
    auto handleA = std::async(std::launch::async,
        [=]() {
            TEMPLATIOUS_REPEAT( ROUNDS ) {
                pack->tryCallFunction<int,int>(
                    [](int& a,int& b) {}
                );
            }
        });

    auto handleB = std::async(std::launch::async,
        [=]() {
            TEMPLATIOUS_REPEAT( ROUNDS ) {
                pack->tryCallFunction<int,int>(
                    [](int& a,int& b) {}
                );
            }
        });

    handleA.wait();
    handleB.wait();

    int outExpected = (ROUNDS * 2) * (1 + 2);
    REQUIRE( *iPtr == outExpected );
}

TEST_CASE( "virtual_pack_custom_unsynchronized_with_callback", "[virtual_pack_tests]" )
{
    // increment this from threads
    auto iPtr = std::make_shared< int >( 0 );

    auto pack = SF::vpackPtrCustomWCallback<
        0,
        int,int
    >([=](const TEMPLATIOUS_VPCORE<int,int>& vp) {
        *iPtr += vp.fGet<0>();
        *iPtr += vp.fGet<1>();
    },1,2);

    const int ROUNDS = 100000;

    // callbacks must also be synchronized
    auto handleA = std::async(std::launch::async,
        [=]() {
            TEMPLATIOUS_REPEAT( ROUNDS ) {
                pack->tryCallFunction<int,int>(
                    [](int& a,int& b) {}
                );
            }
        });

    auto handleB = std::async(std::launch::async,
        [=]() {
            TEMPLATIOUS_REPEAT( ROUNDS ) {
                pack->tryCallFunction<int,int>(
                    [](int& a,int& b) {}
                );
            }
        });

    handleA.wait();
    handleB.wait();

    int outExpected = (ROUNDS * 2) * (1 + 2);
    REQUIRE( *iPtr != outExpected );
}

typedef std::unique_ptr<
    templatious::VirtualMatchFunctor > VmfPtr;

static long someOutVarA = -1;
static long someOutVarB = -1;

VmfPtr aHandler() {
    return SF::virtualMatchFunctorPtr(
        SF::virtualMatch<int,char>(
            [](int a,char b) {}
        ),
        SF::virtualMatch<char,int>(
            [](char a,int b) {}
        )
    );
}

VmfPtr bHandler() {
    return SF::virtualMatchFunctorPtr(
        SF::virtualMatch<short,long>(
            [](short a,long b) {
                someOutVarA = a;
                someOutVarB = b;
            }
        ),
        SF::virtualMatch<long,short>(
            [](long a,short b) {}
        )
    );
}

VmfPtr cHandler() {
    return SF::virtualMatchFunctorPtr(
        aHandler(),bHandler()
    );
}

TEST_CASE( "virtual_match_functor_composition", "[virtual_pack_tests]" )
{
    auto hndl = cHandler();
    auto p = SF::vpack<short,long>(1,2);
    bool success = hndl->tryMatch(p);

    REQUIRE( success );

    REQUIRE( someOutVarA == 1 );
    REQUIRE( someOutVarB == 2 );
}

TEST_CASE( "virtual_match_functor_dyn_broadcast", "[virtual_pack_tests]" )
{
    int outA = -1;
    int outB = -1;
    auto vmfPtrA = SF::virtualMatchFunctorPtr(
        SF::virtualMatch<int>([&](int i) {
            outA = i;
        })
    );
    auto vmfPtrB = SF::virtualMatchFunctorPtr(
        SF::virtualMatch<int>([&](int i) {
            outB = i;
        })
    );

    tt::t::DynamicVMatchFunctor dvmf(true);
    dvmf.attach(std::move(vmfPtrA));
    dvmf.attach(std::move(vmfPtrB));

    auto vpack = SF::vpack<int>(7);
    bool success = dvmf.tryMatch(vpack);

    REQUIRE( success );
    REQUIRE( outA == 7 );
    REQUIRE( outB == 7 );
}

TEST_CASE( "virtual_pack_only_movable_assign", "[virtual_pack_tests]" )
{
    typedef std::unique_ptr<int> ThePtr;
    ThePtr toMove(new int(7));

    auto vpack = SF::vpack< ThePtr >(std::move(toMove));

    REQUIRE( *vpack.fGet<0>() == 7 );
    REQUIRE( toMove.get() == nullptr );
}

TEST_CASE( "virtual_pack_synced_use_count", "[virtual_pack_tests]" )
{
    auto vpack = SF::vpackPtrCustom<
        templatious::VPACK_COUNT | // pack bitmask
        templatious::VPACK_SYNCED,
        int // the signature
    >(0);

    const int ROUNDS  = 100000;

    auto h1 = std::async(std::launch::async,
        [=]() {
            TEMPLATIOUS_REPEAT( ROUNDS ) {
                vpack->tryCallFunction<int>(
                    [](int& i) {
                        ++i;
                    }
                );
            }
        });

    auto h2 = std::async(std::launch::async,
        [=]() {
            TEMPLATIOUS_REPEAT( ROUNDS ) {
                vpack->tryCallFunction<int>(
                    [](int& i) {
                        i += 2;
                    }
                );
            }
        });

    h1.wait();
    h2.wait();

    int outResult = vpack->fGet<0>();
    int useCount = vpack->useCount();
    REQUIRE( outResult == ROUNDS * (1 + 2) );
    REQUIRE( useCount == ROUNDS * 2 );
}

TEST_CASE( "virtual_match_functor_dyn_const_coverage", "[virtual_pack_tests]" )
{
    templatious::DynamicVMatchFunctor dvmf;
    const templatious::DynamicVMatchFunctor& cref(dvmf);

    auto vp = SF::vpack<int>(1);
    auto vpNo = SF::vpack<char>('7');
    typedef decltype(vp) VPType;
    typedef decltype(vpNo) VPNoType;

    int sum = 0;
    dvmf.attach(
        SF::virtualMatchFunctorPtr(
            SF::virtualMatch<const int>(
                [&](const int& inc) { sum += inc; }
            )
        )
    );

    bool succ = true;
    succ &= dvmf.tryMatch(vp);
    succ &= dvmf.tryMatch(const_cast<const VPType&>(vp));
    succ &= cref.tryMatch(vp);
    succ &= cref.tryMatch(const_cast<const VPType&>(vp));

    bool nosucc = false;
    nosucc |= dvmf.tryMatch(vpNo);
    nosucc |= dvmf.tryMatch(const_cast<const VPNoType&>(vpNo));
    nosucc |= cref.tryMatch(vpNo);
    nosucc |= cref.tryMatch(const_cast<const VPNoType&>(vpNo));

    REQUIRE( succ );
    REQUIRE( !nosucc );
    REQUIRE( sum == 4 );
}

TEST_CASE( "virtual_match_functor_dyn_no_detach_exception", "[virtual_pack_tests]" )
{
    templatious::DynamicVMatchFunctor dvmf;

    bool caught = false;
    try {
        dvmf.detach(7);
    } catch (const tt::t::DynamicVMatchFunctorNoIdToRemoveException& e) {
        caught = true;
    }
    REQUIRE( caught );
}

TEST_CASE( "virtual_match_functor_dyn_move_semantics", "[virtual_pack_tests]" )
{
    templatious::DynamicVMatchFunctor dvmf;

    auto vp = SF::vpack<int>(1);
    int sum = 0;
    dvmf.attach(
        SF::virtualMatchFunctorPtr(
            SF::virtualMatch<const int>(
                [&](const int& inc) { sum += inc; }
            )
        )
    );

    templatious::DynamicVMatchFunctor moved(std::move(dvmf));

    bool succ = moved.tryMatch(vp);
    REQUIRE( succ );
    REQUIRE( sum == 1 );
}

TEST_CASE( "virtual_match_functor_dyn_clear", "[virtual_pack_tests]" )
{
    templatious::DynamicVMatchFunctor dvmf;

    auto vp = SF::vpack<int>(1);
    int sum = 0;
    dvmf.attach(
        SF::virtualMatchFunctorPtr(
            SF::virtualMatch<const int>(
                [&](const int& inc) { sum += inc; }
            )
        )
    );

    bool succ = dvmf.tryMatch(vp);
    REQUIRE( succ );
    REQUIRE( sum == 1 );

    dvmf.clear();

    succ = dvmf.tryMatch(vp);
    REQUIRE( !succ );
    REQUIRE( sum == 1 );
}

template <class... T>
auto makeVImpl(T&&... t)
    -> tt::t::VirtualMatchFunctorVImpl<
        tt::t::util::CopyOnlyStoragePolicy,
        decltype(std::forward<T>(t))...
    >
{
    return tt::t::VirtualMatchFunctorVImpl<
        tt::t::util::CopyOnlyStoragePolicy,
        decltype(std::forward<T>(t))...
    >(std::forward<T>(t)...);
}

TEST_CASE( "virtual_match_functor_const_coverage", "[virtual_pack_tests]" )
{
    int sum = 0;
    auto ptr = makeVImpl(
        SF::virtualMatch<const int>(
            [&](const int& inc) { sum += inc; }
        ), // two matches for linked list testing
        SF::virtualMatch<const int>(
            [&](const int& inc) { sum += inc; }
        )
    );
    typedef decltype(ptr) PtrType;
    const PtrType& cref(ptr);

    auto ncptr = makeVImpl(
        SF::virtualMatch<int>(
            [&](int& inc) { sum += inc; }
        )
    );
    typedef decltype(ncptr) NCPtrType;
    const NCPtrType& crefNc(ncptr);

    auto vp = SF::vpack<char>('7');
    typedef decltype(vp) VPType;
    const VPType& cvp(vp);

    auto vpm = SF::vpack<int>(1);
    typedef decltype(vpm) VPMType;
    const VPMType& cvpm(vpm);

    auto ccvpm = SF::vpack<const int>(1);
    typedef decltype(ccvpm) CVPMType;
    const CVPMType& cccvpm(ccvpm);

    {
        bool caught = false;
        try {
            ptr(vp);
        } catch (const tt::t::VirtualPackMatcherNoMatchException& e) {
            caught = true;
        }
        REQUIRE( caught );
    }

    {
        bool caught = false;
        try {
            ptr(cvp);
        } catch (const tt::t::VirtualPackMatcherNoMatchException& e) {
            caught = true;
        }
        REQUIRE( caught );
    }

    {
        bool caught = false;
        try {
            cref(vp);
        } catch (const tt::t::VirtualPackMatcherNoMatchException& e) {
            caught = true;
        }
        REQUIRE( caught );
    }

    {
        bool caught = false;
        try {
            cref(cvp);
        } catch (const tt::t::VirtualPackMatcherNoMatchException& e) {
            caught = true;
        }
        REQUIRE( caught );
    }

    {
        bool caught = false;
        try {
            ncptr(ccvpm);
        } catch (const tt::t::VirtualPackMatcherNoMatchException& e) {
            caught = true;
        }
        REQUIRE( caught );
    }

    {
        bool caught = false;
        try {
            ncptr(cccvpm);
        } catch (const tt::t::VirtualPackMatcherNoMatchException& e) {
            caught = true;
        }
        REQUIRE( caught );
    }

    {
        bool caught = false;
        try {
            crefNc(ccvpm);
        } catch (const tt::t::VirtualPackMatcherNoMatchException& e) {
            caught = true;
        }
        REQUIRE( caught );
    }

    {
        bool caught = false;
        try {
            crefNc(cccvpm);
        } catch (const tt::t::VirtualPackMatcherNoMatchException& e) {
            caught = true;
        }
        REQUIRE( caught );
    }

    REQUIRE( sum == 0 );

    ptr(vpm);
    ptr(cvpm);
    cref(vpm);
    cref(cvpm);

    REQUIRE( sum == 4 );
}
