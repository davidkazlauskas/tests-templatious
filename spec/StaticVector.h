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
            v.pushFirst(ValType());
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
            auto p = v.popFirst();
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

BOOST_AUTO_TEST_CASE( static_vector_tests_static_buffer_split_integrity )
{
    INIT_BALLER;

    tt::t::StaticBuffer<int,256> sb;

    auto a = sb.getStaticVector(64);
    auto b = sb.getStaticVector(64);
    auto c = sb.getStaticVector(64);
    auto d = sb.getStaticVector(64);

    auto s = SF::seqI(1,256);
    auto filler = SF::seqL(7,64+7);

    SA::add(a,filler);
    SA::add(b,filler);
    SA::add(c,filler);
    SA::add(d,filler);

    SM::set(7,a,b,c,d);
    sum = 0;
    SM::forEach(sf,a,b,c,d);
    int setSum = sum;

    SM::distribute(s,a,b,c,d);

    sum = 0;
    SM::forEach(sf,a,b,c,d);
    int sumNatural = sum;

    sum = 0;
    SM::forEach(sf,s);
    int sumSeq = sum;

    bool canAdd = false;
    canAdd |= SA::canAdd(a);
    canAdd |= SA::canAdd(b);
    canAdd |= SA::canAdd(c);
    canAdd |= SA::canAdd(d);

    BOOST_CHECK( SA::size(filler) == 64 );
    BOOST_CHECK( setSum == 256 * 7 );
    BOOST_CHECK( sumNatural == sumSeq );
    BOOST_CHECK( sumNatural == 32896 );
    BOOST_CHECK( canAdd == false );
}

BOOST_AUTO_TEST_CASE( static_vector_tests_const_val )
{
    INIT_BALLER;

    tt::t::StaticBuffer<const int,256> sb;
    auto v = sb.getStaticVector();

    SA::add(v,SF::seqI(1,256));

    SM::forEach(sf,v);
    BOOST_CHECK( sum == 32896 );
    auto p = v.pop();
    BOOST_CHECK( p == 256 );
    v.push(p);
    BOOST_CHECK( v.top() == p );
    BOOST_CHECK( v.isFull() );
}

BOOST_AUTO_TEST_CASE( static_vector_tests_exception_correct )
{
    INIT_BALLER;

    struct UniqueToken {};
    typedef tt::ConstructorCountCollection<UniqueToken> ValType;

    tt::t::StaticBuffer<ValType,256> sb;
    auto v = sb.getStaticVector();

    v.push(ValType());
    v.top().setState(7);
    BOOST_CHECK( v.top().getState() == 7 );

    ValType::throwUp();

    {
        bool caught = false;
        try {
            v.push(ValType());
        } catch (const tt::CCountCollectionThrowUp& e) {
            caught = true;
        }
        BOOST_CHECK( caught );
        BOOST_CHECK( v.size() == 1 );
        BOOST_CHECK( v.top().getState() == 7 );
    }

    {
        bool caught = false;
        try {
            v.emplace();
        } catch (const tt::CCountCollectionThrowUp& e) {
            caught = true;
        }
        BOOST_CHECK( caught );
        BOOST_CHECK( v.size() == 1 );
        BOOST_CHECK( v.top().getState() == 7 );
    }

    BOOST_CHECK( ValType::count() == 1 );

    {
        bool caught = false;
        try {
            v.pop();
            // don't lose value after exception
            // as internally copy has to be
            // constructed and then moved
            //
            // However, here it is lucky because if
            // both constructors fail state is unchanged,
            // but if first constructor succeeds and move
            // fails, then vector lost an element.
        } catch (const tt::CCountCollectionThrowUp& e) {
            caught = true;
        }
        BOOST_CHECK( caught );
        BOOST_CHECK( v.size() == 1 );
        BOOST_CHECK( v.top().getState() == 7 );
    }

    {
        bool caught = false;
        ValType::heal(); // allow temp var creation
        auto tmp = ValType();
        ValType::throwUp();
        try {
            v.pop(tmp);
        } catch (const tt::CCountCollectionThrowUp& e) {
            caught = true;
        }
        BOOST_CHECK( caught );
        BOOST_CHECK( v.size() == 1 );
        BOOST_CHECK( v.top().getState() == 7 );
    }

    BOOST_CHECK( ValType::count() == 1);
    // exception safe pop
    v.popState();
    BOOST_CHECK( ValType::count() == 0);

    {
        bool caughtAll = true;

        try {
            v.pop();
            caughtAll &= false;
        } catch (const tt::t::StaticVectorEmptyPopException& e) {
            caughtAll &= true;
        }

        try {
            v.popFirst();
            caughtAll &= false;
        } catch (const tt::t::StaticVectorEmptyPopException& e) {
            caughtAll &= true;
        }

        try {
            v.popState();
            caughtAll &= false;
        } catch (const tt::t::StaticVectorEmptyPopException& e) {
            caughtAll &= true;
        }

        BOOST_CHECK( caughtAll );
    }

    // And that's the way the cookie crumbles.
    BOOST_CHECK( ValType::count() == 0);
}

BOOST_AUTO_TEST_SUITE_END();

#endif /* end of include guard: STATICVECTOR_8L32QS9F */
