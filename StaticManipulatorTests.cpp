/*
 * =====================================================================================
 *
 *       Filename:  StaticManipulatorTests.hpp
 *
 *    Description:  Static manipulator tests
 *
 *        Version:  1.0
 *        Created:  12/26/2014 09:20:37 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Kazlauskas (dk), david@templatious.org
 *
 * =====================================================================================
 */

#include "TestDefs.hpp"

TEST_CASE( "static_manipulator_tests_sum_collection", "[static_manipulator_tests]" )
{
    std::vector<int> v;
    SA::add(v,SF::seqI(1,77));

    int sum = SM::sum<int>(v);

    REQUIRE(sum == 3003);
}

TEST_CASE( "static_manipulator_tests_sum_variables", "[static_manipulator_tests]" )
{
    int a = 7;
    char b = '7';

    REQUIRE( SM::sum<int>(a,b,1,2,'8') ==
        7 + 55 + 1 + 2 + 56
    );
}

TEST_CASE( "static_manipulator_tests_sum_pack", "[static_manipulator_tests]" )
{
    int a = 7;
    char b = '7';

    auto p = SF::pack(a,b,1,2,'a');
    REQUIRE( SM::sum<int>(p) ==
        7 + 55 + 1 + 2 + 97
    );
}

TEST_CASE( "static_manipulator_tests_sum_all_at_once", "[static_manipulator_tests]" )
{
    int a = 7;
    char b = '7';

    const int ALL_SUM = 3003 + 121 + 162;
    auto p = SF::pack(a,b,1,2,'a');
    std::vector<int> v;
    SA::add(v,SF::seqI(1,77));

#define STAMA_T_SUM_ROW a,b,1,2,'8'

    { // col, pack, var
        REQUIRE(
            SM::sum<int>(v,p,STAMA_T_SUM_ROW)
            == ALL_SUM
        );
    }

    { // col, var, pack
        REQUIRE(
            SM::sum<int>(v,STAMA_T_SUM_ROW,p)
            == ALL_SUM
        );
    }


    { // var, col, pack
        REQUIRE(
            SM::sum<int>(STAMA_T_SUM_ROW,v,p)
            == ALL_SUM
        );
    }

    { // var, pack, col
        REQUIRE(
            SM::sum<int>(STAMA_T_SUM_ROW,p,v)
            == ALL_SUM
        );
    }


    { // pack, var, col
        REQUIRE(
            SM::sum<int>(p,STAMA_T_SUM_ROW,v)
            == ALL_SUM
        );
    }

    { // pack, col, var
        REQUIRE(
            SM::sum<int>(p,v,STAMA_T_SUM_ROW)
            == ALL_SUM
        );
    }
}

TEST_CASE( "static_manipulator_tests_sum_custom", "[static_manipulator_tests]" )
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
    REQUIRE( std::abs(264.54 - sum) < 0.0000001 );
}

TEST_CASE( "static_manipulator_tests_avg_basic", "[static_manipulator_tests]" )
{
    REQUIRE( SM::avg<int>(SF::seqI(1,99)) == 50 );

    REQUIRE( SM::avg<int>(1,2,3,4,5,6,7) == 4 );

    int a = 7;
    char b = '7';
    REQUIRE( SM::avg<int>(SF::pack(a,b,5,'8',17)) == 28 );
}

TEST_CASE( "static_manipulator_tests_avg_all_at_once", "[static_manipulator_tests]" )
{
    int a = 7;
    char b = '7';
    auto datPack = SF::pack(
        SF::seqI(1,99),
        1,2,3,4,5,6,7,
        SF::pack(a,b,5,'8',17));

    double avg = SM::avg(datPack);
    REQUIRE( std::abs(46.108108108 - avg) < 0.0000001 );
}

TEST_CASE( "static_manipulator_tests_avg_custom", "[static_manipulator_tests]" )
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
    REQUIRE( std::abs(16.53375 - avg) < 0.0000001 );
}

TEST_CASE( "static_manipulator_tests_quadro", "[static_manipulator_tests]" )
{
    auto s = SF::seqI(1,100);
    std::vector<int> v;
    SA::add(v,SF::seqI(7,77));

    unsigned long long sum = 0;
    SM::quadro([&](int x,int y,int z) {
        sum += x * y * z; },s,v,s);

    const unsigned long long QUADRO_SUM = 76048455000;

    REQUIRE( std::numeric_limits<
        unsigned long long>::max() > QUADRO_SUM );
    REQUIRE( sum == QUADRO_SUM);
}

TEST_CASE( "static_manipulator_tests_quadro_single_mutate", "[static_manipulator_tests]" )
{
    auto s = SF::seqI(1,100);
    std::vector<int> v;
    SA::add(v,s);
    long sum = 0;
    SM::quadro([&](int& x) {
        sum += x;
        x = 0;
    },v);

    REQUIRE( sum == SM::sum<long>(s) );
    REQUIRE( 0 == SM::sum<long>(v) );
}

TEST_CASE( "static_manipulator_tests_quadro_mutate_w_index", "[static_manipulator_tests]" )
{
    auto s = SF::seqI(1,100);
    std::vector<int> v;
    std::list<int> l;
    SA::add(v,s);
    SA::add(l,s);

    long sum = 0;
    SM::quadro<true>([&](long idx,int& x,int& y) {
        sum += x;
        sum += y;
        sum += idx;
        x = 0;
        y = 0;
    },v,l);

    long singleSum = SM::sum<int>(s);
    long idxSum = SM::sum<int>(SF::seqL(100*100));

    long expSum = singleSum * 2 + idxSum;

    REQUIRE( sum == expSum );
}

TEST_CASE( "static_manipulator_tests_quadro_stop", "[static_manipulator_tests]" )
{
    auto s = SF::seqI(1,100);
    std::vector<int> v;
    std::list<int> l;
    SA::add(v,s);
    SA::add(l,s);

    long sum = 0;
    int cnt = 3;
    SM::quadro([&](int x,int y) {
        sum += x;
        sum += y;
        --cnt;
        return cnt > 0;
    },v,l);

    long expSum = 1 + 1 + 1 + 1 + 2 + 3;
    REQUIRE( sum == expSum );
}

TEST_CASE( "static_manipulator_tests_map", "[static_manipulator_tests]" )
{
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
    REQUIRE( manSum == mappedSum );
    REQUIRE( diffGood );
}

TEST_CASE( "static_manipulator_tests_map_size_assertion", "[static_manipulator_tests]" )
{
    auto s1 = SF::seqL(100);
    auto s2 = SF::seqI(100);

    bool caught = false;
    try {
        SM::map< int >([](int i,int j) { return i + j; },s1,s2);
    } catch (const templatious::MapFunctionNotEqualException& e) {
        caught = true;
    }

    REQUIRE( caught );
}

TEST_CASE( "static_manipulator_tests_map_size_assertion_proxy", "[static_manipulator_tests]" )
{
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

    REQUIRE( caught );
}

TEST_CASE( "static_manipulator_tests_map_with_pass_index", "[static_manipulator_tests]" )
{
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
    REQUIRE( manSum == mappedSum );
    REQUIRE( diffGood );
}

TEST_CASE( "static_manipulator_tests_traverse", "[static_manipulator_tests]" )
{
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
    REQUIRE( manSum == mappedSum );
    REQUIRE( diffGood );
}

TEST_CASE( "static_manipulator_tests_traverse_size_assertion", "[static_manipulator_tests]" )
{
    auto s1 = SF::seqL(100);
    auto s2 = SF::seqI(100);

    bool caught = false;
    try {
        SM::traverse([](int i,int j) { return i + j; },s1,s2);
    } catch (const templatious::TraverseFunctionNotEqualException& e) {
        caught = true;
    }

    REQUIRE( caught );
}

TEST_CASE( "static_manipulator_tests_traverse_size_assertion_proxy", "[static_manipulator_tests]" )
{
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

    REQUIRE( caught );
}

TEST_CASE( "static_manipulator_tests_traverse_with_pass_index", "[static_manipulator_tests]" )
{
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
    REQUIRE( manSum == mappedSum );
    REQUIRE( diffGood );
}

TEST_CASE( "static_manipulator_tests_traverse_man_stop", "[static_manipulator_tests]" )
{
    std::vector<long> v;
    SA::add(v,SF::seqI(7,107));
    std::list<short> l;
    SA::add(l,SF::seqI(77,177));

    int sum = 0;
    int cnt = 3;
    SM::traverse(
        [&](long a,short b) {
            sum += a;
            sum += b;
            --cnt;
            return cnt > 0;
        },
        v,l
    );

    int expSum = 7 + 8 + 9 + 77 + 78 + 79;
    REQUIRE( sum == expSum );
}

TEST_CASE( "static_manipulator_tests_fold_mul", "[static_manipulator_tests]" )
{
    std::vector<long> v;
    SA::add(v,SF::seqI(2,3));

    auto r = SM::fold(1,[](long a,long b) { return a * b; },
        v,SF::seqI(4,5));

    REQUIRE( r == 120 );
}

TEST_CASE( "static_manipulator_tests_fold_sum", "[static_manipulator_tests]" )
{
    std::vector<long> v;
    SA::add(v,SF::seqI(2,3));

    auto r = SM::fold(0,[](long a,long b) { return a + b; },
        v,SF::seqI(4,5));

    REQUIRE( r == 2 + 3 + 4 + 5 );
}

TEST_CASE( "static_manipulator_collection_equality_basic", "[static_manipulator_tests]" )
{
    std::vector<long> v;
    auto s = SF::seqL(100);
    SA::add(v,s);

    REQUIRE( SM::areCollectionsEqual(v,s) );
}

TEST_CASE( "static_manipulator_collection_equality_multi", "[static_manipulator_tests]" )
{
    std::vector<long> v;
    auto s = SF::seqL(100);
    SA::add(v,s);

    REQUIRE( SM::areCollectionsEqual(v,s,SF::seqL(100)) );
}

TEST_CASE( "static_manipulator_collection_equality_proxy", "[static_manipulator_tests]" )
{
    std::vector<long> v;
    auto s = SF::seqL(100);
    SA::add(v,s);

    auto f = SF::filter(v,[](long i) { return true; });

    REQUIRE( SM::areCollectionsEqual(v,s,f) );
}

TEST_CASE( "static_manipulator_collection_equality_proxy_unequal", "[static_manipulator_tests]" )
{
    std::vector<long> v;
    auto s = SF::seqL(100);
    SA::add(v,s);

    auto f = SF::filter(v,[](long i) { return i % 7; });

    REQUIRE( !SM::areCollectionsEqual(v,f) );
}

TEST_CASE( "static_manipulator_collection_equality_custom_const_fctor", "[static_manipulator_tests]" )
{
    std::vector<long> v;
    auto s = SF::seqL(100);
    SA::add(v,s);

    REQUIRE( SM::areCollectionsEqualS(
        [](long a,long b) { return a == b; },
        v,s,SF::seqL(100)) );
}

TEST_CASE( "static_manipulator_collection_equality_custom_const_fctor_anything_equal", "[static_manipulator_tests]" )
{
    std::vector<long> v;
    auto s = SF::seqL(100);
    SA::add(v,s);

    auto cmp = SF::seqL(100,200);

    auto fctor = [](long a,long b) { return true; };

    // functor says collections are equal
    REQUIRE( SM::areCollectionsEqualS(fctor,v,cmp) );

    // normal check should say they aren't
    REQUIRE( !SM::areCollectionsEqual(v,cmp) );

    // if size is not same collections aren't equal
    // regardless of functor
    REQUIRE( !SM::areCollectionsEqualS(fctor,v,SF::seqL(100,201)) );
}

TEST_CASE( "static_manipulator_collection_equality_almost_equal", "[static_manipulator_tests]" )
{
    auto a = SF::seqL(3);
    auto b = SF::seqL(4);

    auto aFlt = SF::filter(a,[](int) { return true; });
    auto bFlt = SF::filter(b,[](int) { return true; });

    REQUIRE( !SM::areCollectionsEqual(aFlt,bFlt) );
}

TEST_CASE( "static_manipulator_collection_equality_first_two_fail", "[static_manipulator_tests]" )
{
    bool result = SM::areCollectionsEqualS(
        [](int a,int b) { return a == b; },
        SF::seqL(3),
        SF::seqL(4),
        SF::seqL(3)
    );

    REQUIRE( !result );
}

TEST_CASE( "static_manipulator_forall", "[static_manipulator_tests]" )
{
    auto s = SF::seqL(100);

    auto fctor = [](long l) { return l >= 0 && l < 100; };
    REQUIRE( SM::forAll(fctor,s) );
}

TEST_CASE( "static_manipulator_forall_var", "[static_manipulator_tests]" )
{
    auto fctor = [](long l) { return l == 7; };
    REQUIRE( SM::forAll(fctor,
        7,7,7,7,7,7,7,SF::repS(7,7)) );

    REQUIRE( !SM::forAll(fctor,
        7,7,7,77,7,7,SF::repS(7,7)) );
}

TEST_CASE( "static_manipulator_forall_pack", "[static_manipulator_tests]" )
{
    auto p = SF::pack(7,7);
    REQUIRE( SM::forAllP(
        [](int i) { return i == 7; },
        SF::pack(7,7,7),
        7,7,p
    ) );

    REQUIRE( !SM::forAllP(
        [](int i) { return i == 7; },
        SF::pack(7,6,7),
        7,7,p
    ) );

    auto p2 = SF::pack(7,6);
    REQUIRE( !SM::forAllP(
        [](int i) { return i == 7; },
        SF::pack(7,7,7),
        7,7,p2
    ) );
}

TEST_CASE( "static_manipulator_exists", "[static_manipulator_tests]" )
{
    auto s = SF::seqL(100);

    auto fctorGood = [](long l) { return l == 7; };
    auto fctorBad = [](long l) { return l == 777; };

    REQUIRE( SM::exists(fctorGood,s) );
    REQUIRE( !SM::exists(fctorBad,s) );
}


TEST_CASE( "static_manipulator_exists_var", "[static_manipulator_tests]" )
{
    auto s = SF::seqL(100);

    auto fctor = [](long l) { return l == 777; };

    REQUIRE( SM::exists(fctor,s,-1,777,-2) );
}

TEST_CASE( "static_manipulator_exists_pack", "[static_manipulator_tests]" )
{
    auto fctor = [](long l) { return l == 777; };

    REQUIRE( SM::existsP(fctor,1,2,3,
        SF::pack('7',777,'7'),4,5,6) );

    REQUIRE( !SM::existsP(fctor,1,2,3,
        SF::pack('7',77,'7'),4,5,6) );

    auto p1 = SF::pack(2,3,777);
    REQUIRE( SM::existsP(fctor,
        'a','b','c',p1,'d','e','f'));

    auto p2 = SF::pack(2,3,77);
    REQUIRE( !SM::existsP(fctor,
        'a','b','c',p2,'d','e','f'));
}

TEST_CASE( "static_manipulator_tests_is_sorted", "[static_manipulator_tests]" )
{
    auto s = SF::seqL(100);

    REQUIRE( SM::isSorted(s) );

    std::vector<int> v;
    SA::add(v,s);

    REQUIRE( SM::isSorted(v) );

    auto pr = v[7];
    v[7] = 1;
    REQUIRE( !SM::isSorted(v) );
    v[7] = pr;
    REQUIRE( SM::isSorted(v) );
    ++v[7];
    REQUIRE( SM::isSorted(v) );
    ++v[7];
    REQUIRE( !SM::isSorted(v) );
}

TEST_CASE( "static_manipulator_tests_is_sorted_special", "[static_manipulator_tests]" )
{
    std::vector<int> v;
    SA::add(v,SF::seqL(100));

    auto c = [](int a,int b) { return a > b; };

    REQUIRE( SM::isSorted(v) );
    REQUIRE( !SM::isSortedS(v,c) );

    std::reverse(SA::begin(v),SA::end(v));

    REQUIRE( !SM::isSorted(v) );
    REQUIRE( SM::isSortedS(v,c) );

    ++v[7];
    REQUIRE( SM::isSortedS(v,c) );
    ++v[7];
    REQUIRE( !SM::isSortedS(v,c) );
}

TEST_CASE( "static_manipulator_tests_is_sorted_edge_cases", "[static_manipulator_tests]" )
{
    std::vector<int> v;

    // empty collection is always sorted
    REQUIRE( SM::isSorted(v) );

    // collection with one element is always sorted
    SA::add(v,7);
    REQUIRE( SM::isSorted(v) );
}

TEST_CASE( "static_manipulator_tests_min", "[static_manipulator_tests]" ) {

    std::vector<int> v;

    SA::add(v,3,7,2,1,7,9);

    int r = SM::min<int>(v);
    REQUIRE( r == 1 );
}

TEST_CASE( "static_manipulator_tests_max", "[static_manipulator_tests]" ) {

    std::vector<int> v;

    SA::add(v,3,7,2,1,7,9);

    int r = SM::max<int>(v);
    REQUIRE( r == 9 );
}

TEST_CASE( "static_manipulator_tests_maxS_lambda", "[static_manipulator_tests]" ) {

    std::vector<int> v;

    SA::add(v,3,7,2,1,7,9);

    int r = SM::maxS<int>([](int a,int b) { return a > b; },v);
    REQUIRE( r == 9 );
}

TEST_CASE( "static_manipulator_tests_maxS_mutate", "[static_manipulator_tests]" ) {

    std::vector<int> v;

    SA::add(v,3,7,2,1,7,9);

    struct CompFctor {
        CompFctor() : _cnt(0) {}
        CompFctor(const CompFctor& c) : _cnt(c._cnt) {}

        bool operator()(int a,int b) { ++_cnt; return a > b; }

        int _cnt;
    };

    CompFctor c;
    int r = SM::maxS<int>(c,v);

    // make sure to use reference on stack
    REQUIRE( c._cnt == 5 );
    REQUIRE( r == 9 );

    // don't alter since copy
    r = SM::maxS<int>(CompFctor(c),v);
    REQUIRE( c._cnt == 5 );
    REQUIRE( r == 9 );
}

TEST_CASE( "static_manipulator_tests_max_reference", "[static_manipulator_tests]" ) {
    std::vector<int> v;

    SA::add(v,3,7,2,1,7,9);

    auto& ref = SM::max<int&>(v);
    ref = 4;
    REQUIRE( SA::getByIndex(v,5) == 4 );
}

TEST_CASE( "static_manipulator_tests_max_reference_first", "[static_manipulator_tests]" ) {
    std::vector<int> v;

    SA::add(v,3,7,2,1,7,1);

    SECTION("max") {
        auto& ref = SM::max<int&>(v);
        auto& expRef = SA::getByIndex(v,1);
        REQUIRE( ref == 7 );
        REQUIRE( std::addressof(ref) == std::addressof(expRef) );
    }

    SECTION("min") {
        auto& ref = SM::min<int&>(v);
        auto& expRef = SA::getByIndex(v,3);
        REQUIRE( ref == 1 );
        REQUIRE( std::addressof(ref) == std::addressof(expRef) );
    }
}

TEST_CASE( "static_manipulator_tests_min_transition", "[static_manipulator_tests]" ) {
    struct MyPod {
        MyPod(int a,double b) : _a(a), _b(b) {}
        int _a;
        double _b;
    };

    std::vector<MyPod> v;
    SA::add(v,MyPod(1,2),MyPod(3,4),MyPod(5,6),MyPod(7,8));

    SM::min<double&>(
        SF::select(
            SF::filter(
                v,
                [](const MyPod& c) { return c._a > 1; }
            ),
            [](MyPod& c) -> double& { return c._b; }
        )
    ) = 777;

    auto& r = SA::getByIndex(v,1);
    REQUIRE( r._a == 3 );
    REQUIRE( r._b == 777 );
}

TEST_CASE( "static_manipulator_tests_max_multitype", "[static_manipulator_tests]" ) {
    std::vector<int> v;

    SA::add(v,3,7,2,1,7,9);
    auto p = SF::pack(77,2,3,2);

    auto& ref = SM::max<int&>(p,v);
    auto& pref = p.get<0>();
    REQUIRE( ref == 77 );
    pref = 777;
    REQUIRE( ref == 777 );
    REQUIRE( std::addressof(ref) == std::addressof(pref) );

    int i = SM::max<int>(v,777,p);
    REQUIRE( i == 777 );
}
