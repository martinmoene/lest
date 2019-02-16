// C++03 - use assertions, observe reports with expression left and right side expanded.

#include "lest/lest_cpp03.hpp"

char a() { return 'a'; }
char b() { return 'b'; }

int  x() { return 3; }
int  y() { return 7; }

double pi() { return 3.14; }
double  e() { return 2.72; }

bool t() { return true; }
bool f() { return false; }

void * ptr() { return 0; }

class C {};

std::string std_hello( "hello" );
std::string std_world( "world" );

char const* hello = "hello";
char const* world = "world";

#define CASE( name ) lest_CASE( specification, name )

lest::test_specification specification;

CASE( "character" )
{
    EXPECT( a() < b() );
    EXPECT( b() < a() );
}

CASE( "integral" )
{
    EXPECT(  x() != y() );
    EXPECT(  x() == y() );
}

CASE( "real" )
{
    EXPECT(  pi() != e() );
    EXPECT(  pi() != pi() );
}

CASE( "mixed real, integral" )
{
    EXPECT(  pi() != x() );
    EXPECT(  pi() == x() );
}

CASE( "mixed integral, real" )
{
    EXPECT(  x() != pi() );
    EXPECT(  x() == pi() );
}

CASE( "boolean")
{
    EXPECT(  t() == t() );
    EXPECT(  t() == f() );
}

CASE( "pointer")
{
#if __cplusplus >= 201103L
    EXPECT(  ptr() == nullptr );
    EXPECT(  ptr() != nullptr );
#else
    EXPECT(  ptr() == (void*)NULL );
    EXPECT(  ptr() != (void*)NULL );
#endif
}

CASE( "class object pointer" )
{
    C a; C b;
    EXPECT(  &a == &a );
    EXPECT(  &a == &b );
}

CASE( "std::string" )
{
    EXPECT( std_world > std_hello );
    EXPECT( std_world < std_hello );
}

CASE( "C-string" )
{
    EXPECT( world > std_hello );
    EXPECT( world < std_hello );
}

CASE( "single evaluation" )
{
    int n = 0;
    EXPECT( 1 == ++n );
}

CASE( "duplicate evaluation" )
{
    int n = 0;
    EXPECT( 2 == ++n );
}

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv );
}

// cl -nologo -W3 -EHsc -I../include 04-c++03.cpp && 04-c++03
// cl -nologo -Wall -EHsc -I../include 04-c++03.cpp && 04-c++03
// g++ -Wall -Wextra -std=c++11 -I../include -o 04-c++03.exe 04-c++03.cpp && 04-c++03
// g++ -Wall -Wextra -std=c++03 -I../include -o 04-c++03.exe 04-c++03.cpp && 04-c++03

// 04-c++03.cpp(34): failed: character: b() < a() for 'b' < 'a'
// 04-c++03.cpp(40): failed: integral: x() == y() for 3 == 7
// 04-c++03.cpp(46): failed: real: pi() != pi() for 3.14 != 3.14
// 04-c++03.cpp(52): failed: mixed real, integral: pi() == x() for 3.14 == 3
// 04-c++03.cpp(58): failed: mixed integral, real: x() == pi() for 3 == 3.14
// 04-c++03.cpp(64): failed: boolean: t() == f() for true == false
// 04-c++03.cpp(74): failed: pointer: ptr() != (void*)NULL for 00000000 != 00000000
// 04-c++03.cpp(82): failed: class object pointer: &a == &b for 0018FCB8 == 0018FCB4
// 04-c++03.cpp(88): failed: std::string: std_world < std_hello for "world" < "hello"
// 04-c++03.cpp(94): failed: C-string: world < std_hello for "world" < "hello"
// 04-c++03.cpp(106): failed: duplicate evaluation: 2 == ++n for 2 == 1
// 11 out of 12 selected tests failed.
