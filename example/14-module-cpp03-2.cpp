// C++03 - use multiple source files.

#include "lest_cpp03.hpp"

#define CASE( name ) lest_CASE( specification(), name )

extern lest::tests & specification();

namespace {

CASE( "A passing test" "[pass]" ) 
{
    EXPECT( 42 == 42 );
}

}
