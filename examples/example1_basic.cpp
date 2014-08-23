#include "lest_basic.hpp"

using namespace std;

const lest::test specification[] =
{
    "Empty string has length zero (succeed)", []
    {
        EXPECT( 0 == string(  ).length() );
        EXPECT( 0 == string("").length() );
    },

    "Text compares lexically (fail)", []
    {
        EXPECT( string("hello") > string("world") );
    },

    "Unexpected exception is reported", []
    {
        EXPECT( (throw std::runtime_error("surprise!"), true) );
    },

    "Unspecified expected exception is captured", []
    {
        EXPECT_THROWS( (throw std::runtime_error("surprise!"), true) );
    },

    "Specified expected exception is captured", []
    {
        EXPECT_THROWS_AS( (throw std::bad_alloc(), true), std::bad_alloc );
    },

    "Expected exception is reported missing", []
    {
        EXPECT_THROWS( true );
    },

    "Specific expected exception is reported missing", []
    {
        EXPECT_THROWS_AS( true, std::runtime_error );
    },
};

int main()
{
    return lest::run( specification );
}

// cl -nologo -Wall -EHsc -I.. example1.cpp && example1
// g++ -Wall -Wextra -Weffc++ -std=c++11 -I.. -o example1.exe example1.cpp && example1

// example1.cpp:15: failed: Text compares lexically (fail): string("hello") > string("world")
// example1.cpp:20: failed: got unexpected exception with message "surprise!": Unexpected exception is reported: (throw std::runtime_error("surprise!"), true)
// example1.cpp:35: failed: didn't get exception: Expected exception is reported missing: true
// example1.cpp:40: failed: didn't get exception of type std::runtime_error: Specific expected exception is reported missing: true
// 4 out of 7 tests failed.
