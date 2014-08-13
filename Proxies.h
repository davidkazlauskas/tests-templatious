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

// VARIATIONS
// 1 level:
#define _1_oneSkip(c) SF::skip(c,2)
#define _1_twoSkip(c) SF::skip(c,3)
#define _1_oneFilter(c) SF::filter(c,[](int i) {\
            return i > 19; \
        })
#define _1_oneRange(c) SF::range(c,17,57)
// --> 1 level

// 2 level:
#define _2_rangeSkip(c) SF::skip(SF::range(c,17,57),3)
#define _2_skipRange(c) SF::range(SF::skip(c,3),1,10)
#define _2_skipFilter(c) SF::filter(SF::skip(c,3),\
        [](int i) { return i > 50; })
#define _2_filterSkip(c) SF::skip(SF::filter(c,\
            [](int i) { return i > 50; }),3)
#define _2_filterRange(c) SF::range(SF::filter(c,\
            [](int i) { return i > 50; }),17,47)
#define _2_rangeFilter(c) SF::filter(SF::range(c,17,57),\
        [](int i) { return i > 50; })
// --> 2 level

// 3 level:
#define _3_rangeFilterSkip(c) SF::skip(SF::filter(SF::range(c,15,17),\
            [](int i) { return i > 50; }),3)
#define _3_rangeSkipFilter(c) SF::filter(SF::skip(SF::range(c,17,57),3),\
        [](int i) { return i > 20; })
#define _3_filterRangeSkip(c) SF::skip(SF::range(SF::filter(c,\
        [](int i) { return i > 50; }),17,37),3)
#define _3_filterSkipRange(c) SF::range(SF::skip(SF::filter(c,\
        [](int i) { return i > 50; }),3),7,14)
#define _3_skipRangeFilter(c) SF::filter(SF::range(SF::skip(c,3),17,27),\
        [](int i) { return i > 60; })
#define _3_skipFilterRange(c) SF::range(SF::filter(SF::skip(c,3),\
        [](int i) { return i > 37; }),7,17)
// --> 3 level


// VARIATIONS

template <class T>
bool proxyTest(T&& c) {
    DEF_ADAPTER(T,Ad);

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

    return true;
}

static const int PROX_SUM = 4950;

template <class T>
bool oneSkipTest(T&& c) {
    setCollection_prx(c);

    auto s = SF::skip(c,2);
    //std::cout << sum(s) << std::endl;

    return sum(s) == 2450;
}

template <class T>
bool twoSkipTest(T&& c) {
    setCollection_prx(c);

    auto s = SF::skip(c,3);
    //std::cout << sum(s) << std::endl;
    return sum(s) == 1683;
}

template <class T>
bool oneFilterTest(T&& c) {
    setCollection_prx(c);

    auto f = SF::filter(c,[](int i) { return i > 19; });

    return sum(f) == 4760;
}

template <class T>
bool oneRangeTest(T&& c) {
    setCollection_prx(c);

    auto r = SF::range(c,17,57);

    return sum(r) == 1460;
}

template <class T>
bool _2_rangeSkipTest(T&& c) {
    setCollection_prx(c);

    auto r = SF::skip(
        SF::range(c,17,57),
        3);

    return sum(r) == 511;
}

template <class T>
bool _2_skipRangeTest(T&& c) {
    setCollection_prx(c);

    auto r = SF::range(
            SF::skip(c,3),1,10);

    return sum(r) == 135;
}

template <class T>
bool _2_filterSkipTest(T&& c) {
    setCollection_prx(c);

    auto r = SF::filter(SF::skip(c,3),[](int i) {
            return i > 50;
        });

    return sum(r) == 1275;
}

template <class T>
bool _2_skipFilterTest(T&& c) {
    setCollection_prx(c);

    auto r = SF::skip(SF::filter(c,[](int i) {
            return i > 50;
        }),3);

    return sum(r) == 1275;
}

template <class T>
bool _2_filterRangeTest(T&& c) {
    setCollection_prx(c);

    auto r = SF::range(SF::filter(c,[](int i) {
            return i > 50;
        }),17,47);

    return sum(r) == 2475;
}

template <class T>
bool _2_rangeFilterTest(T&& c) {
    setCollection_prx(c);

    auto r = SF::filter(SF::range(c,17,57),
        [](int i) {
            return i > 50;
        });

    return sum(r) == 321;
}

// 3RD LEVEL
template <class T>
bool _3_rangeFilterSkipTest(T&& c) {
    setCollection_prx(c);

    auto r =
        SF::skip(
        SF::filter(
        SF::range(c, 17, 57), [](int i) {
            return i > 50;
        }),3);

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

    auto r =
        SF::skip(
            SF::range(
                SF::filter(c,
                    [](int i) {
                        return i > 50;
                    }),17,37)
            ,3);

    return sum(r) == 539;
}

template <class T>
bool _3_filterSkipRangeTest(T&& c) {
    setCollection_prx(c);

    auto r =
        SF::range(
            SF::skip(
                SF::filter(c,
                    [](int i) {
                        return i > 50;
                    })
            ,3),7,14);

    return sum(r) == 567;
}

template <class T>
bool _3_skipRangeFilterTest(T&& c) {
    setCollection_prx(c);

    auto r =
        SF::filter(
            SF::range(
                SF::skip(c,3)
            ,17,27),
        [](int i) {
            return i > 60;
        });

    return sum(r) == 423;
}

template <class T>
bool _3_skipFilterRangeTest(T&& c) {
    setCollection_prx(c);

    auto r =
        SF::range(
            SF::filter(
                SF::skip(c,3)
            ,[](int i) {
                return i > 37;
            })
        ,7,17);

    return sum(r) == 735;
}
// 3RD LEVEL

template <class T>
bool setColTest(T&& c) {
    setCollection_prx(c);

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
    SA::add(c,SF::loopL(100));
}

}

#endif /* end of include guard: PROXIES_SM4YKVBX */
