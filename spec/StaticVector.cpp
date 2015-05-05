/*
 * =====================================================================================
 *
 *       Filename:  StaticVector.hpp
 *
 *    Description:  Static vector test
 *
 *        Version:  1.0
 *        Created:  07/28/2014 04:49:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Kazlauskas (dk), david@templatious.org
 *
 * =====================================================================================
 */

#include "../TestAlgs.hpp"

static const int SUM_1_TO_256 = 32896;

TEST_CASE( "static_vector_tests_basic", "[static_vector_tests]" )
{
    const size_t SIZE = 256;
    tt::t::StaticBuffer<int,SIZE> b;
    auto v = b.getStaticVector();

    REQUIRE(tt::existantCollectionTest(v));
}

TEST_CASE( "static_vector_tests_raw", "[static_vector_tests]" )
{
    const size_t SIZE = 256;
    tt::t::StaticBuffer<int,SIZE> b;
    auto v = b.getStaticVector();

    SA::add(v,SF::seqI(1,256));

    auto rb = v.rawBegin();
    auto re = v.rawEnd();

    int sum = 0;
    while (rb != re) {
        sum += *rb;
        ++(*rb);
        ++rb;
    }

    REQUIRE( sum == SUM_1_TO_256 );

    auto rcb = v.rawCBegin();
    auto rce = v.rawCEnd();
    sum = 0;
    while (rcb != rce) {
        sum += *rcb;
        ++rcb;
    }

    REQUIRE( sum == SUM_1_TO_256 + 256 );
}

TEST_CASE( "static_vector_tests_proxy", "[static_vector_tests]" )
{

    const size_t SIZE = 256;
    tt::t::StaticBuffer<int,SIZE> b;
    auto v = b.getStaticVector();

    REQUIRE(tt::proxyTest(v));
}

TEST_CASE( "static_vector_tests_proxy_const", "[static_vector_tests]" )
{

    const size_t SIZE = 256;
    tt::t::StaticBuffer<int,SIZE> b;
    auto v = b.getStaticVector();

    REQUIRE(tt::constProxyTest(v));
}

TEST_CASE( "static_vector_tests_virtual", "[static_vector_tests]" )
{

    const size_t SIZE = 256;
    tt::t::StaticBuffer<int,SIZE> b;
    auto v = b.getStaticVector();

    REQUIRE(tt::virtualTest(v));
}

TEST_CASE( "static_vector_tests_destruction", "[static_vector_tests]" )
{
    struct UniqueToken {};
    typedef tt::ConstructorCountCollection<UniqueToken> ValType;

    const size_t SIZE = 256;
    tt::t::StaticBuffer<ValType,SIZE> b;

    // limited scope
    {
        auto v = b.getStaticVector();

        REQUIRE(tt::constructionCountCollectionTest<UniqueToken>(v));
        REQUIRE(ValType::count() == 0);
        REQUIRE(SA::size(v) == 0);

        // native functionality
        for (int i = 0; i < 100; ++i) {
            v.pushFirst(ValType());
        }
        REQUIRE(ValType::count() == 100);

        int cnt = 0;
        while (v.size() > 0) {
            auto p = v.pop();
            ++cnt;
        }
        REQUIRE(ValType::count() == 0);
        REQUIRE(cnt == 100);


        for (int i = 0; i < 100; ++i) {
            v.push(ValType());
        }
        REQUIRE(ValType::count() == 100);
        REQUIRE(cnt == 100);

        cnt = 0;
        while (v.size() > 0) {
            auto p = v.popFirst();
            ++cnt;
        }
        REQUIRE(ValType::count() == 0);
        REQUIRE(cnt == 100);

        // should be taken care by destructor
        v.push(ValType());
        REQUIRE(ValType::count() == 1);
    }
    REQUIRE(ValType::count() == 0);
}

TEST_CASE( "static_vector_tests_static_buffer_basic", "[static_vector_tests]" )
{
    {
        tt::t::StaticBuffer<int,256> b;

        bool caught = false;
        try {
            b.getStaticVector(257);
        } catch (templatious::StaticBufferExceedException e) {
            caught = true;
        }
        REQUIRE( caught );
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
        REQUIRE( caught );
    }

    {
        tt::t::StaticBuffer<int,256> b;

        bool caught = false;
        try {
            b.getStaticVector(0);
        } catch (templatious::StaticBufferWrongSize e) {
            caught = true;
        }

        REQUIRE( caught );
    }

    {
        tt::t::StaticBuffer<int,256> b;

        bool caught = false;
        try {
            b.getStaticVector(-7);
        } catch (templatious::StaticBufferWrongSize e) {
            caught = true;
        }

        REQUIRE( caught );
    }
}

TEST_CASE( "static_vector_tests_static_buffer_split_integrity", "[static_vector_tests]" )
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

    REQUIRE( SA::size(filler) == 64 );
    REQUIRE( setSum == 256 * 7 );
    REQUIRE( sumNatural == sumSeq );
    REQUIRE( sumNatural == SUM_1_TO_256 );
    REQUIRE( canAdd == false );
}

TEST_CASE( "static_vector_tests_const_val", "[static_vector_tests]" )
{
    INIT_BALLER;

    tt::t::StaticBuffer<const int,256> sb;
    auto v = sb.getStaticVector();

    SA::add(v,SF::seqI(1,256));

    SM::forEach(sf,v);
    REQUIRE( sum == SUM_1_TO_256 );
    auto p = v.pop();
    REQUIRE( p == 256 );
    v.push(p);
    REQUIRE( v.top() == p );
    REQUIRE( v.isFull() );
}

TEST_CASE( "static_vector_tests_exception_correct", "[static_vector_tests]" )
{
    struct UniqueToken {};
    typedef tt::ConstructorCountCollection<UniqueToken> ValType;

    tt::t::StaticBuffer<ValType,256> sb;
    auto v = sb.getStaticVector();

    v.push(ValType());
    v.top().setState(7);
    REQUIRE( v.top().getState() == 7 );

    ValType::throwUp();

    {
        bool caught = false;
        try {
            v.push(ValType());
        } catch (const tt::CCountCollectionThrowUp& e) {
            caught = true;
        }
        REQUIRE( caught );
        REQUIRE( v.size() == 1 );
        REQUIRE( v.top().getState() == 7 );
    }

    {
        bool caught = false;
        try {
            v.emplaceBack();
        } catch (const tt::CCountCollectionThrowUp& e) {
            caught = true;
        }
        REQUIRE( caught );
        REQUIRE( v.size() == 1 );
        REQUIRE( v.top().getState() == 7 );
    }

    REQUIRE( ValType::count() == 1 );

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
        REQUIRE( caught );
        REQUIRE( v.size() == 1 );
        REQUIRE( v.top().getState() == 7 );
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
        REQUIRE( caught );
        REQUIRE( v.size() == 1 );
        REQUIRE( v.top().getState() == 7 );
    }

    REQUIRE( ValType::count() == 1);
    // exception safe pop
    v.popState();
    REQUIRE( ValType::count() == 0);

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

        REQUIRE( caughtAll );
    }

    // And that's the way the cookie crumbles.
    REQUIRE( ValType::count() == 0);
}

TEST_CASE( "static_vector_tests_move_semantics", "[static_vector_tests]" )
{
    INIT_BALLER;

    // this is the only correct/intended way to
    // instantiate StaticVector by the way
    tt::t::StaticBuffer<int,256> sb;
    auto v = sb.getStaticVector();
    SA::add(v,SF::seqI(1,256));
    REQUIRE(SA::size(v) == 256);

    auto b = std::move(v);

    REQUIRE( SA::size(v) == 0 );
    REQUIRE( SA::size(b) == 256 );

    sum = 0;
    SM::forEach(sf,b);
    REQUIRE( sum == SUM_1_TO_256 );

    sum = 0;
    SM::forEach(sf,v);
    REQUIRE( sum == 0 );
    REQUIRE( !SA::canAdd(v) );

    bool caught = false;
    try {
        SA::add(v,7);
    } catch (const templatious::StaticVectorMovedOperationException& e) {
        caught = true;
    }
    REQUIRE( caught );
    REQUIRE( SA::size(v) == 0 );
}

TEST_CASE( "static_vector_element_move", "[static_vector_tests]" )
{
    tt::t::StaticBuffer<MovablePod,256> sb;
    auto v = sb.getStaticVector();

    REQUIRE( moveTest(v) );
}

struct DummyDefVar {
    DummyDefVar() : _memb(7) {}

    int _memb;
};

struct DummyNoDefCtor {
    DummyNoDefCtor() = delete;

    DummyNoDefCtor(int val) : _memb(val) {}

    int _memb;
};

TEST_CASE( "static_vector_preallocate", "[static_vector_tests]" )
{
    std::aligned_storage<sizeof(DummyDefVar),alignof(DummyDefVar)>::type stor[16];

    tt::t::StaticVector<DummyDefVar> vct(reinterpret_cast<DummyDefVar*>(stor),16,8);

    int sum = SM::fold(0,[](int i,const DummyDefVar& d) { return i + d._memb; },vct);

    REQUIRE( sum == 7 * 8 );
}

TEST_CASE( "static_vector_no_def_ctor_alloc", "[static_vector_tests]" )
{
    tt::t::StaticBuffer<DummyNoDefCtor,16> b;

    auto v = b.getStaticVector();
    auto val = DummyNoDefCtor(7);
    SA::add(v,val,val,val,val,val,val,val);

    int sum = SM::fold(0,[](int i,const DummyNoDefCtor& d)
            { return i + d._memb; },v);

    REQUIRE( sum == 7 * 7 );
}

TEST_CASE( "static_vector_full_throw", "[static_vector_tests]" )
{
    tt::t::StaticBuffer<int,16> b;
    auto v = b.getStaticVector();
    SA::add(v,SF::seqL(16));

    bool caught = false;
    try {
        SA::add(v,7);
    } catch (const tt::t::StaticVectorFullAddException& e) {
        caught = true;
    }

    REQUIRE( caught );
}
