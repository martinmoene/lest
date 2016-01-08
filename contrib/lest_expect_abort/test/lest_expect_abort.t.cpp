// Copyright 2016 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if __cplusplus < 201103
# include "lest_expect_abort_cpp03.hpp"
#else
# include "lest_expect_abort.hpp"
#endif

#include <cassert>

#define CASE( name ) lest_CASE( specification, name )

static lest::tests specification;

struct user_type{};

// report value of __cplusplus:

CASE( "__cplusplus" )
{
    EXPECT( __cplusplus == 0 );
}

// test for abort:

CASE( "Expect_aborts succeeds for std::abort() " "[pass]" )
{
#if lest_COMPILER_MSVC_VERSION == 6
    EXPECT_ABORTS( ::abort() );
#else
    EXPECT_ABORTS( std::abort() );
#endif
}

CASE( "Expect_aborts succeeds for assert(false) " "[pass]" )
{
    EXPECT_ABORTS( assert( false ) );
}

CASE( "Expect_aborts reports assert(true) " "[fail]" )
{
    EXPECT_ABORTS( assert( true ) );
}

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

CASE( "Expect_no_abort reports std::abort() " "[fail]" )
{
#if lest_COMPILER_MSVC_VERSION == 6
    EXPECT_NO_ABORT( ::abort() );
#else
    EXPECT_NO_ABORT( std::abort() );
#endif
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

// Compilation:

#if 0
// VC14 (VS2015)
cl -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I../ -I../../../ lest_expect_abort.t.cpp && lest_expect_abort.t.exe --pass
// pre VC14 (VS2015)
cl -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I../ -I../../../ lest_expect_abort.t.cpp /link /FORCE:MULTIPLE && lest_expect_abort.t.exe --pass

g++ -Wall -std=c++03 -Dlest_FEATURE_AUTO_REGISTER=1 -I../ -I../../../ -o lest_expect_abort_cpp03.t.exe lest_expect_abort.t.cpp && lest_expect_abort_cpp03.t.exe --pass
g++ -Wall -std=c++11 -Dlest_FEATURE_AUTO_REGISTER=1 -I../ -I../../../ -o lest_expect_abort.t.exe       lest_expect_abort.t.cpp && lest_expect_abort.t.exe       --pass
#endif
