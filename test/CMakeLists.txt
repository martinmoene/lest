# Copyright 2013-2018 by Martin Moene
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# -Dlest_FEATURE_LITERAL_SUFFIX=0
# -Dlest_FEATURE_REGEX_SEARCH=0
# -Dlest_FEATURE_TIME=1

cmake_minimum_required( VERSION 2.8 )

project( test )

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

    set( cpp_options
        -Wall -Wextra
    )

    if ( LEST_EXTRA_WARNINGS )
        set( cpp98_extra_options
#        -Wundef
#        -Wno-missing-braces
        -fvisibility=hidden
        -fstrict-aliasing
        -fdiagnostics-show-option
        -Wlogical-op
        -Wstrict-overflow=5
        -Wwrite-strings
        -Wpointer-arith
        -Wcast-qual
        -Wformat=2
        -Wmissing-include-dirs
        -Wcast-align
        -Wdisabled-optimization
        -Winline
        -Winvalid-pch
        -Woverloaded-virtual
        -Wnoexcept
        -Wdouble-promotion
        -Wtrampolines
        -Wvector-operation-performance
        -Weffc++
        -Wmissing-declarations
        -Wuseless-cast
        -Wswitch-default
        -Wctor-dtor-privacy
        -Wnon-virtual-dtor
        -Wredundant-decls
        -Wswitch-enum
#        -pedantic
#        -pedantic-errors
#        -Waggregate-return
#        -Wconversion
#        -Werror: error: ISO C++11 requires at least one argument for the "..." in a variadic macro
#        -Wfloat-equal
#        -Wold-style-cast
#        -Wshadow
#        -Wsign-conversion
        )

        set( cpp11_extra_options
            ${cpp98_extra_options}
            -Wzero-as-null-pointer-constant
        )

    endif() # LEST_ALL_WARNINGS
endif()

# configure unit tests via CTest:

enable_testing()

if( HAS_CPP11 )
    add_executable( test_lest             test_lest.cpp )
    add_executable( test_lest_basic       test_lest_basic.cpp )
    add_executable( test_lest_decompose   test_lest_decompose.cpp )
    add_executable( test_lest_cpp03_cpp11 test_lest_cpp03.cpp )

    target_compile_options( test_lest             PUBLIC ${std11} ${cpp11_options} ${cpp11_extra_options} )
    target_compile_options( test_lest_basic       PUBLIC ${std11} ${cpp11_options} ${cpp11_extra_options} )
    target_compile_options( test_lest_decompose   PUBLIC ${std11} ${cpp11_options} ${cpp11_extra_options} )
    target_compile_options( test_lest_cpp03_cpp11 PUBLIC ${std11} ${cpp11_options} ${cpp11_extra_options} )

    add_test( NAME test_lest                 COMMAND test_lest )
    add_test( NAME list_version              COMMAND test_lest --version )
    add_test( NAME list_tags                 COMMAND test_lest --list-tags )
    add_test( NAME list_tests                COMMAND test_lest --list-tests )

    add_test( NAME test_lest_basic           COMMAND test_lest_basic )
    add_test( NAME test_lest_decompose       COMMAND test_lest_decompose )
    add_test( NAME test_lest_cpp03_cpp11     COMMAND test_lest_cpp03_cpp11 )

    set_property( TEST test_lest             PROPERTY LABELS lest unittest )
    set_property( TEST list_version          PROPERTY LABELS lest unittest )
    set_property( TEST list_tags             PROPERTY LABELS lest unittest )
    set_property( TEST list_tests            PROPERTY LABELS lest unittest )

    set_property( TEST test_lest_basic       PROPERTY LABELS lest unittest )
    set_property( TEST test_lest_decompose   PROPERTY LABELS lest unittest )
    set_property( TEST test_lest_cpp03_cpp11 PROPERTY LABELS lest unittest )
endif()

add_executable(         test_lest_cpp03 test_lest_cpp03.cpp )
target_compile_options( test_lest_cpp03 PUBLIC ${std98} ${cpp98_options} ${cpp98_extra_options} )

add_test(          NAME test_lest_cpp03 COMMAND test_lest_cpp03 )
set_property(      TEST test_lest_cpp03 PROPERTY LABELS lest unittest )

# end of file
