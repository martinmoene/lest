// C++11 - use BDD style, adapted from Catch tutorial.

#include "lest.hpp"
#include <vector>

#define SCENARIO( sketch ) lest_SCENARIO( specification, sketch )

using text = std::string;

static lest::tests specification;

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
}

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv );
}

// cl -nologo -W3 -Dlest_FEATURE_AUTO_REGISTER=1 -EHsc -I../include/lest 10-bdd-auto.cpp && 10-bdd-auto
// g++ -Wall -Wextra -Wmissing-include-dirs -std=c++11 -Dlest_FEATURE_AUTO_REGISTER=1 -I../include/lest -o 10-bdd-auto.exe 10-bdd-auto.cpp && 10-bdd-auto
