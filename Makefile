# Copyright 2013 by Martin Moene
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

IS_WIN := $(shell ver)

ifneq "$(IS_WIN)" ""
EXT := .exe
endif

CXXFLAGS := -Wall -Wextra -Weffc++ -std=c++11

all: test_lest$(EXT) example1$(EXT) example2$(EXT)

test_lest$(EXT): test_lest.cpp lest.hpp
	$(CXX) $(CXXFLAGS) -o test_lest test_lest.cpp && test_lest

example1$(EXT): example1.cpp lest.hpp
	-$(CXX) $(CXXFLAGS) -o example1 example1.cpp && example1

example2$(EXT): example2.cpp lest.hpp
	-$(CXX) $(CXXFLAGS) -o example2 example2.cpp && example2

clean:
	rm test_lest$(EXT) example1$(EXT) example2$(EXT)

#
# end of
#
