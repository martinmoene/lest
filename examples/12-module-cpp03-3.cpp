// C++03 - use multiple source files.

#include "lest_cpp03.hpp"

#define CASE( name ) lest_CASE( specifications(), name )

extern lest::tests & specifications();

namespace {

CASE( "A failing test" "[fail]" ) 
{
    EXPECT( 42 == 7 );
}

}
