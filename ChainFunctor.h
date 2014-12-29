/*
 * =====================================================================================
 *
 *       Filename:  ChainFunctor.h
 *
 *    Description:  Chain functor tests
 *
 *        Version:  1.0
 *        Created:  10/21/2014 07:54:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef CHAINFUNCTOR_QZZUXKYU
#define CHAINFUNCTOR_QZZUXKYU

#include <string>
#include <boost/algorithm/string.hpp>

#include "TestDefs.h"

BOOST_AUTO_TEST_SUITE( chain_functor )

std::string profanityFilter(const std::string& s) {
    std::string mutant = s;
    boost::replace_all(mutant,"shizzle","*******");
    boost::replace_all(mutant,"drizzle","*******");
    boost::replace_all(mutant,"SHIZZLE","*******");
    boost::replace_all(mutant,"DRIZZLE","*******");
    return mutant;
}

BOOST_AUTO_TEST_CASE( chain_functor_string_filter )
{
    TEMPLATIOUS_TRIPLET_STD;

    std::string str = "What I'm I doing? Don't shizzle my drizzle yo.";
    auto underSqr = [](std::string s) { return boost::replace_all_copy(s," ","_"); };
    auto func = SF::chainFunctor(
            boost::to_upper_copy<std::string>,
            profanityFilter,
            underSqr);

    // unfortunately, if template function has
    // default argument we HAVE to pass it
    // or it won't compile.
    std::string res = func(str,std::locale());
    BOOST_CHECK( res == "WHAT_I'M_I_DOING?_DON'T_*******_MY_*******_YO." );
}

namespace
{
TEMPLATIOUS_TRIPLET_STD;

// STATEFUL
auto mulDo = [](int& a) { a = a * 7; };
auto mulUndo = [](int& a) { a = a / 7; };

auto addDo = [](int& a) { a = a + 7; };
auto addUndo = [](int& a) { a = a - 7; };

auto mulDo2 = [](int& a) { a = a * 17; };
auto mulUndo2 = [](int& a) { a = a / 17; };

auto addDo2 = [](int& a) { a = a + 17; };
auto addUndo2 = [](int& a) { a = a - 17; };

// FUNCTIONAL
auto mulDoF = [](int a) { return a * 7; };
auto mulUndoF = [](int a) { return a / 7; };

auto addDoF = [](int a) { return a + 7; };
auto addUndoF = [](int a) { return a - 7; };

auto mulDo2F = [](int a) { return a * 17; };
auto mulUndo2F = [](int a) { return a / 17; };

auto addDo2F = [](int a) { return a + 17; };
auto addUndo2F = [](int a) { return a - 17; };

auto fF = SF::chainFunctor(
    SF::functorPair(mulDoF,mulUndoF),
    SF::functorPair(addDoF,addUndoF),
    SF::functorPair(mulDo2F,mulUndo2F),
    SF::functorPair(addDo2F,addUndo2F)
);

auto fS = SF::chainFunctor<true>(
    SF::functorPair(mulDo,mulUndo),
    SF::functorPair(addDo,addUndo),
    SF::functorPair(mulDo2,mulUndo2),
    SF::functorPair(addDo2,addUndo2)
);

}

BOOST_AUTO_TEST_CASE( chain_functor_math_reverse_functional )
{
    int inter = fF(7);

    BOOST_CHECK(inter == 969);

    int back = fF.doBwd(inter);

    BOOST_CHECK(back == 7);
}

BOOST_AUTO_TEST_CASE( chain_functor_math_reverse_stateful )
{
    int curr = 7;

    fS(curr);

    BOOST_CHECK(curr == 969);

    fS.doBwd(curr);

    BOOST_CHECK(curr == 7);
}

BOOST_AUTO_TEST_CASE( chain_functor_math_invert_functional )
{
    auto rv = fF.reverse();

    int curr = rv.doBwd(7);

    BOOST_CHECK(curr == 969);

    int back = rv(curr);

    BOOST_CHECK(back == 7);
}

BOOST_AUTO_TEST_CASE( chain_functor_math_invert_stateful )
{
    auto rv = fS.reverse();

    int curr = 7;

    rv.doBwd(curr);

    BOOST_CHECK(curr == 969);

    rv(curr);

    BOOST_CHECK(curr == 7);
}

BOOST_AUTO_TEST_CASE( chain_functor_math_get_do_undo_functional )
{
    auto d = fF.getDo();
    auto u = fF.getUndo();

    int curr = d(7);

    BOOST_CHECK(curr == 969);

    int back = u(curr);

    BOOST_CHECK(back == 7);
}

BOOST_AUTO_TEST_CASE( chain_functor_math_get_do_undo_stateful )
{
    auto d = fS.getDo();
    auto u = fS.getUndo();

    int curr = 7;

    d(curr);

    BOOST_CHECK(curr == 969);

    u(curr);

    BOOST_CHECK(curr == 7);
}

namespace
{
    struct SomeData {
        SomeData(int a,double b,char c)
            : _a(a), _b(b), _c(c) {}
        int _a;
        double _b;
        char _c;

        bool operator==(const SomeData& s) const {
            return s._a == _a && s._b == _b && s._c == _c;
        }
    };

    auto encryptF =
    [](const std::vector<char>& v) {
        std::vector<char> vn;
        SA::addCustom(vn,
            [](char c) { return c ^ '7'; },
            v
        );
        return std::move(vn);
    };

    auto turnToBytesF =
    [](const SomeData& d) {
        std::vector<char> res;

        const int sz = sizeof(SomeData);
        char tmp[sz];
        memcpy(tmp,&d,sz);

        SA::add(res,tmp);
        return std::move(res);
    };

    auto makeFromBytesF =
    [](const std::vector<char>& v) {
        const int sz = sizeof(SomeData);
        char tmp[sz];
        SM::distribute(v,tmp);

        SomeData* ptr = reinterpret_cast<SomeData*>(tmp);
        return SomeData(*ptr);
    };

    auto sdFctor = SF::chainFunctor(
        SF::functorPair(turnToBytesF,makeFromBytesF),
        SF::functorPair(encryptF,encryptF)
    );

// PR UTIL
    auto prnt = SF::streamFunctor(std::cout);

    auto prSomeData =
    [](const SomeData& s) {
        SM::callEach(prnt,
            "_a: ",s._a,", _b: ",s._b,", _c: ",s._c,"\n");
    };

    auto prAny = SF::matchFunctor(
        SF::matchLoose<SomeData>(prSomeData),
        SF::matchAny(prnt)
    );
}

BOOST_AUTO_TEST_CASE( chain_functor_serialize_and_deserialize )
{
    SomeData d(7,7.777,'7');
    auto gargle = sdFctor(d);
    auto back = sdFctor.doBwd(gargle);
    BOOST_CHECK( back == d );
}

BOOST_AUTO_TEST_SUITE_END()

#endif /* end of include guard: CHAINFUNCTOR_QZZUXKYU */

