// Copyright 2013-2018 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Suppress warning: comparing floating point with == or != is unsafe [-Werror=float-equal]
// for CASE( "Expect succeeds for mixed integer, real comparison" ):

#ifdef __clang__
# pragma clang diagnostic ignored "-Wfloat-equal"
#elif defined __GNUC__
# pragma GCC   diagnostic ignored "-Wfloat-equal"
#endif

#include "lest/lest_cpp03.hpp"
#include <string>

// Suppress:
// - unused parameter, for cases without assertions such as [.std...]
#ifdef __clang__
# pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined __GNUC__
# pragma GCC   diagnostic ignored "-Wunused-parameter"
#endif

lest::test_specification no_using_namespace_lest;

struct S { void f(){} };

lest_CASE( no_using_namespace_lest, "Namespace lest is specified correctly in lest_cpp03.hpp [compile-only]" )
{
    EXPECT(  true );
    EXPECT_NOT(  false );
    EXPECT_NO_THROW( true );
    EXPECT_THROWS( true );
    EXPECT_THROWS_AS( true, std::exception );
}

#define CASE( name ) lest_CASE( specification, name )

using namespace lest;

test_specification specification;

CASE( "Function to suppress warning \"expression has no effect\" acts as identity function" )
{
    EXPECT( false == is_true( false ) );
    EXPECT(  true == is_true( true  ) );
}

CASE( "Function with_message() returns correct string" )
{
    std::string with = "with message \"" ;
    std::string msg = "Let writing tests become irresistibly easy and attractive.";
    EXPECT( ( with + msg + "\"" ) == with_message( msg ) );
}

CASE( "Function of_type() returns correct string" )
{
    std::string msg = "this_type";
    EXPECT( ( "of type " + msg ) == of_type( msg ) );
}

CASE( "Function pluralise() adds 's' except for 1 item" )
{
    std::string word = "hammer";
    EXPECT( word == pluralise( word, 1 ) );
    int range[] = {0,2,3,4,5,6,7,8,9,10,11,12};
    for ( int * pos = range; pos != range + lest_DIMENSION_OF(range); ++pos )
        EXPECT( ( word + "s" ) == pluralise( word, *pos ) );
}

CASE( "Location constructs properly" )
{
    char const * file = __FILE__; int line = __LINE__;
    location where( file, line );
    EXPECT( file == where.file );
    EXPECT( line == where.line );
}

CASE( "Comment constructs properly" )
{
    std::string info = __FILE__;
    comment note = info;
    EXPECT( info == note.info );
}

CASE( "Comment converted to bool indicates absence or presence of comment" )
{
    EXPECT( false == bool( comment( "") ) );
    EXPECT(  true == bool( comment("x") ) );
}

CASE( "Failure exception type constructs and prints properly" )
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

CASE( "Expected exception type constructs and prints properly" )
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

CASE( "Unexpected exception type constructs and prints properly" )
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

CASE( "Expect generates no message exception for a succeeding test" )
{
    struct f { static void pass(env & lest_env) { EXPECT(true); } };

    test pass( "P", f::pass );

    try { pass.behaviour( lest_env ); }
    catch(...) { throw failure(location(__FILE__,__LINE__), "unexpected error generated", "true"); }
}

CASE( "Expect generates a message exception for a failing test" )
{
    struct f { static void fail(env & lest_env) { EXPECT(false); } };

    test fail( "F", f::fail );

    for (;;)
    {
        try { fail.behaviour( lest_env ); } catch ( message & ) { break; }
        throw failure(location(__FILE__,__LINE__), "no error generated", "false");
    }
}

CASE( "Expect succeeds for success (true) and failure (false)" )
{
    struct f { static void pass(env & lest_env) { EXPECT(true);}
               static void fail(env & lest_env) { EXPECT(false);} };

    test pass[] = { test( "P", f::pass ) };
    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );
}

CASE( "Expect succeeds for integer comparison" )
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

CASE( "Expect succeeds for mixed integer, real comparison" )
{
    EXPECT( 7.0 == 7   );
    EXPECT( 7.0 != 8   );
    EXPECT( 7   == 7.0 );
    EXPECT( 7   != 8.0 );

    EXPECT_NOT( 7.0 == 8   );
    EXPECT_NOT( 7  !=  7.0 );
}

CASE( "Expect succeeds for string comparison" )
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

namespace {

struct logic_t
{
    int value;

    logic_t( int v = 0 ) : value( v ) {}

    logic_t operator==( logic_t rhs ) const { return value == rhs.value; }

#if lest_CPP11_OR_GREATER
    explicit operator bool() const { return value != 0; }
#else
    /*explicit*/ operator bool() const { return value != 0; }
#endif
};

std::ostream & operator<<( std::ostream & os, logic_t x )
{
    return os << "[logic_t:" << x.value << "]";
}
} // anonymous namepace

CASE( "Expect succeeds for comparison that yields user-defined type that (explicitly) converts to bool" )
{
    EXPECT(     logic_t( 7 ) == logic_t(  7 ) );
    EXPECT_NOT( logic_t( 7 ) == logic_t( 42 ) );
}

CASE( "Expect expression RHS can use * / % + -" )
{
    EXPECT( 7 == 1 * 7 );
    EXPECT( 7 == 7 / 1 );
    EXPECT( 0 == 7 % 1 );
    EXPECT( 7 == 1 + 6 );
    EXPECT( 7 == 8 - 1 );
}

CASE( "Expect expression LHS can use * / % + -" )
{
    EXPECT( 1 * 7 == 7 );
    EXPECT( 7 / 1 == 7 );
    EXPECT( 7 % 1 == 0 );
    EXPECT( 1 + 6 == 7 );
    EXPECT( 8 - 1 == 7 );
}

CASE( "Function run() returns the right failure count" )
{
    struct f { static void pass(env & lest_env) { EXPECT( 1==1 ); }
               static void fail(env & lest_env) { EXPECT( 0==1 ); }};

    test pass  [] = { test( "P" , f::pass ) };
    test fail_1[] = { test( "F1", f::fail ) };
    test fail_3[] = { test( "F1", f::fail ),
                      test( "F2", f::fail ),
                      test( "F3", f::fail ),};

    std::ostringstream os;

    EXPECT( 0 == run( pass  , os ) );
    EXPECT( 1 == run( fail_1, os ) );
    EXPECT( 3 == run( fail_3, os ) );
}

std::string std_hello_world = "hello-world";

CASE( "Expect reports an unexpected standard exception" )
{
    struct f { static void fail(env & lest_env) { EXPECT( (throw std::runtime_error(std_hello_world), true) ); }};

    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 1 == run( fail, os ) );
    EXPECT( std::string::npos != os.str().find(std_hello_world) );
}

CASE( "Expect reports an unexpected non-standard exception" )
{
    struct f { static void fail(env & lest_env) { EXPECT( (throw 77, true) ); }};

    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 1 == run( fail, os ) );
}

CASE( "Expect_no_throw succeeds without an exception" )
{
    struct f { static void pass(env & lest_env) { EXPECT_NO_THROW( true ); }};

    test pass[] = { test( "P", f::pass ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
}

CASE( "Expect_no_throw reports a standard exception" )
{
    struct f { static void fail(env & lest_env) { EXPECT_NO_THROW( throw std::runtime_error(std_hello_world) ); }};

    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 1 == run( fail, os ) );
    EXPECT( std::string::npos != os.str().find(std_hello_world) );
}

CASE( "Expect_no_throw reports a non-standard exception" )
{
    struct f { static void fail(env & lest_env) { EXPECT_NO_THROW( (throw 77, true) ); }};

    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 1 == run( fail, os ) );
}

CASE( "Expect_throws reports a missing exception" )
{
    struct f { static void fail(env & lest_env) { EXPECT_THROWS( true ); }};

    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 1 == run( fail, os ) );
}

CASE( "Expect_throws succeeds with a standard exception" )
{
    struct f { static void pass(env & lest_env) { EXPECT_THROWS( throw std::runtime_error(std_hello_world) ); }};

    test pass[] = { test( "P", f::pass ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
}

CASE( "Expect_throws succeeds with a non-standard exception" )
{
    struct f { static void pass(env & lest_env) { EXPECT_THROWS( throw 77 ); }};

    test pass[] = { test( "P", f::pass ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
}

CASE( "Expect_throws_as reports a missing exception" )
{
    struct f { static void fail(env & lest_env) { EXPECT_THROWS_AS( true, std::runtime_error ); }};

    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 1 == run( fail, os ) );
}

CASE( "Expect_throws_as reports getting a different exception" )
{
    struct f { static void fail(env & lest_env) { EXPECT_THROWS_AS( throw std::bad_alloc(), std::runtime_error ); }};

    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 1 == run( fail, os ) );
}

CASE( "Expect_throws_as succeeds with a specific standard exception" )
{
    struct f { static void pass(env & lest_env) { EXPECT_THROWS_AS( throw std::bad_alloc(), std::bad_alloc ); }};

    test pass[] = { test( "P", f::pass ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
}

CASE( "Expect_throws_as succeeds with a specific non-standard exception" )
{
    struct f { static void pass(env & lest_env) { EXPECT_THROWS_AS( throw 77, int ); }};

    test pass[] = { test( "P", f::pass ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
}

CASE( "Setup creates a fresh fixture for each section" )
{
    SETUP("Context") {
        int i = 7;

        SECTION("S1") {         i = 42;   }
        SECTION("S2") { EXPECT( i == 7 ); }
    }
}

CASE( "Setup runs as many times as there are sections" )
{
    int i = 0;

    SETUP("Context") {
        ++i;

        SECTION("S1") { }
        SECTION("S2") { }
    }
    EXPECT( i == 2 );
}

#if lest_CPP11_OR_GREATER || lest_COMPILER_MSVC_VERSION

// GNUC -std=c++03: error: ISO C++ forbids comparison between pointer and integer [-fpermissive]

CASE( "Decomposition formats nullptr or NULL as 'nullptr' or 'NULL'" )
{
    struct f { static void pass(env & lest_env) { EXPECT( lest_nullptr == lest_nullptr ); }
               static void fail(env & lest_env) { EXPECT( lest_nullptr == reinterpret_cast<void*>(1) ); }};

    test pass[] = { test( "P", f::pass ) };
    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );

    EXPECT( std::string::npos != os.str().find( lest_STRING(lest_nullptr) " == 0x000" /*...1*/ ) );
}
#endif

void *p = reinterpret_cast<void*>( 0x123 );

CASE( "Decomposition formats a pointer as hexadecimal number" )
{
    struct f { static void pass(env & lest_env) { EXPECT( p == p ); }
               static void fail(env & lest_env) { EXPECT( p != p ); }};

    test pass[] = { test( "P", f::pass ) };
    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );

    EXPECT( std::string::npos != os.str().find( "123 != 0x" ) );
}

void (S::*q)() = &S::f;

CASE( "Decomposition formats a member function pointer as hexadecimal number" )
{
    struct f { static void pass(env & lest_env) { EXPECT( q == q ); }
               static void fail(env & lest_env) { EXPECT( q != q ); }};

    test pass[] = { test( "P", f::pass ) };
    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );

    EXPECT( std::string::npos != os.str().find( "0x" ) );
    EXPECT( std::string::npos != os.str().find( "!=" ) );
}

CASE( "Decomposition formats boolean as strings 'true' and 'false'" )
{
    struct f { static void pass(env & lest_env) { EXPECT( true == true  ); }
               static void fail(env & lest_env) { EXPECT( true == false ); }};

    test pass[] = { test( "P", f::pass ) };
    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );

    EXPECT( std::string::npos != os.str().find( "true == false for true == false" ) );
}

CASE( "Decomposition formats character with single quotes" )
{
    struct f { static void pass(env & lest_env) { EXPECT( 'a' < 'b' ); }
               static void fail(env & lest_env) { EXPECT( 'b' < 'a' ); }};

    test pass[] = { test( "P", f::pass ) };
    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );

    EXPECT( std::string::npos != os.str().find( "'b' < 'a' for 'b' < 'a'" ) );
}

CASE( "Decomposition formats unprintable characters as number" )
{
    struct f { static void fail(env & lest_env) { EXPECT( '\x8' > '\t' ); }};

    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 1 == run( fail, os ) );

    EXPECT( std::string::npos != os.str().find( "'\\x8' > '\\t' for '\\x08' > '\\t'" ) );
}

std::string std_hello( "hello" );
std::string std_world( "world" );
char const  *  hello = "hello";
char const  *  world = "world";

CASE( "Decomposition formats std::string with double quotes" )
{
    struct f { static void pass(env & lest_env) { EXPECT( std_hello < "world" ); }
               static void fail(env & lest_env) { EXPECT( std_world < "hello" ); }};

    test pass[] = { test( "P", f::pass ) };
    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );

    // lifted from assertion for VC6:
    std::size_t pos = os.str().find( "std_world < \"hello\" for \"world\" < \"hello\"" );
    EXPECT( std::string::npos != pos );
}

CASE( "Decomposition formats C string with double quotes" )
{
    struct f { static void pass(env & lest_env) { EXPECT( hello < std_world ); }
               static void fail(env & lest_env) { EXPECT( world < std_hello ); }};

    test pass[] = { test( "P", f::pass ) };
    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );

    // lifted from assertion for VC6:
    std::size_t pos = os.str().find( "world < std_hello for \"world\" < \"hello\"" );
    EXPECT( std::string::npos != pos );
}

CASE( "Decomposition formats a pair with elements between curly braces" )
{
    EXPECT( "{ 42, 3.14 }" == lest::to_string( std::make_pair( 42, 3.14 ) ) );
}

#if lest_CPP11_OR_GREATER

CASE( "Decomposition formats a tuple with elements between curly braces" )
{
    typedef std::tuple<> type;

    EXPECT( "{ }" == lest::to_string( type{} ) );
    EXPECT( "{ 'a', 42, 3.14, \"hello world\" }" == lest::to_string( std::make_tuple( 'a', 42, 3.14, "hello world" ) ) );
}
#endif

CASE( "Has single expression evaluation" )
{
    struct f { static void pass(env & lest_env) { int n = 0; EXPECT( 1 == ++n ); }
               static void fail(env & lest_env) { int n = 0; EXPECT( 2 == ++n ); }};

    test pass[] = { test( "P", f::pass ) };
    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );

    EXPECT( std::string::npos != os.str().find( "for 2 == 1" ) );
}

CASE( "Approximate compares properly [approx][basic]" )
{
    EXPECT( 1.23 == approx( 1.23 ) );
    EXPECT( 1.23 <= approx( 1.23 ) );
    EXPECT( 1.23 >= approx( 1.23 ) );
    EXPECT( 1.23 != approx( 1.24 ) );

    EXPECT_NOT( 1.24 <= approx( 1.23 ) );
    EXPECT_NOT( 1.23 >= approx( 1.24 ) );
}

CASE( "Approximate using epsilon compares properly [approx][epsilon]" )
{
    EXPECT( 1.23  != approx( 1.231 ) );
    EXPECT( 1.231 >= approx( 1.23  ) );
    EXPECT( 1.23  == approx( 1.231 ).epsilon( 0.1 ) );
    EXPECT( 1.23  <= approx( 1.231 ).epsilon( 0.1 ) );
}

CASE( "Approximate using custom epsilon compares properly [approx][custom]" )
{
    approx custom = approx::custom().epsilon( 0.1 );

    EXPECT( approx( 1.231 ) != 1.23 );
    EXPECT( custom( 1.231 ) == 1.23 );
}

CASE( "Approximate to Pi compares properly [approx][pi]" )
{
    struct f { static double divide( double a, double b ) { return a / b; }};

    EXPECT( f::divide( 22, 7 ) == approx( 3.141 ).epsilon( 0.001  ) );
    EXPECT( f::divide( 22, 7 ) != approx( 3.141 ).epsilon( 0.0001 ) );
}

CASE( "Skips tests tagged [hide]" )
{
    EXPECT( false );
}

CASE( "Skips tests tagged [.]" )
{
    EXPECT( false );
}

CASE( "Skips tests with tags that start with [.followed by anything]" )
{
    EXPECT( false );
}

#if lest_FEATURE_REGEX_SEARCH

CASE( "Test specifications select tests [commandline]" )
{
    struct f { static void fail(env & lest_env) { EXPECT( false ); }};

    test fail[] = { test( "Hello world [tag1]" , f::fail ),
                    test( "Good morning [tag1]", f::fail ),
                    test( "Good noon [tag2]"   , f::fail ),
                    test( "Good bye tags"      , f::fail )};

    std::ostringstream os;
    char const * args1[] = { "world"    };
    char const * args2[] = { "1\\]"     };
    char const * args3[] = { "\\[.*\\]" };

    EXPECT( 1 == run( fail, make_texts( args1 ), os ) );
    EXPECT( 2 == run( fail, make_texts( args2 ), os ) );
    EXPECT( 3 == run( fail, make_texts( args3 ), os ) );
}

CASE( "Test specifications omit tests [commandline]" )
{
    struct f { static void fail(env & lest_env) { EXPECT( false ); }};

    test fail[] = { test( "Hello world [tag1]" , f::fail ),
                    test( "Good morning [tag1]", f::fail ),
                    test( "Good noon [tag2]"   , f::fail ),
                    test( "Good bye tags"      , f::fail )};

    std::ostringstream os;
    char const * args1[] = { "!world"    };
    char const * args2[] = { "!1\\]"     };
    char const * args3[] = { "!\\[.*\\]" };

    EXPECT( 3 == run( fail, make_texts( args1 ), os ) );
    EXPECT( 2 == run( fail, make_texts( args2 ), os ) );
    EXPECT( 1 == run( fail, make_texts( args3 ), os ) );
}

CASE( "Test specification series select tests [commandline]" )
{
    struct f { static void fail(env & lest_env) { EXPECT( false ); }};

    test fail[] = { test( "a [x1]"  , f::fail ),
                    test( "b [x1]"  , f::fail ),
                    test( "c [x2]"  , f::fail ),
                    test( "d [hide]", f::fail ),
                    test( "e [.]"   , f::fail )};

    std::ostringstream os;
    char const * args1[] = {  "!\\[x"              };
    char const * args2[] = { "!\\[x1"              };
    char const * args3[] = {  "!\\[x"    , "\\[x2" };
    char const * args4[] = {  "\\[\\.\\]", "!\\[x" };
    char const * args5[] = { "@"         , "!\\[x" };
    char const * args6[] = { "*"         , "!\\[x" };
    char const * args7[] = { "^.*lest_env"      , "!\\[x" };

    EXPECT( 0 == run( fail, make_texts( args1 ), os ) );
    EXPECT( 1 == run( fail, make_texts( args2 ), os ) );
    EXPECT( 1 == run( fail, make_texts( args3 ), os ) );
    EXPECT( 1 == run( fail, make_texts( args4 ), os ) );
    EXPECT( 2 == run( fail, make_texts( args5 ), os ) );
    EXPECT( 2 == run( fail, make_texts( args6 ), os ) );
    EXPECT( 2 == run( fail, make_texts( args7 ), os ) );
}
#else // regex_search:

CASE( "Test specifications select tests [commandline]" )
{
    struct f { static void fail(env & lest_env) { EXPECT( false ); }};

    test fail[] = { test( "Hello world [tag1]" , f::fail ),
                    test( "Good morning [tag1]", f::fail ),
                    test( "Good noon [tag2]"   , f::fail ),
                    test( "Good bye tags"      , f::fail )};

    std::ostringstream os;
    char const * args1[] = { "Hello"  };
    char const * args2[] = { "[tag1]" };
    char const * args3[] = { "[tag2]" };

    char const * args5[] = { "@"       };
    char const * args6[] = { "*"       };
    char const * args7[] = { "AAA*BBB" };

    EXPECT( 1 == run( fail, make_texts( args1 ), os ) );
    EXPECT( 2 == run( fail, make_texts( args2 ), os ) );
    EXPECT( 1 == run( fail, make_texts( args3 ), os ) );

    EXPECT( 4 == run( fail,      texts(       ), os ) );
    EXPECT( 4 == run( fail, make_texts( args5 ), os ) );
    EXPECT( 4 == run( fail, make_texts( args6 ), os ) );
    EXPECT( 0 == run( fail, make_texts( args7 ), os ) );
}

CASE( "Test specifications omit tests [commandline]" )
{
    struct f { static void fail(env & lest_env) { EXPECT( false ); }};

    test fail[] = { test( "Hello world [tag1]" , f::fail ),
                    test( "Good morning [tag1]", f::fail ),
                    test( "Good bye [tag2]"    , f::fail )};

    std::ostringstream os;
    char const * args1[] = { "![tag1]" };
    char const * args2[] = { "![tag2]" };

    EXPECT( 1 == run( fail, make_texts( args1 ), os ) );
    EXPECT( 2 == run( fail, make_texts( args2 ), os ) );
}

CASE( "Test specification series select tests [commandline]" )
{
    struct f { static void fail(env & lest_env) { EXPECT( false ); }};

    test fail[] = { test( "a [x1]"  , f::fail ),
                    test( "b [x1]"  , f::fail ),
                    test( "c [x2]"  , f::fail ),
                    test( "d [hide]", f::fail ),
                    test( "e [.]"   , f::fail )};

    std::ostringstream os;
    char const * args1[] = {  "![x"        };
    char const * args2[] = { "![x1"        };
    char const * args3[] = { "![x" , "[x2" };
    char const * args4[] = { "[.]" , "![x" };
    char const * args5[] = { "@"   , "![x" };
    char const * args6[] = { "*"   , "![x" };

    EXPECT( 0 == run( fail, make_texts( args1 ), os ) );
    EXPECT( 1 == run( fail, make_texts( args2 ), os ) );
    EXPECT( 1 == run( fail, make_texts( args3 ), os ) );
    EXPECT( 1 == run( fail, make_texts( args4 ), os ) );
    EXPECT( 2 == run( fail, make_texts( args5 ), os ) );
    EXPECT( 2 == run( fail, make_texts( args6 ), os ) );
}
#endif

CASE( "Unrecognised option recognised as such [commandline]" )
{
    struct f { static void fail(env &) { ; }};

    test fail[] = { test( "", f::fail ) };

    std::ostringstream os;
    char const * args[] = { "--nonexisting-option" };

    EXPECT( 1 == run( fail, make_texts( args ), os ) );
}

CASE( "Option -h,--help shows help message [commandline]" )
{
    struct f { static void pass(env &) { ; }};

    test pass[] = { test( "", f::pass ) };

    std::ostringstream os;
    char const * args1[] = { "-h"     };
    char const * args2[] = { "--help" };

    EXPECT( 0 == run( pass, make_texts( args1 ), os ) );
    EXPECT( 0 == run( pass, make_texts( args2 ), os ) );
}

CASE( "Option -a,--abort aborts selected tests [commandline]" )
{
    struct f { static void fail(env & lest_env) { EXPECT( false ); }};

    test fail[] = { test( "", f::fail ),
                    test( "", f::fail ) };

    std::ostringstream os;
    char const * args1[] = { "-a"      };
    char const * args2[] = { "--abort" };

    EXPECT( 2 == run( fail,      texts(       ), os ) );
    EXPECT( 1 == run( fail, make_texts( args1 ), os ) );
    EXPECT( 1 == run( fail, make_texts( args2 ), os ) );
}

CASE( "Option -c,--count counts selected tests [commandline]" )
{
    struct f { static void abc(env &) { ; }
               static void xyz(env &) { ; }};

    test pass[] = { test( "a b c", f::abc ),
                    test( "x y z", f::xyz ) };

    {   std::ostringstream os;
        char const * args1[] = { "-c"      };
        char const * args2[] = { "--count" };

        EXPECT( 0 == run( pass, make_texts( args1 ), os ) );
        EXPECT( 0 == run( pass, make_texts( args2 ), os ) );

        EXPECT( std::string::npos != os.str().find( "2 " ) );
    }{
        std::ostringstream os;
        char const * args[] = { "-c", "y" };

        EXPECT( 0 == run( pass, make_texts( args ), os ) );

        EXPECT( std::string::npos != os.str().find( "1 " ) );
    }
}

CASE( "Option -g,--list-tags lists tags of selected tests [commandline]" )
{
    struct f { static void abc(env &) { ; }
               static void xyz(env &) { ; }};

    test pass[] = { test( "a [b][c]" , f::abc ),
                    test( "[x] y [z]", f::xyz ) };

    {   std::ostringstream os;

        char const * args1[] = { "-g"          };
        char const * args2[] = { "--list-tags" };

        EXPECT( 0 == run( pass, make_texts( args1 ), os ) );
        EXPECT( 0 == run( pass, make_texts( args2 ), os ) );

        EXPECT( std::string::npos != os.str().find( "[b]" ) );
        EXPECT( std::string::npos != os.str().find( "[c]" ) );
        EXPECT( std::string::npos != os.str().find( "[x]" ) );
        EXPECT( std::string::npos != os.str().find( "[z]" ) );
    }{
        std::ostringstream os;

        char const * args1[] = { "-g", "[x]" };

        EXPECT( 0 == run( pass, make_texts( args1 ), os ) );

        EXPECT( std::string::npos == os.str().find( "[b]" ) );
        EXPECT( std::string::npos == os.str().find( "[c]" ) );
        EXPECT( std::string::npos != os.str().find( "[x]" ) );
        EXPECT( std::string::npos != os.str().find( "[z]" ) );
    }
}

CASE( "Option -l,--list-tests lists selected tests [commandline]" )
{
    struct f { static void abc(env &) { ; }
               static void xyz(env &) { ; }};

    test pass[] = { test( "a b c", f::abc ),
                    test( "x y z", f::xyz ) };

    {   std::ostringstream os;
        char const * args1[] = { "-l"           };
        char const * args2[] = { "--list-tests" };

        EXPECT( 0 == run( pass, make_texts( args1 ), os ) );
        EXPECT( 0 == run( pass, make_texts( args2 ), os ) );

        EXPECT( std::string::npos != os.str().find( "a b c" ) );
        EXPECT( std::string::npos != os.str().find( "x y z" ) );
    }{
        std::ostringstream os;
        char const * args[] = { "-l", "b" };

        EXPECT( 0 == run( pass, make_texts( args ), os ) );

        EXPECT( std::string::npos != os.str().find( "b" ) );
        EXPECT( std::string::npos == os.str().find( "y" ) );
    }
}

CASE( "Option -p,--pass also reports passing selected tests [commandline]" )
{
    struct f { static void pass(env & lest_env) { EXPECT( true ); }};

    test pass[] = { test( "a b c", f::pass ) };

    {   std::ostringstream os;
        char const * args[] = { "-p" };

        EXPECT( 0 == run( pass, make_texts( args ), os ) );

        EXPECT( std::string::npos != os.str().find( "a b c"  ) );
        EXPECT( std::string::npos != os.str().find( "passed" ) );
        EXPECT( std::string::npos != os.str().find( " for "  ) );
    }{
        std::ostringstream os;
        char const * args[] = { "--pass" };

        EXPECT( 0 == run( pass, make_texts( args ), os ) );

        EXPECT( std::string::npos != os.str().find( "a b c"  ) );
        EXPECT( std::string::npos != os.str().find( "passed" ) );
        EXPECT( std::string::npos != os.str().find( " for "  ) );
    }
}

CASE( "Option -z,--pass-zen also reports passing selected tests, but not expansion [commandline]" )
{
    struct f { static void pass(env & lest_env) { EXPECT( true ); }};

    test pass[] = { test( "a b c", f::pass ) };

    {   std::ostringstream os;
        char const * args[] = { "-z" };

        EXPECT( 0 == run( pass, make_texts( args ), os ) );

        EXPECT( std::string::npos != os.str().find( "a b c"  ) );
        EXPECT( std::string::npos != os.str().find( "passed" ) );
        EXPECT( std::string::npos == os.str().find( " for "  ) );
    }{
        std::ostringstream os;
        char const * args[] = { "--pass-zen" };

        EXPECT( 0 == run( pass, make_texts( args ), os ) );

        EXPECT( std::string::npos != os.str().find( "a b c"  ) );
        EXPECT( std::string::npos != os.str().find( "passed" ) );
        EXPECT( std::string::npos == os.str().find( " for "  ) );
    }
}

CASE( "Option -t,--time reports execution time of selected tests [commandline]" )
{
    struct f { static void pass(env & lest_env) { EXPECT( true ); }};

    test pass[] = { test( "a b c", f::pass ) };

    {   std::ostringstream os;
        char const * args[] = { "-t" };

        EXPECT( 0 == run( pass, make_texts( args ), os ) );

        EXPECT( std::string::npos != os.str().find( "a b c"   ) );
        EXPECT( std::string::npos != os.str().find( "ms:"     ) );
        EXPECT( std::string::npos != os.str().find( "Elapsed" ) );
    }{
        std::ostringstream os;
        char const * args[] = { "--time" };

        EXPECT( 0 == run( pass, make_texts( args ), os ) );

        EXPECT( std::string::npos != os.str().find( "a b c"   ) );
        EXPECT( std::string::npos != os.str().find( "ms:"     ) );
        EXPECT( std::string::npos != os.str().find( "Elapsed" ) );
    }
}

CASE( "Option -v,--verbose also report passing or failing sections [commandline]" )
{
    struct f {
        static void pass(env & lest_env) { SETUP("Setup") { SECTION("Section") { EXPECT( 1==1 ); }}}
        static void fail(env & lest_env) { SETUP("Setup") { SECTION("Section") { EXPECT( 1==2 ); }}}
    };

    test pass[] = { test( "pass", f::pass ) };
    test fail[] = { test( "fail", f::fail ) };

    {
        std::ostringstream os;
        char const * args[] = { "-v" };

        EXPECT( 0 == run( pass, make_texts( args ), os ) );

        EXPECT( "" == os.str() );
    }{
        std::ostringstream os;
        char const * args[] = { "--verbose" };

        EXPECT( 0 == run( pass, make_texts( args ), os ) );

        EXPECT( "" == os.str() );
    }{
        std::ostringstream os;
        char const * args[] = { "--verbose" };

        EXPECT( 1 == run( fail, make_texts( args ), os ) );

        EXPECT( std::string::npos != os.str().find( "Setup" ) );
        EXPECT( std::string::npos != os.str().find( "Section" ) );
    }{
        std::ostringstream os;
        char const * args[] = { "--verbose", "--pass" };

        EXPECT( 0 == run( pass, make_texts( args ), os ) );

        EXPECT( std::string::npos != os.str().find( "Setup" ) );
        EXPECT( std::string::npos != os.str().find( "Section" ) );
    }
}

CASE( "Option --repeat=N is recognised [commandline]" )
{
    std::ostringstream os;
    char const * args[] = { "--repeat=42" };

    EXPECT( 0 == run( tests(), make_texts( args ), os ) );
}

CASE( "Option --repeat=3 repeats 3x [commandline]" )
{
    struct f { static void fail(env & lest_env) { EXPECT( false ); }};

    test fail[] = { test( "", f::fail ) };

    std::ostringstream os;
    char const * args[] = { "--repeat=3" };

    EXPECT( 3 == run( fail, make_texts( args ), os ) );
}

CASE( "Option --repeat=-1 (indefinite) is recognised [commandline]" )
{
    struct f { static void fail(env & lest_env) { EXPECT( false ); }};

    test fail[] = { test( "", f::fail ) };

    std::ostringstream os;
    char const * args[] = { "--abort", "--repeat=-1" };

    // currently no-tests are also repeated indefinitely hence the aborting a failing test:

    EXPECT( 1 == run( fail, make_texts( args ), os ) );

    EXPECT( std::string::npos == os.str().find( "Error" ) );
}

CASE( "Option --repeat={negative-number} is recognised as invalid [commandline]" )
{
    struct f { static void fail(env & lest_env) { EXPECT( false ); }};

    test fail[] = { test( "", f::fail ) };

    std::ostringstream os;
    char const * args[] = { "--repeat=-3" };

    EXPECT( 1 == run( fail, make_texts( args ), os ) );

    EXPECT( std::string::npos != os.str().find( "Error" ) );
}

CASE( "Option --version is recognised [commandline]" )
{
    std::ostringstream os;
    char const * args[] = { "--version" };

    EXPECT( 0 == run( tests(), make_texts( args ), os ) );
}

CASE( "Option -- ends option section [commandline]" )
{
    struct f { static void pass(env & lest_env) { EXPECT( true ); }};

    test pass[] = { test( "a-b", f::pass ) };

    std::ostringstream os;
    char const * args1[] = { "-l", "--", "-" };
    char const * args2[] = { "-c", "--", "-" };

    EXPECT( 0 == run( pass, make_texts( args1 ), os ) );
    EXPECT( 0 == run( pass, make_texts( args2 ), os ) );

    EXPECT( std::string::npos != os.str().find( "a-b" ) );
    EXPECT( std::string::npos != os.str().find( "1 " ) );
}

// Report versions and capabilities:

#define lest_PRESENT( x ) \
    std::cout << #x << ": " << x << "\n"

#define lest_ABSENT( x ) \
    std::cout << #x << ": (undefined)\n"

CASE( "lest version" "[.version]" )
{
    lest_PRESENT( lest_MAJOR );
    lest_PRESENT( lest_MINOR );
    lest_PRESENT( lest_PATCH );
    lest_PRESENT( lest_VERSION );
}

CASE( "lest features" "[.feature]" )
{
    lest_PRESENT( lest_FEATURE_COLOURISE );
    lest_PRESENT( lest_FEATURE_LITERAL_SUFFIX );
    lest_PRESENT( lest_FEATURE_REGEX_SEARCH );
    lest_PRESENT( lest_FEATURE_TIME );
    lest_PRESENT( lest_FEATURE_TIME_PRECISION );
}

CASE( "C++ compiler version" "[.compiler]" )
{
#ifdef lest_COMPILER_GNUC_VERSION
    lest_PRESENT( lest_COMPILER_CLANG_VERSION );
#else
    lest_ABSENT(  lest_COMPILER_CLANG_VERSION );
#endif

#ifdef lest_COMPILER_GNUC_VERSION
    lest_PRESENT( lest_COMPILER_GNUC_VERSION );
#else
    lest_ABSENT(  lest_COMPILER_GNUC_VERSION );
#endif

#ifdef lest_COMPILER_MSVC_VERSION
    lest_PRESENT( lest_COMPILER_MSVC_VERSION );
#else
    lest_ABSENT(  lest_COMPILER_MSVC_VERSION );
#endif
}

CASE( "C++ standard version" "[.stdc++]" )
{
    lest_PRESENT( __cplusplus );
#ifdef _MSVC_LANG
    lest_PRESENT( _MSVC_LANG );
#else
    lest_ABSENT(  _MSVC_LANG );
#endif
    lest_PRESENT( lest_CPP11_OR_GREATER );
    lest_PRESENT( lest_CPP14_OR_GREATER );
    lest_PRESENT( lest_CPP17_OR_GREATER );
    lest_PRESENT( lest_CPP20_OR_GREATER );
}

CASE( "Presence of C++ language features" "[.stdlanguage]" )
{
    lest_PRESENT( lest_HAVE_NULLPTR );
}

CASE( "Presence of C++ library features" "[.stdlibrary]" )
{
#ifdef _HAS_CPP0X
    lest_PRESENT( _HAS_CPP0X );
#else
    lest_ABSENT(  _HAS_CPP0X );
#endif
}

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv );
}

// cl -nologo -W3 -EHsc -I../include test_lest_cpp03.cpp && test_lest_cpp03
// cl -nologo -Wall -EHsc -I../include test_lest_cpp03.cpp && test_lest_cpp03
// g++ -Wall -Wextra -std=c++11 -I../include -o test_lest_cpp03.exe test_lest_cpp03.cpp && test_lest_cpp03
// g++ -Wall -Wextra -std=c++03 -I../include -o test_lest_cpp03.exe test_lest_cpp03.cpp && test_lest_cpp03
