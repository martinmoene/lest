// Copyright 2013, 2014 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "lest_basic.hpp"

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
    TEST( "Numbers compare as usual" )
    {
        EXPECT( 1 < 2  );
        EXPECT( blow() );
        EXPECT( 2 > 1  );
    },

    TEST( "Some tests simply succeed" )
    {
        EXPECT( 1 == 1 );
        EXPECT( 2 != 1 );
    },

    TEST( "Some tests throw unknown exceptions" )
    {
        EXPECT( 1 < 2 );
        EXPECT( phew() );
        EXPECT( 3 < 4 );
    },

    TEST( "Expect exception (success)" )
    {
        EXPECT_THROWS( phew() );
        EXPECT_THROWS_AS( blow(), std::runtime_error );
    },

    TEST( "Expect exception (fail)" )
    {
        EXPECT_THROWS( 1 == 1 );
    },

    TEST( "Expect given exception (fail)" )
    {
        EXPECT_THROWS_AS( blow(), std::bad_alloc );
    },

    TEST( "Text compares lexically" )
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

// cl -nologo -Wall -EHsc -I.. example2_basic.cpp && example2_basic
// g++ -Wall -Wextra -std=c++11 -I.. -o example2_basic.exe example2_basic.cpp && example2_basic

// example2_basic.cpp:27: failed: got unexpected exception with message "this is unexpected!": Numbers compare as usual: blow()
// example2_basic.cpp:40: failed: got unexpected exception of unknown type: Some tests throw unknown exceptions: phew()
// example2_basic.cpp:52: failed: didn't get exception: Expect exception (fail): 1 == 1
// example2_basic.cpp:57: failed: didn't get exception of type std::bad_alloc: Expect given exception (fail): blow()
// example2_basic.cpp:63: failed: Text compares lexically: string("hello") > string("world")
// 5 out of 7 tests failed.
