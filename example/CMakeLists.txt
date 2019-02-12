# Copyright 2013, 2014, 2015 by Martin Moene
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# -Dlest_FEATURE_LITERAL_SUFFIX=0
# -Dlest_FEATURE_REGEX_SEARCH=0
# -Dlest_FEATURE_TIME=1

if( NOT DEFINED CMAKE_MINIMUM_REQUIRED_VERSION )
    cmake_minimum_required( VERSION 3.5 FATAL_ERROR )
endif()

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
    10-bdd-auto.cpp
    11-auto-reg.cpp
    15-extract-function.cpp
    15-extract-lambda.cpp
    15-extract-template-function.cpp
    17-syntax-catch.cpp
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
    10-bdd-auto
    11-auto-reg
    13-module-auto-reg
    15-extract-function
    15-extract-lambda
    15-extract-template-function
    17-syntax-catch
)

# note: here variable must be quoted to create semicolon separated list:

string( REPLACE ".cpp" "" BASENAMES_CPP03 "${SOURCES_CPP03}" )
string( REPLACE ".cpp" "" BASENAMES_CPP11 "${SOURCES_CPP11}" )

set( TARGETS_NOMOD_CPP03 ${BASENAMES_CPP03} )
set( TARGETS_NOMOD_CPP11 ${BASENAMES_CPP11} )
set( TARGETS_CPP03       ${TARGETS_NOMOD_CPP03} ${TARGETS_MODULES_CPP03} )
set( TARGETS_CPP11       ${TARGETS_NOMOD_CPP11} ${TARGETS_MODULES_CPP11} )
set( TARGETS_ALL         ${TARGETS_CPP03}   ${TARGETS_CPP11} )

# set compiler options:

set( HAS_CPP11 TRUE )

if( MSVC )
    set( std98 "" )
    set( std11 "" )
    set( cpp_options -W3 -EHsc )

    if( CMAKE_CXX_COMPILER_VERSION VERSION_LESS 18.00 )
        set( HAS_CPP11 FALSE )
    endif()
else()
    set( std98 "-std=c++98" )
    set( std11 "-std=c++11" )
    set( cpp_options -Wall -Wextra -Wno-missing-braces )
endif()

# configure unit tests via CTest:
enable_testing()

if( HAS_CPP11 )
    # add C++11 single-file targets:
    foreach( name ${TARGETS_NOMOD_CPP11} )
        add_executable( ${name} ${name}.cpp )
    endforeach()

    # add module targets, single-file and multiple-file:
    add_executable( 12-module          ${SOURCES_MODULE} )
    add_executable( 13-module-auto-reg ${SOURCES_MODULE_AUTO} )

    # define auto-register:
    foreach( name ${TARGETS_AUTO} )
        target_compile_options( ${name} PUBLIC -Dlest_FEATURE_AUTO_REGISTER=1 )
    endforeach()

    # compile as C++11, specify other compilation options, define ctest aliases:
    foreach( name ${TARGETS_CPP11} )
        target_compile_options( ${name} PUBLIC   ${std11} ${cpp_options} )
        add_test         ( NAME ${name} COMMAND  ${name} )
        set_property     ( TEST ${name} PROPERTY LABELS lest example )
    endforeach()
endif()  # HAS_CPP11

# add C++98/03 single-file targets:
foreach( name ${TARGETS_NOMOD_CPP03} )
    add_executable( ${name} ${name}.cpp )
endforeach()

# add C++98/03 module targets, single-file and multiple-file:
add_executable( 14-module-cpp03 ${SOURCES_MODULE_CPP03} )

# compile as C++98, specify other compilation options, define ctest aliases:
foreach( name ${TARGETS_CPP03} )
    target_compile_options( ${name} PUBLIC   ${std98} ${cpp_options} )
    add_test         ( NAME ${name} COMMAND  ${name} )
    set_property     ( TEST ${name} PROPERTY LABELS lest example )
endforeach()

# end of file
