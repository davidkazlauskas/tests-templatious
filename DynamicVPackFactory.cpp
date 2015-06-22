
#include "TestDefs.hpp"

#include <templatious/detail/DynamicPackCreator.hpp>

struct CTorThrow {
    CTorThrow() {
        throw 7;
    }
};

struct BoxedInt {
    BoxedInt() : _ptr(new int(7)) {}
    ~BoxedInt() {
        delete _ptr;
    }

    int* _ptr;
};

typedef tt::t::TypeNodeFactory TNF;
static auto intNode = TNF::makePodNode<int>(
    [](void* ptr,const char* param) {
        *reinterpret_cast<int*>(ptr) = std::atoi(param);
    },
    [](const void* ptr,std::string& fmt) {
        const int* rein = reinterpret_cast<const int*>(ptr);
        fmt = std::to_string(*rein);
    });

static auto cIntNode = TNF::makePodNode<const int>(
    [](void* ptr,const char* param) {
        *reinterpret_cast<int*>(ptr) = std::atoi(param);
    },
    [](const void* ptr,std::string& fmt) {
        const int* rein = reinterpret_cast<const int*>(ptr);
        fmt = std::to_string(*rein);
    });

static auto charNode = TNF::makePodNode<char>(
    [](void* ptr,const char* param) {
        *reinterpret_cast<char*>(ptr) = param[0];
    },
    [](const void* ptr,std::string& fmt) {
        const char* rein = reinterpret_cast<const char*>(ptr);
        fmt = std::to_string(rein[0]);
    });

static auto ctorThowNode = TNF::makeFullNode<CTorThrow>(
    [](void* ptr,const char*) {
        new (ptr) CTorThrow();
    },
    [](void*) {},
    [](const void* ptr,std::string& fmt) {
        fmt = "<CTorThrow>";
    });

static auto innerAllocNode = TNF::makeFullNode<BoxedInt>(
    [](void* ptr,const char*) {
        new (ptr) BoxedInt();
    },
    [](void* ptr) {
        BoxedInt* rein = reinterpret_cast<BoxedInt*>(ptr);
        rein->~BoxedInt();
    },
    [](const void* ptr,std::string& fmt) {
        fmt = "<BoxedInt>";
    });

struct DummyA {};
struct DummyB {};

static auto dummyADefaultNode = TNF::makeDummyNode<DummyA>();
static auto dummyBCustomNode = TNF::makeDummyNode<DummyB>("[DummyB]");

templatious::DynVPackFactory makeTrivialFactory() {
    tt::t::DynVPackFactoryBuilder bld;
    bld.attachNode("int",intNode);
    bld.attachNode("const int",cIntNode);
    bld.attachNode("char",charNode);
    bld.attachNode("DummyA",dummyADefaultNode);
    bld.attachNode("DummyB",dummyBCustomNode);
    return bld.getFactory();
}

static auto trivialFactory = makeTrivialFactory();

static auto virtualPackNode = TNF::makeFullNode< std::shared_ptr<templatious::VirtualPack> >(
    [](void* ptr,const char*) {
        const char* types[] = {"int","char"};
        const char* values[] = {"7","7"};
        auto premade = trivialFactory.makePack(2,types,values);
        new (ptr) std::shared_ptr<templatious::VirtualPack>(premade);
    },
    [](void* ptr) {
        typedef std::shared_ptr<templatious::VirtualPack>* SPtr;
        SPtr rein = reinterpret_cast<SPtr>(ptr);
        rein->~shared_ptr();
    },
    [](const void* ptr,std::string& fmt) {
        fmt = "<shared composite>";
    });

TEST_CASE("type_node_pod_basic","[dynamic_vpack_tests]")
{

    REQUIRE( intNode->type() == std::type_index(typeid(int)) );
    REQUIRE( intNode->size() == sizeof(int) );
    REQUIRE( intNode->align() == alignof(int) );

    REQUIRE( innerAllocNode->type() == std::type_index(typeid(BoxedInt)) );
    REQUIRE( innerAllocNode->size() == sizeof(BoxedInt) );
    REQUIRE( innerAllocNode->align() == alignof(BoxedInt) );

    long outSize,outAlign;
    intNode->sizeAndAlign(outSize,outAlign);

    REQUIRE( outSize == sizeof(int) );
    REQUIRE( outAlign == alignof(int) );

    char buf[64];
    SM::set(7,buf);

    intNode->construct(buf,"3244");
    int* reint = reinterpret_cast<int*>(buf);
    REQUIRE( *reint == 3244 );

    auto r = SF::range(buf,sizeof(*reint));
    bool res = SM::forAll([](char c) { return c == 7; },r);
    REQUIRE( res );

    std::string out;
    intNode->toString(buf,out);

    REQUIRE( out == "3244" );
}

TEST_CASE("dyn_vpack_construct_single","[dynamic_vpack_tests]")
{
    const tt::t::TypeNode* nodes[1];
    nodes[0] = intNode;
    const char* init[] = { "128" };

    char bufMem[1024]; // should be enough
    SM::set(7,bufMem);
    auto created = new (bufMem) tt::t::detail::DynamicVirtualPack<0,void>(1,nodes,init,0);
    char* valStart = created->getCore().valueStart();
    int* reint = reinterpret_cast<int*>(valStart);
    REQUIRE( *reint == 128 );
}

TEST_CASE("dyn_vpack_construct_double","[dynamic_vpack_tests]")
{
    const tt::t::TypeNode* nodes[2];
    nodes[0] = intNode;
    nodes[1] = intNode;
    const char* init[] = { "128", "127" };

    char bufMem[1024]; // should be enough
    SM::set(7,bufMem);
    auto created = new (bufMem) tt::t::detail::DynamicVirtualPack<0,void>(2,nodes,init,0);
    char* valStart = created->getCore().valueStart();
    int* reint = reinterpret_cast<int*>(valStart);
    REQUIRE( reint[0] == 128 );
    REQUIRE( reint[1] == 127 );
}

TEST_CASE("dyn_vpack_construct_double_misaligned","[dynamic_vpack_tests]")
{
    const tt::t::TypeNode* nodes[2];
    nodes[0] = charNode;
    nodes[1] = intNode;
    const char* init[] = { "7", "128" };

    char bufMem[1024]; // should be enough
    SM::set(7,bufMem);
    auto created = new (bufMem) tt::t::detail::DynamicVirtualPack<0,void>(2,nodes,init,0);
    char* valStart = created->getCore().valueStart();
    int* reint = reinterpret_cast<int*>(valStart);
    REQUIRE( valStart[0] == '7' );

    char* iter = valStart + 1;
    bool allPass = true;
    while (iter != reinterpret_cast<char*>(reint + 1)) {
        allPass &= *iter == 7;
        ++iter;
    }

    REQUIRE( reint[1] == 128 );
}

TEST_CASE("dyn_vpack_as_normal_pack","[dynamic_vpack_tests]")
{
    const tt::t::TypeNode* nodes[2];
    nodes[0] = charNode;
    nodes[1] = intNode;
    const char* init[] = { "7", "128" };

    char bufMem[1024]; // should be enough
    SM::set(7,bufMem);
    auto created = new (bufMem) tt::t::detail::DynamicVirtualPack<0,void>(2,nodes,init,0);


    int sumChar = 0;
    int sumInt = 0;
    auto mf = SF::virtualMatchFunctor(
        SF::virtualMatch<char,int>(
            [&](char& c,int& i) {
                sumChar += c;
                sumInt += i;
                --c;
                --i;
            }
        )
    );

    bool succ = mf.tryMatch(*created);
    REQUIRE( succ );
    REQUIRE( sumChar == '7' );
    REQUIRE( sumInt == 128 );
    REQUIRE( created->useCount() == -1 );
    REQUIRE( created->size() == 2 );

    succ = mf.tryMatch(*created);
    REQUIRE( succ );
    REQUIRE( sumChar == '7' + '6' );
    REQUIRE( sumInt == 128 + 127 );
}

TEST_CASE("dyn_vpack_const_node","[dynamic_vpack_tests]")
{
    const tt::t::TypeNode* nodes[2];
    nodes[0] = charNode;
    nodes[1] = cIntNode;
    const char* init[] = { "7", "128" };

    char bufMem[1024]; // should be enough
    SM::set(7,bufMem);
    auto created = new (bufMem) tt::t::detail::DynamicVirtualPack<0,void>(2,nodes,init,0);

    int sumChar = 0;
    int sumInt = 0;
    auto mf = SF::virtualMatchFunctor(
        SF::virtualMatch<char,int>(
            [&](char c,int i) {
                sumChar += c;
                sumInt += i;
            }
        )
    );

    bool succ = mf.tryMatch(*created);
    REQUIRE( !succ );
    REQUIRE( sumChar == 0 );
    REQUIRE( sumInt == 0 );

    auto mfc = SF::virtualMatchFunctor(
        SF::virtualMatch<char,const int>(
            [&](char c,const int& i) {
                sumChar += c;
                sumInt += i;
            }
        )
    );

    succ = mfc.tryMatch(*created);
    REQUIRE( succ );
    REQUIRE( sumChar == '7' );
    REQUIRE( sumInt == 128 );
    REQUIRE( !created->constAt(0) );
    REQUIRE(  created->constAt(1) );
    REQUIRE( created->typeAt(0) == std::type_index(typeid(char)) );
    REQUIRE( created->typeAt(1) == std::type_index(typeid(const int)) );

}

TEST_CASE("dyn_vpack_range_check","[dynamic_vpack_tests]")
{
    const tt::t::TypeNode* nodes[2];
    nodes[0] = charNode;
    nodes[1] = cIntNode;
    const char* init[] = { "7", "128" };

    char bufMem[1024]; // should be enough
    SM::set(7,bufMem);
    auto created = new (bufMem) tt::t::detail::DynamicVirtualPack<0,void>(2,nodes,init,0);

    {
        bool caught = false;
        try {
            created->typeAt(2);
        } catch (const std::exception&) {
            caught = true;
        }
        REQUIRE( caught );
    }

    {
        bool caught = false;
        try {
            created->typeAt(-1);
        } catch (const std::exception&) {
            caught = true;
        }
        REQUIRE( caught );
    }

    {
        bool caught = false;
        try {
            created->constAt(2);
        } catch (const std::exception&) {
            caught = true;
        }
        REQUIRE( caught );
    }

    {
        bool caught = false;
        try {
            created->constAt(-1);
        } catch (const std::exception&) {
            caught = true;
        }
        REQUIRE( caught );
    }
}

TEST_CASE("dyn_vpack_real_factory","[dynamic_vpack_tests]")
{
    tt::t::DynVPackFactoryBuilder bld;
    bld.attachNode("int",intNode);
    bld.attachNode("char",charNode);
    auto fact = bld.getFactory();

    const char* types[] = {"int","char"};
    const char* values[] = {"7","7"};

    {
        int sumChar = 0;
        int sumInt = 0;

        auto p = fact.makePack(2,types,values);
        auto mf = SF::virtualMatchFunctor(
            SF::virtualMatch<int,char>(
                [&](int i,char c) {
                    sumInt += i;
                    sumChar += c;
                }
            )
        );

        bool success = mf.tryMatch(*p);

        REQUIRE( success );
        REQUIRE( sumChar == '7' );
        REQUIRE( sumInt == 7 );

        std::string buf;
        intNode->toString(p->getCore().get<int>(0),buf);
        REQUIRE( buf == "7" );
        charNode->toString(p->getCore().get<char>(1),buf);
        REQUIRE( buf == "55" );
    }
}

TEST_CASE("dyn_vpack_throw_construct","[dynamic_vpack_tests]")
{
    tt::t::DynVPackFactoryBuilder bld;
    bld.attachNode("throw",ctorThowNode);
    bld.attachNode("boxed_int",innerAllocNode);
    auto fact = bld.getFactory();

    const char* types[] = {"boxed_int","throw"};
    const char* values[] = {"7","7"};

    {
        bool caught = false;
        try {
            // this segment should not leak
            auto p = fact.makePack(2,types,values);
        } catch (int i) {
            caught = true;
        }
        REQUIRE( caught );
    }
}

TEST_CASE("dyn_vpack_composition","[dynamic_vpack_tests]")
{
    tt::t::DynVPackFactoryBuilder bld;
    bld.attachNode("comp",virtualPackNode);
    auto fact = bld.getFactory();

    const char* types[] = {"comp"};
    const char* values[] = {"7"};
    auto p = fact.makePack(1,types,values);

    int iOut = 0;
    char cOut = 0;
    bool successA = false;
    bool successB = false;

    typedef std::shared_ptr<templatious::VirtualPack> SPtr;
    auto mfInner = SF::virtualMatchFunctor(
        SF::virtualMatch<int,char>(
            [&](int i,char c) {
                iOut = i;
                cOut = c;
            }
        )
    );

    auto mf = SF::virtualMatchFunctor(
        SF::virtualMatch< SPtr >(
            [&](SPtr& sptr) {
                successB = mfInner.tryMatch(*sptr);
            }
        )
    );

    successA = mf.tryMatch(*p);
    REQUIRE( successA );
    REQUIRE( successB );
    REQUIRE( iOut == 7 );
    REQUIRE( cOut == '7' );

    std::string name;
    virtualPackNode->toString(
        p->getCore().get<
            std::shared_ptr<templatious::VirtualPack>
        >(0),name);

    REQUIRE( name == "<shared composite>" );
}

TEST_CASE("dyn_vpack_wcallback","[dynamic_vpack_tests]")
{
    const char* types[] = {"int","char"};
    const char* values[] = {"7","7"};

    int outInt = -1;
    char outChar = -1;
    auto p = trivialFactory.makePackWCallback(2,types,values,
        [&](const templatious::detail::DynamicVirtualPackCore& c) {
            outInt = *c.get<int>(0);
            outChar = *c.get<char>(1);

            REQUIRE( nullptr == c.get<short>(0) );
            REQUIRE( nullptr == c.get<short>(-1) );
            REQUIRE( nullptr == c.get<char>(2) );
            REQUIRE( nullptr == c.getNthPointer(-1) );
            REQUIRE( nullptr == c.getNthPointer(2) );
        });

    auto mf = SF::virtualMatchFunctor(
        SF::virtualMatch<int,char>([](int,char) {})
    );

    bool success = mf.tryMatch(*p);
    REQUIRE( success );
    REQUIRE( outInt == 7 );
    REQUIRE( outChar == '7' );
    REQUIRE( p->useCount() == 1 );
}

TEST_CASE("dyn_vpack_custom_synced_count","[dynamic_vpack_tests]")
{
    const char* types[] = {"int","char"};
    const char* values[] = {"7","7"};

    auto p = trivialFactory.makePackCustom<
        templatious::VPACK_WAIT |
        templatious::VPACK_SYNCED |
        templatious::VPACK_COUNT
    >(2,types,values);

    auto mf = SF::virtualMatchFunctor(
        SF::virtualMatch<int,char>([](int,char) {})
    );

    const int ROUNDS = 1000000;

    auto runner =
        [&,p]() {
            TEMPLATIOUS_REPEAT(ROUNDS) {
                mf.tryMatch(*p);
            }
        };

    auto f1 = std::async(std::launch::async,runner);
    auto f2 = std::async(std::launch::async,runner);

    f1.wait();
    f2.wait();

    REQUIRE( p->useCount() == 2*ROUNDS );
}

TEST_CASE("dyn_vpack_custom_no_count_wait","[dynamic_vpack_tests]")
{
    const char* types[] = {"int","char"};
    const char* values[] = {"7","7"};

    auto p = trivialFactory.makePackCustom<
        templatious::VPACK_SYNCED |
        templatious::VPACK_WAIT
    >(2,types,values);

    int outVar = -1;
    auto mf = SF::virtualMatchFunctor(
        SF::virtualMatch<int,char>([&](int,char) {
            outVar = 7;
        })
    );

    auto f1 = std::async(std::launch::async,
        [&]() {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(100)
            );
            mf.tryMatch(*p);
        });

    p->wait();
    REQUIRE( outVar == 7 );
    REQUIRE( p->useCount() == -1 );
}

TEST_CASE("dyn_vpack_custom_wcallback","[dynamic_vpack_tests]")
{
    const char* types[] = {"int","char"};
    const char* values[] = {"7","7"};

    volatile int victim = 0;

    auto p = trivialFactory.makePackCustomWCallback<
        templatious::VPACK_SYNCED
    >(2,types,values,
        [&](const templatious::detail::DynamicVirtualPackCore& c) {
            victim += *c.get<int>(0);
            victim += *c.get<char>(1);
        });

    auto mf = SF::virtualMatchFunctor(
        SF::virtualMatch<int,char>([](int,char) {})
    );

    const int ROUNDS = 1000000;
    auto runner =
        [&]() {
            TEMPLATIOUS_REPEAT(ROUNDS) {
                mf.tryMatch(*p);
            }
        };

    auto f1 = std::async(std::launch::async,runner);
    auto f2 = std::async(std::launch::async,runner);

    f1.wait();
    f2.wait();

    REQUIRE( victim == 2*ROUNDS*(7 + '7'));
}

TEST_CASE("dyn_vpack_factory_exceptions","[dynamic_vpack_tests]")
{
    tt::t::DynVPackFactoryBuilder bld;
    bld.attachNode("int",intNode);
    bld.attachNode("char",charNode);

    bool caught = false;
    try {
        bld.attachNode("int",charNode);
    } catch (const std::exception&) {
        caught = true;
    }
    REQUIRE( caught );

    const char* types[] = {
        "int","char","int","char","int","char","int","char",
        "int","char","int","char","int","char","int","char",
        "int","char","int","char","int","char","int","char",
        "int","char","int","char","int","char","int","char",
        "int","char","int","char","int","char","int","char"
    };
    const char* values[] = {
        "7","7","7","7","7","7","7","7",
        "7","7","7","7","7","7","7","7",
        "7","7","7","7","7","7","7","7",
        "7","7","7","7","7","7","7","7",
        "7","7","7","7","7","7","7","7"
    };

    auto fact = bld.getFactory();
    fact.makePack(32,types,values);

    {
        caught = false;
        try {
            bld.getFactory();
        } catch (const std::exception& e) {
            caught = true;
        }
        REQUIRE( caught );
    }

    {
        caught = false;
        try {
            fact.makePack(33,types,values);
        } catch (const std::exception& e) {
            caught = true;
        }
        REQUIRE( caught );
    }

    {
        caught = false;
        try {
            fact.makePack(-1,types,values);
        } catch (const std::exception& e) {
            caught = true;
        }
        REQUIRE( caught );
    }

    {
        caught = false;
        try {
            fact.makePack(0,types,values);
        } catch (const std::exception& e) {
            caught = true;
        }
        REQUIRE( caught );
    }

    const char* wrongTypes[] = {"baller_type"};
    {
        caught = false;
        try {
            fact.makePack(1,wrongTypes,values);
        } catch (const std::exception& e) {
            caught = true;
        }
        REQUIRE( caught );
    }
}

TEST_CASE("dyn_vpack_factory_dummy_types","[dynamic_vpack_tests]")
{
    const char* types[] = {"DummyA","DummyB"};
    const char* values[] = {"",""};

    auto p = trivialFactory.makePack(2,types,values);
    std::string buf[32];
    int size = p->formatAll(32,buf);
    REQUIRE( size == 2 );
    REQUIRE( buf[0] == typeid(DummyA).name() );
    REQUIRE( buf[1] == "[DummyB]" );
    REQUIRE( buf[2] == "" );

    SM::set("unchanged",buf);

    bool caught = false;
    try {
        p->formatAll(1,buf);
    } catch (const std::exception& e) {
        caught = true;
    }
    REQUIRE( caught );

    bool allUnchanged =
        SM::forAll([](const std::string& str) {
            return str == "unchanged";
        },buf);

    REQUIRE( allUnchanged );
}

TEST_CASE("dyn_vpack_factory_serialize","[dynamic_vpack_tests]")
{
    std::string arr[32];
    SM::set("unset",arr);
    auto p = SF::vpack<int,char>(7,'7');
    int outSize = trivialFactory.serializePack(p,32,arr);
    REQUIRE( outSize == 2 );
    REQUIRE( arr[0] == "7" );
    REQUIRE( arr[1] == "55" );

    auto rng = SF::range(arr,2);
    bool allUnset = SM::forAll(
        [](const std::string& str) { return str == "unset"; },
        rng);
    REQUIRE( allUnset );
}

TEST_CASE("dyn_vpack_factory_throw_unknown_serialize","[dynamic_vpack_tests]")
{
    struct MyHipsterStruct {};

    std::string arr[32];
    SM::set("unset",arr);
    auto p = SF::vpack<int,MyHipsterStruct>(7,MyHipsterStruct());

    bool caught = false;
    try {
        trivialFactory.serializePack(p,32,arr);
    } catch (const std::exception&) {
        caught = true;
    }
    REQUIRE( caught );
}

TEST_CASE("dyn_vpack_factory_no_space_throw_serialize","[dynamic_vpack_tests]")
{
    std::string arr[32];
    SM::set("unset",arr);
    auto p = SF::vpack<int,int>(1,2);

    bool caught = false;
    try {
        trivialFactory.serializePack(p,1,arr);
    } catch (const std::exception&) {
        caught = true;
    }
    REQUIRE( caught );
}

TEST_CASE("dyn_vpack_vect_ser","[dynamic_vpack_tests]")
{
    auto p = SF::vpack<int,int>(1,2);
    auto out = trivialFactory.serializePack(p);
    REQUIRE( SA::size(out) == 2 );
    REQUIRE( out[0] == "1" );
    REQUIRE( out[1] == "2" );
}

TEST_CASE("dyn_vpack_core_ser_gen","[dynamic_vpack_tests]")
{
    const char* types[] = {"int","char"};
    const char* values[] = {"7","7"};

    std::string out[2];
    templatious::TNodePtr outArr[32];
    SM::set("-1",out);
    SM::set(nullptr,outArr);

    auto mf = SF::virtualMatchFunctor(
        SF::virtualMatch<int,char>([](int,char) {})
    );

    auto p1 = trivialFactory.makePackCustomWCallback<
        0
    >(2,types,values,
        [&](const templatious::detail::DynamicVirtualPackCore& c) {
            trivialFactory.serializeDynamicCore(c,2,out,outArr);
        }
    );

    mf.tryMatch(*p1);

    REQUIRE( out[0] == "7" );
    REQUIRE( out[1] == "55" );
    REQUIRE( outArr[0] == intNode );
    REQUIRE( outArr[1] == charNode );
    REQUIRE( outArr[2] == nullptr );

    SM::set("-1",out);
    auto p2 = trivialFactory.makePackCustomWCallback<
        0
    >(2,types,values,
        [&](const templatious::detail::DynamicVirtualPackCore& c) {
            auto outVec = trivialFactory.serializeDynamicCore(c);
            REQUIRE( SA::size(outVec) == 2 );

            out[0] = outVec[0];
            out[1] = outVec[1];
        }
    );

    mf.tryMatch(*p2);

    REQUIRE( out[0] == "7" );
    REQUIRE( out[1] == "55" );
}
