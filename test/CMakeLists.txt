# Copyright 2013, 2014, 2015 by Martin Moene
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# -Dlest_FEATURE_LITERAL_SUFFIX=0
# -Dlest_FEATURE_REGEX_SEARCH=0
# -Dlest_FEATURE_TIME=1

cmake_minimum_required( VERSION 2.8 )

project( test )

include_directories  ( "${PROJECT_SOURCE_DIR}/.." )

add_executable( test_lest           test_lest.cpp )
add_executable( test_lest_cpp03     test_lest_cpp03.cpp )
add_executable( test_lest_basic     test_lest_basic.cpp )
add_executable( test_lest_decompose test_lest_decompose.cpp )

if( ${CMAKE_GENERATOR} MATCHES "Visual" )
    add_compile_options( -W3 -EHsc )
else()
    add_compile_options( -Wall -Wno-missing-braces )

    target_compile_options( test_lest           PUBLIC -std=c++11 )
    target_compile_options( test_lest_cpp03     PUBLIC -std=c++98 )
    target_compile_options( test_lest_basic     PUBLIC -std=c++11 )
    target_compile_options( test_lest_decompose PUBLIC -std=c++11 )
endif()

# configure unit tests via CTest:

enable_testing()

add_test( NAME test_lest           COMMAND test_lest )
add_test( NAME list_version        COMMAND test_lest --version )
add_test( NAME list_tags           COMMAND test_lest --list-tags )
add_test( NAME list_tests          COMMAND test_lest --list-tests )

add_test( NAME test_lest_cpp03     COMMAND test_lest_cpp03 )
add_test( NAME test_lest_basic     COMMAND test_lest_basic )
add_test( NAME test_lest_decompose COMMAND test_lest_decompose )

# end of file
