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

BOOST_AUTO_TEST_SUITE_END()

#endif /* end of include guard: CHAINFUNCTOR_QZZUXKYU */

