// C++11 - auto-register test cases.

#include "lest.hpp"

#define CASE( name ) lest_CASE( specification(), name )

extern lest::tests & specification();

namespace {

CASE( "A failing test" "[fail]" ) 
{
    EXPECT( 42 == 7 );
}

}
