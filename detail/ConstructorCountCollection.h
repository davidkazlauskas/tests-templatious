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

TEMPLATIOUS_BOILERPLATE_EXCEPTION( CCountCollectionThrowUp,
    "I threw up because I was told to.");

struct MoveException: public std::exception {

    virtual const char* what() const noexcept override {
        return "Trying to move from empty class.";
    }

    virtual ~MoveException() {}
};

template <class UniquenessToken>
struct ConstructorCountCollection {

    ConstructorCountCollection() : _moved(false), _someState(0) { ++_count; }

    ConstructorCountCollection(const ConstructorCountCollection& other) :
        _moved(other._moved), _someState(other._someState)
    {
        throwIfNeeded();
        if (!_moved) {
            ++_count;
        }
    }

    ConstructorCountCollection(ConstructorCountCollection&& other) :
        _moved(other._moved), _someState(other._someState)
    {
        throwIfNeeded();
        if (!other._moved) {
            other._moved = true;
        }
    }

    ConstructorCountCollection& operator=(ConstructorCountCollection&& other) {
        throwIfNeeded();
        if (!_moved) {
            --_count;
        }

        _moved = other._moved;
        _someState = other._someState;
        other._moved = true;

        return *this;
    }

    ConstructorCountCollection& operator=(const ConstructorCountCollection& other) {
        throwIfNeeded();
        if (!_moved) {
            --_count;
        }

        _moved = other._moved;
        _someState = other._someState;
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
    static void throwUp() { _throwUp = true; }
    static void heal() { _throwUp = false; }
private:
    static void throwIfNeeded() {
        if (_throwUp) {
            throw CCountCollectionThrowUp();
        }
    }

    static int _count;
    static bool _throwUp;
    bool _moved;
    int _someState;
};

template <class UniquenessToken>
int ConstructorCountCollection<UniquenessToken>::_count = 0;

template <class UniquenessToken>
bool ConstructorCountCollection<UniquenessToken>::_throwUp = false;

}

#endif /* end of include guard: CONSTRUCTORCOUNTCOLLECTION_APM2CV9I */

