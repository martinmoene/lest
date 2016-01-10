# Copyright 2016 by Martin Moene
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

project( examples )

set( SOURCES
    00_basic.cpp
    01_specification.cpp
)

string( REPLACE ".cpp" "" BASENAMES "${SOURCES}" )

set( TARGETS ${BASENAMES} )

# for pre-VC14:
# set( CMAKE_LINKER_FLAGS "-FORCE:MULTIPLE")

# add single-file and multiple-file module targets:

foreach( name ${TARGETS} )
    add_executable( ${name} ${name}.cpp )
    target_compile_options( ${name} PUBLIC -Dlest_FEATURE_AUTO_REGISTER=1 )
endforeach()

if( ${CMAKE_GENERATOR} MATCHES "Visual" )
    foreach( name ${TARGETS} )
        # Note: do not use 'c' (extern "C" defaults to nothrow) in -EHsc in combination with optimisation -O1, -O2, -Ox
        # For warning suppression, _CRT_NONSTDC_NO_DEPRECATE can be replaced with _CRT_NONSTDC_NO_WARNINGS
        target_compile_options( ${name} PUBLIC -W3 -EHs -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS )
    endforeach()
else()
    foreach( name ${TARGETS} )
        target_compile_options( ${name} PUBLIC -std=c++11 -Wall -Wno-missing-braces )
    endforeach()
endif()

# configure unit tests via CTest:

enable_testing()

foreach( name ${TARGETS} )
    add_test    ( NAME ${name} COMMAND ${name} )
    set_property( TEST ${name} PROPERTY LABELS lest-abort example )
endforeach()

# end of file
