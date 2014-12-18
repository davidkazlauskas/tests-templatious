/*
 * =====================================================================================
 *
 *       Filename:  StaticVector.h
 *
 *    Description:  Static vector test
 *
 *        Version:  1.0
 *        Created:  07/28/2014 04:49:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef STATICVECTOR_8L32QS9F
#define STATICVECTOR_8L32QS9F

#include "../TestAlgs.h"

BOOST_AUTO_TEST_SUITE( static_vector_tests );

BOOST_AUTO_TEST_CASE( static_vector_tests_basic )
{

    const size_t SIZE = 256; 
    tt::t::StaticBuffer<int,SIZE> b;
    auto v = b.getStaticVector();

    BOOST_CHECK(tt::existantCollectionTest(v));
}

BOOST_AUTO_TEST_CASE( static_vector_tests_proxy )
{

    const size_t SIZE = 256;
    tt::t::StaticBuffer<int,SIZE> b;
    auto v = b.getStaticVector();

    BOOST_CHECK(tt::proxyTest(v));
}

BOOST_AUTO_TEST_CASE( static_vector_tests_proxy_const )
{

    const size_t SIZE = 256;
    tt::t::StaticBuffer<int,SIZE> b;
    auto v = b.getStaticVector();

    BOOST_CHECK(tt::constProxyTest(v));
}

BOOST_AUTO_TEST_CASE( static_vector_tests_virtual )
{

    const size_t SIZE = 256;
    tt::t::StaticBuffer<int,SIZE> b;
    auto v = b.getStaticVector();

    BOOST_CHECK(tt::virtualTest(v));
}

BOOST_AUTO_TEST_CASE( static_vector_tests_destruction )
{
    struct UniqueToken {};
    typedef tt::ConstructorCountCollection<UniqueToken> ValType;

    const size_t SIZE = 256;
    tt::t::StaticBuffer<ValType,SIZE> b;

    // limited scope
    {
        auto v = b.getStaticVector();

        BOOST_CHECK(tt::constructionCountCollectionTest<UniqueToken>(v));
        BOOST_CHECK(ValType::count() == 0);
        BOOST_CHECK(SA::size(v) == 0);

        // native functionality
        for (int i = 0; i < 100; ++i) {
            v.push_first(ValType());
        }
        BOOST_CHECK(ValType::count() == 100);

        int cnt = 0;
        while (v.size() > 0) {
            auto p = v.pop();
            ++cnt;
        }
        BOOST_CHECK(ValType::count() == 0);
        BOOST_CHECK(cnt == 100);


        for (int i = 0; i < 100; ++i) {
            v.push(ValType());
        }
        BOOST_CHECK(ValType::count() == 100);
        BOOST_CHECK(cnt == 100);

        cnt = 0;
        while (v.size() > 0) {
            auto p = v.pop_first();
            ++cnt;
        }
        BOOST_CHECK(ValType::count() == 0);
        BOOST_CHECK(cnt == 100);

        // should be taken care by destructor
        v.push(ValType());
        BOOST_CHECK(ValType::count() == 1);
    }
    BOOST_CHECK(ValType::count() == 0);
}

BOOST_AUTO_TEST_CASE( static_vector_tests_static_buffer_basic )
{
    TEMPLATIOUS_TRIPLET_STD;

    {
        tt::t::StaticBuffer<int,256> b;

        bool caught = false;
        try {
            b.getStaticVector(257);
        } catch (templatious::StaticBufferExceedException e) {
            caught = true;
        }
        BOOST_CHECK( caught );
    }

    {
        tt::t::StaticBuffer<int,256> b;

        b.getStaticVector(256);
        bool caught = false;
        try {
            b.getStaticVector(1);
        } catch (templatious::StaticBufferExceedException e) {
            caught = true;
        }
        BOOST_CHECK( caught );
    }

    {
        tt::t::StaticBuffer<int,256> b;

        bool caught = false;
        try {
            b.getStaticVector(0);
        } catch (templatious::StaticBufferWrongSize e) {
            caught = true;
        }

        BOOST_CHECK( caught );
    }

    {
        tt::t::StaticBuffer<int,256> b;

        bool caught = false;
        try {
            b.getStaticVector(-7);
        } catch (templatious::StaticBufferWrongSize e) {
            caught = true;
        }

        BOOST_CHECK( caught );
    }
}

BOOST_AUTO_TEST_SUITE_END();

#endif /* end of include guard: STATICVECTOR_8L32QS9F */
