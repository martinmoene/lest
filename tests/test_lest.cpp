// Copyright 2013, 2014 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "lest.hpp"
#include <set>

#define TEST_E( name ) \
    name, []( env & )

const lest::test no_using_namespace_lest[] =
{
    TEST( "Ensure namespace lest is specified correctly in lest.hpp [compile-only]" )
    {
        EXPECT(  true );
        EXPECT_NOT(  false );
        EXPECT_THROWS( true );
        EXPECT_THROWS_AS( true, std::exception );
    },
};

using namespace lest;

const lest::test specification[] =
{
    TEST( "Function to suppress warning \"expression has no effect\" acts as identity function" )
    {
        EXPECT( false == is_true( false ) );
        EXPECT(  true == is_true( true  ) );
    },

    TEST( "Function with_message() returns correct string" )
    {
        std::string msg = "Let writing tests become irresistibly easy and attractive.";
        EXPECT( ( "with message \"" + msg + "\"" ) == with_message( msg ) );
    },

    TEST( "Function of_type() returns correct string" )
    {
        std::string msg = "this_type";
        EXPECT( ( "of type " + msg ) == of_type( msg ) );
    },

    TEST( "Function pluralise() adds 's' except for 1 item" )
    {
        std::string word = "hammer";
        EXPECT( word == pluralise( 1, word ) );
        for ( auto i : {0,2,3,4,5,6,7,8,9,10,11,12} )
        {
            EXPECT( ( word + "s" ) == pluralise( i, word ) );
        }
    },

    TEST( "Location constructs properly" )
    {
        char const * file = __FILE__; int line = __LINE__;
        location where{ file, line };
        EXPECT( file == where.file );
        EXPECT( line == where.line );
    },

    TEST( "Comment constructs properly" )
    {
        std::string info = __FILE__;
        comment note = info;
        EXPECT( info == note.info );
    },

    TEST( "Comment converted to bool indicates absence or presence of comment" )
    {
        EXPECT( false == bool( comment( "") ) );
        EXPECT(  true == bool( comment("x") ) );
    },

    TEST( "Failure exception type constructs and prints properly" )
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

    TEST( "Expected exception type constructs and prints properly" )
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

    TEST( "Unexpected exception type constructs and prints properly" )
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

    TEST( "Expect generates no message exception for a succeeding test" )
    {
        test pass = { TEST( "P" ) { EXPECT( true  ); } };

        try { pass.behaviour( $ ); }
        catch(...) { throw failure(location{__FILE__,__LINE__}, "unexpected error generated", "true"); }
    },

    TEST( "Expect generates a message exception for a failing test")
    {
        test fail = { TEST("F") { EXPECT( false ); } };

        for (;;)
        {
            try { fail.behaviour( $ ); } catch ( message & ) { break; }
            throw failure(location{__FILE__,__LINE__}, "no error generated", "false");
        }
    },

    TEST( "Expect succeeds for success (true) and failure (false)" )
    {
        test pass[] = {{ TEST("P") { EXPECT( true  ); } }};
        test fail[] = {{ TEST("F") { EXPECT( false ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );
    },

    TEST( "Expect-not succeeds for success (false) and failure (true)" )
    {
        test pass[] = {{ TEST("P") { EXPECT_NOT( false ); } }};
        test fail[] = {{ TEST("F") { EXPECT_NOT( true  ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );
    },

    TEST( "Expect succeeds for integer comparation" )
    {
        test pass  [] = {{ TEST( "P"  ) { EXPECT( 7 == 7 ); EXPECT( 7 != 8 );
                                          EXPECT( 7 >= 6 ); EXPECT( 7 <= 8 );
                                          EXPECT( 7 >  6 ); EXPECT( 7 <  8 ); } }};
        test fail_1[] = {{ TEST( "F1" ) { EXPECT( 7 == 8 ); } }};
        test fail_2[] = {{ TEST( "F2" ) { EXPECT( 7 != 7 ); } }};
        test fail_3[] = {{ TEST( "F3" ) { EXPECT( 7 <= 6 ); } }};
        test fail_4[] = {{ TEST( "F4" ) { EXPECT( 7 >= 8 ); } }};
        test fail_5[] = {{ TEST( "F5" ) { EXPECT( 7 <  6 ); } }};
        test fail_6[] = {{ TEST( "F6" ) { EXPECT( 7 >  8 ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass  , os ) );
        EXPECT( 1 == run( fail_1, os ) );
        EXPECT( 1 == run( fail_2, os ) );
        EXPECT( 1 == run( fail_3, os ) );
        EXPECT( 1 == run( fail_4, os ) );
        EXPECT( 1 == run( fail_5, os ) );
        EXPECT( 1 == run( fail_6, os ) );
    },

    TEST( "Expect succeeds for mixed integer, real comparation" )
    {
        test pass  [] = {{ TEST( "P"  ) { EXPECT( 7.0 == 7   ); EXPECT( 7.0 != 8   );
                                          EXPECT( 7   == 7.0 ); EXPECT( 7   != 8.0 );} }};
        test fail_1[] = {{ TEST( "F1" ) { EXPECT( 7.0 == 8   ); } }};
        test fail_2[] = {{ TEST( "F2" ) { EXPECT( 7  !=  7.0 ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass  , os ) );
        EXPECT( 1 == run( fail_1, os ) );
        EXPECT( 1 == run( fail_2, os ) );
    },

    TEST( "Expect succeeds for string comparation" )
    {
        std::string a("a"); std::string b("b");
        test pass  [] = {{ TEST( "P", =  ) { EXPECT( a == a ); EXPECT( a != b );
                                             EXPECT( b >= a ); EXPECT( a <= b );
                                             EXPECT( b >  a ); EXPECT( a <  b ); } }};
        test fail_1[] = {{ TEST( "F1", = ) { EXPECT( a == b ); } }};
        test fail_2[] = {{ TEST( "F2", = ) { EXPECT( a != a ); } }};
        test fail_3[] = {{ TEST( "F3", = ) { EXPECT( b <= a ); } }};
        test fail_4[] = {{ TEST( "F4", = ) { EXPECT( a >= b ); } }};
        test fail_5[] = {{ TEST( "F5", = ) { EXPECT( b <  a ); } }};
        test fail_6[] = {{ TEST( "F6", = ) { EXPECT( a >  b ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass  , os ) );
        EXPECT( 1 == run( fail_1, os ) );
        EXPECT( 1 == run( fail_2, os ) );
        EXPECT( 1 == run( fail_3, os ) );
        EXPECT( 1 == run( fail_4, os ) );
        EXPECT( 1 == run( fail_5, os ) );
        EXPECT( 1 == run( fail_6, os ) );
    },

    TEST( "Function run() returns the right failure count" )
    {
        test pass  [] = {{ TEST( "P"  ) { EXPECT( 1==1 ); } }};
        test fail_1[] = {{ TEST( "F1" ) { EXPECT( 0==1 ); } }};
        test fail_3[] = {{ TEST( "F1" ) { EXPECT( 0==1 ); } },
                         { TEST( "F2" ) { EXPECT( 0==1 ); } },
                         { TEST( "F3" ) { EXPECT( 0==1 ); } },};

        std::ostringstream os;

        EXPECT( 0 == run( pass  , os ) );
        EXPECT( 1 == run( fail_1, os ) );
        EXPECT( 3 == run( fail_3, os ) );
    },

    TEST( "Expect succeeds with an unexpected standard exception" )
    {
        std::string text = "hello-world";
        test pass[] = {{ TEST( "P", = ) { EXPECT( (throw std::runtime_error(text), true) ); } }};

        std::ostringstream os;

        EXPECT( 1 == run( pass, os ) );
        EXPECT( std::string::npos != os.str().find(text) );
    },

    TEST( "Expect succeeds with an unexpected non-standard exception" )
    {
        test pass[] = {{ TEST( "P" ) { EXPECT( (throw 77, true) ); } }};

        std::ostringstream os;

        EXPECT( 1 == run( pass, os ) );
    },

    TEST( "Expect_throws succeeds with an expected standard exception" )
    {
        std::string text = "hello-world";
        test pass[] = {{ TEST( "P", = ) { EXPECT_THROWS( (throw std::runtime_error(text), true) ); } }};
        test fail[] = {{ TEST( "F"    ) { EXPECT_THROWS(  true ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );
    },

    TEST( "Expect_throws succeeds with an expected non-standard exception" )
    {
        test pass[] = {{ TEST( "P" ) { EXPECT_THROWS( (throw 77, true) ); } }};
        test fail[] = {{ TEST( "F" ) { EXPECT_THROWS(  true ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );
    },

    TEST( "Expect_throws_as succeeds with a specific expected standard exception" )
    {
        test pass[] = {{ TEST( "P" ) { EXPECT_THROWS_AS( (throw std::bad_alloc(), true), std::bad_alloc ); } }};
        test fail[] = {{ TEST( "F" ) { EXPECT_THROWS_AS( (throw std::bad_alloc(), true), std::runtime_error ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );
    },

    TEST( "Expect_throws_as succeeds with a specific expected non-standard exception" )
    {
        test pass[] = {{ TEST( "P" ) { EXPECT_THROWS_AS( (throw 77, true), int ); } }};
        test fail[] = {{ TEST( "F" ) { EXPECT_THROWS_AS( (throw 77, true), std::runtime_error ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );
    },

    TEST( "Decomposition formats nullptr as string" )
    {
        test pass[] = {{ TEST( "P" ) { EXPECT(  nullptr == nullptr  ); } }};
        test fail[] = {{ TEST( "F" ) { EXPECT( (void*)1 == nullptr  ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );

        EXPECT( std::string::npos != os.str().find( "1 == nullptr" ) );
    },

    TEST( "Decomposition formats boolean as strings true and false" )
    {
        test pass[] = {{ TEST( "P" ) { EXPECT( true == true  ); } }};
        test fail[] = {{ TEST( "F" ) { EXPECT( true == false ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );

        EXPECT( std::string::npos != os.str().find( "true == false for true == false" ) );
    },

    TEST( "Decomposition formats character with single quotes" )
    {
        test pass[] = {{ TEST( "P" ) { EXPECT( 'a' < 'b' ); } }};
        test fail[] = {{ TEST( "F" ) { EXPECT( 'b' < 'a' ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );

        EXPECT( std::string::npos != os.str().find( "'b' < 'a' for 'b' < 'a'" ) );
    },

    TEST( "Decomposition formats std::string with double quotes" )
    {
        std::string hello( "hello" );
        std::string world( "world" );

        test pass[] = {{ TEST( "P",= ) { EXPECT( hello < "world" ); } }};
        test fail[] = {{ TEST( "F",= ) { EXPECT( world < "hello" ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );

        EXPECT( std::string::npos != os.str().find( "world < \"hello\" for \"world\" < \"hello\"" ) );
    },

    TEST( "Decomposition formats C string with double quotes" )
    {
        char const * hello( "hello" ); std::string std_hello( "hello" );
        char const * world( "world" ); std::string std_world( "world" );

        test pass[] = {{ TEST( "P", = ) { EXPECT( hello < std_world ); } }};
        test fail[] = {{ TEST( "F", = ) { EXPECT( world < std_hello ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );

        EXPECT( std::string::npos != os.str().find( "world < std_hello for \"world\" < \"hello\"" ) );
    },

    TEST( "Decomposition formats container with curly braces" )
    {
        std::set<int> s{ 1, 2, 3, };
        std::set<int> t{ 2, 1, 0, };

        test pass[] = {{ TEST( "P", = ) { EXPECT( s == s ); } }};
        test fail[] = {{ TEST( "F", = ) { EXPECT( s == t ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );

        EXPECT( std::string::npos != os.str().find( "{ 1, 2, 3, }" ) );
        EXPECT( std::string::npos != os.str().find( "{ 0, 1, 2, }" ) );
    },

    TEST( "Has single expression evaluation" )
    {
        test pass[] = {{ TEST( "P" ) { int n = 0; EXPECT( 1 == ++n ); } }};
        test fail[] = {{ TEST( "F" ) { int n = 0; EXPECT( 2 == ++n ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );

        EXPECT( std::string::npos != os.str().find( "for 2 == 1" ) );
    },

    TEST( "Approximate compares properly" )
    {
        EXPECT( 1.23 == approx( 1.23 ) );
        EXPECT( 1.23 != approx( 1.24 ) );
    },

    TEST( "Approximate using epsilon compares properly" )
    {
        EXPECT( 1.23 != approx( 1.231 ) );
        EXPECT( 1.23 == approx( 1.231 ).epsilon( 0.1 ) );
    },

    TEST( "Approximate using custom epsilon compares properly" )
    {
        approx custom = approx::custom().epsilon( 0.1 );

        EXPECT( approx( 1.231 ) != 1.23 );
        EXPECT( custom( 1.231 ) == 1.23 );
    },

    TEST( "Approximate to Pi compares properly" )
    {
        auto divide = []( double a, double b ) { return a / b; };

        EXPECT( divide( 22, 7 ) == approx( 3.141 ).epsilon( 0.001  ) );
        EXPECT( divide( 22, 7 ) != approx( 3.141 ).epsilon( 0.0001 ) );
    },

    TEST( "Skips tests tagged [.]" )
    {
        EXPECT( false );
    },

    TEST( "Skips tests tagged [hide]" )
    {
        EXPECT( false );
    },

#if lest_FEATURE_REGEX_SEARCH

    TEST( "Test specifications select tests [commandline]" )
    {
        test fail[] = {{ TEST( "Hello world [tag1]"  ) { EXPECT( false ); } },
                       { TEST( "Good morning [tag1]" ) { EXPECT( false ); } },
                       { TEST( "Good noon [tag2]"    ) { EXPECT( false ); } },
                       { TEST( "Good bye tags"       ) { EXPECT( false ); } }};

        std::ostringstream os;

        EXPECT( 1 == run( fail, { "world"    }, os ) );
        EXPECT( 2 == run( fail, { "1\\]"     }, os ) );
        EXPECT( 3 == run( fail, { "\\[.*\\]" }, os ) );
    },

    TEST( "Test specifications omit tests [commandline]" )
    {
        test fail[] = {{ TEST( "Hello world [tag1]"  ) { EXPECT( false ); } },
                       { TEST( "Good morning [tag1]" ) { EXPECT( false ); } },
                       { TEST( "Good noon [tag2]"    ) { EXPECT( false ); } },
                       { TEST( "Good bye tags"       ) { EXPECT( false ); } }};

        std::ostringstream os;

        EXPECT( 3 == run( fail, { "!world"    }, os ) );
        EXPECT( 2 == run( fail, { "!1\\]"     }, os ) );
        EXPECT( 1 == run( fail, { "!\\[.*\\]" }, os ) );
    },

    TEST( "Test specification series select tests [commandline]" )
    {
        test fail[] = {{ TEST( "a [x1]"   ) { EXPECT( false ); } },
                       { TEST( "b [x1]"   ) { EXPECT( false ); } },
                       { TEST( "c [x2]"   ) { EXPECT( false ); } },
                       { TEST( "d [hide]" ) { EXPECT( false ); } },
                       { TEST( "e [.]"    ) { EXPECT( false ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( fail, { "!\\[x"              }, os ) );
        EXPECT( 1 == run( fail, { "!\\[x1"             }, os ) );
        EXPECT( 1 == run( fail, { "!\\[x"    , "\\[x2" }, os ) );
        EXPECT( 1 == run( fail, { "\\[\\.\\]", "!\\[x" }, os ) );
        EXPECT( 2 == run( fail, { "*"        , "!\\[x" }, os ) );
    },
#else // regex_search:

    TEST( "Test specifications select tests [commandline]" )
    {
        test fail[] = {{ TEST( "Hello world [tag1]"  ) { EXPECT( false ); } },
                       { TEST( "Good morning [tag1]" ) { EXPECT( false ); } },
                       { TEST( "Good noon [tag2]"    ) { EXPECT( false ); } },
                       { TEST( "Good bye tags"       ) { EXPECT( false ); } }};

        std::ostringstream os;

        EXPECT( 1 == run( fail, {  "Hello" }, os ) );
        EXPECT( 2 == run( fail, { "[tag1]" }, os ) );
        EXPECT( 1 == run( fail, { "[tag2]" }, os ) );

        EXPECT( 4 == run( fail, {           }, os ) );
        EXPECT( 4 == run( fail, { "*"       }, os ) );
        EXPECT( 4 == run( fail, { "^\\*$"   }, os ) );
        EXPECT( 0 == run( fail, { "AAA*BBB" }, os ) );
    },

    TEST( "Test specifications omit tests [commandline]" )
    {
        test fail[] = {{ TEST( "Hello world [tag1]"  ) { EXPECT( false ); } },
                       { TEST( "Good morning [tag1]" ) { EXPECT( false ); } },
                       { TEST( "Good bye [tag2]"     ) { EXPECT( false ); } }};

        std::ostringstream os;

        EXPECT( 1 == run( fail, { "![tag1]" }, os ) );
        EXPECT( 2 == run( fail, { "![tag2]" }, os ) );
    },

    TEST( "Test specification series select tests [commandline]" )
    {
        test fail[] = {{ TEST( "a [x1]"   ) { EXPECT( false ); } },
                       { TEST( "b [x1]"   ) { EXPECT( false ); } },
                       { TEST( "c [x2]"   ) { EXPECT( false ); } },
                       { TEST( "d [hide]" ) { EXPECT( false ); } },
                       { TEST( "e [.]"    ) { EXPECT( false ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( fail, { "![x"        }, os ) );
        EXPECT( 1 == run( fail, { "![x1"       }, os ) );
        EXPECT( 1 == run( fail, { "![x", "[x2" }, os ) );
        EXPECT( 1 == run( fail, { "[.]", "![x" }, os ) );
        EXPECT( 2 == run( fail, { "*"  , "![x" }, os ) );
    },
#endif

    TEST( "Unrecognised option recognised as such [commandline]" )
    {
        test fail[] = {{ TEST_E( "" ) { ; } }};

        std::ostringstream os;

        EXPECT( 1 == run( fail, { "--nonexisting-option" }, os ) );
    },

    TEST( "Option -h,--help show help message [commandline]" )
    {
        test pass[] = {{ TEST_E( "" ) { ; } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, {  "-h"    }, os ) );
        EXPECT( 0 == run( pass, { "--help" }, os ) );
    },

    TEST( "Option -a,--abort selected tests [commandline]" )
    {
        test fail[] = {{ TEST( "" ) { EXPECT( false ); } },
                       { TEST( "" ) { EXPECT( false ); } }};

        std::ostringstream os;

        EXPECT( 2 == run( fail, {           }, os ) );
        EXPECT( 1 == run( fail, {  "-a"     }, os ) );
        EXPECT( 1 == run( fail, { "--abort" }, os ) );
    },

    TEST( "Option -c,--count selected tests [commandline]" )
    {
        test pass[] = {{ TEST_E( "a b c" ) { ; } },
                       { TEST_E( "x y z" ) { ; } }};

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

    TEST( "Option -l,--list list selected tests [commandline]" )
    {
        test pass[] = {{ TEST_E( "a b c" ) { ; } },
                       { TEST_E( "x y z" ) { ; } }};

        {   std::ostringstream os;

            EXPECT( 0 == run( pass, {  "-l"    }, os ) );
            EXPECT( 0 == run( pass, { "--list" }, os ) );

            EXPECT( std::string::npos != os.str().find( "a b c" ) );
            EXPECT( std::string::npos != os.str().find( "x y z" ) );
        }{
            std::ostringstream os;

            EXPECT( 0 == run( pass, {  "-l", "b" }, os ) );

            EXPECT( std::string::npos != os.str().find( "b" ) );
            EXPECT( std::string::npos == os.str().find( "y" ) );
        }
    },

    TEST( "Option -p,--pass also reports passing selected tests [commandline]" )
    {
        test pass[] = {{ TEST( "a b c" ) { EXPECT( true ); } }};

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

    TEST( "Option -- ends option section [commandline]" )
    {
        test pass[] = {{ TEST_E( "a-b" ) { ; } }};

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
