# Copyright 2013-2016 by Martin Moene
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required( VERSION 2.8 )

project( lest )

option( LEST_BUILD_TEST    "Build tests for lest" ON )
option( LEST_BUILD_EXAMPLE "Build examples" ON )
option( LEST_BUILD_CONTRIB "Build contrib" OFF )

include_directories( "include/lest" )

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

# end of file
