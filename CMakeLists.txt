
cmake_minimum_required( VERSION 2.6 )

project ( TTEST CXX )

include_directories ( $ENV{TEMPLATIOUS_DIR} )

ADD_DEFINITIONS (
    -std=c++11
    -Wall
    -x c++
    -g
)

SET( CURR_SANITIZE "$ENV{LLVM_SANITIZER_FLAGS}" )
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CURR_SANITIZE}" )
SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CURR_SANITIZE}" )

if ( "$ENV{TEMPLATIOUS_GCC_COVERAGE}" STREQUAL "yes" )
    MESSAGE( STATUS "Compiling with code coverage for g++" )
    ADD_DEFINITIONS(
        --coverage
    )
    file( COPY "gencov" DESTINATION "${CMAKE_BINARY_DIR}" )
endif()

SET ( generic_tests
    TypeLists.cpp PackTests.cpp
    ChainFunctor.cpp DistributorTests.cpp
    OnceTraversable.cpp CollectionRepeater.cpp
    Sequence.cpp StaticManipulatorTests.cpp
    StaticFactoryTests.cpp IterDumpTests.cpp
    SelectTests.cpp ChainUsageTests.cpp
    VirtualCollectionGeneric.cpp
    VirtualPackTests.cpp ProxyMiscTests.cpp
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

target_link_libraries( a.out pthread gcov )

SET ( CMAKE_EXPORT_COMPILE_COMMANDS ON )
