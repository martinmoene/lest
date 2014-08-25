// Copyright 2013 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "lest_decompose.hpp"

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

char const* hello( "hello" );
char const* world( "world" );

const lest::test specification[] =
{
    "character", []
    {
        EXPECT( a() < b() );
        EXPECT( b() < a() );
    },

    "integral", []
    {
        EXPECT(  x() != y() );
        EXPECT(  x() == y() );
    },

    "real", []
    {
        EXPECT(  pi() != e() );
        EXPECT(  pi() != pi() );
    },

    "mixed real, integral", []
    {
        EXPECT(  pi() != x() );
        EXPECT(  pi() == x() );
    },

    "mixed integral, real", []
    {
        EXPECT(  x() != pi() );
        EXPECT(  x() == pi() );
    },

    "boolean", []
    {
        EXPECT(  t() == t() );
        EXPECT(  t() == f() );
    },

    "pointer", []
    {
        EXPECT(  ptr() == nullptr );
        EXPECT(  ptr() != nullptr );
    },

    "class object pointer", []
    {
        C a; C b;
        EXPECT(  &a == &a );
        EXPECT(  &a == &b );
    },

    "std::string", []
    {
        EXPECT( std_world > std_hello );
        EXPECT( std_world < std_hello );
    },

    "C-string", []
    {
        EXPECT( world > std_hello );
        EXPECT( world < std_hello );
    },

    "single evaluation", []
    {
        int n = 0;
        EXPECT( 1 == ++n );
    },

    "duplicate evaluation", []
    {
        int n = 0;
        EXPECT( 2 == ++n );
    },
};

int main()
{
    return lest::run( specification );
}

// cl -nologo -Wall -EHsc -I.. example3_decompose.cpp && example3_decompose
// g++ -Wall -Wextra -std=c++11 -I.. -o example3_decompose.exe example3_decompose.cpp && example3_decompose

//example3_decompose.cpp:37: failed: character: b() < a() for 'b' < 'a'
//example3_decompose.cpp:43: failed: integral: x() == y() for 3 == 7
//example3_decompose.cpp:49: failed: real: pi() != pi() for 3.14 != 3.14
//example3_decompose.cpp:55: failed: mixed real, integral: pi() == x() for 3.14 == 3
//example3_decompose.cpp:61: failed: mixed integral, real: x() == pi() for 3 == 3.14
//example3_decompose.cpp:67: failed: boolean: t() == f() for true == false
//example3_decompose.cpp:73: failed: pointer: ptr() != nullptr for 0 != nullptr
//example3_decompose.cpp:80: failed: class object pointer: &a == &b for 0x22fd8f == 0x22fd8e
//example3_decompose.cpp:86: failed: std::string: std_world < std_hello for "world" < "hello"
//example3_decompose.cpp:92: failed: C-string: world < std_hello for "world" < "hello"
//example3_decompose.cpp:104: failed: duplicate evaluation: 2 == ++n for 2 == 1
//11 out of 12 tests failed.
