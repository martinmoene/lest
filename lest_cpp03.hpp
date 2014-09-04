// Copyright 2013, 2014 by Martin Moene
//
// lest is based on ideas by Kevlin Henney, see video at
// http://skillsmatter.com/podcast/agile-testing/kevlin-henney-rethinking-unit-testing-in-c-plus-plus
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LEST_LEST_H_INCLUDED
#define LEST_LEST_H_INCLUDED

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <cctype>
#include <cmath>

#if defined( lest_USE_REGEX_SEARCH )
# include <regex>
#endif

#if ( __cplusplus >= 201103L )
# define lest_CPP11_OR_GREATER
#endif

#if defined( _MSC_VER ) && ( 1200 <= _MSC_VER && _MSC_VER < 1300 )
# define lest_COMPILER_IS_MSVC6
#endif

#ifdef lest_CPP11_OR_GREATER

# include <tuple>
namespace lest
{
    using std::tie;
}
#else

namespace lest
{
    template< typename T1, typename T2>
    struct Tie
    {
        Tie( T1 & first, T2 & second )
        : first( first ), second( second ) {}

        std::pair<T1, T2> const &
        operator=( std::pair<T1, T2> const & rhs )
        {
            first  = rhs.first;
            second = rhs.second;
            return rhs;
        }

    private:
        void operator=( Tie const & );

        T1 & first;
        T2 & second;
    };

    template<typename T1, typename T2>
    inline Tie<T1,T2> tie( T1 & first, T2 & second )
    {
      return Tie<T1, T2>( first, second );
    }
}
#endif // lest_CPP11_OR_GREATER

namespace lest
{
#ifdef lest_COMPILER_IS_MSVC6
    double abs( double x ) { return ::fabs( x ); }
    template<typename T> T const & max(T const & a, T const & b) { return a >= b ? a : b; }
#else
    using std::abs;
    using std::max;
#endif
}

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
    do { \
        try \
        { \
            if ( lest::result failed = lest_DECOMPOSE( expr ) ) \
                throw lest::failure( lest_LOCATION, #expr, failed.decomposition ); \
        } \
        catch(...) \
        { \
            lest::inform( lest_LOCATION, #expr ); \
        } \
    } while ( lest::is_false() )

#define lest_EXPECT_THROWS( expr ) \
    do \
    { \
        try { lest::is_true( expr ); } catch (...) { break; } \
        throw lest::expected( lest_LOCATION, #expr ); \
    } while ( lest::is_false() )

#define lest_EXPECT_THROWS_AS( expr, excpt ) \
    do \
    { \
        try { lest::is_true( expr ); } catch ( excpt & ) { break; } catch (...) {} \
        throw lest::expected( lest_LOCATION, #expr, lest::of_type( #excpt ) ); \
    } while ( lest::is_false() )

#define lest_DECOMPOSE( expr ) ( lest::expression_decomposer()->* expr )

#define lest_NAME2( name, line ) name##line
#define lest_NAME(  name, line ) lest_NAME2( name, line )

#define lest_LOCATION  lest::location(__FILE__, __LINE__)

#define lest_FUNCTION  lest_NAME(__lest_function__, __LINE__)
#define lest_REGISTRAR lest_NAME(__lest_registrar__, __LINE__)

#define lest_DIMENSION_OF( a ) ( sizeof(a) / sizeof(0[a]) )

namespace lest {

typedef std::string       text;
typedef std::vector<text> texts;

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
    const text decomposition;

    result( bool passed, text decomposition )
    : passed( passed ), decomposition( decomposition ) {}
    operator bool() { return ! passed; }
};

struct location
{
    const text file;
    const int line;

    location( text file, int line )
    : file( file ), line( line ) {}
};

struct comment
{
    const text info;

    comment( text info ) : info( info ) {}
    operator bool() { return ! info.empty(); }
};

struct message : std::runtime_error
{
    const text kind;
    const location where;
    const comment note;

    ~message() throw() {}

    message( text kind, location where, text expr, text note = "" )
    : std::runtime_error( expr ), kind( kind ), where( where ), note( note ) {}
};

struct failure : message
{
    failure( location where, text expr, text decomposition )
    : message( "failed", where, expr + " for " + decomposition ) {}
};

struct expected : message
{
    expected( location where, text expr, text excpt = "" )
    : message( "failed: didn't get exception", where, expr, excpt ) {}
};

struct unexpected : message
{
    unexpected( location where, text expr, text note )
    : message( "failed: got unexpected exception", where, expr, note ) {}
};

class approx
{
public:
    explicit approx ( double magnitude )
    : epsilon_  ( std::numeric_limits<float>::epsilon() * 100 )
    , scale_    ( 1.0 )
    , magnitude_( magnitude ) {}

    static approx custom() { return approx( 0 ); }

    approx operator()( double magnitude )
    {
        approx approx ( magnitude );
        approx.epsilon( epsilon_  );
        approx.scale  ( scale_    );
        return approx;
    }

    double magnitude() const { return magnitude_; }

    approx & epsilon( double epsilon ) { epsilon_ = epsilon; return *this; }
    approx & scale  ( double scale   ) { scale_   = scale;   return *this; }

    friend bool operator == ( double lhs, approx const & rhs )
    {
        // Thanks to Richard Harris for his help refining this formula.
        return lest::abs( lhs - rhs.magnitude_ ) < rhs.epsilon_ * ( rhs.scale_ + (lest::max)( lest::abs( lhs ), lest::abs( rhs.magnitude_ ) ) );
    }

    friend bool operator == ( approx const & lhs, double rhs ) { return  operator==( rhs, lhs ); }
    friend bool operator != ( double lhs, approx const & rhs ) { return !operator==( lhs, rhs ); }
    friend bool operator != ( approx const & lhs, double rhs ) { return !operator==( rhs, lhs ); }

private:
    double epsilon_;
    double scale_;
    double magnitude_;
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

inline void inform( location where, char const * expr )
{
    try
    {
        throw;
    }
    catch( lest::failure const & )
    {
        throw;
    }
    catch( std::exception const & e )
    {
        throw lest::unexpected( where, expr, lest::with_message( e.what() ) ); \
    }
    catch(...)
    {
        throw lest::unexpected( where, expr, "of unknown type" ); \
    }
}

// Expression decomposition:

#ifdef lest_CPP11_OR_GREATER
inline std::string to_string( std::nullptr_t const &      ) { return "nullptr"; }
#endif
inline std::string to_string( std::string    const & text ) { return "\"" + text + "\"" ; }
inline std::string to_string( char const *   const & text ) { return "\"" + std::string( text ) + "\"" ; }
inline std::string to_string( char           const & text ) { return "\'" + std::string( 1, text ) + "\'" ; }

inline std::ostream & operator<<( std::ostream & os, approx const & appr )
{
    return os << appr.magnitude();
}

template <typename T>
std::string to_string( T const & value, int=0 /* VC6 */ )
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

// Reporter:

inline text pluralise( int n, text word )
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

inline void report( std::ostream & os, message const & e, text test )
{
    os << e.where << ": " << e.kind << e.note << ": " << test << ": " << e.what() << std::endl;
}

// Test runner:

inline bool case_insensitive_equal( char a, char b )
{
    return tolower( a ) == tolower( b );
}

#ifdef lest_USE_REGEX_SEARCH
    inline bool search( text re, text line )
    {
        return std::regex_search( line, std::regex( re ) );
    }
#else
    inline bool search( text part, text line )
    {
        if ( part == "^\\*$" && "*" == line )
            return true;

        return std::search(
            line.begin(), line.end(),
            part.begin(), part.end(), case_insensitive_equal ) != line.end();
    }
#endif // lest_HAS_REGEX_SEARCH

inline bool match( text what, texts lines )
{
    for ( texts::iterator line = lines.begin(); line != lines.end() ; ++line )
    {
        if ( search( what, *line ) )
            return true;
    }
    return false;
}

inline bool match( texts whats, text line )
{
    for ( texts::iterator what = whats.begin(); what != whats.end() ; ++what )
    {
        if ( search( *what, line ) )
            return true;
    }
    return false;
}

inline bool none( texts args )
{
    return args.size() == 0;
}

inline std::pair<texts, texts> parse( texts args )
{
    texts include, exclude;

    for ( texts::iterator arg = args.begin(); arg != args.end() ; ++arg )
    {
        if ( '!' == (*arg)[0] ) exclude.push_back(  arg->substr(1) );
        else                    include.push_back( *arg            );
    }
    return std::make_pair( include, exclude );
}

// Traversal of test_specification and test[N] set up to also work with MSVC6:

inline test_specification::const_iterator begin( test_specification const & c ) { return c.begin(); }
inline test_specification::const_iterator   end( test_specification const & c ) { return c.end();   }

template <typename C> test const * begin( C const & c ) { return &c[0]; }
template <typename C> test const *   end( C const & c ) { return begin( c ) + lest_DIMENSION_OF( c ); }

template <typename C> struct iter                       { typedef test const * type; };
template <>           struct iter<test_specification>   { typedef test_specification::const_iterator type; };

template <typename C>
int run( C const & specification, texts arguments, std::ostream & os = std::cout )
{
    int selected = 0;
    int failures = 0;

    try
    {
        texts include, exclude;
        lest::tie( include, exclude ) = parse( arguments );

        bool any = none( include ) || match( "^\\*$", include );

//        const int N = std::distance( begin( specification ), end( specification ) );

        for ( typename iter<C>::type testing = begin( specification ); testing != end( specification ); ++testing )
        {
            if ( match( exclude, testing->name ) )
                continue;

            if ( any || match( include, testing->name ) )
            {
                try
                {
                    ++selected; testing->behaviour();
                }
                catch( message const & e )
                {
                    ++failures; report( os, e, testing->name );
                }
            }
        }

        if ( failures > 0 )
        {
            os << failures << " out of " << selected << " selected " << pluralise(selected, "test") << " failed." << std::endl;
        }
    }
    catch ( std::exception const & e )
    {
        std::cerr << "Error: " << e.what() << "\n";
        return failures + 1;
    }
    return failures;
}

// VC6: make_texts(first,last) replaces texts(first,last)

inline texts make_texts( char * const * first, char * const * last )
{
    texts result;
    for ( ; first != last; ++first )
    {
       result.push_back( *first );
    }
    return result;
}

template <typename C>
int run( C const & specification, int argc, char * argv[], std::ostream & os = std::cout )
{
    return run( specification, make_texts( argv + 1, argv + argc ), os  );
}

template <typename C>
int run( C const & specification, std::ostream & os = std::cout )
{
    return run( specification, texts(), os );
}

} // namespace lest

#endif // LEST_LEST_H_INCLUDED
