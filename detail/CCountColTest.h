/*
 * =====================================================================================
 *
 *       Filename:  CCountColTest.h
 *
 *    Description:  test routine for destruction count
 *
 *        Version:  1.0
 *        Created:  12/15/2014 06:25:23 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef CCOUNTCOLTEST_NB2GEKN5
#define CCOUNTCOLTEST_NB2GEKN5

#include "../TestDefs.h"

namespace test_templatious {

template <class Token,class T>
bool constructionCountCollectionTest(T& c) {
    IFN_SECTOR_START( "Construction count collection test." );

    TEMPLATIOUS_TRIPLET_STD;

    SA::clear(c);

    typedef ConstructorCountCollection<Token> CCC;
    for (int i = 0; i < 100; ++i) {
        SA::add(c,CCC());
    }
    IFN_RET_FALSE(CCC::count() == 100);

    SA::clear(c);
    IFN_RET_FALSE(CCC::count() == 0);

    for (int i = 0; i < 100; ++i) {
        SA::add(c,CCC());
    }

    IFN_RET_FALSE(CCC::count() == 100);

    auto tmp = CCC();
    IFN_RET_FALSE(CCC::count() == 101);

    TEMPLATIOUS_FOREACH(auto& i,c) {
        tmp = i;
    }
    IFN_RET_FALSE(CCC::count() == 101);

    TEMPLATIOUS_FOREACH(auto& i,c) {
        tmp = std::move(i);
    }
    IFN_RET_FALSE(CCC::count() == 1);

    TEMPLATIOUS_FOREACH(auto& i,c) {
        i = tmp;
    }
    IFN_RET_FALSE(CCC::count() == 101);

    TEMPLATIOUS_FOREACH(auto& i,c) {
        i = std::move(tmp);
    }
    IFN_RET_FALSE(CCC::count() == 1);

    auto iter = SA::begin(c);
    auto front = iter;
    ++front;
    while (front != SA::end(c)) {
        *front = std::move(*iter);
        ++iter;
        ++front;
    }
    IFN_RET_FALSE(CCC::count() == 1);

    tmp = *iter;
    IFN_RET_FALSE(CCC::count() == 2);

    tmp = std::move(*iter);
    IFN_RET_FALSE(CCC::count() == 1);

    return true;
}

}

#endif /* end of include guard: CCOUNTCOLTEST_NB2GEKN5 */
