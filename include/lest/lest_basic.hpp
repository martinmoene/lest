// Copyright 2013, 2014 by Martin Moene
//
// lest is based on ideas by Kevlin Henney, see video at
// http://skillsmatter.com/podcast/agile-testing/kevlin-henney-rethinking-unit-testing-in-c-plus-plus
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LEST_LEST_HPP_INCLUDED
#define LEST_LEST_HPP_INCLUDED

#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <cstddef>

#ifdef __clang__
# pragma clang diagnostic ignored "-Waggregate-return"
# pragma clang diagnostic ignored "-Woverloaded-shift-op-parentheses"
# pragma clang diagnostic push
#elif defined __GNUC__
# pragma GCC   diagnostic ignored "-Waggregate-return"
# pragma GCC   diagnostic push
#endif

// Suppress unused-value warning:

#if defined __clang__
# define lest_SUPPRESS_WUNUSED    _Pragma( "clang diagnostic push" ) \
                                  _Pragma( "clang diagnostic ignored \"-Wunused-value\"" )
# define lest_RESTORE_WARNINGS    _Pragma( "clang diagnostic pop"  )

#elif defined __GNUC__
# define lest_SUPPRESS_WUNUSED    _Pragma( "GCC diagnostic push" ) \
                                  _Pragma( "GCC diagnostic ignored \"-Wunused-value\"" )
# define lest_RESTORE_WARNINGS    _Pragma( "GCC diagnostic pop"  )
#else
# define lest_SUPPRESS_WUNUSED    /*empty*/
# define lest_RESTORE_WARNINGS    /*empty*/
#endif

#ifndef lest_NO_SHORT_ASSERTION_NAMES
# define TEST             lest_TEST
# define CASE             lest_CASE
# define CASE_ON          lest_CASE_ON
# define EXPECT           lest_EXPECT
# define EXPECT_NOT       lest_EXPECT_NOT
# define EXPECT_NO_THROW  lest_EXPECT_NO_THROW
# define EXPECT_THROWS    lest_EXPECT_THROWS
# define EXPECT_THROWS_AS lest_EXPECT_THROWS_AS
#endif

#define lest_TEST \
    lest_CASE

#define lest_CASE( name ) \
    name, []

#define lest_CASE_ON( name, ... ) \
    name, [__VA_ARGS__]

#define lest_EXPECT( expr ) \
    do { \
        try \
        { \
            if ( ! (expr) ) \
                throw lest::failure{ lest_LOCATION, #expr }; \
        } \
        catch(...) \
        { \
            lest::inform( lest_LOCATION, #expr ); \
        } \
    } while ( lest::is_false() )

#define lest_EXPECT_NOT( expr ) \
    lest_EXPECT( !(expr) )

#define lest_EXPECT_NO_THROW( expr ) \
    do \
    { \
    lest_SUPPRESS_WUNUSED \
        try { expr; } \
        catch (...) { lest::inform( lest_LOCATION, #expr ); } \
    lest_RESTORE_WARNINGS \
    } while ( lest::is_false() )

#define lest_EXPECT_THROWS( expr ) \
    do \
    { \
    lest_SUPPRESS_WUNUSED \
        try { expr; } catch (...) { break; } \
        throw lest::expected{ lest_LOCATION, #expr }; \
    lest_RESTORE_WARNINGS \
    } while ( lest::is_false() )

#define lest_EXPECT_THROWS_AS( expr, excpt ) \
    do \
    { \
    lest_SUPPRESS_WUNUSED \
        try { expr; } catch ( excpt & ) { break; } catch (...) {} \
        throw lest::expected{ lest_LOCATION, #expr, lest::of_type( #excpt ) }; \
    lest_RESTORE_WARNINGS \
    } while ( lest::is_false() )

#define lest_LOCATION lest::location{__FILE__, __LINE__}

namespace lest {

using text = std::string;

struct test
{
    const text name;
    const std::function<void()> behaviour;
};

struct location
{
    const text file;
    const int line;

    location( text file_, int line_)
    : file( file_), line( line_) {}
};

struct comment
{
    const text info;

    comment( text info_) : info( info_) {}
    explicit operator bool() { return ! info.empty(); }
};

struct message : std::runtime_error
{
    const text kind;
    const location where;
    const comment note;

    ~message() throw() {}   // GCC 4.6

    message( text kind_, location where_, text expr_, text note_ = "" )
    : std::runtime_error( expr_), kind( kind_), where( where_), note( note_) {}
};

struct failure : message
{
    failure( location where_, text expr_)
    : message{ "failed", where_, expr_} {}
};

struct expected : message
{
    expected( location where_, text expr_, text excpt_ = "" )
    : message{ "failed: didn't get exception", where_, expr_, excpt_} {}
};

struct unexpected : message
{
    unexpected( location where_, text expr_, text note_ = "" )
    : message{ "failed: got unexpected exception", where_, expr_, note_} {}
};

inline bool is_false(           ) { return false; }
inline bool is_true ( bool flag ) { return  flag; }

inline text with_message( text message )
{
    return "with message \"" + message + "\"";
}

inline text of_type( text type )
{
    return "of type " + type;
}

inline void inform( location where, text expr )
{
    try
    {
        throw;
    }
    catch( lest::message const & )
    {
        throw;
    }
    catch( std::exception const & e )
    {
        throw lest::unexpected{ where, expr, lest::with_message( e.what() ) }; \
    }
    catch(...)
    {
        throw lest::unexpected{ where, expr, "of unknown type" }; \
    }
}

inline text pluralise( text word, int n )
{
    return n == 1 ? word : word + "s";
}

inline std::ostream & operator<<( std::ostream & os, comment note )
{
    return os << (note ? " " + note.info : "" );
}

inline std::ostream & operator<<( std::ostream & os, location where )
{
#ifdef __GNUG__
    return os << where.file << ":" << where.line;
#else
    return os << where.file << "(" << where.line << ")";
#endif
}

inline void report( std::ostream & os, message const & e, std::string test )
{
    os << e.where << ": " << e.kind << e.note << ": " << test << ": " << e.what() << std::endl;
}

template<std::size_t N>
int run( test const (&specification)[N], std::ostream & os = std::cout )
{
    int failures = 0;

    for ( auto & testing : specification )
    {
        try
        {
            testing.behaviour();
        }
        catch( message const & e )
        {
            ++failures;
            report( os, e, testing.name );
        }
    }

    if ( failures > 0 )
    {
        os << failures << " out of " << N << " " << pluralise("test", N) << " failed." << std::endl;
    }

    return failures;
}

} // namespace lest

#ifdef __clang__
# pragma clang diagnostic pop
#elif defined __GNUC__
# pragma GCC   diagnostic pop
#endif

#endif // LEST_LEST_HPP_INCLUDED
