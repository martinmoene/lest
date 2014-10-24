// Copyright 2013, 2014 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "lest.hpp"
#include <set>

#define CASE_E( name ) \
    name, []( env & )

const lest::test no_using_namespace_lest[] =
{
    CASE( "Namespace lest is specified correctly in lest.hpp [compile-only]" )
    {
        EXPECT(  true );
        EXPECT_NOT(  false );
        EXPECT_NO_THROW( true );
        EXPECT_THROWS( true );
        EXPECT_THROWS_AS( true, std::exception );
    },
};

using namespace lest;

const lest::test specification[] =
{
    CASE( "Function to suppress warning \"expression has no effect\" acts as identity function" )
    {
        EXPECT( false == is_true( false ) );
        EXPECT(  true == is_true( true  ) );
    },

    CASE( "Function with_message() returns correct string" )
    {
        std::string msg = "Let writing tests become irresistibly easy and attractive.";
        EXPECT( ( "with message \"" + msg + "\"" ) == with_message( msg ) );
    },

    CASE( "Function of_type() returns correct string" )
    {
        std::string msg = "this_type";
        EXPECT( ( "of type " + msg ) == of_type( msg ) );
    },

    CASE( "Function pluralise() adds 's' except for 1 item" )
    {
        std::string word = "hammer";
        EXPECT( word == pluralise( 1, word ) );
        for ( auto i : {0,2,3,4,5,6,7,8,9,10,11,12} )
        {
            EXPECT( ( word + "s" ) == pluralise( i, word ) );
        }
    },

    CASE( "Location constructs properly" )
    {
        char const * file = __FILE__; int line = __LINE__;
        location where{ file, line };
        EXPECT( file == where.file );
        EXPECT( line == where.line );
    },

    CASE( "Comment constructs properly" )
    {
        std::string info = __FILE__;
        comment note = info;
        EXPECT( info == note.info );
    },

    CASE( "Comment converted to bool indicates absence or presence of comment" )
    {
        EXPECT( false == bool( comment( "") ) );
        EXPECT(  true == bool( comment("x") ) );
    },

    CASE( "Failure exception type constructs and prints properly" )
    {
        std::string name = "test-name";
        failure msg( location{"filename.cpp", 765}, "expression", "decomposition" );

        std::ostringstream os;
        report( os, msg, name );

#ifndef __GNUG__
        EXPECT( os.str() == "filename.cpp(765): failed: test-name: expression for decomposition\n" );
#else
        EXPECT( os.str() == "filename.cpp:765: failed: test-name: expression for decomposition\n" );
#endif
    },

    CASE( "Expected exception type constructs and prints properly" )
    {
        std::string name = "test-name";
        expected msg( location{"filename.cpp", 765}, "expression" );

        std::ostringstream os;
        report( os, msg, name );

#ifndef __GNUG__
        EXPECT( os.str() == "filename.cpp(765): failed: didn't get exception: test-name: expression\n" );
#else
        EXPECT( os.str() == "filename.cpp:765: failed: didn't get exception: test-name: expression\n" );
#endif
    },

    CASE( "Unexpected exception type constructs and prints properly" )
    {
        std::string name = "test-name";
        lest::unexpected msg( location{"filename.cpp", 765}, "expression", "exception-type" );

        std::ostringstream os;
        report( os, msg, name );

#ifndef __GNUG__
        EXPECT( os.str() == "filename.cpp(765): failed: got unexpected exception exception-type: test-name: expression\n" );
#else
        EXPECT( os.str() == "filename.cpp:765: failed: got unexpected exception exception-type: test-name: expression\n" );
#endif
    },

    CASE( "Expect generates no message exception for a succeeding test" )
    {
        test pass = { CASE( "P" ) { EXPECT( true  ); } };

        try { pass.behaviour( $ ); }
        catch(...) { throw failure(location{__FILE__,__LINE__}, "unexpected error generated", "true"); }
    },

    CASE( "Expect generates a message exception for a failing test")
    {
        test fail = { CASE("F") { EXPECT( false ); } };

        for (;;)
        {
            try { fail.behaviour( $ ); } catch ( message & ) { break; }
            throw failure(location{__FILE__,__LINE__}, "no error generated", "false");
        }
    },

    CASE( "Expect succeeds for success (true) and failure (false)" )
    {
        test pass[] = {{ CASE("P") { EXPECT( true  ); } }};
        test fail[] = {{ CASE("F") { EXPECT( false ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );
    },

    CASE( "Expect-not succeeds for success (false) and failure (true)" )
    {
        test pass[] = {{ CASE("P") { EXPECT_NOT( false ); } }};
        test fail[] = {{ CASE("F") { EXPECT_NOT( true  ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );
    },

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
    },

    CASE( "Expect succeeds for mixed integer, real comparation" )
    {
        EXPECT( 7.0 == 7   );
        EXPECT( 7.0 != 8   );
        EXPECT( 7   == 7.0 );
        EXPECT( 7   != 8.0 );

        EXPECT_NOT( 7.0 == 8   );
        EXPECT_NOT( 7  !=  7.0 );
    },

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
    },

    CASE( "Function run() returns the right failure count" )
    {
        test pass  [] = {{ CASE( "P"  ) { EXPECT( 1==1 ); } }};
        test fail_1[] = {{ CASE( "F1" ) { EXPECT( 0==1 ); } }};
        test fail_3[] = {{ CASE( "F1" ) { EXPECT( 0==1 ); } },
                         { CASE( "F2" ) { EXPECT( 0==1 ); } },
                         { CASE( "F3" ) { EXPECT( 0==1 ); } },};

        std::ostringstream os;

        EXPECT( 0 == run( pass  , os ) );
        EXPECT( 1 == run( fail_1, os ) );
        EXPECT( 3 == run( fail_3, os ) );
    },

    CASE( "Expect reports an unexpected standard exception" )
    {
        std::string text = "hello-world";
        test fail[] = {{ CASE( "F", = ) { EXPECT( (throw std::runtime_error(text), true) ); } }};

        std::ostringstream os;

        EXPECT( 1 == run( fail, os ) );
        EXPECT( std::string::npos != os.str().find(text) );
    },

    CASE( "Expect reports an unexpected non-standard exception" )
    {
        test fail[] = {{ CASE( "P" ) { EXPECT( (throw 77, true) ); } }};

        std::ostringstream os;

        EXPECT( 1 == run( fail, os ) );
    },

    CASE( "Expect_no_throw succeeds without an exception" )
    {
        test pass[] = {{ CASE( "P" ) { EXPECT_NO_THROW( true ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
    },

    CASE( "Expect_no_throw reports a standard exception" )
    {
        std::string text = "hello-world";
        test fail[] = {{ CASE( "F", = ) { EXPECT_NO_THROW( throw std::runtime_error(text) ); } }};

        std::ostringstream os;

        EXPECT( 1 == run( fail, os ) );
        EXPECT( std::string::npos != os.str().find(text) );
    },

    CASE( "Expect_no_throw reports a non-standard exception" )
    {
        test fail[] = {{ CASE( "F" ) { EXPECT_NO_THROW( (throw 77, true) ); } }};

        std::ostringstream os;

        EXPECT( 1 == run( fail, os ) );
    },

    CASE( "Expect_throws reports a missing exception" )
    {
        test fail[] = {{ CASE( "F" ) { EXPECT_THROWS( true ); } }};

        std::ostringstream os;

        EXPECT( 1 == run( fail, os ) );
    },

    CASE( "Expect_throws succeeds with a standard exception" )
    {
        std::string text = "hello-world";
        test pass[] = {{ CASE( "P", = ) { EXPECT_THROWS( throw std::runtime_error(text) ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
    },

    CASE( "Expect_throws succeeds with a non-standard exception" )
    {
        test pass[] = {{ CASE( "P" ) { EXPECT_THROWS( throw 77 ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
    },

    CASE( "Expect_throws_as reports a missing exception" )
    {
        test fail[] = {{ CASE( "F" ) { EXPECT_THROWS_AS( true, std::runtime_error ); } }};

        std::ostringstream os;

        EXPECT( 1 == run( fail, os ) );
    },

    CASE( "Expect_throws_as reports getting a different exception" )
    {
        test fail[] = {{ CASE( "F" ) { EXPECT_THROWS_AS( throw std::bad_alloc(), std::runtime_error ); } }};

        std::ostringstream os;

        EXPECT( 1 == run( fail, os ) );
    },

    CASE( "Expect_throws_as succeeds with a specific standard exception" )
    {
        test pass[] = {{ CASE( "P" ) { EXPECT_THROWS_AS( throw std::bad_alloc(), std::bad_alloc ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
    },

    CASE( "Expect_throws_as succeeds with a specific non-standard exception" )
    {
        test pass[] = {{ CASE( "P" ) { EXPECT_THROWS_AS( throw 77, int ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
    },

    CASE( "Setup creates a fresh fixture for each section" )
    {
        SETUP("Context") {
            int i = 7;

            SECTION("S1") {         i = 42;   }
            SECTION("S2") { EXPECT( i == 7 ); }
        }
    },

    CASE( "Setup runs as many times as there are sections" )
    {
        int i = 0;

        SETUP("Context") {
            ++i;

            SECTION("S1") { }
            SECTION("S2") { }
        }
        EXPECT( i == 2 );
    },

    CASE( "Decomposition formats nullptr as string" )
    {
        test pass[] = {{ CASE( "P" ) { EXPECT(  nullptr == nullptr  ); } }};
        test fail[] = {{ CASE( "F" ) { EXPECT( (void*)1 == nullptr  ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );

        EXPECT( std::string::npos != os.str().find( "1 == nullptr" ) );
    },

    CASE( "Decomposition formats boolean as strings true and false" )
    {
        test pass[] = {{ CASE( "P" ) { EXPECT( true == true  ); } }};
        test fail[] = {{ CASE( "F" ) { EXPECT( true == false ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );

        EXPECT( std::string::npos != os.str().find( "true == false for true == false" ) );
    },

    CASE( "Decomposition formats character with single quotes" )
    {
        test pass[] = {{ CASE( "P" ) { EXPECT( 'a' < 'b' ); } }};
        test fail[] = {{ CASE( "F" ) { EXPECT( 'b' < 'a' ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );

        EXPECT( std::string::npos != os.str().find( "'b' < 'a' for 'b' < 'a'" ) );
    },

    CASE( "Decomposition formats std::string with double quotes" )
    {
        std::string hello( "hello" );
        std::string world( "world" );

        test pass[] = {{ CASE( "P",= ) { EXPECT( hello < "world" ); } }};
        test fail[] = {{ CASE( "F",= ) { EXPECT( world < "hello" ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );

        EXPECT( std::string::npos != os.str().find( "world < \"hello\" for \"world\" < \"hello\"" ) );
    },

    CASE( "Decomposition formats C string with double quotes" )
    {
        char const * hello( "hello" ); std::string std_hello( "hello" );
        char const * world( "world" ); std::string std_world( "world" );

        test pass[] = {{ CASE( "P", = ) { EXPECT( hello < std_world ); } }};
        test fail[] = {{ CASE( "F", = ) { EXPECT( world < std_hello ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );

        EXPECT( std::string::npos != os.str().find( "world < std_hello for \"world\" < \"hello\"" ) );
    },

    CASE( "Decomposition formats container with curly braces" )
    {
        std::set<int> s{ 1, 2, 3, };
        std::set<int> t{ 2, 1, 0, };

        test pass[] = {{ CASE( "P", = ) { EXPECT( s == s ); } }};
        test fail[] = {{ CASE( "F", = ) { EXPECT( s == t ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );

        EXPECT( std::string::npos != os.str().find( "{ 1, 2, 3, }" ) );
        EXPECT( std::string::npos != os.str().find( "{ 0, 1, 2, }" ) );
    },

    CASE( "Has single expression evaluation" )
    {
        test pass[] = {{ CASE( "P" ) { int n = 0; EXPECT( 1 == ++n ); } }};
        test fail[] = {{ CASE( "F" ) { int n = 0; EXPECT( 2 == ++n ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );

        EXPECT( std::string::npos != os.str().find( "for 2 == 1" ) );
    },

    CASE( "Approximate compares properly" )
    {
        EXPECT( 1.23 == approx( 1.23 ) );
        EXPECT( 1.23 != approx( 1.24 ) );
    },

    CASE( "Approximate using epsilon compares properly" )
    {
        EXPECT( 1.23 != approx( 1.231 ) );
        EXPECT( 1.23 == approx( 1.231 ).epsilon( 0.1 ) );
    },

    CASE( "Approximate using custom epsilon compares properly" )
    {
        approx custom = approx::custom().epsilon( 0.1 );

        EXPECT( approx( 1.231 ) != 1.23 );
        EXPECT( custom( 1.231 ) == 1.23 );
    },

    CASE( "Approximate to Pi compares properly" )
    {
        auto divide = []( double a, double b ) { return a / b; };

        EXPECT( divide( 22, 7 ) == approx( 3.141 ).epsilon( 0.001  ) );
        EXPECT( divide( 22, 7 ) != approx( 3.141 ).epsilon( 0.0001 ) );
    },

    CASE( "Skips tests tagged [.]" )
    {
        EXPECT( false );
    },

    CASE( "Skips tests tagged [hide]" )
    {
        EXPECT( false );
    },

#if lest_FEATURE_REGEX_SEARCH

    CASE( "Test specifications select tests [commandline]" )
    {
        test fail[] = {{ CASE( "Hello world [tag1]"  ) { EXPECT( false ); } },
                       { CASE( "Good morning [tag1]" ) { EXPECT( false ); } },
                       { CASE( "Good noon [tag2]"    ) { EXPECT( false ); } },
                       { CASE( "Good bye tags"       ) { EXPECT( false ); } }};

        std::ostringstream os;

        EXPECT( 1 == run( fail, { "world"    }, os ) );
        EXPECT( 2 == run( fail, { "1\\]"     }, os ) );
        EXPECT( 3 == run( fail, { "\\[.*\\]" }, os ) );
    },

    CASE( "Test specifications omit tests [commandline]" )
    {
        test fail[] = {{ CASE( "Hello world [tag1]"  ) { EXPECT( false ); } },
                       { CASE( "Good morning [tag1]" ) { EXPECT( false ); } },
                       { CASE( "Good noon [tag2]"    ) { EXPECT( false ); } },
                       { CASE( "Good bye tags"       ) { EXPECT( false ); } }};

        std::ostringstream os;

        EXPECT( 3 == run( fail, { "!world"    }, os ) );
        EXPECT( 2 == run( fail, { "!1\\]"     }, os ) );
        EXPECT( 1 == run( fail, { "!\\[.*\\]" }, os ) );
    },

    CASE( "Test specification series select tests [commandline]" )
    {
        test fail[] = {{ CASE( "a [x1]"   ) { EXPECT( false ); } },
                       { CASE( "b [x1]"   ) { EXPECT( false ); } },
                       { CASE( "c [x2]"   ) { EXPECT( false ); } },
                       { CASE( "d [hide]" ) { EXPECT( false ); } },
                       { CASE( "e [.]"    ) { EXPECT( false ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( fail, { "!\\[x"              }, os ) );
        EXPECT( 1 == run( fail, { "!\\[x1"             }, os ) );
        EXPECT( 1 == run( fail, { "!\\[x"    , "\\[x2" }, os ) );
        EXPECT( 1 == run( fail, { "\\[\\.\\]", "!\\[x" }, os ) );
        EXPECT( 2 == run( fail, { "@"        , "!\\[x" }, os ) );
        EXPECT( 2 == run( fail, { "*"        , "!\\[x" }, os ) );
        EXPECT( 2 == run( fail, { "^.*$"     , "!\\[x" }, os ) );
    },
#else // regex_search:

    CASE( "Test specifications select tests [commandline]" )
    {
        test fail[] = {{ CASE( "Hello world [tag1]"  ) { EXPECT( false ); } },
                       { CASE( "Good morning [tag1]" ) { EXPECT( false ); } },
                       { CASE( "Good noon [tag2]"    ) { EXPECT( false ); } },
                       { CASE( "Good bye tags"       ) { EXPECT( false ); } }};

        std::ostringstream os;

        EXPECT( 1 == run( fail, {  "Hello" }, os ) );
        EXPECT( 2 == run( fail, { "[tag1]" }, os ) );
        EXPECT( 1 == run( fail, { "[tag2]" }, os ) );

        EXPECT( 4 == run( fail, {           }, os ) );
        EXPECT( 4 == run( fail, { "@"       }, os ) );
        EXPECT( 4 == run( fail, { "*"       }, os ) );
        EXPECT( 0 == run( fail, { "AAA*BBB" }, os ) );
    },

    CASE( "Test specifications omit tests [commandline]" )
    {
        test fail[] = {{ CASE( "Hello world [tag1]"  ) { EXPECT( false ); } },
                       { CASE( "Good morning [tag1]" ) { EXPECT( false ); } },
                       { CASE( "Good bye [tag2]"     ) { EXPECT( false ); } }};

        std::ostringstream os;

        EXPECT( 1 == run( fail, { "![tag1]" }, os ) );
        EXPECT( 2 == run( fail, { "![tag2]" }, os ) );
    },

    CASE( "Test specification series select tests [commandline]" )
    {
        test fail[] = {{ CASE( "a [x1]"   ) { EXPECT( false ); } },
                       { CASE( "b [x1]"   ) { EXPECT( false ); } },
                       { CASE( "c [x2]"   ) { EXPECT( false ); } },
                       { CASE( "d [hide]" ) { EXPECT( false ); } },
                       { CASE( "e [.]"    ) { EXPECT( false ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( fail, { "![x"        }, os ) );
        EXPECT( 1 == run( fail, { "![x1"       }, os ) );
        EXPECT( 1 == run( fail, { "![x", "[x2" }, os ) );
        EXPECT( 1 == run( fail, { "[.]", "![x" }, os ) );
        EXPECT( 2 == run( fail, { "@"  , "![x" }, os ) );
        EXPECT( 2 == run( fail, { "*"  , "![x" }, os ) );
    },
#endif

    CASE( "Unrecognised option recognised as such [commandline]" )
    {
        test fail[] = {{ CASE_E( "" ) { ; } }};

        std::ostringstream os;

        EXPECT( 1 == run( fail, { "--nonexisting-option" }, os ) );
    },

    CASE( "Option -h,--help shows help message [commandline]" )
    {
        test pass[] = {{ CASE_E( "" ) { ; } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, {  "-h"    }, os ) );
        EXPECT( 0 == run( pass, { "--help" }, os ) );
    },

    CASE( "Option -a,--abort aborts selected tests [commandline]" )
    {
        test fail[] = {{ CASE( "" ) { EXPECT( false ); } },
                       { CASE( "" ) { EXPECT( false ); } }};

        std::ostringstream os;

        EXPECT( 2 == run( fail, {           }, os ) );
        EXPECT( 1 == run( fail, {  "-a"     }, os ) );
        EXPECT( 1 == run( fail, { "--abort" }, os ) );
    },

    CASE( "Option -c,--count counts selected tests [commandline]" )
    {
        test pass[] = {{ CASE_E( "a b c" ) { ; } },
                       { CASE_E( "x y z" ) { ; } }};

        {   std::ostringstream os;

            EXPECT( 0 == run( pass, {  "-c"     }, os ) );
            EXPECT( 0 == run( pass, { "--count" }, os ) );

            EXPECT( std::string::npos != os.str().find( "2 " ) );
        }{
            std::ostringstream os;

            EXPECT( 0 == run( pass, {  "-c", "y" }, os ) );

            EXPECT( std::string::npos != os.str().find( "1 " ) );
        }
    },

    CASE( "Option -g,--list-tags lists tags of selected tests [commandline]" )
    {
        test pass[] = {{ CASE_E( "a [b][c]"  ) { ; } },
                       { CASE_E( "[x] y [z]" ) { ; } }};

        {   std::ostringstream os;

            EXPECT( 0 == run( pass, {  "-g"         }, os ) );
            EXPECT( 0 == run( pass, { "--list-tags" }, os ) );

            EXPECT( std::string::npos != os.str().find( "[b]" ) );
            EXPECT( std::string::npos != os.str().find( "[c]" ) );
            EXPECT( std::string::npos != os.str().find( "[x]" ) );
            EXPECT( std::string::npos != os.str().find( "[z]" ) );
        }{
            std::ostringstream os;

            EXPECT( 0 == run( pass, {  "-g", "[x]" }, os ) );

            EXPECT( std::string::npos == os.str().find( "[b]" ) );
            EXPECT( std::string::npos == os.str().find( "[c]" ) );
            EXPECT( std::string::npos != os.str().find( "[x]" ) );
            EXPECT( std::string::npos != os.str().find( "[z]" ) );
        }
    },

    CASE( "Option -l,--list-tests lists selected tests [commandline]" )
    {
        test pass[] = {{ CASE_E( "a b c" ) { ; } },
                       { CASE_E( "x y z" ) { ; } }};

        {   std::ostringstream os;

            EXPECT( 0 == run( pass, {  "-l"          }, os ) );
            EXPECT( 0 == run( pass, { "--list-tests" }, os ) );

            EXPECT( std::string::npos != os.str().find( "a b c" ) );
            EXPECT( std::string::npos != os.str().find( "x y z" ) );
        }{
            std::ostringstream os;

            EXPECT( 0 == run( pass, {  "-l", "b" }, os ) );

            EXPECT( std::string::npos != os.str().find( "b" ) );
            EXPECT( std::string::npos == os.str().find( "y" ) );
        }
    },

    CASE( "Option -p,--pass also reports passing selected tests [commandline]" )
    {
        test pass[] = {{ CASE( "a b c" ) { EXPECT( true ); } }};

        {   std::ostringstream os;

            EXPECT( 0 == run( pass, { "-p" }, os ) );

            EXPECT( std::string::npos != os.str().find( "a b c"  ) );
            EXPECT( std::string::npos != os.str().find( "passed" ) );
        }
        {
            std::ostringstream os;

            EXPECT( 0 == run( pass, { "--pass" }, os ) );

            EXPECT( std::string::npos != os.str().find( "a b c"  ) );
            EXPECT( std::string::npos != os.str().find( "passed" ) );
        }
    },

    CASE( "Option -t,--time reports execution time of selected tests [commandline]" )
    {
        test pass[] = {{ CASE( "a b c" ) { EXPECT( true ); } }};

        {   std::ostringstream os;

            EXPECT( 0 == run( pass, { "-t" }, os ) );

            EXPECT( std::string::npos != os.str().find( "a b c"   ) );
            EXPECT( std::string::npos != os.str().find( "ms:"     ) );
            EXPECT( std::string::npos != os.str().find( "Elapsed" ) );
        }
        {
            std::ostringstream os;

            EXPECT( 0 == run( pass, { "--time" }, os ) );

            EXPECT( std::string::npos != os.str().find( "a b c"   ) );
            EXPECT( std::string::npos != os.str().find( "ms:"     ) );
            EXPECT( std::string::npos != os.str().find( "Elapsed" ) );
        }
    },

    CASE( "Option --order=declared tests in source code order [commandline]" )
    {
        test pass[] = {{ CASE_E( "b" ) { ; } },
                       { CASE_E( "a" ) { ; } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, { "--list-tests", "--order=declared" }, os ) );

        EXPECT( std::string::npos != os.str().find( "b\na" ) );
    },

    CASE( "Option --order=lexical tests in sorted order [commandline]" )
    {
        test pass[] = {{ CASE_E( "b" ) { ; } },
                       { CASE_E( "a" ) { ; } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, { "--list-tests", "--order=lexical" }, os ) );

        EXPECT( std::string::npos != os.str().find( "a\nb" ) );
    },

    CASE( "Option --order=random tests in random order, seed:0..9 [commandline]" )
    {
        test pass[] = {{ CASE_E( "b" ) { ; } },
                       { CASE_E( "a" ) { ; } }};

        const int N = 10;

        int i = 0;
        while ( i++ < N )
        {
            std::ostringstream os;

            text opt_seed = "--random-seed=" + to_string( i );

            EXPECT( 0 == run( pass, { "--list-tests", "--order=random", opt_seed.c_str() }, os ) );

            if ( std::string::npos != os.str().find( "a\nb" ) )
                break;
        }
        EXPECT( i < N ); //  "no randomness observed after N tries";
    },

    CASE( "Option --order=random tests in random order, seed:time [commandline]" )
    {
        test pass[] = {{ CASE_E( "b" ) { ; } },
                       { CASE_E( "a" ) { ; } }};

        const int N = 1000;

        int i = 0;
        while( i++ < N )
        {
            std::ostringstream os;

            EXPECT( 0 == run( pass, { "--list-tests", "--order=random", "--random-seed=time" }, os ) );

            if ( std::string::npos != os.str().find( "a\nb" ) )
                break;
        }
        EXPECT( i < N ); //  "no randomness observed after N tries";
    },

    CASE( "Option --order=foo is recognised as invalid [commandline]" )
    {
        std::ostringstream os;

        EXPECT( 1 == run( { }, { "--list-tests", "--order=foo" }, os ) );

        EXPECT( std::string::npos != os.str().find( "Error" ) );
    },

    CASE( "Option --random-seed=time is recognised [commandline]" )
    {
        std::ostringstream os;

        EXPECT( 0 == run( { }, { "--list-tests", "--random-seed=time" }, os ) );
    },

    CASE( "Option --random-seed=N is recognised [commandline]" )
    {
        std::ostringstream os;

        EXPECT( 0 == run( { }, { "--list-tests", "--random-seed=42" }, os ) );
    },

    CASE( "Option --random-seed={negative-number} is recognised as invalid [commandline]" )
    {
        std::ostringstream os;

        EXPECT( 1 == run( { }, { "--list-tests", "--random-seed=-1" }, os ) );

        EXPECT( std::string::npos != os.str().find( "Error" ) );
    },

    CASE( "Option --random-seed=no-time/no-num is recognised as invalid [commandline]" )
    {
        std::ostringstream os;

        EXPECT( 1 == run( { }, { "--list-tests", "--random-seed=1x" }, os ) );
        EXPECT( 1 == run( { }, { "--list-tests", "--random-seed=x1" }, os ) );

        EXPECT( std::string::npos != os.str().find( "Error" ) );
    },

    CASE( "Option --repeat=N is recognised [commandline]" )
    {
        std::ostringstream os;

        EXPECT( 0 == run( { }, { "--repeat=42" }, os ) );
    },

    CASE( "Option --repeat=3 repeats 3x [commandline]" )
    {
        test fail[] = {{ CASE("F") { EXPECT( false ); } }};

        std::ostringstream os;

        EXPECT( 3 == run( fail, { "--repeat=3" }, os ) );
    },

    CASE( "Option --repeat=-1 (indefinite) is recognised [commandline]" )
    {
        test fail[] = {{ CASE("F") { EXPECT( false ); } }};

        std::ostringstream os;

        // currently no-tests are also repeated indefinitely hence the aborting a failing test:

        EXPECT( 1 == run( fail, { "--abort", "--repeat=-1" }, os ) );

        EXPECT( std::string::npos == os.str().find( "Error" ) );
    },

    CASE( "Option --repeat={negative-number} is recognised as invalid [commandline]" )
    {
        std::ostringstream os;

        EXPECT( 1 == run( { }, { "--repeat=-3" }, os ) );

        EXPECT( std::string::npos != os.str().find( "Error" ) );
    },

    CASE( "Option --version is recognised [commandline]" )
    {
        std::ostringstream os;

        EXPECT( 0 == run( { }, { "--version" }, os ) );
    },

    CASE( "Option -- ends option section [commandline]" )
    {
        test pass[] = {{ CASE_E( "a-b" ) { ; } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, { "-l", "--", "-" }, os ) );
        EXPECT( 0 == run( pass, { "-c", "--", "-" }, os ) );

        EXPECT( std::string::npos != os.str().find( "a-b" ) );
        EXPECT( std::string::npos != os.str().find( "1 " ) );
    },
};

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv );
}

// cl -nologo -Wall -EHsc -I.. test_lest.cpp && test_lest
// g++ -Wall -Wextra -std=c++11 -I.. -o test_lest.exe test_lest.cpp && test_lest

// test_lest "spec" "!spec""
