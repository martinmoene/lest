// C++11 - auto-register test cases.

#include "lest/lest.hpp"

#define CASE( name ) lest_CASE( specification(), name )

extern lest::tests & specification();

CASE( "A failing test" "[fail]" ) 
{
    EXPECT( 42 == 7 );
}
