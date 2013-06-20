#include "lest.hpp"
#include <string>

using namespace std;

const lest::test specification[] =
{
    "Empty string has length zero", []()
    {
        REQUIRE( 0 == string(  ).length() );
        REQUIRE( 0 == string("").length() );
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

// cl -nologo -Wall -EHsc example1.cpp && example1
// g++ -Wall -Wextra -Weffc++ -std=c++11 -o example1.exe example1.cpp && example1

// example1.cpp:17: error: 'Text compares lexically': string("hello") > string("world")
// 1 out of 2 tests failed.
