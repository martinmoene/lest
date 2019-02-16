// C++03 - use multiple source files.

#include "lest/lest_cpp03.hpp"

#define CASE( name ) lest_CASE( specification(), name )

extern lest::tests & specification();

CASE( "A failing test" "[fail]" ) 
{
    EXPECT( 42 == 7 );
}
