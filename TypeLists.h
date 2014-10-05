/*
 * =====================================================================================
 *
 *       Filename:  TypeLists.h
 *
 *    Description:  Typelist unit tests
 *
 *        Version:  1.0
 *        Created:  10/05/2014 10:20:06 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef TYPELISTS_T8ZGTVM9
#define TYPELISTS_T8ZGTVM9

#include "TestDefs.h"


BOOST_AUTO_TEST_CASE( typelist_test_equality_0 )
{
    typedef tt::u::TypeList<void,int,char> AList;
    typedef tt::u::TypeList<void,int,char> BList;

    static const bool res = tt::u::TypelistsEqual<AList,BList>::value;
    BOOST_CHECK(res);
}

BOOST_AUTO_TEST_CASE( typelist_test_equality_1 )
{
    typedef tt::u::TypeList<long> AList;
    typedef tt::u::TypeList<long> BList;

    static const bool res = tt::u::TypelistsEqual<AList,BList>::value;
    BOOST_CHECK(res);
}

BOOST_AUTO_TEST_CASE( typelist_test_unequality_0 )
{
    typedef tt::u::TypeList<long,int> AList;
    typedef tt::u::TypeList<long> BList;

    static const bool res = tt::u::TypelistsEqual<AList,BList>::value;
    BOOST_CHECK(!res);
}

BOOST_AUTO_TEST_CASE( typelist_test_unequality_1 )
{
    typedef tt::u::TypeList<long,int> AList;
    typedef tt::u::TypeList<long,int,void> BList;

    static const bool res = tt::u::TypelistsEqual<AList,BList>::value;
    BOOST_CHECK(!res);
}

BOOST_AUTO_TEST_CASE( typelist_test_unequality_2 )
{
    typedef tt::u::TypeList<long,int,int> alist;
    typedef tt::u::TypeList<long,int,void> blist;

    static const bool res = tt::u::TypelistsEqual<alist,blist>::value;
    BOOST_CHECK(!res);
}

BOOST_AUTO_TEST_CASE( typelist_test_unequality_3 )
{
    typedef tt::u::TypeList<int> AList;
    typedef tt::u::TypeList<bool> BList;

    static const bool res = tt::u::TypelistsEqual<AList,BList>::value;
    BOOST_CHECK(!res);
}

BOOST_AUTO_TEST_CASE( typelist_test_containment_0 )
{
    typedef tt::u::TypeList<int,int> AList;
    typedef tt::u::TypeList<int> BList;

    static const bool res = tt::u::TypelistContains<AList,BList>::value;
    BOOST_CHECK(res);
}

BOOST_AUTO_TEST_CASE( typelist_test_containment_1 )
{
    typedef tt::u::TypeList<int,bool> AList;
    typedef tt::u::TypeList<int,bool> BList;

    static const bool res = tt::u::TypelistContains<AList,BList>::value;
    BOOST_CHECK(res);
}

BOOST_AUTO_TEST_CASE( typelist_test_containment_2 )
{
    typedef tt::u::TypeList<int,bool,char,long,short> AList;
    typedef tt::u::TypeList<int,bool,char> BList;

    static const bool res = tt::u::TypelistContains<AList,BList>::value;
    BOOST_CHECK(res);
}

BOOST_AUTO_TEST_CASE( typelist_test_uncontainment_0 )
{
    typedef tt::u::TypeList<bool,char,long,short> AList;
    typedef tt::u::TypeList<int,bool,char> BList;

    static const bool res = tt::u::TypelistContains<AList,BList>::value;
    BOOST_CHECK(!res);
}

BOOST_AUTO_TEST_CASE( typelist_test_uncontainment_1 )
{
    typedef tt::u::TypeList<bool,char,long,short> AList;
    typedef tt::u::TypeList<bool,char,long,short,int> BList;

    static const bool res = tt::u::TypelistContains<AList,BList>::value;
    BOOST_CHECK(!res);
}

BOOST_AUTO_TEST_CASE( typelist_test_uncontainment_2 )
{
    typedef tt::u::TypeList<bool> AList;
    typedef tt::u::TypeList<int> BList;

    static const bool res = tt::u::TypelistContains<AList,BList>::value;
    BOOST_CHECK(!res);
}

BOOST_AUTO_TEST_CASE( typelist_test_uncontainment_3 )
{
    typedef tt::u::TypeList<bool,int> AList;
    typedef tt::u::TypeList<bool,void> BList;

    static const bool res = tt::u::TypelistContains<AList,BList>::value;
    BOOST_CHECK(!res);
}

BOOST_AUTO_TEST_CASE( typelist_test_uncontainment_4 )
{
    typedef tt::u::TypeList<bool,int,char,long,short> AList;
    typedef tt::u::TypeList<int,char,long,short> BList;

    static const bool res = tt::u::TypelistContains<AList,BList>::value;
    BOOST_CHECK(!res);
}

#endif /* end of include guard: TYPELISTS_T8ZGTVM9 */
