// C++11 - select and omit tests from the command line.

#include "lest/lest.hpp"

using namespace std;

const lest::test specification[] =
{
    CASE( "Empty string has length zero (succeed)" )
    {
        EXPECT(     0u == string(  ).length() );
        EXPECT(     0u == string("").length() );
        EXPECT_NOT( 0u <  string("").length() );
    },

    CASE( "Text compares lexically (fail)" )
    {
        EXPECT( string("hello") > string("world") );
    },

    CASE( "Unexpected exception is reported" )
    {
        EXPECT( (throw std::runtime_error("surprise!"), true) );
    },

    CASE( "Unspecified expected exception is captured" )
    {
        EXPECT_THROWS( throw std::runtime_error("surprise!") );
    },

    CASE( "Specified expected exception is captured" )
    {
        EXPECT_THROWS_AS( throw std::bad_alloc(), std::bad_alloc );
    },

    CASE( "Expected exception is reported missing" )
    {
        EXPECT_THROWS( true );
    },

    CASE( "Specific expected exception is reported missing" )
    {
        EXPECT_THROWS_AS( true, std::runtime_error );
    },
};

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv );
}

// cl -nologo -Wall -EHsc -I../include 05-select.cpp && 05-select
// g++ -Wall -Wextra -std=c++11 -I../include -o 05-select.exe 05-select.cpp && 05-select

// 05-select.cpp:17: failed: Text compares lexically (fail): string("hello") > string("world") for "hello" > "world"
// 05-select.cpp:22: failed: got unexpected exception with message "surprise!": Unexpected exception is reported: (throw std::runtime_error("surprise!"), true)
// 05-select.cpp:37: failed: didn't get exception: Expected exception is reported missing: true
// 05-select.cpp:42: failed: didn't get exception of type std::runtime_error: Specific expected exception is reported missing: true
// 4 out of 7 selected tests failed.
