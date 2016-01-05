// C++11 - call a template function that uses EXPECT().

#include "lest.hpp"

#define CASE( name ) lest_CASE( specification, name )

static lest::tests specification; 

template< typename T >
void fun( lest::env & $, T const & a, T const & b )
{
    EXPECT( a == b );
}

CASE( "Call a templated function that uses EXPECT" )
{
    fun( $, 1, 1  );
    fun( $, std::make_pair(1,2), std::make_pair(1,2) );
    fun( $, std::make_pair(1,1), std::make_pair(1,2) );
}

// or, via a macro if you like:

#define mfun( a, b ) \
    fun( $, a, b )

CASE( "Call a macro that calls a templated function that uses EXPECT" )
{
    mfun( 1, 1  );
    mfun( std::make_pair(1,2), std::make_pair(1,2) );
    mfun( std::make_pair(1,1), std::make_pair(1,2) );
}

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv /*, std::cout */ );
}

// cl -nologo -W3 -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I.. -Fe15-extract-template-function.exe 15-extract-template-function.cpp && 15-extract-template-function
// g++ -Wall -Wextra -std=c++11 -Dlest_FEATURE_AUTO_REGISTER=1 -I.. -o 15-extract-template-function.exe 15-extract-template-function.cpp && 15-extract-template-function
