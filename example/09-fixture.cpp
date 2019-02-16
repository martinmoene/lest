// C++11 - use a fixture, adapted from Catch tutorial.

#include "lest/lest.hpp"
#include <vector>

using text = std::string;

const lest::test specification[] = {

CASE( "vectors can be sized and resized" "[vector]" ) {

    SETUP( "A vector with some items" )  {
        std::vector<int> v( 5 );

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
}};

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv );
}

// cl -nologo -W3 -EHsc -I../include 09-fixture.cpp && 09-fixture
// g++ -Wall -Wextra -Wmissing-include-dirs -std=c++11 -I../include -o 09-fixture.exe 09-fixture.cpp && 09-fixture
