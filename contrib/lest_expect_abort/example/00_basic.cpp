#include "lest_expect_abort.hpp"
#include <cassert>

#define CASE( name ) lest_CASE( specification, name )

static lest::tests specification;

CASE( "Expect_aborts succeeds for assert(false) " "[pass]" )
{
    EXPECT_ABORTS( assert( false ) );
}

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv /*, std::cout */ );
}

#if 0
// VC14 (VS2015)
cl -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I../ -I../../../ 00_basic.cpp && 00_basic.exe --pass
// pre-VC14
cl -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I../ -I../../../ 00_basic.cpp /link /FORCE:MULTIPLE && 00_basic.exe --pass

g++ -Wall -std=c++11 -Dlest_FEATURE_AUTO_REGISTER=1 -I../ -I../../../ -o 00_basic.exe 00_basic.cpp && 00_basic.exe --pass
#endif
