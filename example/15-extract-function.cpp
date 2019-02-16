// C++11 - call a function that uses EXPECT().

#include "lest/lest.hpp"

#define CASE( name ) lest_CASE( specification, name )

static lest::tests specification;

void fun( lest::env & lest_env, int a, int b )
{
    EXPECT( a == b );
}

CASE( "Call a function that uses EXPECT" )
{
    fun( lest_env, 1, 1 );
    fun( lest_env, 1, 2 );
}

// or, via a macro if you like:

#define mfun( a, b ) \
    fun( lest_env, a, b )

CASE( "Call a macro that calls a function that uses EXPECT" )
{
    mfun( 1, 1 );
    mfun( 1, 2 );
}

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv /*, std::cout */ );
}

// cl -nologo -W3 -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I../include -Fe15-extract-function.exe 15-extract-function.cpp && 15-extract-function
// g++ -Wall -Wextra -std=c++11 -Dlest_FEATURE_AUTO_REGISTER=1 -I../include -o 15-extract-function.exe 15-extract-function.cpp && 15-extract-function
