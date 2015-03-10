
cmake_minimum_required( VERSION 2.6 )

project ( TTEST CXX )

include_directories ( "/home/deividas/Desktop/Dropbox/MyPrj/Templatious" )


ADD_DEFINITIONS (
    -std=c++11
    -lstdc++
    -Wall
    -x c++
    -ggdb3
    -O0
    -DBOOST_TEST_DYN_LINK
    -fcolor-diagnostics
    -Wno-unused-local-typedefs # keep g++ happy
)

#SET ( CMAKE_CXX_COMPILER "g++-4.9" )
SET ( CMAKE_CXX_COMPILER "clang++-3.5" )
#SET ( CMAKE_CXX_COMPILER "g++" )

find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable ( a.out
        main.cpp TypeLists.cpp PackTests.cpp
        ChainFunctor.cpp DistributorTests.cpp
        OnceTraversable.cpp CollectionRepeater.cpp
        Sequence.cpp
    )
    target_link_libraries(a.out
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
else()
    message( SEND_ERROR "Boost 1.48.0 not found." )
endif()

SET ( CMAKE_EXPORT_COMPILE_COMMANDS ON )
