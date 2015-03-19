/*
 * =====================================================================================
 *
 *       Filename:  MoveTests.hpp
 *
 *    Description:  Attemp to move into collection
 *
 *        Version:  1.0
 *        Created:  2015.03.19 18:37:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef MOVETESTS_UD8CT3TJ
#define MOVETESTS_UD8CT3TJ

#include "TestDefs.hpp"

struct MovablePod {
    MovablePod(int i) : _i(i) {}
    MovablePod(const MovablePod& c) : _i(c._i) {}
    MovablePod(MovablePod&& c) : _i(c._i) {
        c._i = -7;
    }

    MovablePod& operator=(const MovablePod& c) {
        _i = c._i;
        return *this;
    }

    MovablePod& operator=(MovablePod&& c) {
        _i = c._i;
        c._i = -7;
        return *this;
    }

    int _i;
};

template <class T>
bool moveTest(T& t) {
    IFN_SECTOR_START( "movable pods" );

    SA::clear(t);

    MovablePod m(7);
    SA::add(t,m);

    IFN_RET_FALSE( m._i == 7 );
}


#endif /* end of include guard: MOVETESTS_UD8CT3TJ */
