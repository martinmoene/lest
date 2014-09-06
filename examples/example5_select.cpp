// C++11 - select and omit tests from the command line.

#include "lest.hpp"

using namespace std;

const lest::test specification[] =
{
    TEST( "Empty string has length zero (succeed)" )
    {
        EXPECT( 0 == string(  ).length() );
        EXPECT( 0 == string("").length() );
    },

    TEST( "Text compares lexically (fail)" )
    {
        EXPECT( string("hello") > string("world") );
    },

    TEST( "Unexpected exception is reported" )
    {
        EXPECT( (throw std::runtime_error("surprise!"), true) );
    },

    TEST( "Unspecified expected exception is captured" )
    {
        EXPECT_THROWS( (throw std::runtime_error("surprise!"), true) );
    },

    TEST( "Specified expected exception is captured" )
    {
        EXPECT_THROWS_AS( (throw std::bad_alloc(), true), std::bad_alloc );
    },

    TEST( "Expected exception is reported missing" )
    {
        EXPECT_THROWS( true );
    },

    TEST( "Specific expected exception is reported missing" )
    {
        EXPECT_THROWS_AS( true, std::runtime_error );
    },
};

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv );
}

// cl -nologo -Wall -EHsc -I.. example5_select.cpp && example5_select
// g++ -Wall -Wextra -std=c++11 -I.. -o example5_select.exe example5_select.cpp && example5_select

// example5_select.cpp:17: failed: Text compares lexically (fail): string("hello") > string("world") for "hello" > "world"
// example5_select.cpp:22: failed: got unexpected exception with message "surprise!": Unexpected exception is reported: (throw std::runtime_error("surprise!"), true)
// example5_select.cpp:37: failed: didn't get exception: Expected exception is reported missing: true
// example5_select.cpp:42: failed: didn't get exception of type std::runtime_error: Specific expected exception is reported missing: true
// 4 out of 7 selected tests failed.
