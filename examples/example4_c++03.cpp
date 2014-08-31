// Copyright 2013, 2014 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "lest_cpp03.hpp"

using namespace lest;

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

#define TEST( name ) lest_TEST( specification, name )

test_specification specification;

TEST( "character" )
{
    EXPECT( a() < b() );
    EXPECT( b() < a() );
}

TEST( "integral" )
{
    EXPECT(  x() != y() );
    EXPECT(  x() == y() );
}

TEST( "real" )
{
    EXPECT(  pi() != e() );
    EXPECT(  pi() != pi() );
}

TEST( "mixed real, integral" )
{
    EXPECT(  pi() != x() );
    EXPECT(  pi() == x() );
}

TEST( "mixed integral, real" )
{
    EXPECT(  x() != pi() );
    EXPECT(  x() == pi() );
}

TEST( "boolean")
{
    EXPECT(  t() == t() );
    EXPECT(  t() == f() );
}

TEST( "pointer")
{
#if __cplusplus >= 201103L
    EXPECT(  ptr() == nullptr );
    EXPECT(  ptr() != nullptr );
#else
//    EXPECT(  ptr() == NULL );
//    EXPECT(  ptr() != NULL );
#endif
}

TEST( "class object pointer" )
{
    C a; C b;
    EXPECT(  &a == &a );
    EXPECT(  &a == &b );
}

TEST( "std::string" )
{
    EXPECT( std_world > std_hello );
    EXPECT( std_world < std_hello );
}

TEST( "C-string" )
{
    EXPECT( world > std_hello );
    EXPECT( world < std_hello );
}

TEST( "single evaluation" )
{
    int n = 0;
    EXPECT( 1 == ++n );
}

TEST( "duplicate evaluation" )
{
    int n = 0;
    EXPECT( 2 == ++n );
}

int main()
{   
    return lest::run( specification );
}

// cl -nologo -W3 -EHsc -I.. example4_c++03.cpp && example4_c++03
// cl -nologo -Wall -EHsc -I.. example4_c++03.cpp && example4_c++03
// g++ -Wall -Wextra -std=c++11 -I.. -o example4_c++03.exe example4_c++03.cpp && example4_c++03
// g++ -Wall -Wextra -std=c++03 -I.. -o example4_c++03.exe example4_c++03.cpp && example4_c++03

//example4_c++03.cpp:39: failed: character: b() < a() for 'b' < 'a'
//example4_c++03.cpp:45: failed: integral: x() == y() for 3 == 7
//example4_c++03.cpp:51: failed: real: pi() != pi() for 3.14 != 3.14
//example4_c++03.cpp:57: failed: mixed real, integral: pi() == x() for 3.14 == 3
//example4_c++03.cpp:63: failed: mixed integral, real: x() == pi() for 3 == 3.14
//example4_c++03.cpp:69: failed: boolean: t() == f() for true == false
//example4_c++03.cpp:87: failed: class object pointer: &a == &b for 0x22fddf == 0x22fdde
//example4_c++03.cpp:93: failed: std::string: std_world < std_hello for "world" < "hello"
//example4_c++03.cpp:99: failed: C-string: world < std_hello for "world" < "hello"
//example4_c++03.cpp:111: failed: duplicate evaluation: 2 == ++n for 2 == 1
//10 out of 12 tests failed.
