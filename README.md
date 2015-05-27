# tests-templatious
Test suite for templatious library
[![Build Status](https://travis-ci.org/davidkazlauskas/tests-templatious.svg?branch=master)](https://travis-ci.org/davidkazlauskas/tests-templatious)
[![Coverage Status](https://coveralls.io/repos/davidkazlauskas/tests-templatious/badge.svg?branch=master)](https://coveralls.io/r/davidkazlauskas/tests-templatious?branch=master)

99.9% templatious library line coverage as measured with gcov.

To generate coverage report set enviroment variable TEMPLATIOUS_GCC_COVERAGE to "yes",
run cmake (using g++), run the program (a.out) at least once and execute ./gencov in build directory.

Example (with g++-4.8, linux bash):
~~~~~~~
export TEMPLATIOUS_GCC_COVERAGE=yes
mkdir build
cd build
# During cmake step you should see
# "Compiling with code coverage for g++" message
cmake -DCMAKE_CXX_COMPILER=g++-4.8 ..
make
./a.out
./gencov

# view results
firefox htmlrep/index.html
~~~~~~~

