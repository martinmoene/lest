// Copyright 2013 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "lest.hpp"

using namespace std;

bool blow()
{
    throw std::runtime_error( "this is unexpected!");
    return true;
}

bool phew()
{
    throw 77;
    return true;
}

const lest::test specification[] =
{
    "Numbers compare as usual", []()
    {
        EXPECT( 1 < 2  );
        EXPECT( blow() );
        EXPECT( 2 > 1  );
    },

    "Some tests simply succeed", []()
    {
        EXPECT( 1 == 1 );
        EXPECT( 2 != 1 );
    },

    "Some tests throw unknown exceptions", []()
    {
        EXPECT( 1 < 2 );
        EXPECT( phew() );
        EXPECT( 3 < 4 );
    },

    "Expect exception (success)", []()
    {
        EXPECT_THROWS( phew() );
        EXPECT_THROWS_AS( blow(), std::runtime_error );
    },

    "Expect exception (fail)", []()
    {
        EXPECT_THROWS( 1 == 1 );
    },

    "Expect given exception (fail)", []()
    {
        EXPECT_THROWS_AS( blow(), std::bad_alloc );
    },

    "Text compares lexically", []()
    {
        EXPECT( string("hello") < string("world") );
        EXPECT( string("hello") > string("world") );
        EXPECT( string("skipped") == string("assertion") );
    },
};

int main()
{
    return lest::run( specification );
}

// cl -nologo -Wall -EHsc example2.cpp && example2
// g++ -Wall -Wextra -Weffc++ -std=c++11 -o example2.exe example2.cpp && example2

// example2.cpp:27: failed: got unexpected exception with message "this is unexpected!": Numbers compare as usual: blow()
// example2.cpp:40: failed: got unexpected exception of unknown type: Some tests throw unknown exceptions: phew()
// example2.cpp:52: failed: didn't get exception: Expect exception (fail): 1 == 1
// example2.cpp:57: failed: didn't get exception of type std::bad_alloc: Expect given exception (fail): blow()
// example2.cpp:63: failed: Text compares lexically: string("hello") > string("world")
// 5 out of 7 tests failed.
