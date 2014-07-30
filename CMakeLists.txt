
cmake_minimum_required( VERSION 2.6 )

project ( TTEST CXX )

include_directories ( "/home/deividas/Desktop/Dropbox/MyPrj/Templatious" )


ADD_DEFINITIONS (
    -std=c++11
    -lstdc++
)

SET ( CMAKE_CXX_COMPILER "g++-4.8" )

find_package(Boost)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(progname ${Boost_LIBRARIES})
    add_executable ( a.out main.cpp )
else()
    message( SEND_ERROR "Boost 1.48.0 not found." )
endif()

SET ( CMAKE_EXPORT_COMPILE_COMMANDS ON )
