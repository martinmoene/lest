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
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

#include <cctype>
#include <cstddef>

#if defined( lest_USE_REGEX_SEARCH )
# include <regex>
#endif

#ifndef lest_NO_SHORT_ASSERTION_NAMES
# define EXPECT           lest_EXPECT
# define EXPECT_THROWS    lest_EXPECT_THROWS
# define EXPECT_THROWS_AS lest_EXPECT_THROWS_AS
#endif

#define lest_EXPECT( expr ) \
    do { \
        try \
        { \
            if ( lest::result failed = lest_DECOMPOSE( expr ) ) \
                throw lest::failure{ lest_LOCATION, #expr, failed.decomposition }; \
        } \
        catch(...) \
        { \
            lest::inform( lest_LOCATION, #expr ); \
        } \
    } while ( is_true( false ) )

#define lest_EXPECT_THROWS( expr ) \
    do \
    { \
        try { lest::is_true( expr ); } catch (...) { break; } \
        throw lest::expected{ lest_LOCATION, #expr }; \
    } while ( is_true( false ) )

#define lest_EXPECT_THROWS_AS( expr, excpt ) \
    do \
    { \
        try { lest::is_true( expr ); } catch ( excpt & ) { break; } catch (...) {} \
        throw lest::expected{ lest_LOCATION, #expr, lest::of_type( #excpt ) }; \
    } while ( is_true( false ) )

#define lest_DECOMPOSE( expr ) ( lest::expression_decomposer()->* expr )

#define lest_LOCATION lest::location{__FILE__, __LINE__}

namespace lest {

using text  = std::string;
using texts = std::vector<text>;

struct test
{
    const text name;
    const std::function<void()> behaviour;
};

struct tests
{
    const test * spec;
    const int N;

    template<int N>
    tests( test const (&spec)[N] )
    : spec{ spec }, N{ N } {}

    test const * begin() const { return spec; }
    test const * end()   const { return spec + N; }
};

struct result
{
    const bool passed;
    const text decomposition;

    explicit operator bool() { return ! passed; }
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
    explicit operator bool() { return ! info.empty(); }
};

struct message : std::runtime_error
{
    const text kind;
    const location where;
    const comment note;

    ~message() throw() {}   // GCC 4.6

    message( text kind, location where, text expr, text note = "" )
    : std::runtime_error( expr ), kind( kind ), where( where ), note( note ) {}
};

struct failure : message
{
    failure( location where, text expr, text decomposition )
    : message{ "failed", where, expr + " for " + decomposition } {}
};

struct expected : message
{
    expected( location where, text expr, text excpt = "" )
    : message{ "failed: didn't get exception", where, expr, excpt } {}
};

struct unexpected : message
{
    unexpected( location where, text expr, text note )
    : message{ "failed: got unexpected exception", where, expr, note } {}
};

class approx
{
public:
    explicit approx ( double magnitude )
    : epsilon_  { std::numeric_limits<float>::epsilon() * 100 }
    , scale_    { 1.0 }
    , magnitude_{ magnitude } {}

    approx( approx const & other ) = default;

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
        return std::abs( lhs - rhs.magnitude_ ) < rhs.epsilon_ * ( rhs.scale_ + (std::max)( std::abs( lhs ), std::abs( rhs.magnitude_ ) ) );
    }

    friend bool operator == ( approx const & lhs, double rhs ) { return  operator==( rhs, lhs ); }
    friend bool operator != ( double lhs, approx const & rhs ) { return !operator==( lhs, rhs ); }
    friend bool operator != ( approx const & lhs, double rhs ) { return !operator==( rhs, lhs ); }

private:
    double epsilon_;
    double scale_;
    double magnitude_;
};

inline bool is_true( bool flag ) { return flag; }

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
        throw lest::unexpected{ where, expr, lest::with_message( e.what() ) }; \
    }
    catch(...)
    {
        throw lest::unexpected{ where, expr, "of unknown type" }; \
    }
}

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

// Expression decomposition:

inline std::string to_string( std::nullptr_t const &      ) { return "nullptr"; }
inline std::string to_string( std::string    const & text ) { return "\"" + text + "\"" ; }
inline std::string to_string( char const *   const & text ) { return "\"" + std::string( text ) + "\"" ; }
inline std::string to_string( char           const & text ) { return "\'" + std::string( 1, text ) + "\'" ; }

template<typename T>
struct is_container
{
    template<typename U>
    static auto test( int ) -> decltype( std::declval<U>().begin() == std::declval<U>().end(), std::true_type() );

    template<typename>
    static auto test( ... ) -> std::false_type;

#ifdef _MSC_VER
    enum { value = std::is_same< decltype( test<T>(0) ), std::true_type >::value };
#else
    static constexpr bool value = std::is_same< decltype( test<T>(0) ), std::true_type >::value;
#endif
};

template <typename T, typename R>
using ForContainer = typename std::enable_if< is_container<T>::value, R>::type;

template <typename T, typename R>
using ForNonContainer = typename std::enable_if< ! is_container<T>::value, R>::type;

template <typename T>
inline auto to_string( T const & value ) -> ForNonContainer<T, std::string>
{
    std::ostringstream os; os << std::boolalpha << value; return os.str();
}

template <typename C>
inline auto to_string( C const & cont ) -> ForContainer<C, std::string>
{
    std::stringstream os;
    os << "{ "; std::copy( cont.begin(), cont.end(), std::ostream_iterator<typename C::value_type>( os, ", " ) ); os << "}";
    return os.str();
}

inline std::string to_string( approx const & appr )
{
    return to_string( appr.magnitude() );
}

template <typename L, typename R>
inline std::string to_string( L const & lhs, std::string op, R const & rhs )
{
    std::ostringstream os; os << to_string( lhs ) << " " << op << " " << to_string( rhs ); return os.str();
}

template <typename L>
struct expression_lhs
{
    const L lhs;

    expression_lhs( L lhs ) : lhs( lhs ) {}

    operator result() { return result{ lhs, to_string( lhs ) }; }

    template <typename R> result operator==( R const & rhs ) { return result{ lhs == rhs, to_string( lhs, "==", rhs ) }; }
    template <typename R> result operator!=( R const & rhs ) { return result{ lhs != rhs, to_string( lhs, "!=", rhs ) }; }
    template <typename R> result operator< ( R const & rhs ) { return result{ lhs <  rhs, to_string( lhs, "<" , rhs ) }; }
    template <typename R> result operator<=( R const & rhs ) { return result{ lhs <= rhs, to_string( lhs, "<=", rhs ) }; }
    template <typename R> result operator> ( R const & rhs ) { return result{ lhs >  rhs, to_string( lhs, ">" , rhs ) }; }
    template <typename R> result operator>=( R const & rhs ) { return result{ lhs >= rhs, to_string( lhs, ">=", rhs ) }; }
};

struct expression_decomposer
{
    template <typename L>
    expression_lhs<L const &> operator->* ( L const & operand )
    {
        return expression_lhs<L const &>( operand );
    }
};

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
    for ( auto & line : lines )
    {
        if ( search( what, line ) )
            return true;
    }
    return false;
}

inline bool match( texts whats, text line )
{
    for ( auto & what : whats )
    {
        if ( search( what, line ) )
            return true;
    }
    return false;
}

inline bool none( texts args )
{
    return args.size() == 0;
}

inline auto parse( texts args ) -> std::tuple<texts, texts>
{
    texts include, exclude;

    for ( auto & arg : args )
    {
        if ( '!' == arg[0] ) exclude.push_back( arg.substr(1) );
        else                 include.push_back( arg           );
    }
    return std::make_tuple( include, exclude );
}

inline int run( tests specification, texts arguments, std::ostream & os = std::cout )
{
    int selected = 0;
    int failures = 0;

    try
    {
        texts include, exclude;
        std::tie( include, exclude ) = parse( arguments );

        bool any = none( include ) || match( "^\\*$", include );

        for ( auto & testing : specification )
        {
            if ( match( exclude, testing.name ) )
                continue;

            if ( any || match( include, testing.name ) )
            {
                try
                {
                    ++selected; testing.behaviour();
                }
                catch( message const & e )
                {
                    ++failures; report( os, e, testing.name );
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

template <std::size_t N>
int run( test const (&specification)[N], texts arguments, std::ostream & os = std::cout )
{
    return run( tests( specification ), arguments, os  );
}

template <std::size_t N>
int run( test const (&specification)[N], std::ostream & os = std::cout )
{
    return run( tests( specification ), {}, os  );
}

template <std::size_t N>
int run( test const (&specification)[N], int argc, char * argv[], std::ostream & os = std::cout )
{
    return run( tests( specification ), texts( argv + 1, argv + argc ), os  );
}

} // namespace lest

#endif // LEST_LEST_H_INCLUDED
