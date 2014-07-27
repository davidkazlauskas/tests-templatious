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

#include <templatious/adapters/All.h>
#include <templatious/StaticFactory.h>


namespace test_templatious {

namespace t = templatious;
namespace ta = t::adapters;
typedef t::StaticAdapter SA;
typedef t::StaticFactory SF;

#define DEF_ADAPTER(cl,name) typedef ta::CollectionAdapter<cl> name
#define IFN_RET_FALSE(expr) \
    if (!(expr)) {\
        return false;\
    }

}

namespace tt = test_templatious;

#endif /* end of include guard: TESTDEFS_JD3RBI46 */
