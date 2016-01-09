# Copyright 2013, 2014, 2015 by Martin Moene
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# -Dlest_FEATURE_LITERAL_SUFFIX=0
# -Dlest_FEATURE_REGEX_SEARCH=0
# -Dlest_FEATURE_TIME=1

cmake_minimum_required( VERSION 2.8 )

project( examples )

# single-file sources:

set( SOURCES_CPP03
    04-c++03.cpp
)

set( SOURCES_CPP11
    01-basic.cpp
    02-basic.cpp
    03-decompose.cpp
    05-select.cpp
    06-approx.cpp
    07-udt.cpp
    08-tag.cpp
    09-fixture.cpp
    10-bdd.cpp
    11-auto-reg.cpp
    15-extract-function.cpp
    15-extract-lambda.cpp
    15-extract-template-function.cpp
)

# multiple-file modules:

set( SOURCES_MODULE
    12-module-1.cpp
    12-module-2.cpp
    12-module-3.cpp
)

set( SOURCES_MODULE_AUTO
    13-module-auto-reg-1.cpp
    13-module-auto-reg-2.cpp
    13-module-auto-reg-3.cpp
)

set( SOURCES_MODULE_CPP03
    14-module-cpp03-1.cpp
    14-module-cpp03-2.cpp
    14-module-cpp03-3.cpp
)

# module targets per standard:

set( TARGETS_MODULES_CPP03
    14-module-cpp03
)

set( TARGETS_MODULES_CPP11
    12-module
    13-module-auto-reg
)

# files that need auto registration #define:

set( TARGETS_AUTO
    11-auto-reg
    13-module-auto-reg
    15-extract-function
    15-extract-lambda
    15-extract-template-function
)

# note: here variable must be quoted to create semicolon separated list:

string( REPLACE ".cpp" "" BASENAMES_CPP03 "${SOURCES_CPP03}" )
string( REPLACE ".cpp" "" BASENAMES_CPP11 "${SOURCES_CPP11}" )

set( TARGETS_NOMOD ${BASENAMES_CPP03} ${BASENAMES_CPP11} )
set( TARGETS_CPP03 ${BASENAMES_CPP03} ${TARGETS_MODULES_CPP03} )
set( TARGETS_CPP11 ${BASENAMES_CPP11} ${TARGETS_MODULES_CPP11} )
set( TARGETS_ALL   ${TARGETS_CPP03}   ${TARGETS_CPP11} )

# add single-file and multiple-file module targets:

foreach( name ${TARGETS_NOMOD} )
    add_executable( ${name} ${name}.cpp )
endforeach()

add_executable( 12-module          ${SOURCES_MODULE} )
add_executable( 13-module-auto-reg ${SOURCES_MODULE_AUTO} )
add_executable( 14-module-cpp03    ${SOURCES_MODULE_CPP03} )

# set compiler options:

if( ${CMAKE_GENERATOR} MATCHES "Visual" )
    add_compile_options( -W3 -EHsc )
else()
    add_compile_options( -Wall -Wno-missing-braces )

    foreach( name ${TARGETS_CPP03} )
        target_compile_options( ${name} PUBLIC -std=c++03 )
    endforeach()

    foreach( name ${TARGETS_CPP11} )
        target_compile_options( ${name} PUBLIC -std=c++11 )
    endforeach()
endif()

foreach( name ${TARGETS_AUTO} )
    target_compile_options( ${name} PUBLIC -Dlest_FEATURE_AUTO_REGISTER=1 )
endforeach()

# configure unit tests via CTest:

enable_testing()

foreach( name ${TARGETS_ALL} )
    add_test    ( NAME ${name} COMMAND ${name} )
    set_property( TEST ${name} PROPERTY LABELS lest example )
endforeach()

# end of file
