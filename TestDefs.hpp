/*
 * =====================================================================================
 *
 *       Filename:  TestDefs.h
 *
 *    Description:  Test utilities and definitions
 *
 *        Version:  1.0
 *        Created:  07/27/2014 07:16:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef TESTDEFS_JD3RBI46
#define TESTDEFS_JD3RBI46

#define TEMPLATIOUS_TESTING

#include <iostream>

#include "catch.hpp"

#include <templatious/FullPack.hpp>

#include "detail/ConstructorCountCollection.hpp"

namespace test_templatious {

namespace t = templatious;
namespace ta = t::adapters;
namespace u = t::util;
typedef t::StaticAdapter SA;
typedef t::StaticFactory SF;
typedef t::StaticManipulator SM;

struct IfnCaseCounter {
    static int count;

    IfnCaseCounter() { }
    ~IfnCaseCounter() {
        std::cout << "Total number of mini tests executed: "
                  << count << std::endl;
    }
};

template <class T,class U,class Prec>
bool almostEqualPrec(T&& a,U&& b,Prec&& p) {
    auto diff = b - a;
    if (diff < 0) {
        diff = -diff;
    }
    return diff < p;
}

template <class T,class U>
bool almostEqual(T&& a,U&& b) {
    return almostEqualPrec(
        std::forward<T>(a),
        std::forward<U>(b),
        0.0000001);
}

#define DEF_ADAPTER(cl,name) typedef ta::CollectionAdapter<cl> name

#define IFN_SECTOR_START(name) \
    int counter = 0;\
    const char* SECTOR_NAME = name;

#define IFN_RET_FALSE(expr) \
    if (!(expr)) {\
        std::cout << "FAILED AT: " << counter << std::endl;\
        std::cout << "SECTOR: |" << SECTOR_NAME << "|" << std::endl;\
        return false;\
    }\
    ++counter;\
    ::test_templatious::IfnCaseCounter::count++;

}

namespace tt = test_templatious;

TEMPLATIOUS_CALLEACH_FCTOR_WSTOR( Sum, _c += i );

#define SUM_VAR( vname, fctorname ) \
    int vname = 0;\
    auto fctorname = templatious::StaticFactory::storageFunctor<Sum>(vname);

#define INIT_BALLER \
    SUM_VAR( sum, sf );

#endif /* end of include guard: TESTDEFS_JD3RBI46 */
