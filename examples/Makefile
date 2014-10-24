# Copyright 2013, 2014 by Martin Moene
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# -Dlest_FEATURE_COLOURISE=0
# -Dlest_FEATURE_LITERAL_SUFFIX=0
# -Dlest_FEATURE_REGEX_SEARCH=0
# -Dlest_FEATURE_TIME=1

CXXFLAGS   := -Wall -Wextra -Wno-missing-braces -I..
CXXFLAGS03 := $(CXXFLAGS) -std=c++03
CXXFLAGS11 := $(CXXFLAGS) -std=c++11

vpath %.hpp ..

all: 01-basic 02-basic 03-decompose 04-c++03 \
     05-select 06-approx 07-udt 08-tag.exe \
     09-fixture.exe 10-bdd.exe

01-basic: 01-basic.cpp lest_basic.hpp
	$(CXX) $(CXXFLAGS11) -o 01-basic 01-basic.cpp
	-./01-basic

02-basic: 02-basic.cpp lest_basic.hpp
	$(CXX) $(CXXFLAGS11) -o 02-basic 02-basic.cpp
	-./02-basic

03-decompose: 03-decompose.cpp lest_decompose.hpp
	$(CXX) $(CXXFLAGS11) -o 03-decompose 03-decompose.cpp
	-./03-decompose

04-c++03: 04-c++03.cpp lest_cpp03.hpp
	$(CXX) $(CXXFLAGS03) -o 04-c++03 04-c++03.cpp
	-./04-c++03

04-c++03: 04-c++03.cpp lest_cpp03.hpp
	$(CXX) $(CXXFLAGS03) -o 04-c++03 04-c++03.cpp
	-./04-c++03

05-select: 05-select.cpp lest.hpp
	$(CXX) $(CXXFLAGS11) -o 05-select 05-select.cpp
	-./05-select

06-approx: 06-approx.cpp lest.hpp
	$(CXX) $(CXXFLAGS11) -o 06-approx 06-approx.cpp
	-./06-approx

07-udt: 07-udt.cpp lest.hpp
	$(CXX) $(CXXFLAGS11) -o 07-udt 07-udt.cpp
	-./07-udt

08-tag: 08-tag.cpp lest.hpp
	$(CXX) $(CXXFLAGS11) -o 08-tag 08-tag.cpp
	-./08-tag

09-fixture: 09-fixture.cpp lest.hpp
	$(CXX) $(CXXFLAGS11) -o 09-fixture 09-fixture.cpp
	-./09-fixture

10-bdd: 10-bdd.cpp lest.hpp
	$(CXX) $(CXXFLAGS11) -o 10-bdd 10-bdd.cpp
	-./10-bdd

clean:
	-rm 01-basic 02-basic 03-decompose 04-c++03 \
	    05-select 06-approx 07-udt 08-tag.exe \
	    09-fixture.exe 10-bdd.exe
