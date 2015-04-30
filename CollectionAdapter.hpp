/*
 * =====================================================================================
 *
 *       Filename:  CollectionAdapter.hpp
 *
 *    Description:  Collection adapter tests
 *
 *        Version:  1.0
 *        Created:  07/27/2014 07:05:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Kazlauskas (dk), david@templatious.org
 *
 * =====================================================================================
 */

#ifndef COLLECTIONADAPTER_HIPES5LC
#define COLLECTIONADAPTER_HIPES5LC

#include "TestDefs.hpp"

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
bool eraseTest4(T&& t);
template <class T>
bool indexTest(T&& t);
template <class T>
bool insertTest(T&& t);
template <class T>
bool iterIntegrityTest(T&& c);
template <class T>
bool iterAssigmentTest(T&& c);
template <class T>
bool iterAtIntegrityTest(T&& c);
template <class T>
bool sizeTest(T&& c);
template <class T>
bool sortTest(T&& c);

template <class T>
bool existantCollectionTest(T&& c) {
    DEF_ADAPTER(T,Ad);
    IFN_SECTOR_START( "existant collection test" );

    static_assert(Ad::is_valid,"Adapter is not supported.");
    IFN_RET_FALSE(Ad::is_valid);
    IFN_RET_FALSE(clearTest(c));
    IFN_RET_FALSE(addTest(c));
    IFN_RET_FALSE(integrityTest(c));
    IFN_RET_FALSE(eraseTest(c));
    IFN_RET_FALSE(eraseTest2(c));
    IFN_RET_FALSE(eraseTest3(c));
    IFN_RET_FALSE(eraseTest4(c));
    IFN_RET_FALSE(indexTest(c));
    IFN_RET_FALSE(insertTest(c));
    IFN_RET_FALSE(iterIntegrityTest(c));
    IFN_RET_FALSE(iterAssigmentTest(c));
    IFN_RET_FALSE(iterAtIntegrityTest(c));
    IFN_RET_FALSE(sizeTest(c));
    IFN_RET_FALSE(sortTest(c));

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
    if (0 != Ad::size(c)) {
        return false;
    }

    return true;
}

template <class T>
bool addTest(T&& c) {
    DEF_ADAPTER(T,Ad);

    int sz = Ad::size(c);

    Ad::add(c,7);

    return (Ad::size(c) == sz + 1);
}

template <class T>
bool integrityTest(T&& c) {
    setCollection(c);

    size_t res = 0;
    for (auto i = SA::begin(c); i != SA::end(c); ++i) {
        res += *i;
    }

    return res == 3 * 7;
}

template <class T>
bool eraseTest(T&& c) {
    setCollection(c);
    auto it = SA::begin(c);
    for (int i = 0; i < 2; ++i) {
        ++it;
    }

    SA::eraseTillEnd(c,it);
    size_t res = 0;
    for (auto i = SA::begin(c); i != SA::end(c); ++i) {
        res += *i;
    }

    return res == 3;
}

template <class T>
bool eraseTest2(T&& c) {
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

    return res == 18;
}

template <class T>
bool eraseTest3(T&& c) {
    IFN_SECTOR_START( "erase test 3" );

    setCollection(c);

    SA::erase(c,SA::begin(c),SA::end(c));

    IFN_RET_FALSE(SA::size(c) == 0);

    size_t res = 0;
    for (auto i = SA::begin(c); i != SA::end(c); ++i) {
        res += *i;
    }

    return res == 0;
}

template <class T>
bool eraseTest4(T&& c) {
    IFN_SECTOR_START( "erase test 4" );

    setCollection(c);

    auto i = SA::begin(c);
    for (int j = 0; j < 2; ++j) {
        ++i;
    }

    SA::erase(c,i);

    IFN_RET_FALSE(SA::size(c) == 5);

    IFN_RET_FALSE(SA::getByIndex(c,0) == 1);
    IFN_RET_FALSE(SA::getByIndex(c,1) == 2);
    IFN_RET_FALSE(SA::getByIndex(c,2) == 4);
    IFN_RET_FALSE(SA::getByIndex(c,3) == 5);
    IFN_RET_FALSE(SA::getByIndex(c,4) == 6);

    size_t res = 0;
    for (auto i = SA::begin(c); i != SA::end(c); ++i) {
        res += *i;
    }

    return res == 18;
}


template <class T>
bool indexTest(T&& c) {
    IFN_SECTOR_START( "index test" );

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
    IFN_SECTOR_START( "insert test" );
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
    IFN_SECTOR_START( "iter integrity test" );
    setCollection(c);

    IFN_RET_FALSE(SA::iterAt(c,0) == SA::begin(c));
    IFN_RET_FALSE(SA::iterAt(c,6) == SA::end(c));

    return true;
}

template <class T>
bool iterAssigmentTest(T&& c) {
    IFN_SECTOR_START( "iter assignment test" );

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

template <class T>
bool iterAtIntegrityTest(T&& c) {
    IFN_SECTOR_START( "iter at integrity test" );

    typedef t::adapters::CollectionAdapter<T> Ad;
    typedef t::adapters::CollectionAdapter<typename Ad::ConstCol> CAd;
    auto& cref = static_cast<typename Ad::ConstCol&>(c);

    IFN_RET_FALSE( Ad::canAdd(c) );

    SA::clear(c);
    SA::add(c,SF::seqL(100));

    int size = SA::size(c);
    IFN_RET_FALSE(size == 100);

    std::vector< typename Ad::ValueType > v;

    SA::add(v,c);

    IFN_RET_FALSE(SA::size(v) == size);

    auto beg = v.cbegin();
    auto end = v.cend();
    int prVal = -1;
    bool testPassed = true;
    int cnt = 0;
    while (beg != end) {
        testPassed &= *beg - 1 == prVal;
        auto it = SA::iterAt(c,cnt);
        auto cIt = SA::citerAt(c,cnt);
        auto itC = Ad::iterAt(cref,cnt);
        auto &rAt = SA::getByIndex(c,cnt);
        auto &rcAt = SA::getByIndex(cref,cnt);
        auto &cAdIdx = Ad::getByIndex(cref,cnt);
        auto itCC = CAd::iterAt(c,cnt);
        testPassed &= *it == *beg;
        testPassed &= *cIt == *beg;
        testPassed &= *itC == *beg;
        testPassed &= *itCC == *beg;
        testPassed &= rAt == *beg;
        testPassed &= rcAt == *beg;
        testPassed &= cAdIdx == *beg;
        ++cnt;

        prVal = *beg;
        ++beg;
    }

    testPassed &= Ad::first(cref) == 0;
    testPassed &= Ad::last(cref) == 99;

    IFN_RET_FALSE(testPassed);

    // invalid comparison,
    // nobody said cend has to refer to a valid
    // element, nevertheless, at the end of the sequence
    //IFN_RET_FALSE( *SA::cend(c) == *SA::citerAt(c,size) )

    {
        bool caught = false;
        try {
            SA::citerAt(c,size + 1);
        } catch (std::exception e) {
            caught = true;
        }
        IFN_RET_FALSE(caught);
    }

    {
        bool caught = false;
        try {
            SA::citerAt(c,-1);
        } catch (std::exception e) {
            caught = true;
        }
        IFN_RET_FALSE(caught);
    }

    return true;
}

template <class T>
bool sizeTest(T&& c) {
    IFN_SECTOR_START( "iter at integrity test" );

    SA::clear(c);
    SA::add(c,SF::seqL(100));

    IFN_RET_FALSE( SA::size(c) == 100 );
    IFN_RET_FALSE( SA::size(c) == SA::trueSize(c) );

    int j = 0;
    auto e = SA::cend(c);
    for (auto i = SA::cbegin(c); i != e; ++i) {
        ++j;
    }
    IFN_RET_FALSE( SA::trueSize(c) == j );
    IFN_RET_FALSE( j == 100 );

    return true;
}

template <class T>
bool sortTest(T&& c) {
    IFN_SECTOR_START( "sorting test" );

    { // Simple
        SA::clear(c);
        SA::add(c,3,2,1);
        SM::sort(c);

        IFN_RET_FALSE( SA::getByIndex(c,0) == 1 );
        IFN_RET_FALSE( SA::getByIndex(c,1) == 2 );
        IFN_RET_FALSE( SA::getByIndex(c,2) == 3 );
    }

    { // Custom
        SA::clear(c);
        SA::add(c,1,2,3);
        SM::sortS(c,[](int a,int b) { return a > b; });

        IFN_RET_FALSE( SA::getByIndex(c,0) == 3 );
        IFN_RET_FALSE( SA::getByIndex(c,1) == 2 );
        IFN_RET_FALSE( SA::getByIndex(c,2) == 1 );
    }

    return true;
}

}



#endif /* end of include guard: COLLECTIONADAPTER_HIPES5LC */
