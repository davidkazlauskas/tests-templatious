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

#include "TestDefs.hpp"
#include "CollectionAdapter.hpp"

namespace test_templatious {

template <class T>
bool fakeVirtualTest(T& t) {
    IFN_SECTOR_START( "fake virtual test" );
    auto vc = SF::vcollectionCustom(t);

    // throw by default
    { // ADDITION
        const char* THROW_STRING = "Adding is disabled in current collection.";
        {
            bool caught = false;
            try {
                SA::add(vc,7);
            } catch (const tt::t::util::FeatureDisabled& e) {
                caught = true;
                caught &= std::string(e.what()) == THROW_STRING;
            }
            IFN_RET_FALSE( caught );
        }

        {
            bool caught = false;
            try {
                SA::canAdd(vc);
            } catch (const tt::t::util::FeatureDisabled& e) {
                caught = true;
                caught &= std::string(e.what()) == THROW_STRING;
            }
            IFN_RET_FALSE( caught );
        }

        // FIX THIS - we can't test insertion
        // because getting iterator also throws.
        //{
            //bool caught = false;
            //try {
                //SA::insert(vc,templatious::VIterator<int>(nullptr),7);
            //} catch (const tt::t::util::FeatureDisabled& e) {
                //caught = true;
                //caught &= std::string(e.what()) == THROW_STRING;
            //}
            //IFN_RET_FALSE( caught );
        //}
    }

    return true;
}

template <class T>
bool virtualTest(T&& t) {
    auto vc = SF::vcollection(t);

    bool isGood = true;
    isGood &= existantCollectionTest(vc);
    isGood &= fakeVirtualTest(vc);

    return isGood;
}

}


#endif /* end of include guard: VIRTUALCOLLECTION_8NIGJKYU */
