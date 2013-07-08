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
    "character", []()
    {
        EXPECT( a() < b() );
        EXPECT( b() < a() );
    },

    "integral", []()
    {
        EXPECT(  x() != y() );
        EXPECT(  x() == y() );
    },

    "boolean", []()
    {
        EXPECT(  t() == t() );
        EXPECT(  t() == f() );
    },

    "pointer", []()
    {
        EXPECT(  ptr() == nullptr );
        EXPECT(  ptr() != nullptr );
    },

    "class object pointer", []()
    {
        C a; C b;
        EXPECT(  &a == &a );
        EXPECT(  &a == &b );
    },

    "std::string", []()
    {
        EXPECT( std_world > std_hello );
        EXPECT( std_world < std_hello );
    },

    "C-string", []()
    {
        EXPECT( world > std_hello );
        EXPECT( world < std_hello );
    },

    "single evaluation", []()
    {
        int n = 0;
        EXPECT( 1 == ++n );
    },

    "duplicate evaluation", []()
    {
        int n = 0;
        EXPECT( 2 == ++n );
    },
};

int main()
{
    return lest::run( specification );
}

// cl -nologo -Wall -EHsc example3.cpp && example3
// g++ -Wall -Wextra -Weffc++ -std=c++11 -o example3.exe example3.cpp && example3

//example3.cpp:35: failed: character: b() < a() for 'b' < 'a'
//example3.cpp:41: failed: integral: x() == y() for 3 == 7
//example3.cpp:47: failed: boolean: t() == f() for true == false
//example3.cpp:53: failed: pointer: ptr() != nullptr for 0 != nullptr
//example3.cpp:60: failed: class object pointer: &a == &b for 0x22fd8f == 0x22fd8e
//example3.cpp:66: failed: std::string: std_world < std_hello for "world" < "hello"
//example3.cpp:72: failed: C-string: world < std_hello for "world" < "hello"
//example3.cpp:84: failed: duplicate evaluation: 2 == ++n for 2 == 1
//8 out of 9 tests failed.
