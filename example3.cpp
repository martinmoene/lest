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
};

int main()
{
    return lest::run( specification );
}

// cl -nologo -Wall -EHsc test_simple.cpp && test_simple
// g++ -Wall -Wextra -Weffc++ -std=c++11 -o test_simple.exe test_simple.cpp && test_simple

//example3.cpp:33: failed: Characters: b() < a() for 'b' < 'a'
//example3.cpp:39: failed: Integral: x() == y() for 3 == 7
//example3.cpp:51: failed: Pointer: ptr() != nullptr for 0 != nullptr
//example3.cpp:58: failed: Class object pointer: &a == &b for 0x22fd97 == 0x22fd96
//example3.cpp:64: failed: std::string: std_world < std_hello for "world" < "hello"
//example3.cpp:70: failed: C-string: world < std_hello for "world" < "hello"
//6 out of 7 tests failed.
