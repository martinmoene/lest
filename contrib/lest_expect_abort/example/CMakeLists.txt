# Copyright 2016 by Martin Moene
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

project( examples )

set( SOURCES
    00_basic.cpp
    01_specification.cpp
)

string( REPLACE ".cpp" "" BASENAMES "${SOURCES}" )

set( TARGETS ${BASENAMES} )

# for pre-VC14:
# set( CMAKE_LINKER_FLAGS "-FORCE:MULTIPLE")

# add targets with common options:

foreach( name ${TARGETS} )
    set( target "contrib-abort-${name}" )
    add_executable( ${target} ${name}.cpp )
    target_compile_options( ${target} PUBLIC -Dlest_FEATURE_AUTO_REGISTER=1 )
endforeach()

# add compiler-specific options:

if( ${CMAKE_GENERATOR} MATCHES "Visual" )
    foreach( name ${TARGETS} )
        set( target "contrib-abort-${name}" )
        # Note: omit 'c' (extern "C" defaults to nothrow) in -EHsc when you throw through C functions
        # For warning suppression, _CRT_NONSTDC_NO_DEPRECATE can be replaced with _CRT_NONSTDC_NO_WARNINGS
        target_compile_options( ${target} PUBLIC -W3 -EHs -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS )
    endforeach()
else()
    foreach( name ${TARGETS} )
        set( target "contrib-abort-${name}" )
        target_compile_options( ${target} PUBLIC -std=c++11 -Wall -Wno-missing-braces )
    endforeach()
endif()

# configure unit tests via CTest:

enable_testing()

foreach( name ${TARGETS} )
    set         ( target "contrib-abort-${name}" )
    add_test    ( NAME ${target} COMMAND ${target} )
    set_property( TEST ${target} PROPERTY LABELS contrib abort example )
endforeach()

# end of file
