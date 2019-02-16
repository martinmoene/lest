// C++11 - call a lambda that uses EXPECT().

#include "lest/lest.hpp"

#define CASE( name ) lest_CASE( specification, name )

static lest::tests specification;

CASE( "Call a lambda that uses EXPECT" )
{
    auto fun = [ &lest_env ]( int a, int b ){ EXPECT( a == b ); };

    fun( 1, 1 );
    fun( 1, 2 );
}

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv /*, std::cout */ );
}

// cl -nologo -W3 -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I../include -Fe15-extract-lambda.exe 15-extract-lambda.cpp && 15-extract-lambda
// g++ -Wall -Wextra -std=c++11 -Dlest_FEATURE_AUTO_REGISTER=1 -I../include -o 15-extract-lambda.exe 15-extract-lambda.cpp && 15-extract-lambda
