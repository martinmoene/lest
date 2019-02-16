// Copyright 2016 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Your .cpp code that uses assert must include "lest_assert.hpp", here simulated by:

#include "lest/lest_assert.hpp"

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

// End of user code.

#if __cplusplus >= 201103 || _MSC_VER >= 1800
# include "lest/lest_expect_assert.hpp"
#else
# include "lest/lest_expect_assert_cpp03.hpp"
#endif

#define CASE( name ) lest_CASE( specification, name )

static lest::tests specification;

struct user_type{};

// report value of __cplusplus:

CASE( "__cplusplus" )
{
    EXPECT( __cplusplus == 0 );
}

CASE( "Expect_asserts succeeds for assert(false) " "[pass]" )
{
    EXPECT_ASSERTS( assert( false ) );
}

CASE( "Expect_asserts reports assert(true) " "[fail]" )
{
    EXPECT_ASSERTS( assert( true ) );
}

CASE( "Expect_asserts succeeds for assert(false) in non-noexcept function " "[pass]" )
{
    EXPECT_ASSERTS( user() );
}

#if __cplusplus >= 201103 || _MSC_VER >= 1800

CASE( "Expect_asserts terminates for assert(false) in noexcept function " "[.pass]" )
{
    EXPECT_ASSERTS( user_noexcept() );
}
#endif

CASE( "Expect_asserts reports an unexpected standard exception " "[fail]" )
{
    EXPECT_ASSERTS( throw std::runtime_error("augh") );
}

CASE( "Expect_asserts reports an unexpected non-standard exception " "[fail]" )
{
#if __cplusplus < 201103
    EXPECT_ASSERTS( throw user_type() );
#else
    EXPECT_ASSERTS( throw user_type{} );
#endif
}

CASE( "Expect_no_assert succeeds for assert(true) " "[pass]" )
{
    EXPECT_NO_ASSERT( assert( true ) );
}

CASE( "Expect_no_assert reports assert(false) " "[fail]" )
{
    EXPECT_NO_ASSERT( assert( false ) );
}

CASE( "Expect_no_assert reports an unexpected standard exception " "[fail]" )
{
    EXPECT_NO_ASSERT( throw std::runtime_error("augh") );
}

CASE( "Expect_no_assert reports an unexpected non-standard exception " "[fail]" )
{
#if __cplusplus < 201103
    EXPECT_NO_ASSERT( throw user_type() );
#else
    EXPECT_NO_ASSERT( throw user_type{} );
#endif
}

// run tests:

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv /*, std::cout */ );
}

#if 0
// VC:
cl -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I.. -I../../../include 01_specification.cpp && 01_specification.exe --pass
// suppress portability/security warnings
cl -W3 -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -I.. -I../../../include 01_specification.cpp && 01_specification.exe --pass
// Note: omit 'c' (extern "C" defaults to nothrow) in -EHsc when you throw through C functions
cl -O2 -W3 -EHs -Dlest_FEATURE_AUTO_REGISTER=1 -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -I.. -I../../../include 01_specification.cpp && 01_specification.exe --pass

// GNUC:
g++ -Wall -Wextra -std=c++03 -Dlest_FEATURE_AUTO_REGISTER=1 -I.. -I../../../include -o 01_specification.exe 01_specification.cpp && 01_specification.exe --pass
g++ -Wall -Wextra -std=c++11 -Dlest_FEATURE_AUTO_REGISTER=1 -I.. -I../../../include -o 01_specification.exe 01_specification.cpp && 01_specification.exe --pass
#endif
