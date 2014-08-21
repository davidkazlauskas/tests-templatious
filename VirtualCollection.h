/*
 * =====================================================================================
 *
 *       Filename:  VirtualCollection.h
 *
 *    Description:  Virtual collection test reusing old tests
 *
 *        Version:  1.0
 *        Created:  08/21/2014 05:35:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef VIRTUALCOLLECTION_8NIGJKYU
#define VIRTUALCOLLECTION_8NIGJKYU

#include "TestDefs.h"
#include "CollectionAdapter.h"

namespace test_templatious {

template <class T>
bool virtualTest(T&& t) {
    auto vc = SF::vcollection(t);

    bool isGood = true;
    isGood &= existantCollectionTest(vc);

    return isGood;
}

}


#endif /* end of include guard: VIRTUALCOLLECTION_8NIGJKYU */
