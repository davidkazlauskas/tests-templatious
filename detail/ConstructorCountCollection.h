/*
 * =====================================================================================
 *
 *       Filename:  ConstructorCountCollection.h
 *
 *    Description:  Collection that counts construction and destruction of objects
 *
 *        Version:  1.0
 *        Created:  12/15/2014 05:01:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Deividas Kazlauskas (dk), ne-bobu@reikalas.lt
 *        Company:  Company and shit
 *
 * =====================================================================================
 */

#ifndef CONSTRUCTORCOUNTCOLLECTION_APM2CV9I
#define CONSTRUCTORCOUNTCOLLECTION_APM2CV9I

namespace test_templatious {

template <class UniquenessToken>
struct ConstructorCountCollection {

    static int count() { return _count; }
private:
    static int _count;
};

template <class UniquenessToken>
int ConstructorCountCollection<UniquenessToken>::_count = 0;

}

#endif /* end of include guard: CONSTRUCTORCOUNTCOLLECTION_APM2CV9I */

