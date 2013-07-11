// Copyright 2013 by Martin Moene
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

void test_character()
{
    EXPECT( a() < b() );
    EXPECT( b() < a() );
}

void test_integral()
{
    EXPECT(  x() != y() );
    EXPECT(  x() == y() );
}

void test_real()
{
    EXPECT(  pi() != e() );
    EXPECT(  pi() != pi() );
}

void test_mixed_real_integral()
{
    EXPECT(  pi() != x() );
    EXPECT(  pi() == x() );
}

void test_mixed_integral_real()
{
    EXPECT(  x() != pi() );
    EXPECT(  x() == pi() );
}

void test_boolean()
{
    EXPECT(  t() == t() );
    EXPECT(  t() == f() );
}

void test_pointer()
{
//    EXPECT(  ptr() == NULL );
//    EXPECT(  ptr() != NULL );
}

void test_class_object_pointer()
{
    C a; C b;
    EXPECT(  &a == &a );
    EXPECT(  &a == &b );
}

void test_std_string()
{
    EXPECT( std_world > std_hello );
    EXPECT( std_world < std_hello );
}

void test_C_string()
{
    EXPECT( world > std_hello );
    EXPECT( world < std_hello );
}

void test_single_evaluation()
{
    int n = 0;
    EXPECT( 1 == ++n );
}

void test_duplicate_evaluation()
{
    int n = 0;
    EXPECT( 2 == ++n );
}

const lest::test specification[] =
{
    { "character", test_character, },
    { "integral", test_integral, },
    { "real", test_real, },
    { "mixed real, integral", test_mixed_real_integral, },
    { "mixed integral, real", test_mixed_integral_real, },
    { "boolean", test_boolean, },
    { "pointer", test_pointer, },
    { "class object pointer", test_class_object_pointer, },
    { "std::string", test_std_string, },
    { "C-string", test_C_string, },
    { "single evaluation", test_single_evaluation, },
    { "duplicate evaluation", test_duplicate_evaluation, },
};

int main()
{
    return lest::run( specification );
}

// cl -nologo -W3 -EHsc example4.cpp && example4
// cl -nologo -Wall -EHsc example4.cpp && example4
// g++ -Wall -Wextra -Weffc++ -std=c++11 -o example4.exe example4.cpp && example4
// g++ -Wall -Wextra -Weffc++ -std=c++03 -o example4.exe example4.cpp && example4

//example4.cpp:35: failed: character: b() < a() for 'b' < 'a'
//example4.cpp:41: failed: integral: x() == y() for 3 == 7
//example4.cpp:47: failed: real: pi() != pi() for 3.14 != 3.14
//example4.cpp:53: failed: mixed real, integral: pi() == x() for 3.14 == 3
//example4.cpp:59: failed: mixed integral, real: x() == pi() for 3 == 3.14
//example4.cpp:65: failed: boolean: t() == f() for true == false
//example4.cpp:78: failed: class object pointer: &a == &b for 0x22fddf == 0x22fdde
//example4.cpp:84: failed: std::string: std_world < std_hello for "world" < "hello"
//example4.cpp:90: failed: C-string: world < std_hello for "world" < "hello"
//example4.cpp:102: failed: duplicate evaluation: 2 == ++n for 2 == 1
//10 out of 12 tests failed.
