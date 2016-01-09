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

# add single-file and multiple-file module targets:

foreach( name ${TARGETS} )
    add_executable( ${name}       ${name}.cpp )
    add_executable( ${name}_cpp03 ${name}.cpp )

    target_compile_options( ${name}       PUBLIC -Dlest_FEATURE_AUTO_REGISTER=1 )
    target_compile_options( ${name}_cpp03 PUBLIC -Dlest_FEATURE_AUTO_REGISTER=1 )
endforeach()

if( ${CMAKE_GENERATOR} MATCHES "Visual" )
    foreach( name ${TARGETS} )
        # Important: don't optimize: -Od (it is not sufficient to inhibit inline expansion through -Ob0)
        # For warning suppression, _CRT_NONSTDC_NO_DEPRECATE can be replaced with _CRT_NONSTDC_NO_WARNINGS
        target_compile_options( ${name} PUBLIC -W3 -EHsc -Od -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS )
    endforeach()
else()
    foreach( name ${TARGETS} )
        target_compile_options( ${name}       PUBLIC -std=c++11 -Wall -Wno-missing-braces )
        target_compile_options( ${name}_cpp03 PUBLIC -std=c++03 -Wall -Wno-missing-braces )
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
