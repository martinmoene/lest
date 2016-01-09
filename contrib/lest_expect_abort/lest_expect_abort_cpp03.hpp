// Copyright 2016 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LEST_LEST_EXPECT_ABORT_H_INCLUDED
#define LEST_LEST_EXPECT_ABORT_H_INCLUDED

#include "lest_cpp03.hpp"

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
            lest::scoped_abort_substitute lest_UNIQUE( id ); \
            try \
            { \
                expr; \
            } \
            catch( lest::death const & ) \
            { \
                throw lest::aborted( "failed", lest_LOCATION, #expr ); \
            } \
        } \
        catch (...) \
        { \
            lest::inform( lest_LOCATION, #expr ); \
        } \
        if ( lest_env.pass ) \
            lest::report( lest_env.os, lest::not_aborted( "passed", lest_LOCATION, #expr ), lest_env.testing ); \
    } while ( lest::is_false() )

#define lest_EXPECT_ABORTS( expr ) \
    do \
    { \
        try \
        { \
            lest::scoped_abort_substitute lest_UNIQUE( id ); \
            try \
            { \
                expr; \
            } \
            catch( lest::death const & ) \
            { \
                if ( lest_env.pass ) \
                    lest::report( lest_env.os, lest::aborted( "passed", lest_LOCATION, #expr ), lest_env.testing ); \
                break; \
            } \
        }  \
        catch (...) \
        { \
            lest::inform( lest_LOCATION, #expr ); \
        } \
        throw lest::not_aborted( "failed", lest_LOCATION, #expr ); \
    } \
    while ( lest::is_false() )

#ifndef lest_ABORT_SIGNATURE
# if lest_COMPILER_MSVC_VERSION > 0
#  define lest_NORETURN  __declspec(noreturn)
#  if   lest_COMPILER_MSVC_VERSION < 11
#   define lest_ABORT_SIGNATURE()  /*_CRTIMP */ void __cdecl abort(void)
#  elif lest_COMPILER_MSVC_VERSION < 14
#   define lest_ABORT_SIGNATURE()  /*_CRTIMP */  lest_NORETURN void __cdecl abort(void)
#  elif lest_COMPILER_MSVC_VERSION < 15
#   define lest_ABORT_SIGNATURE()  /*_ACRTIMP */ lest_NORETURN void __cdecl abort(void)
#  endif
# else // lest_COMPILER_MSVC_VERSION > 0
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

#if lest_COMPILER_MSVC_VERSION == 6
namespace std { using ::abort; }
#endif

namespace lest {

struct death{};

struct aborted : message
{
    aborted( text kind, location where, text expr )
    : message( kind + ": aborted", where, expr ) {}
};

struct not_aborted : message
{
    not_aborted( text kind, location where, text expr )
    : message( kind + ": didn't abort", where, expr ) {}
};

// substitute for ::abort(),
// inhibit/restore output to stderr to suppress output of assert().
// non-thread-safe
class scoped_abort_substitute
{
public:
    scoped_abort_substitute()
    : stderr_org( -1 )
    , stderr_new( -1 )
    {
        inhibit_stderr();
    }

    ~scoped_abort_substitute()
    {
        restore_stderr();
    }

    lest_NORETURN static void abort()
    {
        throw death();
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

    int stderr_org;
    int stderr_new;
};

} // namespace lest

// substitute ::abort():

#if lest_COMPILER_MSVC_VERSION
# pragma warning( push )
# pragma warning( disable : 4273 )
#endif

lest_ABORT_SIGNATURE()
{
    lest::scoped_abort_substitute::abort();
}

#if lest_COMPILER_MSVC_VERSION
# pragma warning( pop )
#endif

#endif // LEST_LEST_EXPECT_ABORT_H_INCLUDED
