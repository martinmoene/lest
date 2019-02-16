// Copyright 2016 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LEST_LEST_EXPECT_ABORT_HPP_INCLUDED
#define LEST_LEST_EXPECT_ABORT_HPP_INCLUDED

#include "lest/lest.hpp"

#include <csetjmp>

#include <io.h>
#include <fcntl.h>

#if ! defined( lest_NO_SHORT_MACRO_NAMES )
# define EXPECT_NO_ABORT  lest_EXPECT_NO_ABORT
# define EXPECT_ABORTS    lest_EXPECT_ABORTS
#endif

#define lest_EXPECT_NO_ABORT( expr ) \
    do \
    { \
        try \
        { \
            lest::scoped_abort lest_UNIQUE( id ); \
            try \
            { \
                expr; \
            } \
            catch ( lest::aborted_ const & ) \
            { \
                throw lest::aborted{ "failed", lest_LOCATION, #expr }; \
            } \
        } \
        catch (...) \
        { \
            lest::inform( lest_LOCATION, #expr ); \
        } \
        if ( lest_env.pass ) \
            lest::report( lest_env.os, lest::not_aborted{ "passed", lest_LOCATION, #expr }, lest_env.testing ); \
    } while ( lest::is_false() )

#define lest_EXPECT_ABORTS( expr ) \
    do \
    { \
        try \
        { \
            lest::scoped_abort lest_UNIQUE( id ); \
            try \
            { \
                expr; \
            } \
            catch ( lest::aborted_ const & ) \
            { \
                if ( lest_env.pass ) \
                    lest::report( lest_env.os, lest::aborted{ "passed", lest_LOCATION, #expr }, lest_env.testing ); \
                break; \
            } \
        }  \
        catch (...) \
        { \
            lest::inform( lest_LOCATION, #expr ); \
        } \
        throw lest::not_aborted{ "failed", lest_LOCATION, #expr }; \
    } \
    while ( lest::is_false() )

#if defined(_MSC_VER)
# define lest_COMPILER_MSVC_VERSION   (_MSC_VER / 100 - 5 - (_MSC_VER < 1900))
#else
# define lest_COMPILER_MSVC_VERSION   0
#endif

#ifndef lest_ABORT_SIGNATURE
# if lest_COMPILER_MSVC_VERSION
#  define lest_NORETURN  __declspec(noreturn)
#  define lest_ABORT_SIGNATURE()  /*_[A]CRTIMP*/  lest_NORETURN void __cdecl abort(void)
# else
#  define lest_NORETURN  [[noreturn]]
#  define lest_ABORT_SIGNATURE()  lest_NORETURN void __cdecl abort()
# endif
#else
# ifndef  lest_NORETURN
#  define lest_NORETURN
# endif
#endif

#if _WIN32
# define lest_DEV_NULL "nul"
# else
# define lest_DEV_NULL "/dev/null"
#endif

namespace lest {

struct aborted : message
{
    aborted( text kind, location where, text expr )
    : message{ kind + ": aborted", where, expr } {}
};

struct not_aborted : message
{
    not_aborted( text kind, location where, text expr )
    : message{ kind + ": didn't abort", where, expr } {}
};

struct aborted_{};

class scoped_abort
{
public:
    scoped_abort()
    {
        inhibit_stderr();
    }

    ~scoped_abort()
    {
        restore_stderr();
    }

    lest_NORETURN static void abort()
    {
        throw aborted_{};
    }

private:
    void inhibit_stderr()
    {
        fflush( stderr );
        stderr_org = dup( 2 );
        stderr_new = open( lest_DEV_NULL, O_WRONLY );
        dup2 ( stderr_new, 2 );
        close( stderr_new    );
    }

    void restore_stderr()
    {
        fflush( stderr );
        dup2  ( stderr_org, 2 );
        close ( stderr_org    );
    }

    int stderr_org = -1;
    int stderr_new = -1;
};

} // namespace lest

// substitute ::abort():

lest_ABORT_SIGNATURE()
{
    lest::scoped_abort::abort();
}

#endif // LEST_LEST_EXPECT_ABORT_HPP_INCLUDED
