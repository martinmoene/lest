// Copyright 2016 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if __cplusplus >= 201103 || _MSC_VER >= 1800
# include "lest/lest_expect_abort.hpp"
#else
# include "lest/lest_expect_abort_cpp03.hpp"
#endif

#include <cassert>

#define CASE( name ) lest_CASE( specification, name )

static lest::tests specification;

struct user_type{};

void user()
{
    assert( false );
}

#if __cplusplus >= 201103 || _MSC_VER >= 1800
void user_noexcept() noexcept
{
    assert( false );
}
#endif

// report value of __cplusplus:

CASE( "__cplusplus" )
{
    EXPECT( __cplusplus == 0 );
}

// test for abort:

CASE( "Expect_aborts succeeds for ::abort() " "[pass]" )
{
    EXPECT_ABORTS( ::abort() );
}

CASE( "Expect_aborts succeeds for std::abort() " "[pass]" )
{
    EXPECT_ABORTS( std::abort() );
}

CASE( "Expect_aborts succeeds for assert(false) " "[pass]" )
{
    EXPECT_ABORTS( assert( false ) );
}

CASE( "Expect_aborts reports assert(true) " "[fail]" )
{
    EXPECT_ABORTS( assert( true ) );
}

CASE( "Expect_asserts succeeds for assert(false) in non-noexcept function " "[pass]" )
{
    EXPECT_ABORTS( user() );
}

#if __cplusplus >= 201103 || _MSC_VER >= 1800
// VC: compile with -EHs (no 'c')

CASE( "Expect_aborts terminates for assert(false) in noexcept function " "[.pass]" )
{
    EXPECT_ABORTS( user_noexcept() );
}
#endif

CASE( "Expect_aborts reports an unexpected standard exception " "[fail]" )
{
    EXPECT_ABORTS( throw std::runtime_error("augh") );
}

CASE( "Expect_aborts reports an unexpected non-standard exception " "[fail]" )
{
#if __cplusplus < 201103
    EXPECT_ABORTS( throw user_type() );
#else
    EXPECT_ABORTS( throw user_type{} );
#endif
}

// test for no abort:

CASE( "Expect_no_abort succeeds for assert(true) " "[pass]" )
{
    EXPECT_NO_ABORT( assert( true ) );
}

CASE( "Expect_no_abort reports ::abort() " "[fail]" )
{
    EXPECT_NO_ABORT( ::abort() );
}

CASE( "Expect_no_abort reports std::abort() " "[fail]" )
{
    EXPECT_NO_ABORT( std::abort() );
}

CASE( "Expect_no_abort reports assert(false) " "[fail]" )
{
    EXPECT_NO_ABORT( assert( false ) );
}

CASE( "Expect_no_abort reports an unexpected standard exception " "[fail]" )
{
    EXPECT_NO_ABORT( throw std::runtime_error("augh") );
}

CASE( "Expect_no_abort reports an unexpected non-standard exception " "[fail]" )
{
#if __cplusplus < 201103
    EXPECT_NO_ABORT( throw user_type() );
#else
    EXPECT_NO_ABORT( throw user_type{} );
#endif
}

// run tests:

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv /*, std::cout */ );
}

#if 0
// VC14 (VS2015):
cl -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I.. -I../../../include lest_expect_abort.t.cpp && lest_expect_abort.t.exe --pass
// pre VC14 (VS2015)
cl -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I.. -I../../../include lest_expect_abort.t.cpp /link /FORCE:MULTIPLE && lest_expect_abort.t.exe --pass
// suppress portability/security warnings
cl -W3 -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -I.. -I../../../include lest_expect_abort.t.cpp /link /FORCE:MULTIPLE && lest_expect_abort.t.exe --pass
// Note: omit 'c' (extern "C" defaults to nothrow) in -EHsc when you throw through C
cl -W3 -EHs -Dlest_FEATURE_AUTO_REGISTER=1 -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -I.. -I../../../include lest_expect_abort.t.cpp /link /FORCE:MULTIPLE && lest_expect_abort.t.exe --pass

// GNUC:
g++ -Wall -std=c++03 -Dlest_FEATURE_AUTO_REGISTER=1 -I.. -I../../../include -o lest_expect_abort_cpp03.t.exe lest_expect_abort.t.cpp && lest_expect_abort_cpp03.t.exe --pass
g++ -Wall -std=c++11 -Dlest_FEATURE_AUTO_REGISTER=1 -I.. -I../../../include -o lest_expect_abort.t.exe       lest_expect_abort.t.cpp && lest_expect_abort.t.exe       --pass
#endif
