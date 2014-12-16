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

#include <exception>

#include "../TestDefs.h"

namespace test_templatious {

struct MoveException: public std::exception {

    virtual const char* what() const noexcept override {
        return "Trying to move from empty class.";
    }

    virtual ~MoveException() {}
};

template <class UniquenessToken>
struct ConstructorCountCollection {

    ConstructorCountCollection() : _moved(false) { ++_count; }

    ConstructorCountCollection(const ConstructorCountCollection& other): _moved(other._moved) {
        if (!_moved) {
            ++_count;
        }
    }

    ConstructorCountCollection(ConstructorCountCollection&& other) : _moved(other._moved) {
        if (!other._moved) {
            other._moved = true;
        }
    }

    ConstructorCountCollection& operator=(ConstructorCountCollection&& other) {
        if (!_moved) {
            --_count;
        }

        _moved = other._moved;
        other._moved = true;

        return *this;
    }

    ConstructorCountCollection& operator=(const ConstructorCountCollection& other) {
        if (!_moved) {
            --_count;
        }

        _moved = other._moved;
        if (!_moved) {
            ++_count;
        }

        return *this;
    }

    ~ConstructorCountCollection() {
        if (!_moved) {
            --_count;
        }
    }

    static int count() { return _count; }
private:
    static int _count;
    bool _moved;
};

template <class UniquenessToken>
int ConstructorCountCollection<UniquenessToken>::_count = 0;

}

#endif /* end of include guard: CONSTRUCTORCOUNTCOLLECTION_APM2CV9I */

