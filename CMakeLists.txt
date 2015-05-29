# Copyright 2013, 2014, 2015 by Martin Moene
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# -Dlest_FEATURE_LITERAL_SUFFIX=0
# -Dlest_FEATURE_REGEX_SEARCH=0
# -Dlest_FEATURE_TIME=1

cmake_minimum_required( VERSION 2.8 )

project( lest )

add_subdirectory( test )
add_subdirectory( example )

# end of file
