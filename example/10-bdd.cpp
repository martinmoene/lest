// C++11 - use BDD style, adapted from Catch tutorial.

#include "lest/lest.hpp"
#include <vector>

using text = std::string;

const lest::test specification[] = {

SCENARIO( "vectors can be sized and resized" "[vector]" ) {

    GIVEN( "A vector with some items" ) {
        std::vector<int> v( 5 );

        EXPECT( v.size() == 5u );
        EXPECT( v.capacity() >= 5u );

        WHEN( "the size is increased" ) {
            v.resize( 10 );

            THEN( "the size and capacity change" ) {
                EXPECT( v.size() == 10u);
                EXPECT( v.capacity() >= 10u );
            }
        }
        WHEN( "the size is reduced" ) {
            v.resize( 0 );

            THEN( "the size changes but not capacity" ) {
                EXPECT( v.size() == 0u );
                EXPECT( v.capacity() >= 5u );
            }
        }
        WHEN( "more capacity is reserved" ) {
            v.reserve( 10 );

            THEN( "the capacity changes but not the size" ) {
                EXPECT( v.size() == 5u );
                EXPECT( v.capacity() >= 10u );
            }
            WHEN( "less capacity is reserved again" ) {
                v.reserve( 7 );

                THEN( "capacity remains unchanged" ) {
                    EXPECT( v.capacity() >= 10u );
                }
            }
        }
        WHEN( "less capacity is reserved" ) {
            v.reserve( 0 );

            THEN( "neither size nor capacity are changed" ) {
                EXPECT( v.size() == 5u );
                EXPECT( v.capacity() >= 5u );
            }
        }
    }
}};

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv );
}

// cl -nologo -W3 -EHsc -I../include 10-bdd.cpp && 10-bdd
// g++ -Wall -Wextra -Wmissing-include-dirs -std=c++11 -I../include -o 10-bdd.exe 10-bdd.cpp && 10-bdd
