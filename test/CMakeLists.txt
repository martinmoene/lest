# Copyright 2013-2018 by Martin Moene
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# -Dlest_FEATURE_LITERAL_SUFFIX=0
# -Dlest_FEATURE_REGEX_SEARCH=0
# -Dlest_FEATURE_TIME=1

if( NOT DEFINED CMAKE_MINIMUM_REQUIRED_VERSION )
    cmake_minimum_required( VERSION 3.5 FATAL_ERROR )
endif()

project( test LANGUAGES CXX )

message( STATUS "Subproject '${PROJECT_NAME}', programs 'test_lest-*'")

# Configure lest for testing:

set( LEST_EXTRA_WARNINGS FALSE )

set( OPTIONS "" )
set( LEST_CONFIG "" )

set( HAS_STD_FLAGS  FALSE )
set( HAS_CPP98_FLAG FALSE )
set( HAS_CPP11_FLAG FALSE )
set( HAS_CPP14_FLAG FALSE )
set( HAS_CPP17_FLAG FALSE )
set( HAS_CPP20_FLAG FALSE )
set( HAS_CPPLATEST_FLAG FALSE )

if( NOT MSVC AND LEST_EXTRA_WARNINGS )
    set( cpp98_extra_options
        -fdiagnostics-show-option
        -fstrict-aliasing
        -fvisibility=hidden
        -pedantic # : error: ISO C++11 requires at least one argument for the "..." in a variadic macro
        -pedantic-errors
        -Wundef
        -Wno-missing-braces
        -Waggregate-return
        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wctor-dtor-privacy
        -Wdisabled-optimization
        -Wdouble-promotion
        -Weffc++
        -Wfloat-equal
        -Wformat=2
        -Winline
        -Winvalid-pch
        -Wlogical-op
        -Wmissing-declarations
        -Wmissing-include-dirs
        -Wnoexcept
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wpointer-arith
        -Wredundant-decls
        -Wshadow
        -Wsign-conversion
        -Wstrict-overflow=5
        -Wswitch-default
        -Wswitch-enum
        -Wtrampolines
        -Wuseless-cast
        -Wvector-operation-performance
        -Wwrite-strings
        -Werror
    )

    set( cpp11_extra_options
        ${cpp98_extra_options}
        -Wzero-as-null-pointer-constant
    )
endif() # LEST_EXTRA_WARNINGS

# Handle various compilers and their capabiliteis:

if( MSVC )
    message( STATUS "Matched: MSVC")

    set( HAS_STD_FLAGS TRUE )

    set( OPTIONS     -W3 -EHsc )
    set( DEFINITIONS -D_SCL_SECURE_NO_WARNINGS ${LEST_CONFIG} )

    if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.00 )
        set( HAS_CPP14_FLAG TRUE )
        set( HAS_CPPLATEST_FLAG TRUE )
    endif()
    if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.11 )
        set( HAS_CPP17_FLAG TRUE )
    endif()

elseif( CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang" )
    message( STATUS "CompilerId: '${CMAKE_CXX_COMPILER_ID}'")

    set( HAS_STD_FLAGS  TRUE )
    set( HAS_CPP98_FLAG TRUE )

    set( OPTIONS     -Wall -Wextra -Wconversion -Wsign-conversion -Wno-missing-braces -fno-elide-constructors )
    set( DEFINITIONS ${LEST_CONFIG} )

    # GNU: available -std flags depends on version
    if( CMAKE_CXX_COMPILER_ID MATCHES "GNU" )
        message( STATUS "Matched: GNU")

        if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8.0 )
            set( HAS_CPP11_FLAG TRUE )
        endif()
        if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9.2 )
            set( HAS_CPP14_FLAG TRUE )
        endif()
        if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.1.0 )
            set( HAS_CPP17_FLAG TRUE )
        endif()

    # AppleClang: available -std flags depends on version
    elseif( CMAKE_CXX_COMPILER_ID MATCHES "AppleClang" )
        message( STATUS "Matched: AppleClang")

        if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0.0 )
            set( HAS_CPP11_FLAG TRUE )
        endif()
        if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.1.0 )
            set( HAS_CPP14_FLAG TRUE )
        endif()
        if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.2.0 )
            set( HAS_CPP17_FLAG TRUE )
        endif()

    # Clang: available -std flags depends on version
    elseif( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
        message( STATUS "Matched: Clang")

        if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.3.0 )
            set( HAS_CPP11_FLAG TRUE )
        endif()
        if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.4.0 )
            set( HAS_CPP14_FLAG TRUE )
        endif()
        if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0.0 )
            set( HAS_CPP17_FLAG TRUE )
        endif()
    endif()

elseif( CMAKE_CXX_COMPILER_ID MATCHES "Intel" )
    # as is
    message( STATUS "Matched: Intel")
else()
    # as is
    message( STATUS "Matched: nothing")
endif()

# Enable MS C++ Core Guidelines checker if MSVC:

function( enable_msvs_guideline_checker target )
    if( MSVC )
        set_target_properties( ${target} PROPERTIES
        VS_GLOBAL_EnableCppCoreCheck true
        VS_GLOBAL_CodeAnalysisRuleSet CppCoreCheckRules.ruleset
        VS_GLOBAL_RunCodeAnalysis true )
    endif()
endfunction()

# Make target, compile for given standard if specified:

function( make_target target source std )
    message( STATUS "Make target: '${std}': ${target}" )

    add_executable            ( ${target} ${source} ${HDRPATH} )
    target_link_libraries     ( ${target} PRIVATE ${PACKAGE} )
    target_compile_options    ( ${target} PRIVATE ${OPTIONS} )
    target_compile_definitions( ${target} PRIVATE ${DEFINITIONS} )

    add_test(     NAME ${target} COMMAND ${target} )
    set_property( TEST ${target} PROPERTY LABELS lest unittest )

    if( std )
        if ( std LESS 11 )
            target_compile_options( ${target} PRIVATE ${cpp98_extra_options} )
        else()
            target_compile_options( ${target} PRIVATE ${cpp11_extra_options} )
        endif()

        if( MSVC )
            target_compile_options( ${target} PRIVATE -std:c++${std} )
        else()
            # Necessary for clang 3.x:
            target_compile_options( ${target} PRIVATE -std=c++${std} )
            # Ok for clang 4 and later:
            # set( CMAKE_CXX_STANDARD ${std} )
            # set( CMAKE_CXX_STANDARD_REQUIRED ON )
            # set( CMAKE_CXX_EXTENSIONS OFF )
        endif()
    endif()
endfunction()

# Add generic executable, unless -std flags can be specified:

if( NOT HAS_STD_FLAGS )
    make_target( test_lest_cpp03_cpp98          test_lest_cpp03.cpp     "" )
else()
    # unconditionally add C++98 variant as MSVC has no option for it:
    if( HAS_CPP98_FLAG )
        make_target( test_lest_cpp03_cpp98      test_lest_cpp03.cpp     98 )
    else()
        make_target( test_lest_cpp03_cpp98      test_lest_cpp03.cpp     "" )
    endif()

    if( HAS_CPP11_FLAG )
        make_target( test_lest-cpp11            test_lest.cpp           11 )
        make_target( test_lest_basic-cpp11      test_lest_basic.cpp     11 )
        make_target( test_lest_decompose-cpp11  test_lest_decompose.cpp 11 )
        make_target( test_lest_cpp03_cpp11      test_lest_cpp03.cpp     11 )
    endif()

    if( HAS_CPP14_FLAG )
        make_target( test_lest-cpp14            test_lest.cpp           14 )
        make_target( test_lest_cpp03_cpp14      test_lest_cpp03.cpp     14 )
    endif()

    if( HAS_CPP17_FLAG )
        set( std17 17 )
        if( CMAKE_CXX_COMPILER_ID MATCHES "AppleClang" )
            set( std17 1z )
        endif()
        make_target( test_lest-cpp17            test_lest.cpp       ${std17} )
        make_target( test_lest_cpp03-cpp17      test_lest_cpp03.cpp ${std17} )

        enable_msvs_guideline_checker( test_lest-cpp17 )
        enable_msvs_guideline_checker( test_lest_cpp03-cpp17 )
    endif()

    if( HAS_CPP20_FLAG )
        set( std20 20 )
        if( CMAKE_CXX_COMPILER_ID MATCHES "AppleClang" )
            set( std20 xx )
        endif()
        make_target( test_lest-cpp20            test_lest.cpp       ${std20} )
        make_target( test_lest_cpp03-cpp20      test_lest_cpp03.cpp ${std20} )
    endif()

    if( HAS_CPPLATEST_FLAG )
        make_target( test_lest-cpplatest        test_lest.cpp        latest )
        make_target( test_lest_cpp03-cpplatest  test_lest_cpp03.cpp  latest )
    endif()
endif()

# Configure unit tests via CTest:

enable_testing()

# end of file
