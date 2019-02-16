// C++11 - use assertion macros, observe error reports.

#include "lest/lest_basic.hpp"

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
    CASE( "Numbers compare as usual" )
    {
        EXPECT( 1 < 2  );
        EXPECT( blow() );
        EXPECT( 2 > 1  );
    },

    CASE( "Some tests simply succeed" )
    {
        EXPECT( 1 == 1 );
        EXPECT( 2 != 1 );
    },

    CASE( "Some tests throw unknown exceptions" )
    {
        EXPECT( 1 < 2 );
        EXPECT( phew() );
        EXPECT( 3 < 4 );
    },

    CASE( "Expect exception (success)" )
    {
        EXPECT_THROWS( phew() );
        EXPECT_THROWS_AS( blow(), std::runtime_error );
    },

    CASE( "Expect exception (fail)" )
    {
        EXPECT_THROWS( 1 == 1 );
    },

    CASE( "Expect given exception (fail)" )
    {
        EXPECT_THROWS_AS( blow(), std::bad_alloc );
    },

    CASE( "Text compares lexically" )
    {
        EXPECT( string("hello") < string("world") );
        EXPECT( string("hello") > string("world") );
        EXPECT( string("skipped") == string("assertion") );
    },
};

int main()
{
    return lest::run( specification /*, argc, argv, std::cout */  );
}

// cl -nologo -Wall -EHsc -I../include 02-basic.cpp && 02-basic
// g++ -Wall -Wextra -std=c++11 -I../include -o 02-basic.exe 02-basic.cpp && 02-basic

// 02-basic.cpp:24: failed: got unexpected exception with message "this is unexpected!": Numbers compare as usual: blow()
// 02-basic.cpp:37: failed: got unexpected exception of unknown type: Some tests throw unknown exceptions: phew()
// 02-basic.cpp:49: failed: didn't get exception: Expect exception (fail): 1 == 1
// 02-basic.cpp:54: failed: didn't get exception of type std::bad_alloc: Expect given exception (fail): blow()
// 02-basic.cpp:60: failed: Text compares lexically: string("hello") > string("world")
// 5 out of 7 tests failed.
