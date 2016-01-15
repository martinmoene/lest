// Copyright 2016 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LEST_LEST_EXPECT_ASSERT_HPP_INCLUDED
#define LEST_LEST_EXPECT_ASSERT_HPP_INCLUDED

#include "lest.hpp"

#include <csetjmp>

#if ! defined( lest_NO_SHORT_MACRO_NAMES )
# define EXPECT_NO_ASSERT  lest_EXPECT_NO_ASSERT
# define EXPECT_ASSERTS    lest_EXPECT_ASSERTS
#endif

// "include" lest_assert.hpp:

#ifndef LEST_LEST_ASSERT_HPP_INCLUDED
#define LEST_LEST_ASSERT_HPP_INCLUDED

#ifndef  lest_FEATURE_ASSERT
# define lest_FEATURE_ASSERT  1
#endif

#if lest_FEATURE_ASSERT
# define assert( expr )  ( !!(expr) ? ((void)0) : ::lest::assertion_failed( #expr, lest___FUNC__, __FILE__, __LINE__) )
#else
# include <cassert>
#endif

#if __cplusplus >= 201103 || _MSC_VER >= 1800
# define lest___FUNC__  __func__
#else
# define lest___FUNC__  "(unknown)"
#endif

#endif // LEST_LEST_ASSERT_HPP_INCLUDED

#define lest_EXPECT_NO_ASSERT( expr ) \
    do \
    { \
        try \
        { \
            if ( ! setjmp( lest::assert_env() ) ) \
            { \
                expr; \
            } \
            else \
            { \
                throw lest::asserted{ "failed", lest_LOCATION, #expr }; \
            } \
        } \
        catch (...) \
        { \
            lest::inform( lest_LOCATION, #expr ); \
        } \
        if ( lest_env.pass ) \
            lest::report( lest_env.os, lest::not_asserted{ "passed", lest_LOCATION, #expr }, lest_env.testing ); \
    } while ( lest::is_false() )

#define lest_EXPECT_ASSERTS( expr ) \
    do \
    { \
        try \
        { \
            if ( ! setjmp( lest::assert_env() ) ) \
            { \
                expr; \
            } \
            else \
            { \
                if ( lest_env.pass ) \
                    lest::report( lest_env.os, lest::asserted{ "passed", lest_LOCATION, #expr }, lest_env.testing ); \
                break; \
            } \
        }  \
        catch (...) \
        { \
            lest::inform( lest_LOCATION, #expr ); \
        } \
        throw lest::not_asserted{ "failed", lest_LOCATION, #expr }; \
    } \
    while ( lest::is_false() )

#if _WIN32
# define lest_DEV_NULL "nul"
# else
# define lest_DEV_NULL "/dev/null"
#endif

namespace lest {

struct asserted : message
{
    asserted( text kind, location where, text expr )
    : message{ kind + ": asserted", where, expr } {}
};

struct not_asserted : message
{
    not_asserted( text kind, location where, text expr )
    : message{ kind + ": didn't assert", where, expr } {}
};

jmp_buf & assert_env()
{
    static jmp_buf buf;
    return buf;
}

void assertion_failed( char const * /*expr*/, char const * /*function*/, char const * /*file*/, long /*line*/ )
{
    std::longjmp( assert_env(), 1 );
}

} // namespace lest

#endif // LEST_LEST_EXPECT_ASSERT_HPP_INCLUDED
