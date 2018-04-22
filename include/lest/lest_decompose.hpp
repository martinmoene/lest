// Copyright 2013, 2014 by Martin Moene
//
// lest is based on ideas by Kevlin Henney, see video at
// http://skillsmatter.com/podcast/agile-testing/kevlin-henney-rethinking-unit-testing-in-c-plus-plus
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LEST_LEST_HPP_INCLUDED
#define LEST_LEST_HPP_INCLUDED

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <cstddef>

#ifdef __clang__
# pragma clang diagnostic ignored "-Waggregate-return"
# pragma clang diagnostic ignored "-Woverloaded-shift-op-parentheses"
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunused-comparison"
#elif defined __GNUC__
# pragma GCC   diagnostic ignored "-Waggregate-return"
# pragma GCC   diagnostic push
#endif

// Suppress shadow and unused-value warning for sections:

#if defined __clang__
# define lest_SUPPRESS_WSHADOW    _Pragma( "clang diagnostic push" ) \
                                  _Pragma( "clang diagnostic ignored \"-Wshadow\"" )
# define lest_SUPPRESS_WUNUSED    _Pragma( "clang diagnostic push" ) \
                                  _Pragma( "clang diagnostic ignored \"-Wunused-value\"" )
# define lest_RESTORE_WARNINGS    _Pragma( "clang diagnostic pop"  )

#elif defined __GNUC__
# define lest_SUPPRESS_WSHADOW    _Pragma( "GCC diagnostic push" ) \
                                  _Pragma( "GCC diagnostic ignored \"-Wshadow\"" )
# define lest_SUPPRESS_WUNUSED    _Pragma( "GCC diagnostic push" ) \
                                  _Pragma( "GCC diagnostic ignored \"-Wunused-value\"" )
# define lest_RESTORE_WARNINGS    _Pragma( "GCC diagnostic pop"  )
#else
# define lest_SUPPRESS_WSHADOW    /*empty*/
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
            if ( lest::result failed = lest_DECOMPOSE( expr ) ) \
                throw lest::failure{ lest_LOCATION, #expr, failed.decomposition }; \
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

#define lest_DECOMPOSE( expr ) ( lest::expression_decomposer() << expr )

#define lest_LOCATION lest::location{__FILE__, __LINE__}

namespace lest {

using text = std::string;

struct test
{
    const text name;
    const std::function<void()> behaviour;
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
    failure( location where_, std::string expr_, std::string decomposition_)
    : message{ "failed", where_, expr_ + " for " + decomposition_} {}
};

struct expected : message
{
    expected( location where_, std::string expr_, std::string excpt_ = "" )
    : message{ "failed: didn't get exception", where_, expr_, excpt_} {}
};

struct unexpected : message
{
    unexpected( location where_, std::string expr_, std::string note_ = "" )
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

template <std::size_t N>
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

// Expression decomposition:

inline std::string to_string( std::nullptr_t const &     ) { return "nullptr"; }
inline std::string to_string( std::string    const & txt ) { return "\"" + txt + "\"" ; }
inline std::string to_string( char const *   const & txt ) { return "\"" + std::string( txt ) + "\"" ; }
inline std::string to_string( char           const & txt ) { return "\'" + std::string( 1, txt ) + "\'" ; }

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

template< typename T, typename R >
using ForContainer = typename std::enable_if< is_container<T>::value, R>::type;

template< typename T, typename R >
using ForNonContainerNonPointer = typename std::enable_if< ! (is_container<T>::value || std::is_pointer<T>::value), R>::type;

template< typename T >
inline std::string make_string( T const * ptr )
{
    // Note showbase affects the behavior of /integer/ output;
    std::ostringstream os;
    os << std::internal << std::hex << std::showbase << std::setw( 2 + 2 * sizeof(T*) ) << std::setfill('0') << reinterpret_cast<std::ptrdiff_t>( ptr );
    return os.str();
}

template< typename C, typename R >
inline std::string make_string( R C::* ptr )
{
    std::ostringstream os;
    os << std::internal << std::hex << std::showbase << std::setw( 2 + 2 * sizeof(R C::* ) ) << std::setfill('0') << ptr;
    return os.str();
}

template< typename T >
auto to_string( T const * ptr ) -> std::string
{
    return ! ptr ? "nullptr" : make_string( ptr );
}

template< typename C, typename R >
auto to_string( R C::* ptr ) -> std::string
{
    return ! ptr ? "nullptr" : make_string( ptr );
}

template< typename T >
inline auto to_string( T const & value ) -> ForNonContainerNonPointer<T, std::string>
{
    std::ostringstream os; os << std::boolalpha << value; return os.str();
}

template< typename C >
inline auto to_string( C const & cont ) -> ForContainer<C, std::string>
{
    std::stringstream os;
    os << "{ "; std::copy( cont.begin(), cont.end(), std::ostream_iterator<typename C::value_type>( os, ", " ) ); os << "}";
    return os.str();
}

template< typename L, typename R >
inline std::string to_string( L const & lhs, std::string op, R const & rhs )
{
    std::ostringstream os; os << to_string( lhs ) << " " << op << " " << to_string( rhs ); return os.str();
}

template< typename L >
struct expression_lhs
{
    const L lhs;

    expression_lhs( L lhs_) : lhs( lhs_) {}

    operator result() { return result{ !!lhs, to_string( lhs ) }; }

    template< typename R > result operator==( R const & rhs ) { return result{ lhs == rhs, to_string( lhs, "==", rhs ) }; }
    template< typename R > result operator!=( R const & rhs ) { return result{ lhs != rhs, to_string( lhs, "!=", rhs ) }; }
    template< typename R > result operator< ( R const & rhs ) { return result{ lhs <  rhs, to_string( lhs, "<" , rhs ) }; }
    template< typename R > result operator<=( R const & rhs ) { return result{ lhs <= rhs, to_string( lhs, "<=", rhs ) }; }
    template< typename R > result operator> ( R const & rhs ) { return result{ lhs >  rhs, to_string( lhs, ">" , rhs ) }; }
    template< typename R > result operator>=( R const & rhs ) { return result{ lhs >= rhs, to_string( lhs, ">=", rhs ) }; }
};

struct expression_decomposer
{
    template< typename L >
    expression_lhs<L const &> operator<< ( L const & operand )
    {
        return expression_lhs<L const &>( operand );
    }
};

} // namespace lest

#ifdef __clang__
# pragma clang diagnostic pop
#elif defined __GNUC__
# pragma GCC   diagnostic pop
#endif

#endif // LEST_LEST_HPP_INCLUDED
