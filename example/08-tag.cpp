// C++11 - use tags.

#include "lest/lest.hpp"

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

// cl -nologo -W3 -EHsc -I../include 08-tag.cpp && 08-tag
// g++ -Wall -Wextra -Wmissing-include-dirs -std=c++11 -I../include -o 08-tag.exe 08-tag.cpp && 08-tag
