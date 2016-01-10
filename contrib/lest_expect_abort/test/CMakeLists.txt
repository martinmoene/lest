# Copyright 2016 by Martin Moene
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

project( test )

set( SOURCES
    lest_expect_abort.t.cpp
)

string( REPLACE ".cpp" "" BASENAMES "${SOURCES}" )

set( TARGETS ${BASENAMES} )

# for pre-VC14:
# set( CMAKE_LINKER_FLAGS "-FORCE:MULTIPLE")

if( ${CMAKE_GENERATOR} MATCHES "Visual" )
    foreach( name ${TARGETS} )
        add_executable( ${name} ${name}.cpp )
        # Note: do not use 'c' (extern "C" defaults to nothrow) in -EHsc in combination with optimisation -O1, -O2, -Ox
        # For warning suppression, _CRT_NONSTDC_NO_DEPRECATE can be replaced with _CRT_NONSTDC_NO_WARNINGS
        target_compile_options( ${name} PUBLIC -W3 -EHs -Dlest_FEATURE_AUTO_REGISTER=1 -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS )
    endforeach()
else()
    foreach( name ${TARGETS} )
        add_executable( ${name}       ${name}.cpp )
        add_executable( ${name}_cpp03 ${name}.cpp )

        target_compile_options( ${name}       PUBLIC -std=c++11 -Wall -Wno-missing-braces -Dlest_FEATURE_AUTO_REGISTER=1 )
        target_compile_options( ${name}_cpp03 PUBLIC -std=c++03 -Wall -Wno-missing-braces -Dlest_FEATURE_AUTO_REGISTER=1 )
    endforeach()
endif()

# configure unit tests via CTest:

enable_testing()

foreach( name ${TARGETS} )
    add_test    ( NAME ${name}       COMMAND ${name} )
    add_test    ( NAME ${name}_cpp03 COMMAND ${name} )

    set_property( TEST ${name}       PROPERTY LABELS lest-abort unittest )
    set_property( TEST ${name}_cpp03 PROPERTY LABELS lest-abort unittest )
endforeach()

# end of file
