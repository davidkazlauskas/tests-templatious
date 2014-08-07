/*
 * =====================================================================================
 *
 *       Filename:  StdList.h
 *
 *    Description:  List unit test
 *
 *        Version:  1.0
 *        Created:  07/27/2014 09:09:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef STDLIST_UDCMQSLF
#define STDLIST_UDCMQSLF

#include "../TestAlgs.h"


BOOST_AUTO_TEST_CASE( std_list_tests )
{

    typedef std::list<int> V;
    V v;

    BOOST_CHECK(tt::existantCollectionTest(v));
}

//BOOST_AUTO_TEST_CASE( std_list_tests_proxy )
//{
    //typedef std::list<int> V;
    //V v;

    //BOOST_CHECK(tt::proxyTest(v));
//}

#endif /* end of include guard: STDLIST_UDCMQSLF */
