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
bool throwVirtualTestReadOnly(T& t) {
    IFN_SECTOR_START( "throw virtual test" );
    auto vc = SF::vcollectionCustom(t);

    { // TRAVERSAL
        const char* THROW_STRING = "Traversal is disabled in current collection.";

        {
            bool caught = false;
            try {
                SA::begin(vc);
            } catch (const tt::t::util::FeatureDisabled& e) {
                caught = true;
                caught &= std::string(e.what()) == THROW_STRING;
            }
            IFN_RET_FALSE( caught );
        }

        {
            bool caught = false;
            try {
                SA::end(vc);
            } catch (const tt::t::util::FeatureDisabled& e) {
                caught = true;
                caught &= std::string(e.what()) == THROW_STRING;
            }
            IFN_RET_FALSE( caught );
        }

        {
            bool caught = false;
            try {
                SA::iterAt(vc,7);
            } catch (const tt::t::util::FeatureDisabled& e) {
                caught = true;
                caught &= std::string(e.what()) == THROW_STRING;
            }
            IFN_RET_FALSE( caught );
        }

        {
            bool caught = false;
            try {
                SA::cbegin(vc);
            } catch (const tt::t::util::FeatureDisabled& e) {
                caught = true;
                caught &= std::string(e.what()) == THROW_STRING;
            }
            IFN_RET_FALSE( caught );
        }

        {
            bool caught = false;
            try {
                SA::cend(vc);
            } catch (const tt::t::util::FeatureDisabled& e) {
                caught = true;
                caught &= std::string(e.what()) == THROW_STRING;
            }
            IFN_RET_FALSE( caught );
        }

        {
            bool caught = false;
            try {
                SA::citerAt(vc,7);
            } catch (const tt::t::util::FeatureDisabled& e) {
                caught = true;
                caught &= std::string(e.what()) == THROW_STRING;
            }
            IFN_RET_FALSE( caught );
        }
    }

    { // ACCESS
        const char* THROW_STRING = "Access operations are disabled in current collection.";

        {
            bool caught = false;
            try {
                SA::getByIndex(vc,7);
            } catch (const tt::t::util::FeatureDisabled& e) {
                caught = true;
                caught &= std::string(e.what()) == THROW_STRING;
            }
            IFN_RET_FALSE( caught );
        }

        {
            bool caught = false;
            try {
                vc.first();
            } catch (const tt::t::util::FeatureDisabled& e) {
                caught = true;
                caught &= std::string(e.what()) == THROW_STRING;
            }
            IFN_RET_FALSE( caught );
        }

        {
            bool caught = false;
            try {
                vc.last();
            } catch (const tt::t::util::FeatureDisabled& e) {
                caught = true;
                caught &= std::string(e.what()) == THROW_STRING;
            }
            IFN_RET_FALSE( caught );
        }

        {
            bool caught = false;
            try {
                vc.cgetByIndex(7);
            } catch (const tt::t::util::FeatureDisabled& e) {
                caught = true;
                caught &= std::string(e.what()) == THROW_STRING;
            }
            IFN_RET_FALSE( caught );
        }

        {
            bool caught = false;
            try {
                vc.cfirst();
            } catch (const tt::t::util::FeatureDisabled& e) {
                caught = true;
                caught &= std::string(e.what()) == THROW_STRING;
            }
            IFN_RET_FALSE( caught );
        }

        {
            bool caught = false;
            try {
                vc.clast();
            } catch (const tt::t::util::FeatureDisabled& e) {
                caught = true;
                caught &= std::string(e.what()) == THROW_STRING;
            }
            IFN_RET_FALSE( caught );
        }
    }

    { // SIZABLE
        const char* THROW_STRING = "Size info is disabled in current collection.";

        {
            bool caught = false;
            try {
                SA::size(vc);
            } catch (const tt::t::util::FeatureDisabled& e) {
                caught = true;
                caught &= std::string(e.what()) == THROW_STRING;
            }
            IFN_RET_FALSE( caught );
        }
    }

    return true;
}

template <class T>
bool throwVirtualTest(T& t) {
    IFN_SECTOR_START( "throw virtual test" );
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

        { // GET ITER
            auto vcc = SF::vcollectionCustom<
                t::AP_THROW,
                t::CP_THROW,
                t::TP_ENABLED
            >(t);

            {
                bool caught = false;
                try {
                    SA::insert(vcc,SA::begin(vcc),7);
                } catch (const tt::t::util::FeatureDisabled& e) {
                    caught = true;
                    caught &= std::string(e.what()) == THROW_STRING;
                }
                IFN_RET_FALSE( caught );
            }
        }
    }

    { // CLEARANCE
        const char* THROW_STRING = "Erasing is disabled in current collection.";
        {
            bool caught = false;
            try {
                SA::clear(vc);
            } catch (const tt::t::util::FeatureDisabled& e) {
                caught = true;
                caught &= std::string(e.what()) == THROW_STRING;
            }
            IFN_RET_FALSE( caught );
        }

        { // GET ITER
            auto vcc = SF::vcollectionCustom<
                t::AP_THROW,
                t::CP_THROW,
                t::TP_ENABLED
            >(t);

            {
                bool caught = false;
                try {
                    SA::erase(vcc,SA::begin(vcc));
                } catch (const tt::t::util::FeatureDisabled& e) {
                    caught = true;
                    caught &= std::string(e.what()) == THROW_STRING;
                }
                IFN_RET_FALSE( caught );
            }

            {
                bool caught = false;
                try {
                    SA::erase(vcc,SA::begin(vcc),SA::end(vcc));
                } catch (const tt::t::util::FeatureDisabled& e) {
                    caught = true;
                    caught &= std::string(e.what()) == THROW_STRING;
                }
                IFN_RET_FALSE( caught );
            }
        }
    }

    return throwVirtualTestReadOnly(t);
}

template <class T>
bool fakeVirtualTest(T& t) {
    IFN_SECTOR_START( "fake virtual test" );
    SA::clear(t);

    auto vc = SF::vcollectionCustom<
        t::AP_FAKE,
        t::CP_FAKE,
        t::TP_FAKE,
        t::ACP_ENABLED,
        t::SP_FAKE
    >(t);

    { // ADDABLE
        SA::add(vc,7);
        SA::insert(vc,SA::begin(vc),7);

        IFN_RET_FALSE( SA::size(t) == 0 );
    }

    { // CLEARABLE
        SA::add(t,7);

        SA::clear(vc);
        IFN_RET_FALSE( SA::size(t) == 1 );

        SA::erase(vc,SA::begin(vc));
        IFN_RET_FALSE( SA::size(t) == 1 );

        SA::erase(vc,SA::begin(vc),SA::end(vc));
        IFN_RET_FALSE( SA::size(t) == 1 );
    }

    { // TRAVERSABLE
        int cnt = 0;
        TEMPLATIOUS_FOREACH(auto i,vc) {
            ++cnt;
        }

        IFN_RET_FALSE( cnt == 0 );
        IFN_RET_FALSE( SA::size(t) == 1 );
        IFN_RET_FALSE( SA::getByIndex(t,0) == 7 );

        for (auto i = SA::cbegin(vc); i != SA::cend(vc); ++i) {
            ++cnt;
        }

        IFN_RET_FALSE( cnt == 0 );
        IFN_RET_FALSE( SA::size(t) == 1 );
        IFN_RET_FALSE( SA::getByIndex(t,0) == 7 );
    }

    { // SIZABLE
        IFN_RET_FALSE( SA::size(vc) == 0 );
        IFN_RET_FALSE( SA::size(t) == 1 );
        IFN_RET_FALSE( SA::getByIndex(t,0) == 7 );
    }

    return true;
}

template <class T>
bool virtualTest(T&& t) {
    auto vc = SF::vcollection(t);

    bool isGood = true;
    isGood &= existantCollectionTest(vc);
    isGood &= throwVirtualTest(t);
    isGood &= throwVirtualTestReadOnly(t);
    isGood &= fakeVirtualTest(t);

    return isGood;
}

}


#endif /* end of include guard: VIRTUALCOLLECTION_8NIGJKYU */
