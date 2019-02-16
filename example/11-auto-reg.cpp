// C++11 - auto-register test cases.

#include "lest/lest.hpp"

#define CASE( name ) lest_CASE( specification, name )

static lest::tests specification;

CASE( "A passing test" "[pass]" )
{
    EXPECT( 42 == 42 );
}

CASE( "A failing test" "[fail]" )
{
    EXPECT( 42 == 7 );
}

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv /*, std::cout */ );
}

// cl -nologo -W3 -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I../include -Fe11-auto-reg.exe 11-auto-reg.cpp && 11-auto-reg
// g++ -Wall -Wextra -std=c++11 -Dlest_FEATURE_AUTO_REGISTER=1 -I../include -o 11-auto-reg.exe 11-auto-reg.cpp && 11-auto-reg
