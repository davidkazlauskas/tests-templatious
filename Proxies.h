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
// LEVEL TWO TESTS

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
