# Copyright 2013-2018 by Martin Moene
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required( VERSION 3.5 FATAL_ERROR )

project(
    lest
    VERSION 1.35.2
#   DESCRIPTION "A modern, C++11-native, single-file header-only, tiny framework for unit-tests, TDD and BDD (includes C++98 variant)"
#   HOMEPAGE_URL "https://github.com/martinmoene/lest"
    LANGUAGES CXX )

option( LEST_BUILD_TEST     "Build tests for lest" ON )
option( LEST_BUILD_EXAMPLE  "Build examples" ON )
option( LEST_BUILD_CONTRIB  "Build contrib" OFF )
option( LEST_EXTRA_WARNINGS "Compile with extra warnings" OFF )

include( GNUInstallDirs )
include_directories( "include" )

if ( LEST_BUILD_TEST )
  add_subdirectory( test )
endif()

if ( LEST_BUILD_EXAMPLE )
  add_subdirectory( example )
endif()

if ( LEST_BUILD_CONTRIB )
  add_subdirectory( contrib )
endif()

# configure unit tests via CTest:
enable_testing()

install( DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )

# end of file
