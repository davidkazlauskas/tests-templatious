/*
 * =====================================================================================
 *
 *       Filename:  StaticVector.h
 *
 *    Description:  Static vector test
 *
 *        Version:  1.0
 *        Created:  07/28/2014 04:49:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef STATICVECTOR_8L32QS9F
#define STATICVECTOR_8L32QS9F

#include "../TestAlgs.h"

BOOST_AUTO_TEST_SUITE( static_vector_tests );

BOOST_AUTO_TEST_CASE( static_vector_tests_basic )
{

    const size_t SIZE = 256; 
    size_t arr[SIZE];
    auto v = tt::t::makeStaticVector(arr);

    BOOST_CHECK(tt::existantCollectionTest(v));
}

BOOST_AUTO_TEST_CASE( static_vector_tests_proxy )
{

    const size_t SIZE = 256;
    size_t arr[SIZE];
    auto v = tt::t::makeStaticVector(arr);

    BOOST_CHECK(tt::proxyTest(v));
}

BOOST_AUTO_TEST_CASE( static_vector_tests_proxy_const )
{

    const size_t SIZE = 256;
    size_t arr[SIZE];
    auto v = tt::t::makeStaticVector(arr);

    BOOST_CHECK(tt::constProxyTest(v));
}

BOOST_AUTO_TEST_CASE( static_vector_tests_virtual )
{

    const size_t SIZE = 256;
    size_t arr[SIZE];
    auto v = tt::t::makeStaticVector(arr);

    BOOST_CHECK(tt::virtualTest(v));
}

// AND THIS IS WHEN I FOUND OUT THAT STATIC ARRAYS
// INITIALIZE THEIR ELEMENTS WITH DEFAULT CONSTRUCTOR
//BOOST_AUTO_TEST_CASE( static_vector_tests_destruction )
//{
    //struct UniqueToken {};
    //typedef tt::ConstructorCountCollection<UniqueToken> ValType;

    //const size_t SIZE = 256;
    //ValType arr[SIZE];
    //auto v = tt::t::makeStaticVector(arr);

    //BOOST_CHECK(tt::constructionCountCollectionTest<UniqueToken>(v));
    //BOOST_CHECK(ValType::count() == 0);
//}

BOOST_AUTO_TEST_SUITE_END();

#endif /* end of include guard: STATICVECTOR_8L32QS9F */
