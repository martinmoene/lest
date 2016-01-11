// Copyright 2016 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if __cplusplus >= 201103 || _MSC_VER > 1800
# include "lest_expect_abort.hpp"
#else
# include "lest_expect_abort_cpp03.hpp"
#endif

#include <cassert>

#define CASE( name ) lest_CASE( specification, name )

static lest::tests specification;

struct user_type{};

#if __cplusplus >= 201103 || _MSC_VER > 1800
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

#if __cplusplus >= 201103 || _MSC_VER > 1800

CASE( "Expect_aborts succeeds for assert(false) in user noexcept function " "[pass]" )
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
cl -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I../ -I../../../ 01_specification.cpp && 01_specification.exe --pass
// pre VC14 (VS2015)
cl -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I../ -I../../../ 01_specification.cpp /link /FORCE:MULTIPLE && 01_specification.exe --pass
// suppress portability/security warnings
cl -W3 -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -I../ -I../../../ 01_specification.cpp /link /FORCE:MULTIPLE && 01_specification.exe --pass

// GNUC:
g++ -Wall -std=c++03 -Dlest_FEATURE_AUTO_REGISTER=1 -I../ -I../../../ -o 01_specification.exe 01_specification.cpp && 01_specification.exe --pass
g++ -Wall -std=c++11 -Dlest_FEATURE_AUTO_REGISTER=1 -I../ -I../../../ -o 01_specification.exe 01_specification.cpp && 01_specification.exe --pass
#endif

// Output:
// 01_specification.cpp:31: failed: __cplusplus: __cplusplus == 0 for 201103 == 0
// 01_specification.cpp:38: passed: aborted: Expect_aborts succeeds for ::abort() [pass]: ::abort()
// 01_specification.cpp:43: passed: aborted: Expect_aborts succeeds for std::abort() [pass]: std::abort()
// 01_specification.cpp:48: passed: aborted: Expect_aborts succeeds for assert(false) [pass]: assert( false )
// 01_specification.cpp:53: failed: didn't abort: Expect_aborts reports assert(true) [fail]: assert( true )
// 01_specification.cpp:60: passed: aborted: Expect_aborts succeeds for assert(false) in user noexcept function[pass]: user_noexcept()
// 01_specification.cpp:66: failed: got unexpected exception with message "augh": Expect_aborts reports an unexpected standard exception [fail]: throw std::runtime_error("augh")
// 01_specification.cpp:74: failed: got unexpected exception of unknown type: Expect_aborts reports an unexpected non-standard exception [fail]: throw user_type{}
// 01_specification.cpp:82: passed: didn't abort: Expect_no_abort succeeds for assert(true) [pass]: assert( true )
// 01_specification.cpp:87: failed: aborted: Expect_no_abort reports ::abort() [fail]: ::abort()
// 01_specification.cpp:92: failed: aborted: Expect_no_abort reports std::abort() [fail]: std::abort()
// 01_specification.cpp:97: failed: aborted: Expect_no_abort reports assert(false) [fail]: assert( false )
// 01_specification.cpp:102: failed: got unexpected exception with message "augh": Expect_no_abort reports an unexpected standard exception [fail]: throw std::runtime_error("augh")
// 01_specification.cpp:110: failed: got unexpected exception of unknown type: Expect_no_abort reports an unexpected non-standard exception [fail]: throw user_type{}
// 9 out of 14 selected tests failed.

// Death test for abort/assert - divert std::abort
//
// Expect_aborts succeeds for ::abort() [pass]
// Expect_aborts succeeds for std::abort() [pass]
// Expect_aborts succeeds for assert(false) [pass]
// Expect_aborts reports assert(true) [fail]
// Expect_aborts succeeds for assert(false) in user noexcept function[pass]
// Expect_aborts reports an unexpected standard exception [fail]
// Expect_aborts reports an unexpected non-standard exception [fail]
// Expect_no_abort succeeds for assert(true) [pass]
// Expect_no_abort reports ::abort() [fail]
// Expect_no_abort reports std::abort() [fail]
// Expect_no_abort reports assert(false) [fail]
// Expect_no_abort reports an unexpected standard exception [fail]
// Expect_no_abort reports an unexpected non-standard exception [fail]
