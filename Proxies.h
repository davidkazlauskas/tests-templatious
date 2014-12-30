/*
 * =====================================================================================
 *
 *       Filename:  Proxies.h
 *
 *    Description:  Proxy tests
 *
 *        Version:  1.0
 *        Created:  08/07/2014 07:59:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef PROXIES_SM4YKVBX
#define PROXIES_SM4YKVBX

#include "TestDefs.h"

namespace test_templatious {


template <class T>
void setCollection_prx(T&& c);

template <class T>
auto sum(T&& c)
    -> decltype(*(SA::begin(c)) + *(SA::begin(c)))
{
    typedef decltype(*(SA::begin(c)) + *(SA::begin(c))) Val;
    Val sum = 0;

    TEMPLATIOUS_FOREACH(Val i,c) {
        sum += i;
    }

    Val sum2 = 0;
    for (auto i = SA::cbegin(c); i != SA::cend(c); ++i) {
        sum2 += *i;
    }

    if (sum != sum2) {
        Val retVal = sum + 1;
        if (sum2 == retVal) {
            return retVal + 1;
        } else {
            return retVal;
        }
    }

    return sum;
}

// ONE LEVEL TESTS
template <class T>
bool proxyTest(T&& c);
template <class T>
bool setColTest(T&& c);

template <class T>
bool oneSkipTest(T&& c);
template <class T>
bool twoSkipTest(T&& c);
template <class T>
bool oneFilterTest(T&& c);
template <class T>
bool oneRangeTest(T&& c);
// ONE LEVEL TESTS
//
// LEVEL TWO TESTS
template <class T>
bool _2_rangeSkipTest(T&& c);
template <class T>
bool _2_skipRangeTest(T&& c);
template <class T>
bool _2_filterSkipTest(T&& c);
template <class T>
bool _2_skipFilterTest(T&& c);
template <class T>
bool _2_filterRangeTest(T&& c);
template <class T>
bool _2_rangeFilterTest(T&& c);
// LEVEL TWO TESTS
//
// LEVEL THREE TESTS
template <class T>
bool _3_rangeFilterSkipTest(T&& c);
template <class T>
bool _3_rangeSkipFilterTest(T&& c);
template <class T>
bool _3_filterRangeSkipTest(T&& c);
template <class T>
bool _3_filterSkipRangeTest(T&& c);
template <class T>
bool _3_skipRangeFilterTest(T&& c);
template <class T>
bool _3_skipFilterRangeTest(T&& c);
// LEVEL THREE TESTS

// CONST DUP TESTS
template <class T>
bool _1_c_oneSkipTest(const T& c);
template <class T>
bool _1_c_twoSkipTest(const T& c);
template <class T>
bool _1_c_oneFilterTest(const T& c);
template <class T>
bool _1_c_oneRangeTest(const T& c);
// ONE LEVEL TESTS
//
// LEVEL TWO TESTS
template <class T>
bool _2_c_rangeSkipTest(const T& c);
template <class T>
bool _2_c_skipRangeTest(const T& c);
template <class T>
bool _2_c_filterSkipTest(const T& c);
template <class T>
bool _2_c_skipFilterTest(const T& c);
template <class T>
bool _2_c_filterRangeTest(const T& c);
template <class T>
bool _2_c_rangeFilterTest(const T& c);
// LEVEL TWO TESTS
//
// LEVEL THREE TESTS
template <class T>
bool _3_c_rangeFilterSkipTest(const T& c);
template <class T>
bool _3_c_rangeSkipFilterTest(const T& c);
template <class T>
bool _3_c_filterRangeSkipTest(const T& c);
template <class T>
bool _3_c_filterSkipRangeTest(const T& c);
template <class T>
bool _3_c_skipRangeFilterTest(const T& c);
template <class T>
bool _3_c_skipFilterRangeTest(const T& c);
// LEVEL THREE TESTS
// CONST DUP TESTS

// CLEARANCE TESTS
// 1st level
template <class T>
bool _1_cl_oneSkip(T&& c);
template <class T>
bool _1_cl_twoSkip(T&& c);
template <class T>
bool _1_cl_oneFilter(T&& c);
template <class T>
bool _1_cl_oneRange(T&& c);
// 2nd level
template <class T>
bool _2_cl_filterRange(T&& c);
template <class T>
bool _2_cl_filterSkip(T&& c);
template <class T>
bool _2_cl_rangeFilter(T&& c);
template <class T>
bool _2_cl_rangeSkip(T&& c);
template <class T>
bool _2_cl_skipFilter(T&& c);
template <class T>
bool _2_cl_skipRange(T&& c);
// 3rd level
template <class T>
bool _3_cl_rangeFilterSkip(T&& c);
template <class T>
bool _3_cl_rangeSkipFilter(T&& c);
template <class T>
bool _3_cl_filterRangeSkip(T&& c);
template <class T>
bool _3_cl_filterSkipRange(T&& c);
template <class T>
bool _3_cl_skipRangeFilter(T&& c);
template <class T>
bool _3_cl_skipFilterRange(T&& c);
// boundary
template <class T>
bool _stf_sum_range(T&& c);
template <class T>
bool _stf_sum_range_end(T&& c);

template <class T>
bool _stf_cl_range(T&& c);
template <class T>
bool _stf_cl_range_end(T&& c);
// CLEARANCE TESTS

// MOVE SEMANTICS
template <class T>
bool moveSemanticsTest(T&& c);

// CLEARANCE ASSERTIONS
template <class T>
bool clearanceAssertionsTest(T&& c);

// SIZE TESTS
template <class T>
bool proxySizeTests(T&& c);

template <class T>
bool proxyEmptyCollectionTests(T&& c);

//
//
// VARIATIONS
// 1 level:
#define _1_oneSkip(c) SF::skip(c,2) // 2450
#define _1_twoSkip(c) SF::skip(c,3) // 1683
#define _1_oneFilter(c) SF::filter(c,[](int i) {\
            return i > 19; \
        }) // 4760
#define _1_oneRange(c) SF::range(c,17,57) // 1460
// --> 1 level

// 2 level:
#define _2_rangeSkip(c) SF::skip(SF::range(c,17,57),3) // 511
#define _2_skipRange(c) SF::range(SF::skip(c,3),1,10) // 135
#define _2_skipFilter(c) SF::filter(SF::skip(c,3),\
        [](int i) { return i > 50; }) // 1275
#define _2_filterSkip(c) SF::skip(SF::filter(c,\
            [](int i) { return i > 50; }),3) // 1275
#define _2_filterRange(c) SF::range(SF::filter(c,\
            [](int i) { return i > 50; }),17,47) // 2475
#define _2_rangeFilter(c) SF::filter(SF::range(c,17,57),\
        [](int i) { return i > 50; }) // 321
// --> 2 level

// 3 level:
#define _3_rangeFilterSkip(c) SF::skip(SF::filter(SF::range(c,17,57),\
            [](int i) { return i > 50; }),3) // 105
#define _3_rangeSkipFilter(c) SF::filter(SF::skip(SF::range(c,17,57),3),\
        [](int i) { return i > 20; }) // 474
#define _3_filterRangeSkip(c) SF::skip(SF::range(SF::filter(c,\
        [](int i) { return i > 50; }),17,37),3) // 539
#define _3_filterSkipRange(c) SF::range(SF::skip(SF::filter(c,\
        [](int i) { return i > 50; }),3),7,14) // 567
#define _3_skipRangeFilter(c) SF::filter(SF::range(SF::skip(c,3),17,27),\
        [](int i) { return i > 60; }) // 423
#define _3_skipFilterRange(c) SF::range(SF::filter(SF::skip(c,3),\
        [](int i) { return i > 37; }),7,17) // 735
// --> 3 level

// start to finish
#define _stf_range(c) SF::range(c,0,17) // 136
#define _stf_range_end(c) SF::range(c,77,100) // 2024
// --> start to finish

// VARIATIONS

template <class T>
bool proxyTest(T&& c) {
    IFN_SECTOR_START( "proxy test" );

    // 1st lv
    IFN_RET_FALSE(setColTest(c));
    IFN_RET_FALSE(oneSkipTest(c));
    IFN_RET_FALSE(twoSkipTest(c));
    IFN_RET_FALSE(oneFilterTest(c));
    IFN_RET_FALSE(oneRangeTest(c));

    // 2nd lv
    IFN_RET_FALSE(_2_rangeSkipTest(c));
    IFN_RET_FALSE(_2_skipRangeTest(c));
    IFN_RET_FALSE(_2_filterSkipTest(c));
    IFN_RET_FALSE(_2_skipFilterTest(c));
    IFN_RET_FALSE(_2_filterRangeTest(c));
    IFN_RET_FALSE(_2_rangeFilterTest(c));

    // 3rd lv
    IFN_RET_FALSE(_3_rangeFilterSkipTest(c));
    IFN_RET_FALSE(_3_rangeSkipFilterTest(c));
    IFN_RET_FALSE(_3_filterRangeSkipTest(c));
    IFN_RET_FALSE(_3_filterSkipRangeTest(c));
    IFN_RET_FALSE(_3_skipRangeFilterTest(c));
    IFN_RET_FALSE(_3_skipFilterRangeTest(c));

    // boundary
    IFN_RET_FALSE(_stf_sum_range(c));
    IFN_RET_FALSE(_stf_sum_range_end(c));

    return true;
}

template <class T>
bool constProxyTest(T&& c) {
    setCollection_prx(c);
    IFN_SECTOR_START( "const proxy test" );

    // 1st lv
    IFN_RET_FALSE(_1_c_oneSkipTest(c));
    IFN_RET_FALSE(_1_c_twoSkipTest(c));
    IFN_RET_FALSE(_1_c_oneFilterTest(c));
    IFN_RET_FALSE(_1_c_oneRangeTest(c));

    // 2nd lv
    IFN_RET_FALSE(_2_c_rangeSkipTest(c));
    IFN_RET_FALSE(_2_c_skipRangeTest(c));
    IFN_RET_FALSE(_2_c_filterSkipTest(c));
    IFN_RET_FALSE(_2_c_skipFilterTest(c));
    IFN_RET_FALSE(_2_c_filterRangeTest(c));
    IFN_RET_FALSE(_2_c_rangeFilterTest(c));

    // 3rd lv
    IFN_RET_FALSE(_3_c_rangeFilterSkipTest(c));
    IFN_RET_FALSE(_3_c_rangeSkipFilterTest(c));
    IFN_RET_FALSE(_3_c_filterRangeSkipTest(c));
    IFN_RET_FALSE(_3_c_filterSkipRangeTest(c));
    IFN_RET_FALSE(_3_c_skipRangeFilterTest(c));
    IFN_RET_FALSE(_3_c_skipFilterRangeTest(c));

    return true;
}

template <class T>
bool proxyDeletionTest(T&& c) {
    IFN_SECTOR_START( "proxy deletion test" );

    // 1st lv
    IFN_RET_FALSE(_1_cl_oneSkip(c));
    IFN_RET_FALSE(_1_cl_oneRange(c));
    IFN_RET_FALSE(_1_cl_oneFilter(c));
    IFN_RET_FALSE(_1_cl_oneRange(c));

    // 2nd lv
    IFN_RET_FALSE(_2_cl_filterRange(c));
    IFN_RET_FALSE(_2_cl_filterSkip(c));
    IFN_RET_FALSE(_2_cl_rangeFilter(c));
    IFN_RET_FALSE(_2_cl_rangeSkip(c));
    IFN_RET_FALSE(_2_cl_skipFilter(c));
    IFN_RET_FALSE(_2_cl_skipRange(c));

    // 3rd lv
    IFN_RET_FALSE(_3_cl_rangeFilterSkip(c));
    IFN_RET_FALSE(_3_cl_rangeSkipFilter(c));
    IFN_RET_FALSE(_3_cl_filterRangeSkip(c));
    IFN_RET_FALSE(_3_cl_filterSkipRange(c));
    IFN_RET_FALSE(_3_cl_skipRangeFilter(c));
    IFN_RET_FALSE(_3_cl_skipFilterRange(c));

    // boundary
    IFN_RET_FALSE(_stf_cl_range(c));
    IFN_RET_FALSE(_stf_cl_range_end(c));

    // move
    IFN_RET_FALSE(moveSemanticsTest(c));

    // clearance assertions
    IFN_RET_FALSE(clearanceAssertionsTest(c));

    // sizing tests
    IFN_RET_FALSE(proxySizeTests(c));

    // empty proxies
    IFN_RET_FALSE(proxyEmptyCollectionTests(c));
    return true;
}

template <class T>
bool moveSemanticsTest(T&& c) {
    TEMPLATIOUS_TRIPLET_STD;
    IFN_SECTOR_START( "move semantics tests" );

    {
        setCollection_prx(c);
        auto s = _1_twoSkip(std::move(c));
        IFN_RET_FALSE(SA::size(c) == 0);
    }

    {
        setCollection_prx(c);
        auto s = _1_oneRange(std::move(c));
        IFN_RET_FALSE(SA::size(c) == 0);
    }

    {
        setCollection_prx(c);
        auto s = _1_oneFilter(std::move(c));
        IFN_RET_FALSE(SA::size(c) == 0);
    }

    {
        setCollection_prx(c);
        auto flt = _1_oneFilter(std::move(c));
        auto s = _1_oneSkip(std::move(flt));

        IFN_RET_FALSE(SA::size(c) == 0);
        IFN_RET_FALSE(SA::size(flt._c.getRef()) == 0);
        IFN_RET_FALSE(SA::size(s._c.getRef()) == -1);
    }

    {
        setCollection_prx(c);
        auto flt = _1_oneFilter(std::move(c));
        auto s = _1_oneRange(std::move(flt));

        IFN_RET_FALSE(SA::size(c) == 0);
        IFN_RET_FALSE(SA::size(flt._c.getRef()) == 0);
        IFN_RET_FALSE(SA::size(s._c.getRef()) == -1);
    }

    {
        setCollection_prx(c);
        auto r = _1_oneSkip(std::move(c));
        auto s = _1_oneRange(std::move(r));

        IFN_RET_FALSE(SA::size(c) == 0);
        IFN_RET_FALSE(SA::size(r._c.getRef()) == 0);
        IFN_RET_FALSE(SA::size(s._c.getRef()) == -1);
    }

    {
        setCollection_prx(c);
        auto r = _1_oneRange(std::move(c));
        auto s = _1_oneSkip(std::move(r));
        auto flt = _1_oneFilter(std::move(s));

        IFN_RET_FALSE(SA::size(c) == 0);
        IFN_RET_FALSE(SA::size(r._c.getRef()) == 0);
        IFN_RET_FALSE(SA::size(s._c.getRef()._c.getRef()) == 0);
        IFN_RET_FALSE(SA::size(flt._c.getRef()) == -1);
    }

    return true;
}

template <class T>
bool proxyExceptionTestSuite(T& c) {
    TEMPLATIOUS_TRIPLET_STD;
    IFN_SECTOR_START( "proxy clearance exception suite" );

    {
        bool caught = false;
        try {
            SA::begin(c);
        } catch (t::ProxyClearedUsageException e) {
            caught = true;
        }
        IFN_RET_FALSE(caught);
    }

    {
        bool caught = false;
        try {
            SA::end(c);
        } catch (t::ProxyClearedUsageException e) {
            caught = true;
        }
        IFN_RET_FALSE(caught);
    }

    {
        bool caught = false;
        try {
            SA::cbegin(c);
        } catch (t::ProxyClearedUsageException e) {
            caught = true;
        }
        IFN_RET_FALSE(caught);
    }

    {
        bool caught = false;
        try {
            SA::cend(c);
        } catch (t::ProxyClearedUsageException e) {
            caught = true;
        }
        IFN_RET_FALSE(caught);
    }

    {
        bool caught = false;
        try {
            SA::iterAt(c,7);
        } catch (t::ProxyClearedUsageException e) {
            caught = true;
        }
        IFN_RET_FALSE(caught);
    }

    return true;
}

template <class T>
bool clearanceAssertionsTest(T&& c) {
    TEMPLATIOUS_TRIPLET_STD;
    IFN_SECTOR_START( "clearance assertion tests" );

    {
        setCollection_prx(c);
        auto r = _1_oneRange(c);
        SA::clear(r);

        IFN_RET_FALSE(proxyExceptionTestSuite(r));
    }

    {
        setCollection_prx(c);
        auto r = _1_oneSkip(c);
        SA::clear(r);

        IFN_RET_FALSE(proxyExceptionTestSuite(r));
    }

    {
        setCollection_prx(c);
        auto r = _1_oneFilter(c);
        SA::clear(r);

        IFN_RET_FALSE(proxyExceptionTestSuite(r));
    }

    return true;
}

template <class T>
bool proxySizeTests(T&& c) {
    // all proxies should return -1
    // as size is not usually clear (like filter)
    // or trivial to calculate, or costly
    // to calculate.
    TEMPLATIOUS_TRIPLET_STD;
    IFN_SECTOR_START( "proxy sizing tests" );

    {
        setCollection_prx(c);
        auto f = _1_oneFilter(c);

        IFN_RET_FALSE(SA::size(f) == -1);
    }

    {
        setCollection_prx(c);
        auto f = _1_oneSkip(c);

        IFN_RET_FALSE(SA::size(f) == -1);
    }

    {
        setCollection_prx(c);
        auto f = _1_oneRange(c);

        IFN_RET_FALSE(SA::size(f) == -1);
    }

    {
        setCollection_prx(c);
        auto f = _1_oneFilter(c);

        IFN_RET_FALSE(SA::trueSize(f) == 80);
    }

    {
        setCollection_prx(c);
        auto f = _1_oneSkip(c);

        IFN_RET_FALSE(SA::trueSize(f) == 50);
    }

    {
        setCollection_prx(c);
        auto f = _1_oneRange(c);

        IFN_RET_FALSE(SA::trueSize(f) == 40);
    }

    return true;
}

template <class T>
bool proxyEmptyCollectionTests(T&& c) {
    IFN_SECTOR_START("proxy empty collections");

    SA::clear(c);
    {
        auto f = _1_oneFilter(c);
        IFN_RET_FALSE( SA::trueSize(f) == 0 );
    }
    {
        // anything else than 0,0
        // throws out of range exception
        auto f = SF::range(c,0,0);
        IFN_RET_FALSE( SA::trueSize(f) == 0 );
    }
    {
        auto f = _1_oneSkip(c);
        IFN_RET_FALSE( SA::trueSize(f) == 0 );
    }
    {
        auto f = _2_filterSkip(c);
        IFN_RET_FALSE( SA::trueSize(f) == 0 );
    }

    return true;
}

static const int PROX_SUM = 4950;

template <class T>
bool oneSkipTest(T&& c) {
    setCollection_prx(c);

    auto s = _1_oneSkip(c);

    return sum(s) == 2450;
}

template <class T>
bool twoSkipTest(T&& c) {
    setCollection_prx(c);

    auto s = _1_twoSkip(c);
    return sum(s) == 1683;
}

template <class T>
bool oneFilterTest(T&& c) {
    setCollection_prx(c);

    auto f = _1_oneFilter(c);

    return sum(f) == 4760;
}

template <class T>
bool oneRangeTest(T&& c) {
    setCollection_prx(c);

    auto r = _1_oneRange(c);

    return sum(r) == 1460;
}

template <class T>
bool _2_rangeSkipTest(T&& c) {
    setCollection_prx(c);

    auto r = _2_rangeSkip(c);

    return sum(r) == 511;
}

template <class T>
bool _2_skipRangeTest(T&& c) {
    setCollection_prx(c);

    auto r = _2_skipRange(c);

    return sum(r) == 135;
}

template <class T>
bool _2_filterSkipTest(T&& c) {
    setCollection_prx(c);

    auto r = _2_filterSkip(c);

    return sum(r) == 1275;
}

template <class T>
bool _2_skipFilterTest(T&& c) {
    setCollection_prx(c);

    auto r = _2_skipFilter(c);

    return sum(r) == 1275;
}

template <class T>
bool _2_filterRangeTest(T&& c) {
    setCollection_prx(c);

    auto r = _2_filterRange(c);

    return sum(r) == 2475;
}

template <class T>
bool _2_rangeFilterTest(T&& c) {
    setCollection_prx(c);

    auto r = _2_rangeFilter(c);

    return sum(r) == 321;
}

// 3RD LEVEL
template <class T>
bool _3_rangeFilterSkipTest(T&& c) {
    setCollection_prx(c);

    auto r = _3_rangeFilterSkip(c);

    return sum(r) == 105;
}

template <class T>
bool _3_rangeSkipFilterTest(T&& c) {
    setCollection_prx(c);

    auto r =
        SF::filter(
            SF::skip(
                SF::range(c, 17, 57)
            ,3),
        [](int i) {
            return i > 20;
        });

    return sum(r) == 474;
}

template <class T>
bool _3_filterRangeSkipTest(T&& c) {
    setCollection_prx(c);

    auto r = _3_filterRangeSkip(c);

    return sum(r) == 539;
}

template <class T>
bool _3_filterSkipRangeTest(T&& c) {
    setCollection_prx(c);

    auto r = _3_filterSkipRange(c);

    return sum(r) == 567;
}

template <class T>
bool _3_skipRangeFilterTest(T&& c) {
    setCollection_prx(c);

    auto r = _3_skipRangeFilter(c);

    return sum(r) == 423;
}

template <class T>
bool _3_skipFilterRangeTest(T&& c) {
    setCollection_prx(c);

    auto r = _3_skipFilterRange(c);

    return sum(r) == 735;
}
// 3RD LEVEL

// CONST TESTS IMPL
template <class T>
bool _2_rangeSkipTest(T&& c);
template <class T>
bool _2_skipRangeTest(T&& c);
template <class T>
bool _2_filterSkipTest(T&& c);
template <class T>
bool _2_skipFilterTest(T&& c);
template <class T>
bool _2_filterRangeTest(T&& c);
template <class T>
bool _2_rangeFilterTest(T&& c);
// LEVEL TWO TESTS
//
// LEVEL THREE TESTS
template <class T>
bool _3_rangeFilterSkipTest(T&& c);
template <class T>
bool _3_rangeSkipFilterTest(T&& c);
template <class T>
bool _3_filterRangeSkipTest(T&& c);
template <class T>
bool _3_filterSkipRangeTest(T&& c);
template <class T>
bool _3_skipRangeFilterTest(T&& c);
template <class T>
bool _3_skipFilterRangeTest(T&& c);
// LEVEL THREE TESTS

///////////////////////////////////////////////////////////////

// CONST DUP TESTS
template <class T>
bool _1_c_oneSkipTest(const T& c) {
    auto r = _1_oneSkip(c);
    return sum(r) == 2450;
}

template <class T>
bool _1_c_twoSkipTest(const T& c) {
    auto r = _1_twoSkip(c);
    return sum(r) == 1683;
}

template <class T>
bool _1_c_oneFilterTest(const T& c) {
    auto r = _1_oneFilter(c);
    return sum(r) == 4760;
}

template <class T>
bool _1_c_oneRangeTest(const T& c) {
    auto r = _1_oneRange(c);
    return sum(r) == 1460;
}
// ONE LEVEL TESTS
//
// LEVEL TWO TESTS
template <class T>
bool _2_c_rangeSkipTest(const T& c) {
    auto r = _2_rangeSkip(c);
    return sum(r) == 511;
}

template <class T>
bool _2_c_skipRangeTest(const T& c) {
    auto r = _2_skipRange(c);
    return sum(r) == 135;
}

template <class T>
bool _2_c_filterSkipTest(const T& c) {
    auto r = _2_skipFilter(c);
    return sum(r) == 1275;
}

template <class T>
bool _2_c_skipFilterTest(const T& c) {
    auto r = _2_filterSkip(c);
    return sum(r) == 1275;
}

template <class T>
bool _2_c_filterRangeTest(const T& c) {
    auto r = _2_filterRange(c);
    return sum(r) == 2475;
}

template <class T>
bool _2_c_rangeFilterTest(const T& c) {
    auto r = _2_rangeFilter(c);
    return sum(r) == 321;
}

// LEVEL TWO TESTS
//
// LEVEL THREE TESTS
template <class T>
bool _3_c_rangeFilterSkipTest(const T& c) {
    auto r = _3_rangeFilterSkip(c);
    return sum(r) == 105;
}

template <class T>
bool _3_c_rangeSkipFilterTest(const T& c) {
    auto r = _3_rangeSkipFilter(c);
    return sum(r) == 474;
}

template <class T>
bool _3_c_filterRangeSkipTest(const T& c) {
    auto r = _3_filterRangeSkip(c);
    return sum(r) == 539;
}

template <class T>
bool _3_c_filterSkipRangeTest(const T& c) {
    auto r = _3_filterSkipRange(c);
    return sum(r) == 567;
}

template <class T>
bool _3_c_skipRangeFilterTest(const T& c) {
    auto r = _3_skipRangeFilter(c);
    return sum(r) == 423;
}

template <class T>
bool _3_c_skipFilterRangeTest(const T& c) {
    auto r = _3_skipFilterRange(c);
    return sum(r) == 735;
}
// CONST TESTS IMPL

// CLEARANCE TESTS
// 1 level
template <class T>
bool _1_cl_oneSkip(T&& c) {
    setCollection_prx(c);
    int s = sum(c);

    auto r = _1_oneSkip(c);

    int a = sum(r);
    SA::clear(r);
    int s2 = sum(c);

    return s == a + s2;
}

template <class T>
bool _1_cl_twoSkip(T&& c) {
    setCollection_prx(c);
    int s = sum(c);

    auto r = _1_twoSkip(c);

    int a = sum(r);
    SA::clear(r);
    int s2 = sum(c);

    return s == a + s2;
}

template <class T>
bool _1_cl_oneFilter(T&& c) {
    setCollection_prx(c);
    int s = sum(c);

    auto r = _1_oneFilter(c);

    int a = sum(r);
    SA::clear(r);
    int s2 = sum(c);

    return s == a + s2;
}

template <class T>
bool _1_cl_oneRange(T&& c) {
    setCollection_prx(c);
    int s = sum(c);

    auto r = _1_oneRange(c);

    int a = sum(r);
    SA::clear(r);
    int s2 = sum(c);

    return s == a + s2;
}
// 1 level
//
// 2 level
template <class T>
bool _2_cl_rangeSkip(T&& c) {
    setCollection_prx(c);
    int s = sum(c);

    auto r = _2_rangeSkip(c);

    int a = sum(r);
    SA::clear(r);
    int s2 = sum(c);

    return s == a + s2;
}

template <class T>
bool _2_cl_skipRange(T&& c) {
    setCollection_prx(c);
    int s = sum(c);

    auto r = _2_skipRange(c);

    int a = sum(r);
    SA::clear(r);
    int s2 = sum(c);

    return s == a + s2;
}

template <class T>
bool _2_cl_filterSkip(T&& c) {
    setCollection_prx(c);
    int s = sum(c);

    auto r = _2_skipFilter(c);

    int a = sum(r);
    SA::clear(r);
    int s2 = sum(c);

    return s == a + s2;
}

template <class T>
bool _2_cl_skipFilter(T&& c) {
    setCollection_prx(c);
    int s = sum(c);

    auto r = _2_filterSkip(c);

    int a = sum(r);
    SA::clear(r);
    int s2 = sum(c);

    return s == a + s2;
}

template <class T>
bool _2_cl_filterRange(T&& c) {
    setCollection_prx(c);
    int s = sum(c);

    auto r = _2_filterRange(c);

    int a = sum(r);
    SA::clear(r);
    int s2 = sum(c);

    return s == a + s2;
}

template <class T>
bool _2_cl_rangeFilter(T&& c) {
    setCollection_prx(c);
    int s = sum(c);

    auto r = _2_rangeFilter(c);

    int a = sum(r);
    SA::clear(r);
    int s2 = sum(c);

    return s == a + s2;
}
// 2 level
//
// 3 level
template <class T>
bool _3_cl_rangeFilterSkip(T&& c) {
    setCollection_prx(c);
    int s = sum(c);

    auto r = _3_rangeFilterSkip(c);

    int a = sum(r);
    SA::clear(r);
    int s2 = sum(c);

    return s == a + s2;
}

template <class T>
bool _3_cl_rangeSkipFilter(T&& c) {
    setCollection_prx(c);
    int s = sum(c);

    auto r = _3_rangeSkipFilter(c);

    int a = sum(r);
    SA::clear(r);
    int s2 = sum(c);

    return s == a + s2;
}

template <class T>
bool _3_cl_filterRangeSkip(T&& c) {
    setCollection_prx(c);
    int s = sum(c);

    auto r = _3_filterRangeSkip(c);

    int a = sum(r);
    SA::clear(r);
    int s2 = sum(c);

    return s == a + s2;
}

template <class T>
bool _3_cl_filterSkipRange(T&& c) {
    setCollection_prx(c);
    int s = sum(c);

    auto r = _3_filterSkipRange(c);

    int a = sum(r);
    SA::clear(r);
    int s2 = sum(c);

    return s == a + s2;
}

template <class T>
bool _3_cl_skipRangeFilter(T&& c) {
    setCollection_prx(c);
    int s = sum(c);

    auto r = _3_skipRangeFilter(c);

    int a = sum(r);
    SA::clear(r);
    int s2 = sum(c);

    return s == a + s2;
}

template <class T>
bool _3_cl_skipFilterRange(T&& c) {
    setCollection_prx(c);
    int s = sum(c);

    auto r = _3_skipFilterRange(c);

    int a = sum(r);
    SA::clear(r);
    int s2 = sum(c);

    return s == a + s2;
}

template <class T>
bool _stf_cl_range(T&& c) {
    setCollection_prx(c);
    int s = sum(c);

    auto r = _stf_range(c);

    int a = sum(r);
    SA::clear(r);
    int s2 = sum(c);

    return s == a + s2;
}

template <class T>
bool _stf_cl_range_end(T&& c) {
    setCollection_prx(c);
    int s = sum(c);

    auto r = _stf_range(c);

    int a = sum(r);
    SA::clear(r);
    int s2 = sum(c);

    return s == a + s2;
}

template <class T>
bool _stf_sum_range(T&& c) {
    auto r = _stf_range(c);
    return sum(r) == 136;
}

template <class T>
bool _stf_sum_range_end(T&& c) {
    auto r = _stf_range_end(c);
    return sum(r) == 2024;
}
// 3 level
// CLEARANCE TESTS

template <class T>
bool setColTest(T&& c) {
    setCollection_prx(c);
    IFN_SECTOR_START( "set col test" );

    int s = 0;
    TEMPLATIOUS_FOREACH(int i,c) {
        s += i;
    }

    IFN_RET_FALSE( s == PROX_SUM );

    return true;
}

template <class T>
void setCollection_prx(T&& c) {
    SA::clear(c);

    // 0 .. 99
    SA::add(c,SF::seqL(100));
}

}

#endif /* end of include guard: PROXIES_SM4YKVBX */
