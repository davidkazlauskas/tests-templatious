/*
 * =====================================================================================
 *
 *       Filename:  ChainUsageTests.h
 *
 *    Description:  This header contains various chained usages
 *                  of templatious classes
 *
 *        Version:  1.0
 *        Created:  12/19/2014 05:22:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef CHAINUSAGETESTS_C94JK7M5
#define CHAINUSAGETESTS_C94JK7M5

#include "TestDefs.h"


TEST_CASE( "chained_usage_tests_varying_addition", "[chained_usage_tests]" )
{
    INIT_BALLER;

    std::vector<int> v;

    { // pack, seq, vars
        SA::clear(v);
        int a,b,c,d,e,f,g;
        auto p = SF::pack(a,b,c,d,e,f,g);
        SM::distribute(SF::seqI(1,7),p);

        SA::add(v,p,SF::seqI(17,27),a,b,c,d,e,f,g);

        sum = 0;
        SM::forEach(sf,v);

        REQUIRE( sum == 28 + 242 + 28 );
    }

    { // pack, vars, seq
        SA::clear(v);
        int a,b,c,d,e,f,g;
        auto p = SF::pack(a,b,c,d,e,f,g);
        SM::distribute(SF::seqI(1,7),p);

        SA::add(v,p,a,b,c,d,e,f,g,SF::seqI(17,27));

        sum = 0;
        SM::forEach(sf,v);

        REQUIRE( sum == 28 + 242 + 28 );
    }


    { // vars, pack, seq
        SA::clear(v);
        int a,b,c,d,e,f,g;
        auto p = SF::pack(a,b,c,d,e,f,g);
        SM::distribute(SF::seqI(1,7),p);

        SA::add(v,a,b,c,d,e,f,g,p,SF::seqI(17,27));

        sum = 0;
        SM::forEach(sf,v);

        REQUIRE( sum == 28 + 242 + 28 );
    }

    { // vars, seq, pack
        SA::clear(v);
        int a,b,c,d,e,f,g;
        auto p = SF::pack(a,b,c,d,e,f,g);
        SM::distribute(SF::seqI(1,7),p);

        SA::add(v,a,b,c,d,e,f,g,SF::seqI(17,27),p);

        sum = 0;
        SM::forEach(sf,v);

        REQUIRE( sum == 28 + 242 + 28 );
    }


    { // seq, vars, pack
        SA::clear(v);
        int a,b,c,d,e,f,g;
        auto p = SF::pack(a,b,c,d,e,f,g);
        SM::distribute(SF::seqI(1,7),p);

        SA::add(v,SF::seqI(17,27),a,b,c,d,e,f,g,p);

        sum = 0;
        SM::forEach(sf,v);

        REQUIRE( sum == 28 + 242 + 28 );
    }

    { // seq, pack, vars
        SA::clear(v);
        int a,b,c,d,e,f,g;
        auto p = SF::pack(a,b,c,d,e,f,g);
        SM::distribute(SF::seqI(1,7),p);

        SA::add(v,SF::seqI(17,27),p,a,b,c,d,e,f,g);

        sum = 0;
        SM::forEach(sf,v);

        REQUIRE( sum == 28 + 242 + 28 );
    }


#define DAT_7_PACK SF::pack(a,b,c,d,e,f,g)
    { // RVALUE PACK, pack, seq, vars
        SA::clear(v);
        int a,b,c,d,e,f,g;
        SM::distribute(SF::seqI(1,7),DAT_7_PACK);

        SA::add(v,DAT_7_PACK,SF::seqI(17,27),a,b,c,d,e,f,g);

        sum = 0;
        SM::forEach(sf,v);

        REQUIRE( sum == 28 + 242 + 28 );
    }

}


#endif /* end of include guard: CHAINUSAGETESTS_C94JK7M5 */
