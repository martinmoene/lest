// Copyright 2013 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LEST_LEST_H_INCLUDED
#define LEST_LEST_H_INCLUDED

#include <iostream>
#include <functional>
#include <sstream>
#include <stdexcept>

#define REQUIRE( expr ) \
    do { \
        lest::record( false, __FILE__, __LINE__ ); \
        if ( !(expr) ) throw lest::message( #expr ); \
    } while( lest::verum(false) );

#define CHECK( expr ) \
    do { \
        lest::record( true, __FILE__, __LINE__ ); \
        if ( !(expr) ) throw lest::message( #expr ); \
    } while( lest::verum(false) );

namespace lest {

bool check = false; std::string file = ""; int line = 0;

void record( bool const check_, std::string const file_, int const line_ )
{
    check = check_; file = file_; line = line_;
}

struct message : public std::runtime_error
{
    message( char const * expr ) : std::runtime_error( expr ) {}
};

struct test
{
    char const * const name;
    std::function<void()> const behaviour;
};

constexpr bool verum( bool const vera )
{
    return vera;
}

std::string location()
{
    std::ostringstream os;
#ifndef __GNUG__
    os << file << "(" << line << ")";
#else
    os << file << ":" << line;
#endif
    return os.str();
}

std::string pluralise( int const n, std::string const text )
{
    return n == 1 ? text : text + "s";
}

void report( std::string const kind, std::exception const & e, std::string const test )
{
    std::cout << location() << ": " << kind << ": '" << test << "': " << e.what() << std::endl;
}

template<std::size_t N>
int breaks( test const (&specification)[N] )
{
    int executed = 0;
    int failures = 0;

    for ( auto testing : specification )
    {
        try
        {
            ++executed;
            testing.behaviour();
        }
        catch( message const & e )
        {
            ++failures;
            report( "error", e, testing.name );
            if ( !check )
                break;
        }
        catch( std::exception const & e )
        {
            ++failures;
            report( "exception", e, testing.name );
            if ( !check )
                break;
        }
        // otherwise terminate
        // catch (...) {}
    }

    if ( failures > 0 )
    {
        const int skipped = N - executed;
        std::cout <<
            failures << " out of " <<
            executed << " " << pluralise(executed, "test") << " failed.";
        if ( skipped > 0 )
            std::cout << " Skipped " << skipped << pluralise(skipped, " test") << ".";
        std::cout << std::endl;
    }

    return failures;
}

} // namespace lest

#endif // LEST_LEST_H_INCLUDED
