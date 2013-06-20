// Copyright 2013 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "lest.hpp"
#include <string>

using namespace std;

bool blow()
{
    throw std::runtime_error( "this is unexpected!");
    return true;
}

const lest::test specification[] =
{
    "Numbers compare as usual", []()
    {
        CHECK( 1 < 2  );
        CHECK( blow() );
        CHECK( 2 > 1  );
    },

    "Some tests just succeed", []()
    {
        REQUIRE( 1 == 1 );
        REQUIRE( 2 != 1 );
    },

    "Text compares lexically", []()
    {
        CHECK( string("hello") < string("world") );
        CHECK( string("hello") > string("world") );
        CHECK( string("skipped") == string("assertion") );
    },
};

int main()
{
    return lest::breaks( specification );
}

// cl -nologo -Wall -EHsc example2.cpp && example2
// g++ -Wall -Wextra -Weffc++ -std=c++11 -o example2.exe example2.cpp && example2

// example2.cpp:22: exception: 'Numbers compare as usual': this is unexpected!
// example2.cpp:35: error: 'Text compares lexically': string("hello") > string("world")
// 2 out of 3 tests failed.
