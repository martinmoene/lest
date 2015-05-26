// C++11 - auto-register test cases.

#include "lest.hpp"

#define CASE( name ) lest_CASE( specification(), name )

extern lest::tests & specification();

namespace {

CASE( "A passing test" "[pass]" ) 
{
    EXPECT( 42 == 42 );
}

}
