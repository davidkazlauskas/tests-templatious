/*
 * =====================================================================================
 *
 *       Filename:  StaticManipulatorTests.h
 *
 *    Description:  Static manipulator tests
 *
 *        Version:  1.0
 *        Created:  12/26/2014 09:20:37 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef STATICMANIPULATORTESTS_JT4V7DJV
#define STATICMANIPULATORTESTS_JT4V7DJV

#include "TestDefs.h"

BOOST_AUTO_TEST_SUITE( static_manipulator_tests );

BOOST_AUTO_TEST_CASE( static_manipulator_tests_sum_collection )
{
    TEMPLATIOUS_TRIPLET_STD;
    std::vector<int> v;
    SA::add(v,SF::seqI(1,77));

    int sum = SM::sum<int>(v);

    BOOST_CHECK(sum == 3003);
}

BOOST_AUTO_TEST_CASE( static_manipulator_tests_sum_variables )
{
    int a = 7;
    char b = '7';

    BOOST_CHECK( SM::sum<int>(a,b,1,2,'8') ==
        7 + 55 + 1 + 2 + 56
    );
}

BOOST_AUTO_TEST_CASE( static_manipulator_tests_sum_pack )
{
    int a = 7;
    char b = '7';

    auto p = SF::pack(a,b,1,2,'a');
    BOOST_CHECK( SM::sum<int>(p) ==
        7 + 55 + 1 + 2 + 97
    );
}

BOOST_AUTO_TEST_CASE( static_manipulator_tests_sum_all_at_once )
{
    int a = 7;
    char b = '7';

    const int ALL_SUM = 3003 + 121 + 162;
    auto p = SF::pack(a,b,1,2,'a');
    std::vector<int> v;
    SA::add(v,SF::seqI(1,77));

#define STAMA_T_SUM_ROW a,b,1,2,'8'

    { // col, pack, var
        BOOST_CHECK(
            SM::sum<int>(v,p,STAMA_T_SUM_ROW)
            == ALL_SUM
        );
    }

    { // col, var, pack
        BOOST_CHECK(
            SM::sum<int>(v,STAMA_T_SUM_ROW,p)
            == ALL_SUM
        );
    }


    { // var, col, pack
        BOOST_CHECK(
            SM::sum<int>(STAMA_T_SUM_ROW,v,p)
            == ALL_SUM
        );
    }

    { // var, pack, col
        BOOST_CHECK(
            SM::sum<int>(STAMA_T_SUM_ROW,p,v)
            == ALL_SUM
        );
    }


    { // pack, var, col
        BOOST_CHECK(
            SM::sum<int>(p,STAMA_T_SUM_ROW,v)
            == ALL_SUM
        );
    }

    { // pack, col, var
        BOOST_CHECK(
            SM::sum<int>(p,v,STAMA_T_SUM_ROW)
            == ALL_SUM
        );
    }
}

BOOST_AUTO_TEST_CASE( static_manipulator_tests_avg_basic )
{
    TEMPLATIOUS_TRIPLET_STD;

    BOOST_CHECK( SM::avg<int>(SF::seqI(1,99)) == 50 );

    BOOST_CHECK( SM::avg<int>(1,2,3,4,5,6,7) == 4 );

    int a = 7;
    char b = '7';
    BOOST_CHECK( SM::avg<int>(SF::pack(a,b,5,'8',17)) == 28 );
}

BOOST_AUTO_TEST_SUITE_END();

#endif /* end of include guard: STATICMANIPULATORTESTS_JT4V7DJV */
