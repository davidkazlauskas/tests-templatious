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

#include "TestDefs.hpp"

TEST_CASE( "typelist_test_equality_0", "[typelist]" )
{
    typedef tt::t::TypeList<void,int,char> AList;
    typedef tt::t::TypeList<void,int,char> BList;

    static const bool res = tt::t::detail::TypelistsEqual<AList,BList>::value;
    REQUIRE(res);
}

TEST_CASE( "typelist_test_equality_1", "[typelist]" )
{
    typedef tt::t::TypeList<long> AList;
    typedef tt::t::TypeList<long> BList;

    static const bool res = tt::t::detail::TypelistsEqual<AList,BList>::value;
    REQUIRE(res);
}

TEST_CASE( "typelist_test_unequality_0", "[typelist]" )
{
    typedef tt::t::TypeList<long,int> AList;
    typedef tt::t::TypeList<long> BList;

    static const bool res = tt::t::detail::TypelistsEqual<AList,BList>::value;
    REQUIRE(!res);
}

TEST_CASE( "typelist_test_unequality_1", "[typelist]" )
{
    typedef tt::t::TypeList<long,int> AList;
    typedef tt::t::TypeList<long,int,void> BList;

    static const bool res = tt::t::detail::TypelistsEqual<AList,BList>::value;
    REQUIRE(!res);
}

TEST_CASE( "typelist_test_unequality_2", "[typelist]" )
{
    typedef tt::t::TypeList<long,int,int> alist;
    typedef tt::t::TypeList<long,int,void> blist;

    static const bool res = tt::t::detail::TypelistsEqual<alist,blist>::value;
    REQUIRE(!res);
}

TEST_CASE( "typelist_test_unequality_3", "[typelist]" )
{
    typedef tt::t::TypeList<int> AList;
    typedef tt::t::TypeList<bool> BList;

    static const bool res = tt::t::detail::TypelistsEqual<AList,BList>::value;
    REQUIRE(!res);
}

TEST_CASE( "typelist_test_containment_0", "[typelist]" )
{
    typedef tt::t::TypeList<int,int> AList;
    typedef tt::t::TypeList<int> BList;

    static const bool res = tt::t::detail::TypelistContains<AList,BList>::value;
    REQUIRE(res);
}

TEST_CASE( "typelist_test_containment_1", "[typelist]" )
{
    typedef tt::t::TypeList<int,bool> AList;
    typedef tt::t::TypeList<int,bool> BList;

    static const bool res = tt::t::detail::TypelistContains<AList,BList>::value;
    REQUIRE(res);
}

TEST_CASE( "typelist_test_containment_2", "[typelist]" )
{
    typedef tt::t::TypeList<int,bool,char,long,short> AList;
    typedef tt::t::TypeList<int,bool,char> BList;

    static const bool res = tt::t::detail::TypelistContains<AList,BList>::value;
    REQUIRE(res);
}

TEST_CASE( "typelist_test_uncontainment_0", "[typelist]" )
{
    typedef tt::t::TypeList<bool,char,long,short> AList;
    typedef tt::t::TypeList<int,bool,char> BList;

    static const bool res = tt::t::detail::TypelistContains<AList,BList>::value;
    REQUIRE(!res);
}

TEST_CASE( "typelist_test_uncontainment_1", "[typelist]" )
{
    typedef tt::t::TypeList<bool,char,long,short> AList;
    typedef tt::t::TypeList<bool,char,long,short,int> BList;

    static const bool res = tt::t::detail::TypelistContains<AList,BList>::value;
    REQUIRE(!res);
}

TEST_CASE( "typelist_test_uncontainment_2", "[typelist]" )
{
    typedef tt::t::TypeList<bool> AList;
    typedef tt::t::TypeList<int> BList;

    static const bool res = tt::t::detail::TypelistContains<AList,BList>::value;
    REQUIRE(!res);
}

TEST_CASE( "typelist_test_uncontainment_3", "[typelist]" )
{
    typedef tt::t::TypeList<bool,int> AList;
    typedef tt::t::TypeList<bool,void> BList;

    static const bool res = tt::t::detail::TypelistContains<AList,BList>::value;
    REQUIRE(!res);
}

TEST_CASE( "typelist_test_uncontainment_4", "[typelist]" )
{
    typedef tt::t::TypeList<bool,int,char,long,short> AList;
    typedef tt::t::TypeList<int,char,long,short> BList;

    static const bool res = tt::t::detail::TypelistContains<AList,BList>::value;
    REQUIRE(!res);
}

template <int i>
struct DummyType {
    DummyType() : _num(i) {}

    int _num;
};

TEST_CASE( "TypeList_test_indexing", "[typelist]" )
{
    typedef tt::t::TypeList<
        DummyType<0>,
        DummyType<1>,
        DummyType<2>,
        DummyType<3>
    > TheList;

    typedef TheList::ByIndex<0>::type AType;
    typedef TheList::ByIndex<1>::type BType;
    typedef TheList::ByIndex<2>::type CType;
    typedef TheList::ByIndex<3>::type DType;

    static const bool aExp = std::is_same< AType,DummyType<0> >::value;
    static const bool bExp = std::is_same< BType,DummyType<1> >::value;
    static const bool cExp = std::is_same< CType,DummyType<2> >::value;
    static const bool dExp = std::is_same< DType,DummyType<3> >::value;

    REQUIRE( aExp );
    REQUIRE( bExp );
    REQUIRE( cExp );
    REQUIRE( dExp );

    static const bool emp4 = std::is_same<
        TheList::ByIndex<4>::type, tt::t::NullType >::value;
    static const bool emp5 = std::is_same<
        TheList::ByIndex<5>::type, tt::t::NullType >::value;
    static const bool emp77 = std::is_same<
        TheList::ByIndex<77>::type, tt::t::NullType >::value;

    REQUIRE( emp4 );
    REQUIRE( emp5 );
    REQUIRE( emp77 );
}
