// C++11 - call a template function that uses EXPECT().

#include "lest/lest.hpp"

#define CASE( name ) lest_CASE( specification, name )

static lest::tests specification;

template< typename T >
void fun( lest::env & lest_env, T const & a, T const & b )
{
    EXPECT( a == b );
}

CASE( "Call a templated function that uses EXPECT" )
{
    fun( lest_env, 1, 1  );
    fun( lest_env, std::make_pair(1,2), std::make_pair(1,2) );
    fun( lest_env, std::make_pair(1,1), std::make_pair(1,2) );
}

// or, via a macro if you like:

#define mfun( a, b ) \
    fun( lest_env, a, b )

CASE( "Call a macro that calls a templated function that uses EXPECT" )
{
    mfun( 1, 1  );
    mfun( std::make_pair(1,2), std::make_pair(1,2) );
    mfun( std::make_pair(1,1), std::make_pair(1,2) );
}

// more elaborate example:

template< typename T >
void case_resize_vector( lest::env & lest_env )
{
    SETUP( "A vector with some items" )  {
        std::vector<T> v( 5 );

        EXPECT( v.size() == 5u );
        EXPECT( v.capacity() >= 5u );

        SECTION( "resizing bigger changes size and capacity" ) {
            v.resize( 10 );

            EXPECT( v.size() == 10u );
            EXPECT( v.capacity() >= 10u );
        }
        SECTION( "resizing smaller changes size but not capacity" ) {
            v.resize( 0 );

            EXPECT( v.size() == 0u );
            EXPECT( v.capacity() >= 5u );
        }
        SECTION( "reserving bigger changes capacity but not size" ) {
            v.reserve( 10 );

            EXPECT( v.size() == 5u );
            EXPECT( v.capacity() >= 10u );

            SECTION( "reserving smaller again does not change capacity" ) {
                v.reserve( 7 );

                EXPECT( v.capacity() >= 10u );
            }
        }
        SECTION( "reserving smaller does not change size or capacity" ) {
            v.reserve( 0 );

            EXPECT( v.size() == 5u );
            EXPECT( v.capacity() >= 5u );
        }
    }
}

CASE( "Call a templated function that uses SETUP, SECTION, EXPECT" )
{
    // vectors can be sized and resized
    case_resize_vector<int   >( lest_env );
    case_resize_vector<double>( lest_env );
}

// test runner:

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv /*, std::cout */ );
}

// cl -nologo -W3 -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I../include -Fe15-extract-template-function.exe 15-extract-template-function.cpp && 15-extract-template-function
// g++ -Wall -Wextra -std=c++11 -Dlest_FEATURE_AUTO_REGISTER=1 -I../include -o 15-extract-template-function.exe 15-extract-template-function.cpp && 15-extract-template-function
