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
    EXPECT( word == pluralise( 1, word ) );
    int range[] = {0,2,3,4,5,6,7,8,9,10,11,12};
    for ( int * pos = range; pos != range + lest_DIMENSION_OF(range); ++pos )
        EXPECT( ( word + "s" ) == pluralise( *pos, word ) );
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
    struct f { static void pass(env & $) { EXPECT(true); } };

    test pass( "P", f::pass );

    try { pass.behaviour( $ ); }
    catch(...) { throw failure(location(__FILE__,__LINE__), "unexpected error generated", "true"); }
}

CASE( "Expect generates a message exception for a failing test" )
{
    struct f { static void fail(env & $) { EXPECT(false); } };

    test fail( "F", f::fail );

    for (;;)
    {
        try { fail.behaviour( $ ); } catch ( message & ) { break; }
        throw failure(location(__FILE__,__LINE__), "no error generated", "false");
    }
}

CASE( "Expect succeeds for success (true) and failure (false)" )
{
    struct f { static void pass(env & $) { EXPECT(true);}
               static void fail(env & $) { EXPECT(false);} };

    test pass[] = { test( "P", f::pass ) };
    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );
}

CASE( "Expect succeeds for integer comparation" )
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

CASE( "Expect succeeds for mixed integer, real comparation" )
{
    EXPECT( 7.0 == 7   );
    EXPECT( 7.0 != 8   );
    EXPECT( 7   == 7.0 );
    EXPECT( 7   != 8.0 );

    EXPECT_NOT( 7.0 == 8   );
    EXPECT_NOT( 7  !=  7.0 );
}

CASE( "Expect succeeds for string comparation" )
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

CASE( "Function run() returns the right failure count" )
{
    struct f { static void pass(env & $) { EXPECT( 1==1 ); }
               static void fail(env & $) { EXPECT( 0==1 ); }};

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
    struct f { static void fail(env & $) { EXPECT( (throw std::runtime_error(std_hello_world), true) ); }};

    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 1 == run( fail, os ) );
    EXPECT( std::string::npos != os.str().find(std_hello_world) );
}

CASE( "Expect reports an unexpected non-standard exception" )
{
    struct f { static void fail(env & $) { EXPECT( (throw 77, true) ); }};

    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 1 == run( fail, os ) );
}

CASE( "Expect_no_throw succeeds without an exception" )
{
    struct f { static void pass(env & $) { EXPECT_NO_THROW( true ); }};

    test pass[] = { test( "P", f::pass ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
}

CASE( "Expect_no_throw reports a standard exception" )
{
    struct f { static void fail(env & $) { EXPECT_NO_THROW( throw std::runtime_error(std_hello_world) ); }};

    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 1 == run( fail, os ) );
    EXPECT( std::string::npos != os.str().find(std_hello_world) );
}

CASE( "Expect_no_throw reports a non-standard exception" )
{
    struct f { static void fail(env & $) { EXPECT_NO_THROW( (throw 77, true) ); }};

    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 1 == run( fail, os ) );
}

CASE( "Expect_throws reports a missing exception" )
{
    struct f { static void fail(env & $) { EXPECT_THROWS( true ); }};

    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 1 == run( fail, os ) );
}

CASE( "Expect_throws succeeds with a standard exception" )
{
    struct f { static void pass(env & $) { EXPECT_THROWS( throw std::runtime_error(std_hello_world) ); }};

    test pass[] = { test( "P", f::pass ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
}

CASE( "Expect_throws succeeds with a non-standard exception" )
{
    struct f { static void pass(env & $) { EXPECT_THROWS( throw 77 ); }};

    test pass[] = { test( "P", f::pass ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
}

CASE( "Expect_throws_as reports a missing exception" )
{
    struct f { static void fail(env & $) { EXPECT_THROWS_AS( true, std::runtime_error ); }};

    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 1 == run( fail, os ) );
}

CASE( "Expect_throws_as reports getting a different exception" )
{
    struct f { static void fail(env & $) { EXPECT_THROWS_AS( throw std::bad_alloc(), std::runtime_error ); }};

    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 1 == run( fail, os ) );
}

CASE( "Expect_throws_as succeeds with a specific standard exception" )
{
    struct f { static void pass(env & $) { EXPECT_THROWS_AS( throw std::bad_alloc(), std::bad_alloc ); }};

    test pass[] = { test( "P", f::pass ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
}

CASE( "Expect_throws_as succeeds with a specific non-standard exception" )
{
    struct f { static void pass(env & $) { EXPECT_THROWS_AS( throw 77, int ); }};

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

#if __cplusplus >= 201103L

CASE( "Decomposition formats nullptr as string" )
{
    struct f { static void pass(env & $) { EXPECT(  nullptr == nullptr  ); }
               static void fail(env & $) { EXPECT( (void*)1 == nullptr  ); }};

    test pass[] = { test( "P", f::pass ) };
    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );

    EXPECT( std::string::npos != os.str().find( "(void*)1 == nullptr for 0x1 == nullptr" ) );
}
#endif

CASE( "Decomposition formats boolean as strings true and false" )
{
    struct f { static void pass(env & $) { EXPECT( true == true  ); }
               static void fail(env & $) { EXPECT( true == false ); }};

    test pass[] = { test( "P", f::pass ) };
    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );

    EXPECT( std::string::npos != os.str().find( "true == false for true == false" ) );
}

CASE( "Decomposition formats character with single quotes" )
{
    struct f { static void pass(env & $) { EXPECT( 'a' < 'b' ); }
               static void fail(env & $) { EXPECT( 'b' < 'a' ); }};

    test pass[] = { test( "P", f::pass ) };
    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );

    EXPECT( std::string::npos != os.str().find( "'b' < 'a' for 'b' < 'a'" ) );
}

std::string std_hello( "hello" );
std::string std_world( "world" );
char const  *  hello = "hello";
char const  *  world = "world";

CASE( "Decomposition formats std::string with double quotes" )
{
    struct f { static void pass(env & $) { EXPECT( std_hello < "world" ); }
               static void fail(env & $) { EXPECT( std_world < "hello" ); }};

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
    struct f { static void pass(env & $) { EXPECT( hello < std_world ); }
               static void fail(env & $) { EXPECT( world < std_hello ); }};

    test pass[] = { test( "P", f::pass ) };
    test fail[] = { test( "F", f::fail ) };

    std::ostringstream os;

    EXPECT( 0 == run( pass, os ) );
    EXPECT( 1 == run( fail, os ) );

    // lifted from assertion for VC6:
    std::size_t pos = os.str().find( "world < std_hello for \"world\" < \"hello\"" );
    EXPECT( std::string::npos != pos );
}

CASE( "Has single expression evaluation" )
{
    struct f { static void pass(env & $) { int n = 0; EXPECT( 1 == ++n ); }
               static void fail(env & $) { int n = 0; EXPECT( 2 == ++n ); }};

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
    EXPECT( 1.23 != approx( 1.24 ) );
}

CASE( "Approximate using epsilon compares properly [approx][epsilon]" )
{
    EXPECT( 1.23 != approx( 1.231 ) );
    EXPECT( 1.23 == approx( 1.231 ).epsilon( 0.1 ) );
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

CASE( "Skips tests tagged [.]" )
{
    EXPECT( false );
}

CASE( "Skips tests tagged [hide]" )
{
    EXPECT( false );
}

#if lest_FEATURE_REGEX_SEARCH

CASE( "Test specifications select tests [commandline]" )
{
    struct f { static void fail(env & $) { EXPECT( false ); }};

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
    struct f { static void fail(env & $) { EXPECT( false ); }};

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
    struct f { static void fail(env & $) { EXPECT( false ); }};

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
    char const * args7[] = { "^.*$"      , "!\\[x" };

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
    struct f { static void fail(env & $) { EXPECT( false ); }};

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
    struct f { static void fail(env & $) { EXPECT( false ); }};

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
    struct f { static void fail(env & $) { EXPECT( false ); }};

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
    struct f { static void fail(env & $) { EXPECT( false ); }};

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
    struct f { static void pass(env & $) { EXPECT( true ); }};

    test pass[] = { test( "a b c", f::pass ) };

    {   std::ostringstream os;
        char const * args[] = { "-p" };

        EXPECT( 0 == run( pass, make_texts( args ), os ) );

        EXPECT( std::string::npos != os.str().find( "a b c"  ) );
        EXPECT( std::string::npos != os.str().find( "passed" ) );
    }{
        std::ostringstream os;
        char const * args[] = { "--pass" };

        EXPECT( 0 == run( pass, make_texts( args ), os ) );

        EXPECT( std::string::npos != os.str().find( "a b c"  ) );
        EXPECT( std::string::npos != os.str().find( "passed" ) );
    }
}

CASE( "Option -t,--time reports execution time of selected tests [commandline]" )
{
    struct f { static void pass(env & $) { EXPECT( true ); }};

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

CASE( "Option --repeat=N is recognised [commandline]" )
{
    std::ostringstream os;
    char const * args[] = { "--repeat=42" };

    EXPECT( 0 == run( tests(), make_texts( args ), os ) );
}

CASE( "Option --repeat=3 repeats 3x [commandline]" )
{
    struct f { static void fail(env & $) { EXPECT( false ); }};

    test fail[] = { test( "", f::fail ) };

    std::ostringstream os;
    char const * args[] = { "--repeat=3" };

    EXPECT( 3 == run( fail, make_texts( args ), os ) );
}

CASE( "Option --repeat=-1 (indefinite) is recognised [commandline]" )
{
    struct f { static void fail(env & $) { EXPECT( false ); }};

    test fail[] = { test( "", f::fail ) };

    std::ostringstream os;
    char const * args[] = { "--abort", "--repeat=-1" };

    // currently no-tests are also repeated indefinitely hence the aborting a failing test:

    EXPECT( 1 == run( fail, make_texts( args ), os ) );

    EXPECT( std::string::npos == os.str().find( "Error" ) );
}

CASE( "Option --repeat={negative-number} is recognised as invalid [commandline]" )
{
    struct f { static void fail(env & $) { EXPECT( false ); }};

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
    struct f { static void pass(env & $) { EXPECT( true ); }};

    test pass[] = { test( "a-b", f::pass ) };

    std::ostringstream os;
    char const * args1[] = { "-l", "--", "-" };
    char const * args2[] = { "-c", "--", "-" };

    EXPECT( 0 == run( pass, make_texts( args1 ), os ) );
    EXPECT( 0 == run( pass, make_texts( args2 ), os ) );

    EXPECT( std::string::npos != os.str().find( "a-b" ) );
    EXPECT( std::string::npos != os.str().find( "1 " ) );
}

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv );
}

// cl -nologo -W3 -EHsc -I.. test_lest_cpp03.cpp && test_lest_cpp03
// cl -nologo -Wall -EHsc -I.. test_lest_cpp03.cpp && test_lest_cpp03
// g++ -Wall -Wextra -std=c++11 -I.. -o test_lest_cpp03.exe test_lest_cpp03.cpp && test_lest_cpp03
// g++ -Wall -Wextra -std=c++03 -I.. -o test_lest_cpp03.exe test_lest_cpp03.cpp && test_lest_cpp03
