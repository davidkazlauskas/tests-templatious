
cmake_minimum_required( VERSION 2.6 )

project ( TTEST CXX )

include_directories ( $ENV{TEMPLATIOUS_DIR} )

ADD_DEFINITIONS (
    -std=c++11
    #-lstdc++
    -Wall
    -x c++
    #-ggdb3
    #-O0
    #-pthread
    #-fcolor-diagnostics
    #-Wno-unused-local-typedefs # keep g++ happy
)

#SET ( CMAKE_CXX_COMPILER "g++-4.9" )
SET ( CMAKE_CXX_COMPILER "clang++-3.7" )
#SET ( CMAKE_CXX_COMPILER "g++" )

SET ( generic_tests
    TypeLists.cpp PackTests.cpp
    ChainFunctor.cpp DistributorTests.cpp
    OnceTraversable.cpp CollectionRepeater.cpp
    Sequence.cpp StaticManipulatorTests.cpp
    StaticFactoryTests.cpp IterDumpTests.cpp
    SelectTests.cpp ChainUsageTests.cpp
    VirtualCollectionGeneric.cpp
    VirtualPackTests.cpp
)

SET ( specific_tests
    spec/StdVector.cpp
    spec/StdList.cpp
    spec/StdForwardList.cpp
    spec/StdDeque.cpp
    spec/StaticVector.cpp
)

add_executable ( a.out
    main.cpp
    ${generic_tests} ${specific_tests}
)

target_link_libraries( a.out pthread )

SET ( CMAKE_EXPORT_COMPILE_COMMANDS ON )
