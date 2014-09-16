// Copyright 2013, 2014 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "lest_cpp03.hpp"
#include <string>

#ifdef lest_COMPILER_IS_MSVC6
namespace std { using ::size_t; }
#endif

lest::test_specification no_using_namespace_lest;

lest_TEST( no_using_namespace_lest, "Ensure namespace lest is specified correctly in lest_cpp03.hpp [compile-only]" )
{
    EXPECT(  true );
    EXPECT_NOT(  false );
    EXPECT_THROWS( true );
    EXPECT_THROWS_AS( true, std::exception );
}

#define TEST( name ) lest_TEST( specification, name )

using namespace lest;

test_specification specification;

TEST( "Function to suppress warning \"expression has no effect\" acts as identity function" )
{
    EXPECT( false == is_true( false ) );
    EXPECT(  true == is_true( true  ) );
}

TEST( "Function with_message() returns correct string" )
{
    std::string with = "with message \"" ;
    std::string msg = "Let writing tests become irresistibly easy and attractive.";
    EXPECT( ( with + msg + "\"" ) == with_message( msg ) );
}

TEST( "Function of_type() returns correct string" )
{
    std::string msg = "this_type";
    EXPECT( ( "of type " + msg ) == of_type( msg ) );
}

TEST( "Function pluralise() adds 's' except for 1 item" )
{
    std::string word = "hammer";
    EXPECT( word == pluralise( 1, word ) );
    int range[] = {0,2,3,4,5,6,7,8,9,10,11,12};
    for ( int * pos = range; pos != range + lest_DIMENSION_OF(range); ++pos )
        EXPECT( ( word + "s" ) == pluralise( *pos, word ) );
}

TEST( "Location constructs properly" )
{
    char const * file = __FILE__; int line = __LINE__;
    location where( file, line );
    EXPECT( file == where.file );
    EXPECT( line == where.line );
}

TEST( "Comment constructs properly" )
{
    std::string info = __FILE__;
    comment note = info;
    EXPECT( info == note.info );
}

TEST( "Comment converted to bool indicates absence or presence of comment" )
{
    EXPECT( false == bool( comment( "") ) );
    EXPECT(  true == bool( comment("x") ) );
}

TEST( "Failure exception type constructs and prints properly" )
{
    std::string name = "test-name";
    failure msg( location("filename.cpp", 765), "expression", "decomposition" );

    std::ostringstream os;
    report( os, msg, name );

#ifndef __GNUG__
    EXPECT( os.str() == "filename.cpp(765): failed: test-name: expression for decomposition\n" );
#else
    EXPECT( os.str() == "filename.cpp:765: failed: test-name: expression for decomposition\n" );
#endif
}

TEST( "Expected exception type constructs and prints properly" )
{
    std::string name = "test-name";
    expected msg( location("filename.cpp", 765), "expression" );

    std::ostringstream os;
    report( os, msg, name );

#ifndef __GNUG__
    EXPECT( os.str() == "filename.cpp(765): failed: didn't get exception: test-name: expression\n" );
#else
    EXPECT( os.str() == "filename.cpp:765: failed: didn't get exception: test-name: expression\n" );
#endif
}

TEST( "Unexpected exception type constructs and prints properly" )
{
    std::string name = "test-name";
    lest::unexpected msg( location("filename.cpp", 765), "expression", "exception-type" );

    std::ostringstream os;
    report( os, msg, name );

#ifndef __GNUG__
    EXPECT( os.str() == "filename.cpp(765): failed: got unexpected exception exception-type: test-name: expression\n" );
#else
    EXPECT( os.str() == "filename.cpp:765: failed: got unexpected exception exception-type: test-name: expression\n" );
#endif
}

void pass_test_01() { EXPECT( true ); }
void fail_test_01() { EXPECT( false ); }

TEST( "Expect generates no message exception for a succeeding test" )
{
    test pass( "P", pass_test_01 );

    try { pass.behaviour(); }
    catch(...) { throw failure(location(__FILE__,__LINE__), "unexpected error generated", "true"); }
}

TEST( "Expect generates a message exception for a failing test" )
{
    test fail( "F", fail_test_01 );

    for (;;)
    {
        try { fail.behaviour(); } catch ( message & ) { break; }
        throw failure(location(__FILE__,__LINE__), "no error generated", "false");
    }
}

TEST( "Expect succeeds for success (true) and failure (false)" )
{
    test pass[] = { test( "P", pass_test_01 ) };
    test fail[] = { test( "F", fail_test_01 ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );
}

TEST( "Expect succeeds for integer comparation" )
{
    EXPECT( 7 == 7 );
    EXPECT( 7 != 8 );
    EXPECT( 7 >= 6 );
    EXPECT( 7 <= 8 );
    EXPECT( 7 >  6 );
    EXPECT( 7 <  8 );

    EXPECT_NOT( 7 == 8 );
    EXPECT_NOT( 7 != 7 );
    EXPECT_NOT( 7 <= 6 );
    EXPECT_NOT( 7 >= 8 );
    EXPECT_NOT( 7 <  6 );
    EXPECT_NOT( 7 >  8 );
}

TEST( "Expect succeeds for mixed integer, real comparation" )
{
    EXPECT( 7.0 == 7   );
    EXPECT( 7.0 != 8   );
    EXPECT( 7   == 7.0 );
    EXPECT( 7   != 8.0 );

    EXPECT_NOT( 7.0 == 8   );
    EXPECT_NOT( 7  !=  7.0 );
}

TEST( "Expect succeeds for string comparation" )
{
    std::string a("a"); std::string b("b");

    EXPECT( a == a ); EXPECT( a != b );
    EXPECT( b >= a ); EXPECT( a <= b );
    EXPECT( b >  a ); EXPECT( a <  b );

    EXPECT_NOT( a == b );
    EXPECT_NOT( a != a );
    EXPECT_NOT( b <= a );
    EXPECT_NOT( a >= b );
    EXPECT_NOT( b <  a );
    EXPECT_NOT( a >  b );
}

void pass_test_41() { EXPECT( 1==1 ); }
void fail_test_41() { EXPECT( 0==1 ); }

TEST( "Function run() returns the right failure count" )
{
    test pass  [] = { test( "P" , pass_test_41 ) };
    test fail_1[] = { test( "F1", fail_test_41 ) };
    test fail_3[] = { test( "F1", fail_test_41 ),
                      test( "F2", fail_test_41 ),
                      test( "F3", fail_test_41 ),};

    std::ostringstream os;

    EXPECT( 0 == run( pass  , os ) );
    EXPECT( 1 == run( fail_1, os ) );
    EXPECT( 3 == run( fail_3, os ) );
}

std::string std_hello_world = "hello-world";

void pass_test_51() { EXPECT( (throw std::runtime_error(std_hello_world), true) ); }

TEST( "Expect succeeds with an unexpected standard exception" )
{
    test pass[] = { test( "P", pass_test_51 ) };

    std::ostringstream os;

    EXPECT( 1 == run( pass, os ) );
    EXPECT( std::string::npos != os.str().find(std_hello_world) );
}

void pass_test_61() { EXPECT( (throw 77, true) ); }

TEST( "Expect succeeds with an unexpected non-standard exception" )
{
    test pass[] = { test( "P", pass_test_61 ) };

    std::ostringstream os;

    EXPECT( 1 == run( pass, os ) );
}

void pass_test_71() { EXPECT_THROWS( (throw std::runtime_error(std_hello_world), true) ); }
void fail_test_71() { EXPECT_THROWS(  true ); }

TEST( "Expect_throws succeeds with an expected standard exception" )
{
    test pass[] = { test( "P", pass_test_71 ) };
    test fail[] = { test( "F", fail_test_71 ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );
}

void pass_test_81() { EXPECT_THROWS( (throw 77, true) ); }
void fail_test_81() { EXPECT_THROWS(  true ); }

TEST( "Expect_throws succeeds with an expected non-standard exception" )
{
    test pass[] = { test( "P", pass_test_81 ) };
    test fail[] = { test( "F", fail_test_81 ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );
}

void pass_test_91() { EXPECT_THROWS_AS( (throw std::bad_alloc(), true), std::bad_alloc ); }
void fail_test_91() { EXPECT_THROWS_AS( (throw std::bad_alloc(), true), std::runtime_error ); }

TEST( "Expect_throws_as succeeds with a specific expected standard exception" )
{
    test pass[] = { test( "P", pass_test_91 ) };
    test fail[] = { test( "F", fail_test_91 ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );
}

void pass_test_a1() { EXPECT_THROWS_AS( (throw 77, true), int ); }
void fail_test_a1() { EXPECT_THROWS_AS( (throw 77, true), std::runtime_error ); }

TEST( "Expect_throws_as succeeds with a specific expected non-standard exception" )
{
    test pass[] = { test( "P", pass_test_a1 ) };
    test fail[] = { test( "F", fail_test_a1 ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );
}

#if __cplusplus >= 201103L
void pass_test_b1() { EXPECT(  nullptr == nullptr  ); }
void fail_test_b1() { EXPECT( (void*)1 == nullptr  ); }

TEST( "Decomposition formats nullptr as string" )
{
    test pass[] = { test( "P", pass_test_b1 ) };
    test fail[] = { test( "F", fail_test_b1 ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );

    EXPECT( std::string::npos != os.str().find( "(void*)1 == nullptr for 0x1 == nullptr" ) );
}
#endif

void pass_test_c1() { EXPECT( true == true  ); }
void fail_test_c1() { EXPECT( true == false ); }

TEST( "Decomposition formats boolean as strings true and false" )
{
    test pass[] = { test( "P", pass_test_c1 ) };
    test fail[] = { test( "F", fail_test_c1 ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );

    EXPECT( std::string::npos != os.str().find( "true == false for true == false" ) );
}

void pass_test_d1() { EXPECT( 'a' < 'b' ); }
void fail_test_d1() { EXPECT( 'b' < 'a' ); }

TEST( "Decomposition formats character with single quotes" )
{
    test pass[] = { test( "P", pass_test_d1 ) };
    test fail[] = { test( "F", fail_test_d1 ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );

    EXPECT( std::string::npos != os.str().find( "'b' < 'a' for 'b' < 'a'" ) );
}

std::string std_hello( "hello" );
std::string std_world( "world" );

void pass_test_e1() { EXPECT( std_hello < "world" ); }
void fail_test_e1() { EXPECT( std_world < "hello" ); }

TEST( "Decomposition formats std::string with double quotes" )
{
    test pass[] = { test( "P", pass_test_e1 ) };
    test fail[] = { test( "F", fail_test_e1 ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );

    // lifted from assertion for VC6:
    std::size_t pos = os.str().find( "world < \"hello\" for \"world\" < \"hello\"" );
    EXPECT( std::string::npos != pos );
}

char const * hello = "hello";
char const * world = "world";

void pass_test_f1() { EXPECT( hello < std_world ); }
void fail_test_f1() { EXPECT( world < std_hello ); }

TEST( "Decomposition formats C string with double quotes" )
{
    test pass[] = { test( "P", pass_test_f1 ) };
    test fail[] = { test( "F", fail_test_f1 ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );

    // lifted from assertion for VC6:
    std::size_t pos = os.str().find( "world < std_hello for \"world\" < \"hello\"" );
    EXPECT( std::string::npos != pos );
}

void pass_test_g1() { int n = 0; EXPECT( 1 == ++n ); }
void fail_test_g1() { int n = 0; EXPECT( 2 == ++n ); }

TEST( "Has single expression evaluation" )
{
    test pass[] = { test( "P", pass_test_g1 ) };
    test fail[] = { test( "F", fail_test_g1 ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );

    EXPECT( std::string::npos != os.str().find( "for 2 == 1" ) );
}

TEST( "Approximate compares properly [approx][basic]" )
{
    EXPECT( 1.23 == approx( 1.23 ) );
    EXPECT( 1.23 != approx( 1.24 ) );
}

TEST( "Approximate using epsilon compares properly [approx][epsilon]" )
{
    EXPECT( 1.23 != approx( 1.231 ) );
    EXPECT( 1.23 == approx( 1.231 ).epsilon( 0.1 ) );
}

TEST( "Approximate using custom epsiloncompares properly [approx][custom]" )
{
    approx custom = approx::custom().epsilon( 0.1 );

    EXPECT( approx( 1.231 ) != 1.23 );
    EXPECT( custom( 1.231 ) == 1.23 );
}

double divide( double a, double b ) { return a / b; }

TEST( "Approximate to Pi compares properly [approx][pi]" )
{
    EXPECT( divide( 22, 7 ) == approx( 3.141 ).epsilon( 0.001  ) );
    EXPECT( divide( 22, 7 ) != approx( 3.141 ).epsilon( 0.0001 ) );
}

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv );
}

// cl -nologo -W3 -EHsc -I.. test_lest_cpp03.cpp && test_lest_cpp03
// cl -nologo -Wall -EHsc -I.. test_lest_cpp03.cpp && test_lest_cpp03
// g++ -Wall -Wextra -std=c++11 -I.. -o test_lest_cpp03.exe test_lest_cpp03.cpp && test_lest_cpp03
// g++ -Wall -Wextra -std=c++03 -I.. -o test_lest_cpp03.exe test_lest_cpp03.cpp && test_lest_cpp03
