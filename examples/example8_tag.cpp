// C++11 - use tags.

#include "lest.hpp"

#define PASS EXPECT( !!"pass" )
#define FAIL EXPECT(  !"fail" )

const lest::test specification[] =
{

CASE("Pass unconditionally "
     "[pass]" )
{
    PASS;
},

CASE("Fail unconditionally "
     "[fail][.]" )
{
    FAIL;
},

CASE("Implement functionality "
     "[implement][.]" )
{
    EXPECT( !"implement" );
},

CASE("Throw unconditionally "
     "[fail][throw][.]" )
{
    EXPECT_THROWS( false );
},

};

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv );
}

// cl -nologo -W3 -EHsc -I.. example8_tag.cpp && example8_tag
// g++ -Wall -Wextra -Wmissing-include-dirs -std=c++11 -I.. -o example8_tag.exe example8_tag.cpp && example8_tag
