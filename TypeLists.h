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

BOOST_AUTO_TEST_SUITE( typelist );

BOOST_AUTO_TEST_CASE( typelist_test_equality_0 )
{
    typedef tt::t::TypeList<void,int,char> AList;
    typedef tt::t::TypeList<void,int,char> BList;

    static const bool res = tt::t::detail::TypelistsEqual<AList,BList>::value;
    BOOST_CHECK(res);
}

BOOST_AUTO_TEST_CASE( typelist_test_equality_1 )
{
    typedef tt::t::TypeList<long> AList;
    typedef tt::t::TypeList<long> BList;

    static const bool res = tt::t::detail::TypelistsEqual<AList,BList>::value;
    BOOST_CHECK(res);
}

BOOST_AUTO_TEST_CASE( typelist_test_unequality_0 )
{
    typedef tt::t::TypeList<long,int> AList;
    typedef tt::t::TypeList<long> BList;

    static const bool res = tt::t::detail::TypelistsEqual<AList,BList>::value;
    BOOST_CHECK(!res);
}

BOOST_AUTO_TEST_CASE( typelist_test_unequality_1 )
{
    typedef tt::t::TypeList<long,int> AList;
    typedef tt::t::TypeList<long,int,void> BList;

    static const bool res = tt::t::detail::TypelistsEqual<AList,BList>::value;
    BOOST_CHECK(!res);
}

BOOST_AUTO_TEST_CASE( typelist_test_unequality_2 )
{
    typedef tt::t::TypeList<long,int,int> alist;
    typedef tt::t::TypeList<long,int,void> blist;

    static const bool res = tt::t::detail::TypelistsEqual<alist,blist>::value;
    BOOST_CHECK(!res);
}

BOOST_AUTO_TEST_CASE( typelist_test_unequality_3 )
{
    typedef tt::t::TypeList<int> AList;
    typedef tt::t::TypeList<bool> BList;

    static const bool res = tt::t::detail::TypelistsEqual<AList,BList>::value;
    BOOST_CHECK(!res);
}

BOOST_AUTO_TEST_CASE( typelist_test_containment_0 )
{
    typedef tt::t::TypeList<int,int> AList;
    typedef tt::t::TypeList<int> BList;

    static const bool res = tt::t::detail::TypelistContains<AList,BList>::value;
    BOOST_CHECK(res);
}

BOOST_AUTO_TEST_CASE( typelist_test_containment_1 )
{
    typedef tt::t::TypeList<int,bool> AList;
    typedef tt::t::TypeList<int,bool> BList;

    static const bool res = tt::t::detail::TypelistContains<AList,BList>::value;
    BOOST_CHECK(res);
}

BOOST_AUTO_TEST_CASE( typelist_test_containment_2 )
{
    typedef tt::t::TypeList<int,bool,char,long,short> AList;
    typedef tt::t::TypeList<int,bool,char> BList;

    static const bool res = tt::t::detail::TypelistContains<AList,BList>::value;
    BOOST_CHECK(res);
}

BOOST_AUTO_TEST_CASE( typelist_test_uncontainment_0 )
{
    typedef tt::t::TypeList<bool,char,long,short> AList;
    typedef tt::t::TypeList<int,bool,char> BList;

    static const bool res = tt::t::detail::TypelistContains<AList,BList>::value;
    BOOST_CHECK(!res);
}

BOOST_AUTO_TEST_CASE( typelist_test_uncontainment_1 )
{
    typedef tt::t::TypeList<bool,char,long,short> AList;
    typedef tt::t::TypeList<bool,char,long,short,int> BList;

    static const bool res = tt::t::detail::TypelistContains<AList,BList>::value;
    BOOST_CHECK(!res);
}

BOOST_AUTO_TEST_CASE( typelist_test_uncontainment_2 )
{
    typedef tt::t::TypeList<bool> AList;
    typedef tt::t::TypeList<int> BList;

    static const bool res = tt::t::detail::TypelistContains<AList,BList>::value;
    BOOST_CHECK(!res);
}

BOOST_AUTO_TEST_CASE( typelist_test_uncontainment_3 )
{
    typedef tt::t::TypeList<bool,int> AList;
    typedef tt::t::TypeList<bool,void> BList;

    static const bool res = tt::t::detail::TypelistContains<AList,BList>::value;
    BOOST_CHECK(!res);
}

BOOST_AUTO_TEST_CASE( typelist_test_uncontainment_4 )
{
    typedef tt::t::TypeList<bool,int,char,long,short> AList;
    typedef tt::t::TypeList<int,char,long,short> BList;

    static const bool res = tt::t::detail::TypelistContains<AList,BList>::value;
    BOOST_CHECK(!res);
}

BOOST_AUTO_TEST_SUITE_END();

#endif /* end of include guard: TYPELISTS_T8ZGTVM9 */
