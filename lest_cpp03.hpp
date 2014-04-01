// Copyright 2013 by Martin Moene
//
// lest is based on ideas by Kevlin Henney, see video at
// http://skillsmatter.com/podcast/agile-testing/kevlin-henney-rethinking-unit-testing-in-c-plus-plus
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LEST_LEST_H_INCLUDED
#define LEST_LEST_H_INCLUDED

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#ifndef lest_NO_SHORT_ASSERTION_NAMES
# define EXPECT           lest_EXPECT
# define EXPECT_THROWS    lest_EXPECT_THROWS
# define EXPECT_THROWS_AS lest_EXPECT_THROWS_AS
#endif

#define lest_TEST( specification, name ) \
    void lest_FUNCTION(); \
    namespace { lest::registrar lest_REGISTRAR( specification, lest::test( name, lest_FUNCTION ) ); } \
    void lest_FUNCTION()

#define lest_ADD_TEST( specification, test ) \
    specification.push_back( test )

#define lest_EXPECT( expr ) \
    try \
    { \
        if ( lest::result failed = lest_DECOMPOSE( expr ) ) \
            throw lest::failure( lest_LOCATION, #expr, failed.decomposition ); \
    } \
    catch( lest::failure const & ) \
    { \
        throw ; \
    } \
    catch( std::exception const & e ) \
    { \
        throw lest::unexpected( lest_LOCATION, #expr, lest::with_message( e.what() ) ); \
    } \
    catch(...) \
    { \
        throw lest::unexpected( lest_LOCATION, #expr, "of unknown type" ); \
    }

#define lest_EXPECT_THROWS( expr ) \
    for (;;) \
    { \
        try { lest::serum( expr ); } catch (...) { break; } \
        throw lest::expected( lest_LOCATION, #expr ); \
    }

#define lest_EXPECT_THROWS_AS( expr, excpt ) \
    for (;;) \
    { \
        try { lest::serum( expr ); } catch ( excpt & ) { break; } catch (...) {} \
        throw lest::expected( lest_LOCATION, #expr, lest::of_type( #excpt ) ); \
    }

#define lest_DECOMPOSE( expr ) ( lest::expression_decomposer()->* expr )

#define lest_NAME2( name, line ) name##line
#define lest_NAME(  name, line ) lest_NAME2( name, line )

#define lest_LOCATION  lest::location(__FILE__, __LINE__)

#define lest_FUNCTION  lest_NAME(__lest_function__, __LINE__)
#define lest_REGISTRAR lest_NAME(__lest_registrar__, __LINE__)

#define lest_DIMENSION_OF( a ) ( sizeof(a) / sizeof(0[a]) )

namespace lest {

struct test
{
    char const * name;
    void (* behaviour)();

    test( char const * name, void (* behaviour)() )
    : name (name ), behaviour( behaviour ) {}
};

typedef std::vector<test> test_specification;

struct registrar
{
    registrar( test_specification & s, test const & t )
    {
        s.push_back( t );
    }
};

struct result
{
    const bool passed;
    const std::string decomposition;

    result( bool passed, std::string decomposition )
    : passed( passed ), decomposition( decomposition ) {}
    operator bool() { return ! passed; }
};

struct location
{
    const std::string file;
    const int line;

    location( std::string file, int line )
    : file( file ), line( line ) {}
};

struct comment
{
    const std::string text;

    comment( std::string text ) : text( text ) {}
    operator bool() { return ! text.empty(); }
};

struct message : std::runtime_error
{
    const std::string kind;
    const location where;
    const comment note;

    ~message() throw() {}

    message( std::string kind, location where, std::string expr, std::string note = "" )
    : std::runtime_error( expr ), kind( kind ), where( where ), note( note ) {}
};

struct failure : message
{
    failure( location where, std::string expr, std::string decomposition )
    : message( "failed", where, expr + " for " + decomposition ) {}
};

struct expected : message
{
    expected( location where, std::string expr, std::string excpt = "" )
    : message( "failed: didn't get exception", where, expr, excpt ) {}
};

struct unexpected : message
{
    unexpected( location where, std::string expr, std::string note )
    : message( "failed: got unexpected exception", where, expr, note ) {}
};

inline bool serum( bool verum ) { return verum; }

inline std::string with_message( std::string text )
{
    return "with message \"" + text + "\"";
}

inline std::string of_type( std::string text )
{
    return "of type " + text;
}

inline std::string pluralise( int n, std::string text )
{
    return n == 1 ? text : text + "s";
}

inline std::ostream & operator<<( std::ostream & os, comment note )
{
    return os << (note ? " " + note.text : "" );
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

// Traversal of test_specification and test[N] set up to also work with MSVC6:

inline test_specification::const_iterator begin( test_specification const & c ) { return c.begin(); }
inline test_specification::const_iterator   end( test_specification const & c ) { return c.end();   }

template <typename C> test const * begin( C const & c ) { return &c[0]; }
template <typename C> test const *   end( C const & c ) { return begin( c ) + lest_DIMENSION_OF( c ); }

template <typename C> struct iter                       { typedef test const * type; };
template <>           struct iter<test_specification>   { typedef test_specification::const_iterator type; };

template <typename C>
int run( C const & specification, std::ostream & os = std::cout )
{
    int failures = 0;
    const int  N = end( specification ) - begin( specification );

    for ( typename iter<C>::type testing = begin( specification ); testing != end( specification ); ++testing )
    {
        try
        {
            testing->behaviour();
        }
        catch( message const & e )
        {
            ++failures;
            report( os, e, testing->name );
        }
    }

    if ( failures > 0 )
    {
        os << failures << " out of " << N << " " << pluralise(N, "test") << " failed." << std::endl;
    }

    return failures;
}

// Expression decomposition:

#if __cplusplus >= 201103L
inline std::string to_string( std::nullptr_t const &      ) { return "nullptr"; }
#endif
inline std::string to_string( std::string    const & text ) { return "\"" + text + "\"" ; }
inline std::string to_string( char const *   const & text ) { return "\"" + std::string( text ) + "\"" ; }
inline std::string to_string( char           const & text ) { return "\'" + std::string( 1, text ) + "\'" ; }

template <typename T>
std::string to_string( T const & value )
{
    std::ostringstream os; os << std::boolalpha << value; return os.str();
}

template <typename L, typename R>
std::string to_string( L const & lhs, std::string op, R const & rhs )
{
    std::ostringstream os; os << to_string( lhs ) << " " << op << " " << to_string( rhs ); return os.str();
}

template <typename L>
struct expression_lhs
{
    L lhs;

    expression_lhs( L lhs ) : lhs( lhs ) {}

    operator result() { return result( lhs, to_string( lhs ) ); }

    template <typename R> result operator==( R const & rhs ) { return result( lhs == rhs, to_string( lhs, "==", rhs ) ); }
    template <typename R> result operator!=( R const & rhs ) { return result( lhs != rhs, to_string( lhs, "!=", rhs ) ); }
    template <typename R> result operator< ( R const & rhs ) { return result( lhs <  rhs, to_string( lhs, "<" , rhs ) ); }
    template <typename R> result operator<=( R const & rhs ) { return result( lhs <= rhs, to_string( lhs, "<=", rhs ) ); }
    template <typename R> result operator> ( R const & rhs ) { return result( lhs >  rhs, to_string( lhs, ">" , rhs ) ); }
    template <typename R> result operator>=( R const & rhs ) { return result( lhs >= rhs, to_string( lhs, ">=", rhs ) ); }
};

struct expression_decomposer
{
    template <typename L>
    expression_lhs<L const &> operator->* ( L const & operand )
    {
        return expression_lhs<L const &>( operand );
    }
};

} // namespace lest

#endif // LEST_LEST_H_INCLUDED
