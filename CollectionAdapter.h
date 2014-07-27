/*
 * =====================================================================================
 *
 *       Filename:  CollectionAdapter.h
 *
 *    Description:  Collection adapter tests
 *
 *        Version:  1.0
 *        Created:  07/27/2014 07:05:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef COLLECTIONADAPTER_HIPES5LC
#define COLLECTIONADAPTER_HIPES5LC

#include <boost/test/unit_test.hpp>

#include "TestDefs.h"

namespace test_templatious {

template <class T>
void setCollection(T&& c);

template <class T>
bool clearTest(T&& t);
template <class T>
bool addTest(T&& t);
template <class T>
bool integrityTest(T&& t);
template <class T>
bool eraseTest(T&& t);
template <class T>
bool eraseTest2(T&& t);
template <class T>
bool eraseTest3(T&& t);
template <class T>
bool indexTest(T&& t);
template <class T>
bool insertTest(T&& t);
template <class T>
bool iterIntegrityTest(T&& c);
template <class T>
bool iterAssigmentTest(T&& c);

template <class T>
bool existantCollectionTest(T&& c) {
    DEF_ADAPTER(T,Ad);

    static_assert(Ad::is_valid,"Adapter is not supported.");
    IFN_RET_FALSE(Ad::is_valid);
    IFN_RET_FALSE(clearTest(c));
    IFN_RET_FALSE(addTest(c));
    IFN_RET_FALSE(integrityTest(c));
    IFN_RET_FALSE(eraseTest(c));
    IFN_RET_FALSE(eraseTest2(c));
    IFN_RET_FALSE(eraseTest3(c));
    IFN_RET_FALSE(indexTest(c));
    IFN_RET_FALSE(insertTest(c));
    IFN_RET_FALSE(iterIntegrityTest(c));
    IFN_RET_FALSE(iterAssigmentTest(c));

    return true;
}

template <class T>
void setCollection(T&& c) {

    SA::clear(c);
    SA::add(c,1,2,3,4,5,6);

}

template <class T>
bool clearTest(T&& c) {
    DEF_ADAPTER(T,Ad);

    Ad::clear(c);
    if (0 != Ad::getSize(c)) {
        return false;
    }

    return true;
}

template <class T>
bool addTest(T&& c) {
    DEF_ADAPTER(T,Ad);

    size_t sz = Ad::getSize(c);

    Ad::add(c,7);

    return (Ad::getSize(c) == sz + 1);
}

template <class T>
bool integrityTest(T&& c) {
    DEF_ADAPTER(T,Ad);

    setCollection(c);

    size_t res = 0;
    for (auto i = SA::begin(c); i != SA::end(c); ++i) {
        res += *i;
    }

    return res == 3 * 7;
}

template <class T>
bool eraseTest(T&& c) {
    DEF_ADAPTER(T,Ad);

    Ad::clear(c);
    integrityTest(c);
    auto it = SA::begin(c);
    for (int i = 0; i < 2; ++i) {
        ++it;
    }

    SA::erase(c,it);
    size_t res = 0;
    for (auto i = SA::begin(c); i != SA::end(c); ++i) {
        res += *i;
    }

    return res == 2 * 9;
}

template <class T>
bool eraseTest2(T&& c) {
    DEF_ADAPTER(T,Ad);

    setCollection(c);

    auto it = SA::begin(c);
    for (int i = 0; i < 2; ++i) {
        ++it;
    }

    SA::erase(c,SA::begin(c),it);
    size_t res = 0;
    for (auto i = SA::begin(c); i != SA::end(c); ++i) {
        res += *i;
    }

    return res = 3;
}

template <class T>
bool eraseTest3(T&& c) {
    DEF_ADAPTER(T,Ad);

    setCollection(c);

    SA::erase(c,SA::begin(c),SA::end(c));

    IFN_RET_FALSE(SA::getSize(c) == 0);

    size_t res = 0;
    for (auto i = SA::begin(c); i != SA::end(c); ++i) {
        res += *i;
    }

    return res == 0;
}

template <class T>
bool indexTest(T&& c) {
    DEF_ADAPTER(T,Ad);

    setCollection(c);

    IFN_RET_FALSE(SA::getByIndex(c,0) == 1);
    IFN_RET_FALSE(SA::getByIndex(c,1) == 2);
    IFN_RET_FALSE(SA::getByIndex(c,2) == 3);
    IFN_RET_FALSE(SA::getByIndex(c,3) == 4);
    IFN_RET_FALSE(SA::getByIndex(c,4) == 5);
    IFN_RET_FALSE(SA::getByIndex(c,5) == 6);

    return true;
}

template <class T>
bool insertTest(T&& c) {
    DEF_ADAPTER(T,Ad);

    setCollection(c);

    auto i = SA::iterAt(c,4);
    SA::insert(c,i,3);
    i = SA::iterAt(c,4);
    SA::insert(c,i,2);
    i = SA::iterAt(c,4);
    SA::insert(c,i,1);

    IFN_RET_FALSE(SA::getByIndex(c,0) == 1);
    IFN_RET_FALSE(SA::getByIndex(c,1) == 2);
    IFN_RET_FALSE(SA::getByIndex(c,2) == 3);
    IFN_RET_FALSE(SA::getByIndex(c,3) == 4);

    IFN_RET_FALSE(SA::getByIndex(c,4) == 1);
    IFN_RET_FALSE(SA::getByIndex(c,5) == 2);
    IFN_RET_FALSE(SA::getByIndex(c,6) == 3);

    IFN_RET_FALSE(SA::getByIndex(c,7) == 5);
    IFN_RET_FALSE(SA::getByIndex(c,8) == 6);

    size_t res = 0;
    for (auto i = SA::begin(c); i != SA::end(c); ++i) {
        res += *i;
    }

    return res == 3 * 7 + 6;
}

template <class T>
bool iterIntegrityTest(T&& c) {
    DEF_ADAPTER(T,Ad);

    setCollection(c);

    IFN_RET_FALSE(SA::iterAt(c,0) == SA::begin(c));
    IFN_RET_FALSE(SA::iterAt(c,6) == SA::end(c));

    return true;
}

template <class T>
bool iterAssigmentTest(T&& c) {
    DEF_ADAPTER(T,Ad);

    setCollection(c);

    for (auto i = SA::begin(c); i != SA::end(c); ++i) {
        *i = 7 - *i;
    }

    size_t res = 0;
    for (auto i = SA::begin(c); i != SA::end(c); ++i) {
        res += *i;
    }

    IFN_RET_FALSE(res == 3 * 7);

    IFN_RET_FALSE(SA::getByIndex(c,0) == 6);
    IFN_RET_FALSE(SA::getByIndex(c,1) == 5);
    IFN_RET_FALSE(SA::getByIndex(c,2) == 4);
    IFN_RET_FALSE(SA::getByIndex(c,3) == 3);
    IFN_RET_FALSE(SA::getByIndex(c,4) == 2);
    IFN_RET_FALSE(SA::getByIndex(c,5) == 1);

    return true;
}

}



#endif /* end of include guard: COLLECTIONADAPTER_HIPES5LC */
