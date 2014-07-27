/*
 * =====================================================================================
 *
 *       Filename:  tests.h
 *
 *    Description:  some tests
 *
 *        Version:  1.0
 *        Created:  07/27/2014 06:00:48 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef TESTS_HPIIGL7J
#define TESTS_HPIIGL7J

BOOST_AUTO_TEST_CASE(TestThatSucceeds)
{
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE_EXPECTED_FAILURES(TestThatFails, 1)

    BOOST_AUTO_TEST_CASE(TestThatFails)
{
    //BOOST_CHECK(false);
    BOOST_CHECK(true);
}


#endif /* end of include guard: TESTS_HPIIGL7J */
