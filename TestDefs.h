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

#include <iostream>

#include <boost/test/unit_test.hpp>

#include <templatious/adapters/All.h>
#include <templatious/StaticFactory.h>
#include <templatious/StaticVector.h>
#include <templatious/LogicPrimitives.h>


namespace test_templatious {

namespace t = templatious;
namespace ta = t::adapters;
namespace u = t::util;
typedef t::StaticAdapter SA;
typedef t::StaticFactory SF;

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
    ++counter;

}

namespace tt = test_templatious;

TEMPLATIOUS_CALLEACH_FCTOR_WSTOR( Sum, _c += i );

#define SUM_VAR( vname, fctorname ) \
    int vname = 0;\
    auto fctorname = templatious::StaticFactory::storageFunctor<Sum>(vname);

#define INIT_BALLER \
    TEMPLATIOUS_TRIPLET_STD; \
    SUM_VAR( sum, sf );

#endif /* end of include guard: TESTDEFS_JD3RBI46 */
