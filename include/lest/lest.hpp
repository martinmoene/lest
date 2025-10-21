// Copyright 2013-2018 by Martin Moene
//
// lest is based on ideas by Kevlin Henney, see video at
// http://skillsmatter.com/podcast/agile-testing/kevlin-henney-rethinking-unit-testing-in-c-plus-plus
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LEST_LEST_HPP_INCLUDED
#define LEST_LEST_HPP_INCLUDED

#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <set>
#include <tuple>
#include <typeinfo>
#include <type_traits>
#include <utility>
#include <vector>

#include <cctype>
#include <cmath>
#include <cstddef>

#define lest_MAJOR  1
#define lest_MINOR  36
#define lest_PATCH  0

#define  lest_VERSION  lest_STRINGIFY(lest_MAJOR) "." lest_STRINGIFY(lest_MINOR) "." lest_STRINGIFY(lest_PATCH)

#ifndef  lest_FEATURE_AUTO_REGISTER
# define lest_FEATURE_AUTO_REGISTER  0
#endif

#ifndef  lest_FEATURE_COLOURISE
# define lest_FEATURE_COLOURISE  0
#endif

#ifndef  lest_FEATURE_LITERAL_SUFFIX
# define lest_FEATURE_LITERAL_SUFFIX  0
#endif

#ifndef  lest_FEATURE_REGEX_SEARCH
# define lest_FEATURE_REGEX_SEARCH  0
#endif

#ifndef  lest_FEATURE_TIME_PRECISION
# define lest_FEATURE_TIME_PRECISION  0
#endif

#ifndef  lest_FEATURE_WSTRING
# define lest_FEATURE_WSTRING  1
#endif

#ifdef    lest_FEATURE_RTTI
# define  lest__cpp_rtti  lest_FEATURE_RTTI
#elif defined(__cpp_rtti)
# define  lest__cpp_rtti  __cpp_rtti
#elif defined(__GXX_RTTI) || defined (_CPPRTTI)
# define  lest__cpp_rtti  1
#else
# define  lest__cpp_rtti  0
#endif

#if lest_FEATURE_REGEX_SEARCH
# include <regex>
#endif

// Stringify:

#define lest_STRINGIFY(  x )  lest_STRINGIFY_( x )
#define lest_STRINGIFY_( x )  #x

// Compiler warning suppression:

#if defined (__clang__)
# pragma clang diagnostic ignored "-Waggregate-return"
# pragma clang diagnostic ignored "-Woverloaded-shift-op-parentheses"
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunused-comparison"
#elif defined (__GNUC__)
# pragma GCC   diagnostic ignored "-Waggregate-return"
# pragma GCC   diagnostic push
#endif

// Suppress shadow and unused-value warning for sections:

#if defined (__clang__)
# define lest_SUPPRESS_WSHADOW    _Pragma( "clang diagnostic push" ) \
                                  _Pragma( "clang diagnostic ignored \"-Wshadow\"" )
# define lest_SUPPRESS_WUNUSED    _Pragma( "clang diagnostic push" ) \
                                  _Pragma( "clang diagnostic ignored \"-Wunused-value\"" )
# define lest_RESTORE_WARNINGS    _Pragma( "clang diagnostic pop"  )

#elif defined (__GNUC__)
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

// C++ language version detection (C++23 is speculative):
// Note: VC14.0/1900 (VS2015) lacks too much from C++14.

#ifndef   lest_CPLUSPLUS
# if defined(_MSVC_LANG ) && !defined(__clang__)
#  define lest_CPLUSPLUS  (_MSC_VER == 1900 ? 201103L : _MSVC_LANG )
# else
#  define lest_CPLUSPLUS  __cplusplus
# endif
#endif

#define lest_CPP98_OR_GREATER  ( lest_CPLUSPLUS >= 199711L )
#define lest_CPP11_OR_GREATER  ( lest_CPLUSPLUS >= 201103L )
#define lest_CPP14_OR_GREATER  ( lest_CPLUSPLUS >= 201402L )
#define lest_CPP17_OR_GREATER  ( lest_CPLUSPLUS >= 201703L )
#define lest_CPP20_OR_GREATER  ( lest_CPLUSPLUS >= 202002L )
#define lest_CPP23_OR_GREATER  ( lest_CPLUSPLUS >= 202300L )

#ifndef  __has_cpp_attribute
# define __has_cpp_attribute(name)  0
#endif

// Indicate argument as possibly unused, if possible:

#if __has_cpp_attribute(maybe_unused) && lest_CPP17_OR_GREATER
# define lest_MAYBE_UNUSED(ARG)  [[maybe_unused]] ARG
#elif defined (__GNUC__)
# define lest_MAYBE_UNUSED(ARG)  ARG __attribute((unused))
#else
# define lest_MAYBE_UNUSED(ARG)  ARG
#endif

#if ! defined( lest_NO_SHORT_MACRO_NAMES ) && ! defined( lest_NO_SHORT_ASSERTION_NAMES )
# define MODULE            lest_MODULE

# if ! lest_FEATURE_AUTO_REGISTER
#  define CASE             lest_CASE
#  define CASE_ON          lest_CASE_ON
#  define SCENARIO         lest_SCENARIO
# endif

# define SETUP             lest_SETUP
# define SECTION           lest_SECTION

# define EXPECT            lest_EXPECT
# define EXPECT_NOT        lest_EXPECT_NOT
# define EXPECT_NO_THROW   lest_EXPECT_NO_THROW
# define EXPECT_THROWS     lest_EXPECT_THROWS
# define EXPECT_THROWS_AS  lest_EXPECT_THROWS_AS

# define GIVEN             lest_GIVEN
# define WHEN              lest_WHEN
# define THEN              lest_THEN
# define AND_WHEN          lest_AND_WHEN
# define AND_THEN          lest_AND_THEN
#endif

#if lest_FEATURE_AUTO_REGISTER
#define lest_SCENARIO( specification, sketch )  lest_CASE( specification, lest::text("Scenario: ") + sketch  )
#else
#define lest_SCENARIO( sketch  )  lest_CASE(    lest::text("Scenario: ") + sketch  )
#endif
#define lest_GIVEN(    context )  lest_SETUP(   lest::text("   Given: ") + context )
#define lest_WHEN(     story   )  lest_SECTION( lest::text("    When: ") + story   )
#define lest_THEN(     story   )  lest_SECTION( lest::text("    Then: ") + story   )
#define lest_AND_WHEN( story   )  lest_SECTION( lest::text("And then: ") + story   )
#define lest_AND_THEN( story   )  lest_SECTION( lest::text("And then: ") + story   )

#if lest_FEATURE_AUTO_REGISTER

#define lest_CASE( specification, proposition ) \
    static void lest_FUNCTION( lest::env & ); \
    static lest::add_test lest_REGISTRAR( specification, lest::test( proposition, lest_FUNCTION ) ); \
    static void lest_FUNCTION( lest_MAYBE_UNUSED( lest::env & lest_env ) )


#else // lest_FEATURE_AUTO_REGISTER

# define lest_CASE( proposition ) \
    proposition, []( lest::env & lest_env )

# define lest_CASE_ON( proposition, ... ) \
    proposition, [__VA_ARGS__]( lest::env & lest_env )

# define lest_MODULE( specification, module ) \
    namespace { lest::add_module _( specification, module ); }

#endif //lest_FEATURE_AUTO_REGISTER

#define lest_SETUP( context ) \
    for ( int lest__section = 0, lest__count = 1; lest__section < lest__count; lest__count -= 0==lest__section++ ) \
       for ( lest::ctx lest__ctx_setup( lest_env, context ); lest__ctx_setup; )

#define lest_SECTION( proposition ) \
    lest_SUPPRESS_WSHADOW \
    static int lest_UNIQUE( id ) = 0; \
    if ( lest::guard( lest_UNIQUE( id ), lest__section, lest__count ) ) \
        for ( int lest__section = 0, lest__count = 1; lest__section < lest__count; lest__count -= 0==lest__section++ ) \
            for ( lest::ctx lest__ctx_section( lest_env, proposition ); lest__ctx_section; ) \
    lest_RESTORE_WARNINGS

#define lest_EXPECT( expr ) \
    do { \
        try \
        { \
            if ( lest::result score = lest_DECOMPOSE( expr ) ) \
                throw lest::failure{ lest_LOCATION, #expr, score.decomposition }; \
            else if ( lest_env.pass() ) \
                lest::report( lest_env.os, lest::passing{ lest_LOCATION, #expr, score.decomposition, lest_env.zen() }, lest_env.context() ); \
        } \
        catch(...) \
        { \
            lest::inform( lest_LOCATION, #expr ); \
        } \
    } while ( lest::is_false() )

#define lest_EXPECT_NOT( expr ) \
    do { \
        try \
        { \
            if ( lest::result score = lest_DECOMPOSE( expr ) ) \
            { \
                if ( lest_env.pass() ) \
                    lest::report( lest_env.os, lest::passing{ lest_LOCATION, lest::not_expr( #expr ), lest::not_expr( score.decomposition ), lest_env.zen() }, lest_env.context() ); \
            } \
            else \
                throw lest::failure{ lest_LOCATION, lest::not_expr( #expr ), lest::not_expr( score.decomposition ) }; \
        } \
        catch(...) \
        { \
            lest::inform( lest_LOCATION, lest::not_expr( #expr ) ); \
        } \
    } while ( lest::is_false() )

#define lest_EXPECT_NO_THROW( expr ) \
    do \
    { \
        try \
        { \
            lest_SUPPRESS_WUNUSED \
            expr; \
            lest_RESTORE_WARNINGS \
        } \
        catch (...) \
        { \
            lest::inform( lest_LOCATION, #expr ); \
        } \
        if ( lest_env.pass() ) \
            lest::report( lest_env.os, lest::got_none( lest_LOCATION, #expr ), lest_env.context() ); \
    } while ( lest::is_false() )

#define lest_EXPECT_THROWS( expr ) \
    do \
    { \
        try \
        { \
            lest_SUPPRESS_WUNUSED \
            expr; \
            lest_RESTORE_WARNINGS \
        } \
        catch (...) \
        { \
            if ( lest_env.pass() ) \
                lest::report( lest_env.os, lest::got{ lest_LOCATION, #expr }, lest_env.context() ); \
            break; \
        } \
        throw lest::expected{ lest_LOCATION, #expr }; \
    } \
    while ( lest::is_false() )

#define lest_EXPECT_THROWS_AS( expr, excpt ) \
    do \
    { \
        try \
        { \
            lest_SUPPRESS_WUNUSED \
            expr; \
            lest_RESTORE_WARNINGS \
        }  \
        catch ( excpt & ) \
        { \
            if ( lest_env.pass() ) \
                lest::report( lest_env.os, lest::got{ lest_LOCATION, #expr, lest::of_type( #excpt ) }, lest_env.context() ); \
            break; \
        } \
        catch (...) {} \
        throw lest::expected{ lest_LOCATION, #expr, lest::of_type( #excpt ) }; \
    } \
    while ( lest::is_false() )

#define lest_UNIQUE(  name       ) lest_UNIQUE2( name, __LINE__ )
#define lest_UNIQUE2( name, line ) lest_UNIQUE3( name, line )
#define lest_UNIQUE3( name, line ) name ## line

#define lest_DECOMPOSE( expr ) ( lest::expression_decomposer() << expr )

#define lest_FUNCTION  lest_UNIQUE(__lest_function__  )
#define lest_REGISTRAR lest_UNIQUE(__lest_registrar__ )

#define lest_LOCATION  lest::location{__FILE__, __LINE__}

namespace lest {

const int exit_max_value = 255;

using text  = std::string;
using texts = std::vector<text>;

struct env;

struct test
{
    text name;
    std::function<void( env & )> behaviour;

#if lest_FEATURE_AUTO_REGISTER
    test( text name_, std::function<void( env & )> behaviour_ )
    : name( name_), behaviour( behaviour_) {}
#endif
};

using tests = std::vector<test>;

#if lest_FEATURE_AUTO_REGISTER

struct add_test
{
    add_test( tests & specification, test const & test_case )
    {
        specification.push_back( test_case );
    }
};

#else

struct add_module
{
    template< std::size_t N >
    add_module( tests & specification, test const (&module)[N] )
    {
        specification.insert( specification.end(), std::begin( module ), std::end( module ) );
    }
};

#endif

struct result
{
    const bool passed;
    const text decomposition;

    template< typename T >
    result( T const & passed_, text decomposition_)
    : passed( !!passed_), decomposition( decomposition_) {}

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
    failure( location where_, text expr_, text decomposition_)
    : message{ "failed", where_, expr_ + " for " + decomposition_ } {}
};

struct success : message
{
//    using message::message;   // VC is lagging here

    success( text kind_, location where_, text expr_, text note_ = "" )
    : message( kind_, where_, expr_, note_ ) {}
};

struct passing : success
{
    passing( location where_, text expr_, text decomposition_, bool zen )
    : success( "passed", where_, expr_ + (zen ? "":" for " + decomposition_) ) {}
};

struct got_none : success
{
    got_none( location where_, text expr_ )
    : success( "passed: got no exception", where_, expr_ ) {}
};

struct got : success
{
    got( location where_, text expr_)
    : success( "passed: got exception", where_, expr_) {}

    got( location where_, text expr_, text excpt_)
    : success( "passed: got exception " + excpt_, where_, expr_) {}
};

struct expected : message
{
    expected( location where_, text expr_, text excpt_ = "" )
    : message{ "failed: didn't get exception", where_, expr_, excpt_ } {}
};

struct unexpected : message
{
    unexpected( location where_, text expr_, text note_ = "" )
    : message{ "failed: got unexpected exception", where_, expr_, note_ } {}
};

struct guard
{
    int & id;
    int const & section;

    guard( int & id_, int const & section_, int & count )
    : id( id_), section( section_)
    {
        if ( section == 0 )
            id = count++ - 1;
    }
    operator bool() { return id == section; }
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

    approx operator()( double new_magnitude )
    {
        approx appr( new_magnitude );
        appr.epsilon( epsilon_ );
        appr.scale  ( scale_   );
        return appr;
    }

    double magnitude() const { return magnitude_; }

    approx & epsilon( double epsilon ) { epsilon_ = epsilon; return *this; }
    approx & scale  ( double scale   ) { scale_   = scale;   return *this; }

    friend bool operator == ( double lhs, approx const & rhs )
    {
        // Thanks to Richard Harris for his help refining this formula.
        return std::abs( lhs - rhs.magnitude_ ) < rhs.epsilon_ * ( rhs.scale_ + (std::min)( std::abs( lhs ), std::abs( rhs.magnitude_ ) ) );
    }

    friend bool operator == ( approx const & lhs, double rhs ) { return  operator==( rhs, lhs ); }
    friend bool operator != ( double lhs, approx const & rhs ) { return !operator==( lhs, rhs ); }
    friend bool operator != ( approx const & lhs, double rhs ) { return !operator==( rhs, lhs ); }

    friend bool operator <= ( double lhs, approx const & rhs ) { return lhs < rhs.magnitude_ || lhs == rhs; }
    friend bool operator <= ( approx const & lhs, double rhs ) { return lhs.magnitude_ < rhs || lhs == rhs; }
    friend bool operator >= ( double lhs, approx const & rhs ) { return lhs > rhs.magnitude_ || lhs == rhs; }
    friend bool operator >= ( approx const & lhs, double rhs ) { return lhs.magnitude_ > rhs || lhs == rhs; }

private:
    double epsilon_;
    double scale_;
    double magnitude_;
};

inline bool is_false(           ) { return false; }
inline bool is_true ( bool flag ) { return  flag; }

inline text not_expr( text message )
{
    return "! ( " + message + " )";
}

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
    catch( message const & )
    {
        throw;
    }
    catch( std::exception const & e )
    {
        throw unexpected{ where, expr, with_message( e.what() ) }; \
    }
    catch(...)
    {
        throw unexpected{ where, expr, "of unknown type" }; \
    }
}

// Expression decomposition:

template< typename T >
auto make_value_string( T const & value ) -> std::string;

template< typename T >
auto make_memory_string( T const & item ) -> std::string;

#if lest_FEATURE_LITERAL_SUFFIX
inline char const * sfx( char const  * txt ) { return txt; }
#else
inline char const * sfx( char const  *      ) { return ""; }
#endif

inline std::string transformed( char chr )
{
    struct Tr { char chr; char const * str; } table[] =
    {
        {'\\', "\\\\" },
        {'\r', "\\r"  }, {'\f', "\\f" },
        {'\n', "\\n"  }, {'\t', "\\t" },
    };

    for ( auto tr : table )
    {
        if ( chr == tr.chr )
            return tr.str;
    }

    // The cast below helps suppress warnings ("-Wtype-limits" on GCC) on architectures where `char` is unsigned.
    auto unprintable = [](char c){ return 0 <= static_cast<signed char>(c) && c < ' '; };

    auto to_hex_string = [](char c)
    {
        std::ostringstream os;
        os << "\\x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>( static_cast<unsigned char>(c) );
        return os.str();
    };

    return unprintable( chr  ) ? to_hex_string( chr ) : std::string( 1, chr );
}

inline std::string make_tran_string( std::string const & txt ) { std::ostringstream os; for(auto c:txt) os << transformed(c); return os.str(); }
inline std::string make_strg_string( std::string const & txt ) { return "\"" + make_tran_string(                 txt   ) + "\"" ; }
inline std::string make_char_string(                char chr ) { return "\'" + make_tran_string( std::string( 1, chr ) ) + "\'" ; }

inline std::string to_string( std::nullptr_t              ) { return "nullptr"; }
inline std::string to_string( std::string     const & txt ) { return make_strg_string( txt ); }
#if lest_FEATURE_WSTRING
inline std::string to_string( std::wstring    const & txt ) ;
#endif

inline std::string to_string( char    const * const   txt ) { return txt ? make_strg_string( txt ) : "{null string}"; }
inline std::string to_string( char          * const   txt ) { return txt ? make_strg_string( txt ) : "{null string}"; }
#if lest_FEATURE_WSTRING
inline std::string to_string( wchar_t const * const   txt ) { return txt ? to_string( std::wstring( txt ) ) : "{null string}"; }
inline std::string to_string( wchar_t       * const   txt ) { return txt ? to_string( std::wstring( txt ) ) : "{null string}"; }
#endif

inline std::string to_string(          bool          flag ) { return flag ? "true" : "false"; }

inline std::string to_string(   signed short        value ) { return make_value_string( value ) ;             }
inline std::string to_string( unsigned short        value ) { return make_value_string( value ) + sfx("u"  ); }
inline std::string to_string(   signed   int        value ) { return make_value_string( value ) ;             }
inline std::string to_string( unsigned   int        value ) { return make_value_string( value ) + sfx("u"  ); }
inline std::string to_string(   signed  long        value ) { return make_value_string( value ) + sfx("l"  ); }
inline std::string to_string( unsigned  long        value ) { return make_value_string( value ) + sfx("ul" ); }
inline std::string to_string(   signed  long long   value ) { return make_value_string( value ) + sfx("ll" ); }
inline std::string to_string( unsigned  long long   value ) { return make_value_string( value ) + sfx("ull"); }
inline std::string to_string(         double        value ) { return make_value_string( value ) ;             }
inline std::string to_string(          float        value ) { return make_value_string( value ) + sfx("f"  ); }

inline std::string to_string(   signed char           chr ) { return make_char_string( static_cast<char>( chr ) ); }
inline std::string to_string( unsigned char           chr ) { return make_char_string( static_cast<char>( chr ) ); }
inline std::string to_string(          char           chr ) { return make_char_string(                    chr   ); }

template< typename T >
struct is_streamable
{
    template< typename U >
    static auto test( int ) -> decltype( std::declval<std::ostream &>() << std::declval<U>(), std::true_type() );

    template< typename >
    static auto test( ... ) -> std::false_type;

#ifdef _MSC_VER
    enum { value = std::is_same< decltype( test<T>(0) ), std::true_type >::value };
#else
    static constexpr bool value = std::is_same< decltype( test<T>(0) ), std::true_type >::value;
#endif
};

template< typename T >
struct is_container
{
    template< typename U >
    static auto test( int ) -> decltype( std::declval<U>().begin() == std::declval<U>().end(), std::true_type() );

    template< typename >
    static auto test( ... ) -> std::false_type;

#ifdef _MSC_VER
    enum { value = std::is_same< decltype( test<T>(0) ), std::true_type >::value };
#else
    static constexpr bool value = std::is_same< decltype( test<T>(0) ), std::true_type >::value;
#endif
};

template< typename T, typename R >
using ForEnum = typename std::enable_if< std::is_enum<T>::value, R>::type;

template< typename T, typename R >
using ForNonEnum = typename std::enable_if< ! std::is_enum<T>::value, R>::type;

template< typename T, typename R >
using ForStreamable = typename std::enable_if< is_streamable<T>::value, R>::type;

template< typename T, typename R >
using ForNonStreamable = typename std::enable_if< ! is_streamable<T>::value, R>::type;

template< typename T, typename R >
using ForContainer = typename std::enable_if< is_container<T>::value, R>::type;

template< typename T, typename R >
using ForNonContainerNonPointer = typename std::enable_if< ! (is_container<T>::value || std::is_pointer<T>::value), R>::type;

template< typename T >
auto make_enum_string( T const & item ) -> ForNonEnum<T, std::string>
{
#if lest__cpp_rtti
    return text("[type: ") + typeid(T).name() + "]: " + make_memory_string( item );
#else
    return text("[type: (no RTTI)]: ") + make_memory_string( item );
#endif
}

template< typename T >
auto make_enum_string( T const & item ) -> ForEnum<T, std::string>
{
    return to_string( static_cast<typename std::underlying_type<T>::type>( item ) );
}

template< typename T >
auto make_string( T const & item ) -> ForNonStreamable<T, std::string>
{
    return make_enum_string( item );
}

template< typename T >
auto make_string( T const & item ) -> ForStreamable<T, std::string>
{
    std::ostringstream os; os << item; return os.str();
}

template<typename T1, typename T2>
auto make_string( std::pair<T1,T2> const & pair ) -> std::string
{
    std::ostringstream oss;
    oss << "{ " << to_string( pair.first ) << ", " << to_string( pair.second ) << " }";
    return oss.str();
}

template< typename TU, std::size_t N >
struct make_tuple_string
{
    static std::string make( TU const & tuple )
    {
        std::ostringstream os;
        os << to_string( std::get<N - 1>( tuple ) ) << ( N < std::tuple_size<TU>::value ? ", ": " ");
        return make_tuple_string<TU, N - 1>::make( tuple ) + os.str();
    }
};

template< typename TU >
struct make_tuple_string<TU, 0>
{
    static std::string make( TU const & ) { return ""; }
};

template< typename ...TS >
auto make_string( std::tuple<TS...> const & tuple ) -> std::string
{
    return "{ " + make_tuple_string<std::tuple<TS...>, sizeof...(TS)>::make( tuple ) + "}";
}

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

template<typename C, typename R>
auto to_string( R C::* ptr ) -> std::string
{
    return ! ptr ? "nullptr" : make_string( ptr );
}

template< typename T >
auto to_string( T const & item ) -> ForNonContainerNonPointer<T, std::string>
{
    return make_string( item );
}

template< typename C >
auto to_string( C const & cont ) -> ForContainer<C, std::string>
{
    std::ostringstream os;
    os << "{ ";
    for ( auto & x : cont )
    {
        os << to_string( x ) << ", ";
    }
    os << "}";
    return os.str();
}

#if lest_FEATURE_WSTRING
inline
auto to_string( std::wstring const & txt ) -> std::string
{
    std::string result; result.reserve( txt.size() );

    for( auto & chr : txt )
    {
        result += chr <= 0xff ? static_cast<char>( chr ) : '?';
    }
    return to_string( result );
}
#endif

template< typename T >
auto make_value_string( T const & value ) -> std::string
{
    std::ostringstream os; os << value; return os.str();
}

inline
auto make_memory_string( void const * item, std::size_t size ) -> std::string
{
    // reverse order for little endian architectures:

    auto is_little_endian = []
    {
        union U { int i = 1; char c[ sizeof(int) ]; };

        return 1 != U{}.c[ sizeof(int) - 1 ];
    };

    int i = 0, end = static_cast<int>( size ), inc = 1;

    if ( is_little_endian() ) { i = end - 1; end = inc = -1; }

    unsigned char const * bytes = static_cast<unsigned char const *>( item );

    std::ostringstream os;
    os << "0x" << std::setfill( '0' ) << std::hex;
    for ( ; i != end; i += inc )
    {
        os << std::setw(2) << static_cast<unsigned>( bytes[i] ) << " ";
    }
    return os.str();
}

template< typename T >
auto make_memory_string( T const & item ) -> std::string
{
    return make_memory_string( &item, sizeof item );
}

inline
auto to_string( approx const & appr ) -> std::string
{
    return to_string( appr.magnitude() );
}

template< typename L, typename R >
auto to_string( L const & lhs, std::string op, R const & rhs ) -> std::string
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
    template <typename L>
    expression_lhs<L const &> operator<< ( L const & operand )
    {
        return expression_lhs<L const &>( operand );
    }
};

// Reporter:

#if lest_FEATURE_COLOURISE

inline text red  ( text words ) { return "\033[1;31m" + words + "\033[0m"; }
inline text green( text words ) { return "\033[1;32m" + words + "\033[0m"; }
inline text gray ( text words ) { return "\033[1;30m" + words + "\033[0m"; }

inline bool starts_with( text words, text with )
{
    return 0 == words.find( with );
}

inline text replace( text words, text from, text to )
{
    size_t pos = words.find( from );
    return pos == std::string::npos ? words : words.replace( pos, from.length(), to  );
}

inline text colour( text words )
{
    if      ( starts_with( words, "failed" ) ) return replace( words, "failed", red  ( "failed" ) );
    else if ( starts_with( words, "passed" ) ) return replace( words, "passed", green( "passed" ) );

    return replace( words, "for", gray( "for" ) );
}

inline bool is_cout( std::ostream & os ) { return &os == &std::cout; }

struct colourise
{
    const text words;

    colourise( text words )
    : words( words ) {}

    // only colourise for std::cout, not for a stringstream as used in tests:

    std::ostream & operator()( std::ostream & os ) const
    {
        return is_cout( os ) ? os << colour( words ) : os << words;
    }
};

inline std::ostream & operator<<( std::ostream & os, colourise words ) { return words( os ); }
#else
inline text colourise( text words ) { return words; }
#endif

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

inline void report( std::ostream & os, message const & e, text test )
{
    os << e.where << ": " << colourise( e.kind ) << e.note << ": " << test << ": " << colourise( e.what() ) << std::endl;
}

// Test runner:

#if lest_FEATURE_REGEX_SEARCH
    inline bool search( text re, text line )
    {
        return std::regex_search( line, std::regex( re ) );
    }
#else
    inline bool search( text part, text line )
    {
        auto case_insensitive_equal = []( char a, char b )
        {
            return tolower( a ) == tolower( b );
        };

        return std::search(
            line.begin(), line.end(),
            part.begin(), part.end(), case_insensitive_equal ) != line.end();
    }
#endif

inline bool match( texts whats, text line )
{
    for ( auto & what : whats )
    {
        if ( search( what, line ) )
            return true;
    }
    return false;
}

inline bool select( text name, texts include )
{
    auto none = []( texts args ) { return args.size() == 0; };

#if lest_FEATURE_REGEX_SEARCH
    auto hidden = []( text arg ){ return match( { "\\[\\..*", "\\[hide\\]" }, arg ); };
#else
    auto hidden = []( text arg ){ return match( { "[.", "[hide]" }, arg ); };
#endif

    if ( none( include ) )
    {
        return ! hidden( name );
    }

    bool any = false;
    for ( auto pos = include.rbegin(); pos != include.rend(); ++pos )
    {
        auto & part = *pos;

        if ( part == "@" || part == "*" )
            return true;

        if ( search( part, name ) )
            return true;

        if ( '!' == part[0] )
        {
            any = true;
            if ( search( part.substr(1), name ) )
                return false;
        }
        else
        {
            any = false;
        }
    }
    return any && ! hidden( name );
}

inline int indefinite( int repeat ) { return repeat == -1; }

using seed_t = std::mt19937::result_type;

struct options
{
    bool help    = false;
    bool abort   = false;
    bool count   = false;
    bool list    = false;
    bool tags    = false;
    bool time    = false;
    bool pass    = false;
    bool zen     = false;
    bool lexical = false;
    bool random  = false;
    bool verbose = false;
    bool version = false;
    int  repeat  = 1;
    seed_t seed  = 0;
};

struct env
{
    std::ostream & os;
    options opt;
    text testing;
    std::vector< text > ctx;

    env( std::ostream & out, options option )
    : os( out ), opt( option ), testing(), ctx() {}

    env & operator()( text test )
    {
        clear(); testing = test; return *this;
    }

    bool abort() { return opt.abort; }
    bool pass()  { return opt.pass; }
    bool zen()   { return opt.zen; }

    void clear() { ctx.clear(); }
    void pop()   { ctx.pop_back(); }
    void push( text proposition ) { ctx.emplace_back( proposition ); }

    text context() { return testing + sections(); }

    text sections()
    {
        if ( ! opt.verbose )
            return "";

        text msg;
        for( auto section : ctx )
        {
            msg += "\n  " + section;
        }
        return msg;
    }
};

struct ctx
{
    env & environment;
    bool once;

    ctx( env & environment_, text proposition_ )
    : environment( environment_), once( true )
    {
        environment.push( proposition_);
    }

    ~ctx()
    {
#if lest_CPP17_OR_GREATER
        if ( std::uncaught_exceptions() == 0 )
#else
        if ( ! std::uncaught_exception() )
#endif
        {
            environment.pop();
        }
    }

    explicit operator bool() { bool result = once; once = false; return result; }
};

struct action
{
    std::ostream & os;

    action( std::ostream & out ) : os( out ) {}

    action( action const & ) = delete;
    void operator=( action const & ) = delete;

    operator      int() { return 0; }
    bool        abort() { return false; }
    action & operator()( test ) { return *this; }
};

struct print : action
{
    print( std::ostream & out ) : action( out ) {}

    print & operator()( test testing )
    {
        os << testing.name << "\n"; return *this;
    }
};

inline texts tags( text name, texts result = {} )
{
    auto none = std::string::npos;
    auto lb   = name.find_first_of( "[" );
    auto rb   = name.find_first_of( "]" );

    if ( lb == none || rb == none )
        return result;

    result.emplace_back( name.substr( lb, rb - lb + 1 ) );

    return tags( name.substr( rb + 1 ), result );
}

struct ptags : action
{
    std::set<text> result;

    ptags( std::ostream & out ) : action( out ), result() {}

    ptags & operator()( test testing )
    {
        for ( auto & tag : tags( testing.name ) )
            result.insert( tag );

        return *this;
    }

    ~ptags()
    {
        std::copy( result.begin(), result.end(), std::ostream_iterator<text>( os, "\n" ) );
    }
};

struct count : action
{
    int n = 0;

    count( std::ostream & out ) : action( out ) {}

    count & operator()( test ) { ++n; return *this; }

    ~count()
    {
        os << n << " selected " << pluralise("test", n) << "\n";
    }
};

struct timer
{
    using time = std::chrono::high_resolution_clock;

    time::time_point start = time::now();

    double elapsed_seconds() const
    {
        return 1e-6 * static_cast<double>( std::chrono::duration_cast< std::chrono::microseconds >( time::now() - start ).count() );
    }
};

struct times : action
{
    env output;
    int selected = 0;
    int failures = 0;

    timer total;

    times( std::ostream & out, options option )
    : action( out ), output( out, option ), total()
    {
        os << std::setfill(' ') << std::fixed << std::setprecision( lest_FEATURE_TIME_PRECISION );
    }

    operator int() { return failures; }

    bool abort() { return output.abort() && failures > 0; }

    times & operator()( test testing )
    {
        timer t;

        try
        {
            testing.behaviour( output( testing.name ) );
        }
        catch( message const & )
        {
            ++failures;
        }

        os << std::setw(3) << ( 1000 * t.elapsed_seconds() ) << " ms: " << testing.name  << "\n";

        return *this;
    }

    ~times()
    {
        os << "Elapsed time: " << std::setprecision(1) << total.elapsed_seconds() << " s\n";
    }
};

struct confirm : action
{
    env output;
    int selected = 0;
    int failures = 0;

    confirm( std::ostream & out, options option )
    : action( out ), output( out, option ) {}

    operator int() { return failures; }

    bool abort() { return output.abort() && failures > 0; }

    confirm & operator()( test testing )
    {
        try
        {
            ++selected; testing.behaviour( output( testing.name ) );
        }
        catch( message const & e )
        {
            ++failures; report( os, e, output.context() );
        }
        return *this;
    }

    ~confirm()
    {
        if ( failures > 0 )
        {
            os << failures << " out of " << selected << " selected " << pluralise("test", selected) << " " << colourise( "failed.\n" );
        }
        else if ( output.pass() )
        {
            os << "All " << selected << " selected " << pluralise("test", selected) << " " << colourise( "passed.\n" );
        }
    }
};

template< typename Action >
bool abort( Action & perform )
{
    return perform.abort();
}

template< typename Action >
Action && for_test( tests specification, texts in, Action && perform, int n = 1 )
{
    for ( int i = 0; indefinite( n ) || i < n; ++i )
    {
        for ( auto & testing : specification )
        {
            if ( select( testing.name, in ) )
                if ( abort( perform( testing ) ) )
                    return std::move( perform );
        }
    }
    return std::move( perform );
}

inline void sort( tests & specification )
{
    auto test_less = []( test const & a, test const & b ) { return a.name < b.name; };
    std::sort( specification.begin(), specification.end(), test_less );
}

inline void shuffle( tests & specification, options option )
{
    std::shuffle( specification.begin(), specification.end(), std::mt19937( option.seed ) );
}

// workaround MinGW bug, http://stackoverflow.com/a/16132279:

inline int stoi( text num )
{
    return static_cast<int>( std::strtol( num.c_str(), nullptr, 10 ) );
}

inline bool is_number( text arg )
{
    return std::all_of( arg.begin(), arg.end(), ::isdigit );
}

inline seed_t seed( text opt, text arg )
{
    if ( is_number( arg ) )
        return static_cast<seed_t>( lest::stoi( arg ) );

    if ( arg == "time" )
        return static_cast<seed_t>( std::chrono::high_resolution_clock::now().time_since_epoch().count() );

    throw std::runtime_error( "expecting 'time' or positive number with option '" + opt + "', got '" + arg + "' (try option --help)" );
}

inline int repeat( text opt, text arg )
{
    const int num = lest::stoi( arg );

    if ( indefinite( num ) || num >= 0 )
        return num;

    throw std::runtime_error( "expecting '-1' or positive number with option '" + opt + "', got '" + arg + "' (try option --help)" );
}

inline auto split_option( text arg ) -> std::tuple<text, text>
{
    auto pos = arg.rfind( '=' );

    return pos == text::npos
                ? std::make_tuple( arg, "" )
                : std::make_tuple( arg.substr( 0, pos ), arg.substr( pos + 1 ) );
}

inline auto split_arguments( texts args ) -> std::tuple<options, texts>
{
    options option; texts in;

    bool in_options = true;

    for ( auto & arg : args )
    {
        if ( in_options )
        {
            text opt, val;
            std::tie( opt, val ) = split_option( arg );

            if      ( opt[0] != '-'                             ) { in_options     = false;           }
            else if ( opt == "--"                               ) { in_options     = false; continue; }
            else if ( opt == "-h"      || "--help"       == opt ) { option.help    =  true; continue; }
            else if ( opt == "-a"      || "--abort"      == opt ) { option.abort   =  true; continue; }
            else if ( opt == "-c"      || "--count"      == opt ) { option.count   =  true; continue; }
            else if ( opt == "-g"      || "--list-tags"  == opt ) { option.tags    =  true; continue; }
            else if ( opt == "-l"      || "--list-tests" == opt ) { option.list    =  true; continue; }
            else if ( opt == "-t"      || "--time"       == opt ) { option.time    =  true; continue; }
            else if ( opt == "-p"      || "--pass"       == opt ) { option.pass    =  true; continue; }
            else if ( opt == "-z"      || "--pass-zen"   == opt ) { option.zen     =  true; continue; }
            else if ( opt == "-v"      || "--verbose"    == opt ) { option.verbose =  true; continue; }
            else if (                     "--version"    == opt ) { option.version =  true; continue; }
            else if ( opt == "--order" && "declared"     == val ) { /* by definition */   ; continue; }
            else if ( opt == "--order" && "lexical"      == val ) { option.lexical =  true; continue; }
            else if ( opt == "--order" && "random"       == val ) { option.random  =  true; continue; }
            else if ( opt == "--random-seed" ) { option.seed   = seed  ( "--random-seed", val ); continue; }
            else if ( opt == "--repeat"      ) { option.repeat = repeat( "--repeat"     , val ); continue; }
            else throw std::runtime_error( "unrecognised option '" + arg + "' (try option --help)" );
        }
        in.push_back( arg );
    }
    option.pass = option.pass || option.zen;

    return std::make_tuple( option, in );
}

inline int usage( std::ostream & os )
{
    os <<
        "\nUsage: test [options] [test-spec ...]\n"
        "\n"
        "Options:\n"
        "  -h, --help         this help message\n"
        "  -a, --abort        abort at first failure\n"
        "  -c, --count        count selected tests\n"
        "  -g, --list-tags    list tags of selected tests\n"
        "  -l, --list-tests   list selected tests\n"
        "  -p, --pass         also report passing tests\n"
        "  -z, --pass-zen     ... without expansion\n"
        "  -t, --time         list duration of selected tests\n"
        "  -v, --verbose      also report passing or failing sections\n"
        "  --order=declared   use source code test order (default)\n"
        "  --order=lexical    use lexical sort test order\n"
        "  --order=random     use random test order\n"
        "  --random-seed=n    use n for random generator seed\n"
        "  --random-seed=time use time for random generator seed\n"
        "  --repeat=n         repeat selected tests n times (-1: indefinite)\n"
        "  --version          report lest version and compiler used\n"
        "  --                 end options\n"
        "\n"
        "Test specification:\n"
        "  \"@\", \"*\" all tests, unless excluded\n"
        "  empty    all tests, unless tagged [hide] or [.optional-name]\n"
#if lest_FEATURE_REGEX_SEARCH
        "  \"re\"     select tests that match regular expression\n"
        "  \"!re\"    omit tests that match regular expression\n"
#else
        "  \"text\"   select tests that contain text (case insensitive)\n"
        "  \"!text\"  omit tests that contain text (case insensitive)\n"
#endif
        ;
    return 0;
}

inline text compiler()
{
    std::ostringstream os;
#if   defined (__clang__ )
    os << "clang " << __clang_version__;
#elif defined (__GNUC__  )
    os << "gcc " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__;
#elif defined ( _MSC_VER )
    os << "MSVC " << (_MSC_VER / 100 - 5 - (_MSC_VER < 1900)) << " (" << _MSC_VER << ")";
#else
    os << "[compiler]";
#endif
    return os.str();
}

inline int version( std::ostream & os )
{
    os << "lest version "  << lest_VERSION << "\n"
       << "Compiled with " << compiler()   << " on " << __DATE__ << " at " << __TIME__ << ".\n"
       << "For more information, see https://github.com/martinmoene/lest.\n";
    return 0;
}

inline int run( tests specification, texts arguments, std::ostream & os = std::cout )
{
    try
    {
        options option; texts in;
        std::tie( option, in ) = split_arguments( arguments );

        if ( option.lexical ) {    sort( specification         ); }
        if ( option.random  ) { shuffle( specification, option ); }

        if ( option.help    ) { return usage   ( os ); }
        if ( option.version ) { return version ( os ); }
        if ( option.count   ) { return for_test( specification, in, count( os ) ); }
        if ( option.list    ) { return for_test( specification, in, print( os ) ); }
        if ( option.tags    ) { return for_test( specification, in, ptags( os ) ); }
        if ( option.time    ) { return for_test( specification, in, times( os, option ) ); }

        return for_test( specification, in, confirm( os, option ), option.repeat );
    }
    catch ( std::exception const & e )
    {
        os << "Error: " << e.what() << "\n";
        return 1;
    }
}

inline int run( tests specification, int argc, char * argv[], std::ostream & os = std::cout )
{
    return run( specification, texts( argv + 1, argv + argc ), os  );
}

template< std::size_t N >
int run( test const (&specification)[N], texts arguments, std::ostream & os = std::cout )
{
    std::cout.sync_with_stdio( false );
    return (std::min)( run( tests( specification, specification + N ), arguments, os  ), exit_max_value );
}

template< std::size_t N >
int run( test const (&specification)[N], std::ostream & os = std::cout )
{
    return run( tests( specification, specification + N ), {}, os  );
}

template< std::size_t N >
int run( test const (&specification)[N], int argc, char * argv[], std::ostream & os = std::cout )
{
    return run( tests( specification, specification + N ), texts( argv + 1, argv + argc ), os  );
}

} // namespace lest

#if defined (__clang__)
# pragma clang diagnostic pop
#elif defined (__GNUC__)
# pragma GCC   diagnostic pop
#endif

#endif // LEST_LEST_HPP_INCLUDED
