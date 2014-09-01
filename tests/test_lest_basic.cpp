// Copyright 2013, 2014 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "lest_basic.hpp"
#include <sstream>

using namespace lest;

const lest::test specification[] =
{
    "Function to suppress warning \"expression has no effect\" acts as identity function", []
    {
        EXPECT( false == is_true( false ) );
        EXPECT(  true == is_true( true  ) );
    },

    "Function with_message() returns correct string", []
    {
        std::string msg = "Let writing tests become irresistibly easy and attractive.";
        EXPECT( "with message \"" + msg + "\"" == with_message( msg ) );
    },

    "Function of_type() returns correct string", []
    {
        std::string msg = "this_type";
        EXPECT( "of type " + msg == of_type( msg ) );
    },

    "Function pluralise() adds 's' except for 1 item", []
    {
        std::string word = "hammer";
        EXPECT( word == pluralise( 1, word ) );
        for ( auto i : {0,2,3,4,5,6,7,8,9,10,11,12} )
        {
            EXPECT( word + "s" == pluralise( i, word ) );
        }
    },

    "Location constructs properly", []
    {
        char const * file = __FILE__; int line = __LINE__;
        location where{ file, line };
        EXPECT( file == where.file );
        EXPECT( line == where.line );
    },

    "Comment constructs properly", []
    {
        std::string info = __FILE__;
        comment note = info;
        EXPECT( info == note.info );
    },

    "Comment converted to bool indicates absence or presence of comment", []
    {
        EXPECT( false == bool( comment( "") ) );
        EXPECT(  true == bool( comment("x") ) );
    },

    "Failure exception type constructs and prints properly", []
    {
        std::string name = "test-name";
        failure msg( location{"filename.cpp", 765}, "expression" );

        std::ostringstream os;
        report( os, msg, name );

#ifndef __GNUG__
        EXPECT( os.str() == "filename.cpp(765): failed: test-name: expression\n" );
#else
        EXPECT( os.str() == "filename.cpp:765: failed: test-name: expression\n" );
#endif
    },

    "Expected exception type constructs and prints properly", []
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

    "Unexpected exception type constructs and prints properly", []
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

    "Expect generates no message exception for a succeeding test", []
    {
        test pass = { "P", [] { EXPECT( true  ); } };

        try { pass.behaviour(); }
        catch(...) { throw failure(location{__FILE__,__LINE__}, "unexpected error generated"); }
    },

    "Expect generates a message exception for a failing test", []
    {
        test fail = { "F", [] { EXPECT( false ); } };

        for (;;)
        {
            try { fail.behaviour(); } catch ( message & ) { break; }
            throw failure(location{__FILE__,__LINE__}, "no error generated");
        }
    },

    "Expect succeeds for success (true) and failure (false)", []
    {
        test pass[] = {{ "P", [] { EXPECT( true  ); } }};
        test fail[] = {{ "F", [] { EXPECT( false ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );
    },

    "Expect succeeds for integer comparation", []
    {
        test pass  [] = {{ "P" , [] { EXPECT( 7 == 7 ); EXPECT( 7 != 8 );
                                        EXPECT( 7 >= 6 ); EXPECT( 7 <= 8 );
                                        EXPECT( 7 >  6 ); EXPECT( 7 <  8 ); } }};
        test fail_1[] = {{ "F1", [] { EXPECT( 7 == 8 ); } }};
        test fail_2[] = {{ "F2", [] { EXPECT( 7 != 7 ); } }};
        test fail_3[] = {{ "F3", [] { EXPECT( 7 <= 6 ); } }};
        test fail_4[] = {{ "F4", [] { EXPECT( 7 >= 8 ); } }};
        test fail_5[] = {{ "F5", [] { EXPECT( 7 <  6 ); } }};
        test fail_6[] = {{ "F6", [] { EXPECT( 7 >  8 ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass  , os ) );
        EXPECT( 1 == run( fail_1, os ) );
        EXPECT( 1 == run( fail_2, os ) );
        EXPECT( 1 == run( fail_3, os ) );
        EXPECT( 1 == run( fail_4, os ) );
        EXPECT( 1 == run( fail_5, os ) );
        EXPECT( 1 == run( fail_6, os ) );
    },

    "Expect succeeds for string comparation", []
    {
        std::string a("a"); std::string b("b");
        test pass  [] = {{ "P" , [=]() { EXPECT( a == a ); EXPECT( a != b );
                                         EXPECT( b >= a ); EXPECT( a <= b );
                                         EXPECT( b >  a ); EXPECT( a <  b ); } }};
        test fail_1[] = {{ "F1", [=]() { EXPECT( a == b ); } }};
        test fail_2[] = {{ "F2", [=]() { EXPECT( a != a ); } }};
        test fail_3[] = {{ "F3", [=]() { EXPECT( b <= a ); } }};
        test fail_4[] = {{ "F4", [=]() { EXPECT( a >= b ); } }};
        test fail_5[] = {{ "F5", [=]() { EXPECT( b <  a ); } }};
        test fail_6[] = {{ "F6", [=]() { EXPECT( a >  b ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass  , os ) );
        EXPECT( 1 == run( fail_1, os ) );
        EXPECT( 1 == run( fail_2, os ) );
        EXPECT( 1 == run( fail_3, os ) );
        EXPECT( 1 == run( fail_4, os ) );
        EXPECT( 1 == run( fail_5, os ) );
        EXPECT( 1 == run( fail_6, os ) );
    },

    "Function run() returns the right failure count", []
    {
        test pass  [] = {{ "P" , [] { EXPECT( 1==1 ); } }};
        test fail_1[] = {{ "F1", [] { EXPECT( 0==1 ); } }};
        test fail_3[] = {{ "F1", [] { EXPECT( 0==1 ); } },
                         { "F2", [] { EXPECT( 0==1 ); } },
                         { "F3", [] { EXPECT( 0==1 ); } },};

        std::ostringstream os;

        EXPECT( 0 == run( pass  , os ) );
        EXPECT( 1 == run( fail_1, os ) );
        EXPECT( 3 == run( fail_3, os ) );
    },

    "Expect succeeds with an unexpected standard exception", []
    {
        std::string text = "hello-world";
        test pass[] = {{ "P", [=]() { EXPECT( (throw std::runtime_error(text), true) ); } }};

        std::ostringstream os;

        EXPECT( 1 == run( pass, os ) );
        EXPECT( std::string::npos != os.str().find(text) );
    },

    "Expect succeeds with an unexpected non-standard exception", []
    {
        test pass[] = {{ "P", [] { EXPECT( (throw 77, true) ); } }};

        std::ostringstream os;

        EXPECT( 1 == run( pass, os ) );
    },

    "Expect_throws succeeds with an expected standard exception", []
    {
        std::string text = "hello-world";
        test pass[] = {{ "P", [=]() { EXPECT_THROWS( (throw std::runtime_error(text), true) ); } }};
        test fail[] = {{ "F", [ ]() { EXPECT_THROWS(  true ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );
    },

    "Expect_throws succeeds with an expected non-standard exception", []
    {
        test pass[] = {{ "P", [] { EXPECT_THROWS( (throw 77, true) ); } }};
        test fail[] = {{ "F", [] { EXPECT_THROWS(  true ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );
    },

    "Expect_throws_as succeeds with a specific expected standard exception", []
    {
        test pass[] = {{ "P", [] { EXPECT_THROWS_AS( (throw std::bad_alloc(), true), std::bad_alloc ); } }};
        test fail[] = {{ "F", [] { EXPECT_THROWS_AS( (throw std::bad_alloc(), true), std::runtime_error ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );
    },

    "Expect_throws_as succeeds with a specific expected non-standard exception", []
    {
        test pass[] = {{ "P", [] { EXPECT_THROWS_AS( (throw 77, true), int ); } }};
        test fail[] = {{ "F", [] { EXPECT_THROWS_AS( (throw 77, true), std::runtime_error ); } }};

        std::ostringstream os;

        EXPECT( 0 == run( pass, os ) );
        EXPECT( 1 == run( fail, os ) );
    },
};

int main()
{
    return lest::run( specification );
}

// cl -nologo -Wall -EHsc -I.. test_lest_basic.cpp && test_lest_basic
// g++ -Wall -Wextra -std=c++11 -I.. -o test_lest_basic.exe test_lest_basic.cpp && test_lest_basic

