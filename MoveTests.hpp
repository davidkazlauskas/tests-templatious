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
    auto& r = SA::getByIndex(t,0);
    IFN_RET_FALSE( std::addressof(r) != std::addressof(m) );
    IFN_RET_FALSE( r._i == 7 );

    SA::add(t,std::move(m));
    IFN_RET_FALSE( m._i == -7 );

    MovablePod m2(77);
    SA::insert(t,SA::begin(t),m2);
    IFN_RET_FALSE( m2._i == 77 );

    SA::insert(t,SA::begin(t),std::move(m2));
    IFN_RET_FALSE( m2._i == -7 );

    IFN_RET_FALSE( SA::getByIndex(t,0)._i == 77 );
    IFN_RET_FALSE( SA::getByIndex(t,1)._i == 77 );
    IFN_RET_FALSE( SA::getByIndex(t,2)._i == 7 );
    IFN_RET_FALSE( SA::getByIndex(t,3)._i == 7 );

    return true;
}


#endif /* end of include guard: MOVETESTS_UD8CT3TJ */
