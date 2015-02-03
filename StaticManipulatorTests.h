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

BOOST_AUTO_TEST_CASE( static_manipulator_tests_sum_custom )
{
    std::vector< std::string > v;

    SA::add(
        v,
        "3.14",
        "7.7",
        "8.7",
        "77"
    );

    int a = 7;
    char b = '7';
    auto datPack = SF::pack(
        1,2,3,4,5,6,7,
        SF::pack(a,b,5,'8',17));

    // if you encounter string try to convert to double
    auto conv =
        SF::matchFunctor(
            SF::matchLoose< std::string >(
                [](const std::string& s) {
                    return std::atof(s.c_str());
                }),
            SF::matchAnyForward()
        );

    double sum = SM::sumS(conv,v,datPack);
    BOOST_CHECK( std::abs(264.54 - sum) < 0.0000001 );
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

BOOST_AUTO_TEST_CASE( static_manipulator_tests_avg_all_at_once )
{
    TEMPLATIOUS_TRIPLET_STD;

    int a = 7;
    char b = '7';
    auto datPack = SF::pack(
        SF::seqI(1,99),
        1,2,3,4,5,6,7,
        SF::pack(a,b,5,'8',17));

    double avg = SM::avg(datPack);
    BOOST_CHECK( std::abs(46.108108108 - avg) < 0.0000001 );
}

BOOST_AUTO_TEST_CASE( static_manipulator_tests_avg_custom )
{
    std::vector< std::string > v;

    SA::add(
        v,
        "3.14",
        "7.7",
        "8.7",
        "77"
    );

    int a = 7;
    char b = '7';
    auto datPack = SF::pack(
        1,2,3,4,5,6,7,
        SF::pack(a,b,5,'8',17));

    // if you encounter string try to convert to double
    auto conv =
        SF::matchFunctor(
            SF::matchLoose< std::string >(
                [](const std::string& s) {
                    return std::atof(s.c_str());
                }),
            SF::matchAnyForward()
        );

    double avg = SM::avgS(conv,v,datPack);
    BOOST_CHECK( std::abs(16.53375 - avg) < 0.0000001 );
}

BOOST_AUTO_TEST_CASE( static_manipulator_tests_quadro )
{
    TEMPLATIOUS_TRIPLET_STD;

    auto s = SF::seqI(1,100);
    std::vector<int> v;
    SA::add(v,SF::seqI(7,77));

    unsigned long long sum = 0;
    SM::quadro([&](int x,int y,int z) {
        sum += x * y * z; }, s,v,s);

    const unsigned long long QUADRO_SUM = 76048455000;

    BOOST_CHECK( std::numeric_limits<
        unsigned long long>::max() > QUADRO_SUM );
    BOOST_CHECK( sum == QUADRO_SUM);
}

BOOST_AUTO_TEST_CASE( static_manipulator_tests_map )
{
    TEMPLATIOUS_TRIPLET_STD;

    auto s = SF::seqI(100);
    std::vector<long> v;
    SA::add(v,SF::seqI(7,107));
    std::list<short> l;
    SA::add(l,SF::seqI(77,177));

    bool diffGood = true;
    auto res = SM::map< int >(
        [&](int i,long j,short k) {
            diffGood &= (j - i == 7);
            diffGood &= (k - j == 70);
            return i + j + k;
        },
        s,v,l
    );

    int mappedSum = SM::sum(res);
    int manSum = SM::sum(s,v,l);
    BOOST_CHECK( manSum == mappedSum );
    BOOST_CHECK( diffGood );
}

BOOST_AUTO_TEST_CASE( static_manipulator_tests_map_size_assertion )
{
    TEMPLATIOUS_TRIPLET_STD;

    auto s1 = SF::seqL(100);
    auto s2 = SF::seqI(100);

    bool caught = false;
    try {
        SM::map< int >([](int i,int j) { return i + j; },s1,s2);
    } catch (const templatious::MapFunctionNotEqualException& e) {
        caught = true;
    }

    BOOST_CHECK( caught );
}

BOOST_AUTO_TEST_CASE( static_manipulator_tests_map_size_assertion_proxy )
{
    TEMPLATIOUS_TRIPLET_STD;

    auto s = SF::seqL(100);
    auto l = [](int i) { return i > 77; };

    auto p1 = SF::filter(s,l);
    auto p2 = SF::filter(s,l);

    // should not be used with proxies,
    // regardless if actual size is identical
    bool caught = false;
    try {
        SM::map< int >([](int i,int j) { return i + j; },p1,p2);
    } catch (const templatious::MapFunctionNotEqualException& e) {
        caught = true;
    }

    BOOST_CHECK( caught );
}

BOOST_AUTO_TEST_CASE( static_manipulator_tests_map_with_pass_index )
{
    TEMPLATIOUS_TRIPLET_STD;

    std::vector<long> v;
    SA::add(v,SF::seqI(7,107));
    std::list<short> l;
    SA::add(l,SF::seqI(77,177));

    bool diffGood = true;
    auto res = SM::map< int, true >(
        [&](int i,long j,short k) {
            diffGood &= (j - i == 7);
            diffGood &= (k - j == 70);
            return i + j + k;
        },
        v,l
    );

    int mappedSum = SM::sum(res);
    int manSum = SM::sum(SF::seqI(100),v,l);
    BOOST_CHECK( manSum == mappedSum );
    BOOST_CHECK( diffGood );
}

BOOST_AUTO_TEST_CASE( static_manipulator_tests_traverse )
{
    TEMPLATIOUS_TRIPLET_STD;

    auto s = SF::seqI(100);
    std::vector<long> v;
    SA::add(v,SF::seqI(7,107));
    std::list<short> l;
    SA::add(l,SF::seqI(77,177));

    bool diffGood = true;
    int mappedSum = 0;
    SM::traverse(
        [&](int i,long j,short k) {
            diffGood &= (j - i == 7);
            diffGood &= (k - j == 70);
            mappedSum += i + j + k;
        },
        s,v,l
    );

    int manSum = SM::sum(s,v,l);
    BOOST_CHECK( manSum == mappedSum );
    BOOST_CHECK( diffGood );
}

BOOST_AUTO_TEST_CASE( static_manipulator_tests_traverse_size_assertion )
{
    TEMPLATIOUS_TRIPLET_STD;

    auto s1 = SF::seqL(100);
    auto s2 = SF::seqI(100);

    bool caught = false;
    try {
        SM::traverse([](int i,int j) { return i + j; },s1,s2);
    } catch (const templatious::TraverseFunctionNotEqualException& e) {
        caught = true;
    }

    BOOST_CHECK( caught );
}

BOOST_AUTO_TEST_CASE( static_manipulator_tests_traverse_size_assertion_proxy )
{
    TEMPLATIOUS_TRIPLET_STD;

    auto s = SF::seqL(100);
    auto l = [](int i) { return i > 77; };

    auto p1 = SF::filter(s,l);
    auto p2 = SF::filter(s,l);

    // should not be used with proxies,
    // regardless if actual size is identical
    bool caught = false;
    try {
        SM::traverse([](int i,int j) { return i + j; },p1,p2);
    } catch (const templatious::TraverseFunctionNotEqualException& e) {
        caught = true;
    }

    BOOST_CHECK( caught );
}

BOOST_AUTO_TEST_CASE( static_manipulator_tests_traverse_with_pass_index )
{
    TEMPLATIOUS_TRIPLET_STD;

    std::vector<long> v;
    SA::add(v,SF::seqI(7,107));
    std::list<short> l;
    SA::add(l,SF::seqI(77,177));

    bool diffGood = true;
    int mappedSum = 0;
    SM::traverse<true>(
        [&](int i,long j,short k) {
            diffGood &= (j - i == 7);
            diffGood &= (k - j == 70);
            mappedSum += i + j + k;
        },
        v,l
    );

    int manSum = SM::sum(SF::seqI(100),v,l);
    BOOST_CHECK( manSum == mappedSum );
    BOOST_CHECK( diffGood );
}

BOOST_AUTO_TEST_CASE( static_manipulator_tests_fold_mul )
{
    std::vector<long> v;
    SA::add(v,SF::seqI(2,3));

    auto r = SM::fold(1,[](long a,long b) { return a * b; },
        v,SF::seqI(4,5));

    BOOST_CHECK( r == 120 );
}

BOOST_AUTO_TEST_CASE( static_manipulator_tests_fold_sum )
{
    std::vector<long> v;
    SA::add(v,SF::seqI(2,3));

    auto r = SM::fold(0,[](long a,long b) { return a + b; },
        v,SF::seqI(4,5));

    BOOST_CHECK( r == 2 + 3 + 4 + 5 );
}

BOOST_AUTO_TEST_CASE( static_manipulator_collection_equality_basic )
{
    std::vector<long> v;
    auto s = SF::seqL(100);
    SA::add(v,s);

    BOOST_CHECK( SM::areCollectionsEqual(v,s) );
}

BOOST_AUTO_TEST_CASE( static_manipulator_collection_equality_multi )
{
    std::vector<long> v;
    auto s = SF::seqL(100);
    SA::add(v,s);

    BOOST_CHECK( SM::areCollectionsEqual(v,s,SF::seqL(100)) );
}

BOOST_AUTO_TEST_CASE( static_manipulator_collection_equality_proxy )
{
    std::vector<long> v;
    auto s = SF::seqL(100);
    SA::add(v,s);

    auto f = SF::filter(v,[](long i) { return true; });

    BOOST_CHECK( SM::areCollectionsEqual(v,s,f) );
}

BOOST_AUTO_TEST_CASE( static_manipulator_collection_equality_proxy_unequal )
{
    std::vector<long> v;
    auto s = SF::seqL(100);
    SA::add(v,s);

    auto f = SF::filter(v,[](long i) { return i % 7; });

    BOOST_CHECK( !SM::areCollectionsEqual(v,f) );
}

BOOST_AUTO_TEST_CASE( static_manipulator_collection_equality_custom_const_fctor )
{
    std::vector<long> v;
    auto s = SF::seqL(100);
    SA::add(v,s);

    BOOST_CHECK( SM::areCollectionsEqualS(
        [](long a,long b) { return a == b; },
        v,s,SF::seqL(100)) );
}

BOOST_AUTO_TEST_CASE( static_manipulator_collection_equality_custom_const_fctor_anything_equal )
{
    std::vector<long> v;
    auto s = SF::seqL(100);
    SA::add(v,s);

    auto cmp = SF::seqL(100,200);

    auto fctor = [](long a,long b) { return true; };

    // functor says collections are equal
    BOOST_CHECK( SM::areCollectionsEqualS(fctor,v,cmp) );

    // normal check should say they aren't
    BOOST_CHECK( !SM::areCollectionsEqual(v,cmp) );

    // if size is not same collections aren't equal
    // regardless of functor
    BOOST_CHECK( !SM::areCollectionsEqualS(fctor,v,SF::seqL(100,201)) );
}

BOOST_AUTO_TEST_CASE( static_manipulator_forall )
{
    auto s = SF::seqL(100);

    auto fctor = [](long l) { return l >= 0 && l < 100; };
    BOOST_CHECK( SM::forAll(fctor,s) );
}

BOOST_AUTO_TEST_CASE( static_manipulator_forall_var )
{
    auto fctor = [](long l) { return l == 7; };
    BOOST_CHECK( SM::forAll(fctor,
        7,7,7,7,7,7,7,SF::repS(7,7)) );

    BOOST_CHECK( !SM::forAll(fctor,
        7,7,7,77,7,7,SF::repS(7,7)) );
}

BOOST_AUTO_TEST_CASE( static_manipulator_forall_pack )
{
    auto p = SF::pack(7,7);
    BOOST_CHECK( SM::forAllP(
        [](int i) { return i == 7; },
        SF::pack(7,7,7),
        7,7,p
    ) );

    BOOST_CHECK( !SM::forAllP(
        [](int i) { return i == 7; },
        SF::pack(7,6,7),
        7,7,p
    ) );

    auto p2 = SF::pack(7,6);
    BOOST_CHECK( !SM::forAllP(
        [](int i) { return i == 7; },
        SF::pack(7,7,7),
        7,7,p2
    ) );
}

BOOST_AUTO_TEST_CASE( static_manipulator_exists )
{
    auto s = SF::seqL(100);

    auto fctorGood = [](long l) { return l == 7; };
    auto fctorBad = [](long l) { return l == 777; };

    BOOST_CHECK( SM::exists(fctorGood,s) );
    BOOST_CHECK( !SM::exists(fctorBad,s) );
}


BOOST_AUTO_TEST_CASE( static_manipulator_exists_var )
{
    auto s = SF::seqL(100);

    auto fctor = [](long l) { return l == 777; };

    BOOST_CHECK( SM::exists(fctor,s,-1,777,-2) );

}

BOOST_AUTO_TEST_SUITE_END();

#endif /* end of include guard: STATICMANIPULATORTESTS_JT4V7DJV */
