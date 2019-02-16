// C++11 - use multiple source files.

#include "lest/lest.hpp"

const lest::test module[] = {

    CASE( "A failing test" "[fail]" ) 
    {
        EXPECT( 42 == 7 );
    }
};

extern lest::tests & specification();

MODULE( specification(), module )
