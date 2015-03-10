/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Main unit test ground
 *
 *        Version:  1.0
 *        Created:  07/27/2014 05:38:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "tests.h"

int main( int argc, char* const argv[] )
{
    test_templatious::IfnCaseCounter c;

    int result = Catch::Session().run( argc, argv );

    return result;
}

