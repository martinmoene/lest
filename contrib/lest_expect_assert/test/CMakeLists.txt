# Copyright 2016 by Martin Moene
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

project( test )

set( SOURCES
    lest_expect_assert.t.cpp
)

string( REPLACE ".cpp" "" BASENAMES "${SOURCES}" )

set( TARGETS ${BASENAMES} )

# add compiler-specific targets with options:

if( ${CMAKE_GENERATOR} MATCHES "Visual" )
    foreach( name ${TARGETS} )
        set           (   target "contrib-assert-${name}" )
        add_executable( ${target} ${name}.cpp     )
        # Note: omit 'c' (extern "C" defaults to nothrow) in -EHsc when you throw through C functions
        # For warning suppression, _CRT_NONSTDC_NO_DEPRECATE can be replaced with _CRT_NONSTDC_NO_WARNINGS
        target_compile_options( ${target} PUBLIC -W3 -EHs -Dlest_FEATURE_AUTO_REGISTER=1 -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS )
    endforeach()
else()
    foreach( name ${TARGETS} )
        set           (   target "contrib-assert-${name}"   )
        add_executable( ${target}       ${name}.cpp )
        add_executable( ${target}_cpp03 ${name}.cpp )

        target_compile_options( ${target}       PUBLIC -std=c++11 -Wall -Wno-missing-braces -Dlest_FEATURE_AUTO_REGISTER=1 )
        target_compile_options( ${target}_cpp03 PUBLIC -std=c++03 -Wall -Wno-missing-braces -Dlest_FEATURE_AUTO_REGISTER=1 )
    endforeach()
endif()

# configure unit tests via CTest:

enable_testing()

foreach( name ${TARGETS} )
    set( target "contrib-assert-${name}" )

    add_test    ( NAME ${target}       COMMAND ${target} )
    add_test    ( NAME ${target}_cpp03 COMMAND ${target} )

    set_property( TEST ${target}       PROPERTY LABELS contrib assert unittest )
    set_property( TEST ${target}_cpp03 PROPERTY LABELS contrib assert unittest )
endforeach()

# end of file
