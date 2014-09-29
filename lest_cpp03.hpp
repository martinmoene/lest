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
#include <cstddef>

#ifndef  lest_FEATURE_LITERAL_SUFFIX
# define lest_FEATURE_LITERAL_SUFFIX 0
#endif

#ifndef  lest_FEATURE_REGEX_SEARCH
# define lest_FEATURE_REGEX_SEARCH 0
#endif

#ifndef  lest_FEATURE_TIME
# define lest_FEATURE_TIME 1
#endif

#ifdef _WIN32
# define lest_PLATFORM_IS_WINDOWS 1
#endif

#if lest_FEATURE_REGEX_SEARCH
# include <regex>
#endif

#if lest_FEATURE_TIME
# if lest_PLATFORM_IS_WINDOWS
#  include <iomanip>
#  include <windows.h>
# else
#  include <iomanip>
#  include <sys/time.h>
# endif
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
    template<typename T> T const & (max)(T const & a, T const & b) { return a >= b ? a : b; }
#else
    using std::abs;
    using std::max;
#endif
}

#ifndef lest_NO_SHORT_ASSERTION_NAMES
# define EXPECT           lest_EXPECT
# define EXPECT_NOT       lest_EXPECT_NOT
# define EXPECT_THROWS    lest_EXPECT_THROWS
# define EXPECT_THROWS_AS lest_EXPECT_THROWS_AS
#endif

#define lest_TEST \
    lest_CASE

#define lest_CASE( specification, name ) \
    void lest_FUNCTION( lest::env & ); \
    namespace { lest::registrar lest_REGISTRAR( specification, lest::test( name, lest_FUNCTION ) ); } \
    void lest_FUNCTION( lest::env & $ )

#define lest_ADD_TEST( specification, test ) \
    specification.push_back( test )

#define lest_EXPECT( expr ) \
    do { \
        try \
        { \
            if ( lest::result score = lest_DECOMPOSE( expr ) ) \
                throw lest::failure( lest_LOCATION, #expr, score.decomposition ); \
            else if ( $.pass ) \
                lest::report( $.os, lest::passing( lest_LOCATION, #expr, score.decomposition ), $.testing ); \
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
                if ( $.pass ) \
                    lest::report( $.os, lest::passing( lest_LOCATION, lest::not_expr( #expr ), lest::not_expr( score.decomposition ) ), $.testing ); \
            } \
            else \
                throw lest::failure( lest_LOCATION, lest::not_expr( #expr ), lest::not_expr( score.decomposition ) ); \
        } \
        catch(...) \
        { \
            lest::inform( lest_LOCATION, lest::not_expr( #expr ) ); \
        } \
    } while ( lest::is_false() )

#define lest_EXPECT_THROWS( expr ) \
    do \
    { \
        try \
        { \
            lest::is_true( expr ); \
        } \
        catch (...) \
        { \
            if ( $.pass ) \
                lest::report( $.os, lest::got( lest_LOCATION, #expr ), $.testing ); \
            break; \
        } \
        throw lest::expected( lest_LOCATION, #expr ); \
    } \
    while ( lest::is_false() )

#define lest_EXPECT_THROWS_AS( expr, excpt ) \
    do \
    { \
        try \
        { \
            lest::is_true( expr ); \
        }  \
        catch ( excpt & ) \
        { \
            if ( $.pass ) \
                lest::report( $.os, lest::got( lest_LOCATION, #expr, lest::of_type( #excpt ) ), $.testing ); \
            break; \
        } \
        catch (...) {} \
        throw lest::expected( lest_LOCATION, #expr, lest::of_type( #excpt ) ); \
    } \
    while ( lest::is_false() )

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

struct env;

struct test
{
    char const * name;
    void (* behaviour)( env & );

    test( char const * name, void (* behaviour)( env & ) )
    : name (name ), behaviour( behaviour ) {}
};

typedef std::vector<test> tests;
typedef tests test_specification;

struct registrar
{
    registrar( tests & s, test const & t )
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

struct success : message
{
    success( text kind, location where, text expr, text note = "" )
    : message( kind, where, expr, note ) {}
};

struct passing : success
{
    passing( location where, text expr, text decomposition )
    : success( "passed", where, expr + " for " + decomposition ) {}
};

struct got : success
{
    got( location where, text expr )
    : success( "passed: got exception", where, expr ) {}

    got( location where, text expr, text excpt )
    : success( "passed: got exception " + excpt, where, expr ) {}
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

#if lest_FEATURE_REGEX_SEARCH
    inline bool search( text re, text line )
    {
        return std::regex_search( line, std::regex( re ) );
    }
#else
    inline bool case_insensitive_equal( char a, char b )
    {
        return tolower( a ) == tolower( b );
    }

    inline bool search( text part, text line )
    {
        return std::search(
            line.begin(), line.end(),
            part.begin(), part.end(), case_insensitive_equal ) != line.end();
    }
#endif

inline bool match( texts whats, text line )
{
    for ( texts::iterator what = whats.begin(); what != whats.end() ; ++what )
    {
        if ( search( *what, line ) )
            return true;
    }
    return false;
}

inline bool hidden( text name )
{
#if lest_FEATURE_REGEX_SEARCH
    texts skipped; skipped.push_back( "\\[\\.\\]" ); skipped.push_back( "\\[hide\\]" );
#else
    texts skipped; skipped.push_back( "[.]" ); skipped.push_back( "[hide]" );
#endif
    return match( skipped, name );
}

inline bool none( texts args )
{
    return args.size() == 0;
}

inline bool select( text name, texts include )
{
    if ( none( include ) )
    {
        return ! hidden( name );
    }

    bool any = false;
    for ( texts::reverse_iterator pos = include.rbegin(); pos != include.rend(); ++pos )
    {
        text & part = *pos;

        if ( part == "*" || part == "^\\*$" )
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

struct options
{
    options()
    : help(false), abort(false), count(false), list(false), time(false), pass(false) {}

    bool help;
    bool abort;
    bool count;
    bool list;
    bool time;
    bool pass;
};

struct env
{
    std::ostream & os;
    bool pass;
    text testing;

    env( std::ostream & os, bool pass )
    : os( os ), pass( pass ) {}

    env & operator()( text test )
    {
        testing = test; return *this;
    }
};

struct action
{
    std::ostream & os;

    action( std::ostream & os ) : os( os ) {}

    operator      int() { return 0; }
    bool        abort() { return false; }
    action & operator()( test ) { return *this; }

private:
    action( action const & );
};

struct print : action
{
    print( std::ostream & os ) : action( os ) {}

    print &  operator()( test testing )
    {
        os << testing.name << "\n"; return *this;
    }
};

struct count : action
{
    int n;

    count( std::ostream & os ) : action( os ), n( 0 ) {}

    count & operator()( test ) { ++n; return *this; }

    ~count()
    {
        os << n << " selected " << pluralise(n, "test") << "\n";
    }
};

#if lest_FEATURE_TIME

#if lest_PLATFORM_IS_WINDOWS
# ifdef lest_COMPILER_IS_MSVC6
    typedef /*un*/signed __int64 uint64_t;
# else
    typedef unsigned long long uint64_t;
# endif
    uint64_t current_ticks()
    {
        static uint64_t hz = 0, hzo = 0;
        if ( ! hz )
        {
            QueryPerformanceFrequency( (LARGE_INTEGER *) &hz  );
            QueryPerformanceCounter  ( (LARGE_INTEGER *) &hzo );
        }
        uint64_t t; QueryPerformanceCounter( (LARGE_INTEGER *) &t );

        return ( ( t - hzo ) * 1000000 ) / hz;
    }
#else
    uint64_t current_ticks()
    {
        timeval t; gettimeofday( &t, NULL );
        return static_cast<uint64_t>( t.tv_sec ) * 1000000ull + static_cast<uint64_t>( t.tv_usec );
    }
#endif

struct timer
{
    const uint64_t start_ticks;

    timer() : start_ticks( current_ticks() ) {}

    double elapsed_seconds() const
    {
        return ( current_ticks() - start_ticks ) / 1e6;
    }
};

struct times : action
{
    env output;
    options option;
    int selected;
    int failures;

    timer total;

    times( std::ostream & os, options option )
    : action( os ), output( os, option.pass ), option( option ), selected( 0 ), failures( 0 ), total()
    {
        os << std::setfill(' ') << std::fixed << std::setprecision(1);
    }

    operator int() { return failures; }

    bool abort() { return option.abort && failures > 0; }

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

        os << std::setw(5) << ( 1000 * t.elapsed_seconds() ) << " ms: " << testing.name  << "\n";

        return *this;
    }

    ~times()
    {
        os << "Elapsed time: " << total.elapsed_seconds() << " s\n";
    }
};
#else
struct times : action { times( std::ostream &, options ) : action( os ) {} };
#endif

struct confirm : action
{
    env output;
    options option;
    int selected;
    int failures;

    confirm( std::ostream & os, options option )
    : action( os ), output( os, option.pass ), option( option ), selected( 0 ), failures( 0 ) {}

    operator int() { return failures; }

    bool abort() { return option.abort && failures > 0; }

    confirm & operator()( test testing )
    {
        try
        {
            ++selected; testing.behaviour( output( testing.name ) );
        }
        catch( message const & e )
        {
            ++failures; report( os, e, testing.name );
        }
        return *this;
    }

    ~confirm()
    {
        if ( failures > 0 )
        {
            os << failures << " out of " << selected << " selected " << pluralise(selected, "test") << " failed.\n";
        }
    }
};

template<typename Action>
bool abort( Action & perform )
{
    return perform.abort();
}

template< typename Action >
Action & for_test( tests specification, texts in, Action & perform )
{
    for ( tests::iterator pos = specification.begin(); pos != specification.end() ; ++pos )
    {
        test & testing = *pos;

        if ( select( testing.name, in ) )
            if ( abort( perform( testing ) ) )
                break;
    }
    return perform;
}

inline std::pair<options, texts>
parse( texts args )
{
    options option; texts in;

    bool in_options = true;

    for ( texts::iterator pos = args.begin(); pos != args.end() ; ++pos )
    {
        text & arg = *pos;

        if ( in_options )
        {
            if      ( arg[0] != '-'                   ) { in_options   = false;           }
            else if ( arg == "--"                     ) { in_options   = false; continue; }
            else if ( arg == "-h" || "--help"  == arg ) { option.help  =  true; continue; }
            else if ( arg == "-a" || "--abort" == arg ) { option.abort =  true; continue; }
            else if ( arg == "-c" || "--count" == arg ) { option.count =  true; continue; }
            else if ( arg == "-l" || "--list"  == arg ) { option.list  =  true; continue; }
            else if ( arg == "-t" || "--time"  == arg ) { option.time  =  true; continue; }
            else if ( arg == "-p" || "--pass"  == arg ) { option.pass  =  true; continue; }
            else throw std::runtime_error( "unrecognised option '" + arg + "' (try option --help)" );
        }
        in.push_back( arg );
    }
    return std::make_pair( option, in );
}

inline int usage( std::ostream & os )
{
    os <<
        "\nUsage: test [options] [test-spec ...]\n"
        "\n"
        "Options:\n"
        "  -h, --help   this help message\n"
        "  -a, --abort  abort at first failure\n"
        "  -c, --count  count selected tests\n"
        "  -l, --list   list selected tests\n"
        "  -p, --pass   also report passing tests\n"
#if lest_FEATURE_TIME
        "  -t, --time   list duration of selected tests\n"
#endif
        "  --           end options\n"
        "\n"
        "Test specification:\n"
        "  \"*\"          all tests, unless excluded\n"
        "  empty        all tests, unless tagged [.] or [hide]\n"
#if lest_FEATURE_REGEX_SEARCH
        "  \"re\"         select tests that match regular expression\n"
        "  \"!re\"        omit tests that match regular expression\n"
#else
        "  \"text\"       select tests that contain text (case insensitive)\n"
        "  \"!text\"      omit tests that contain text (case insensitive)\n"
#endif
        ;
    return 0;
}

inline int run( tests const & specification, texts arguments, std::ostream & os = std::cout )
{
    int failures = 0;

    try
    {
        options option; texts in;
        tie( option, in ) = parse( arguments );

        if ( option.help  ) { return usage( os ); }
        if ( option.count ) { count count_( os         ); return for_test( specification, in, count_ ); }
        if ( option.list  ) { print print_( os         ); return for_test( specification, in, print_ ); }
        if ( option.time  ) { times times_( os, option ); return for_test( specification, in, times_ ); }

        { confirm confirm_( os, option ); failures = for_test( specification, in, confirm_ ); }
    }
    catch ( std::exception const & e )
    {
        os << "Error: " << e.what() << "\n";
        return failures + 1;
    }
    return failures;
}

// VC6: make<cont>(first,last) replaces cont(first,last)

template<typename C, typename T>
C make( T const * first, T const * const last )
{
    C result;
    for ( ; first != last; ++first )
    {
       result.push_back( *first );
    }
    return result;
}

inline tests make_tests( test const * first, test const * const last )
{
    return make<tests>( first, last );
}

inline texts make_texts( char * const * first, char * const * last )
{
    return make<texts>( first, last );
}

inline int run( tests const & specification, int argc, char * argv[], std::ostream & os = std::cout )
{
    return run( specification, make_texts( argv + 1, argv + argc ), os  );
}

inline int run( tests const & specification, std::ostream & os = std::cout )
{
    return run( specification, texts(), os );
}

// Traversal of test[N] (test_specification[N]) set up to also work with MSVC6:

template <typename C> test const * begin( C const & c ) { return &c[0]; }
template <typename C> test const *   end( C const & c ) { return begin( c ) + lest_DIMENSION_OF( c ); }

template <typename C>
int run(  C const & specification, int argc, char * argv[], std::ostream & os = std::cout )
{
    return run( make_tests( begin( specification ), end( specification ) ), argv, argc, os  );
}

template <typename C>
int run(  C const & specification, std::ostream & os = std::cout )
{
    return run( make_tests( begin( specification ), end( specification ) ), os  );
}

} // namespace lest

#endif // LEST_LEST_H_INCLUDED
