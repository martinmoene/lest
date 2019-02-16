// C++11 - translate Catch syntax to lest, insofar possible

#include "lest/lest.hpp"

#ifdef __clang__
# pragma clang diagnostic ignored "-Wunused-function"
# pragma clang diagnostic ignored "-Wunused-value"
#elif defined __GNUC__
# pragma GCC   diagnostic ignored "-Wunused-function"
# pragma GCC   diagnostic ignored "-Wunused-value"
#endif

// Catch-lest translation:

// Notes:
// - TEST() ans SCENARIO() require c-string literals to concatenate description and tag (if any).
// - There's no direct equivalent for CHECK() variations; REQUIRE is used.

#if !defined( ex_WARN_IF_NOT_IMPLEMENTED )
#define ex_WARN_IF_NOT_IMPLEMENTED  0
#endif

#if ex_WARN_IF_NOT_IMPLEMENTED
# define ex_SYNTAX_WARNING( id )    do { bool id; } while((void)0, 0)
#else
# define ex_SYNTAX_WARNING( id )    do { /*none*/ } while((void)0, 0)
#endif

#define TEST_CASE( name, ...)       lest_CASE( specification, name " " __VA_ARGS__)

#define REQUIRE( expr )             EXPECT( expr )
#define CHECK(   expr )             EXPECT( expr )

#define REQUIRE_FALSE( expr )       EXPECT_NOT( expr )
#define CHECK_FALSE(   expr )       EXPECT_NOT( expr )

#define REQUIRE_NOTHROW( expr )     EXPECT_NO_THROW( expr )
#define CHECK_NOTHROW(   expr )     EXPECT_NO_THROW( expr )

#define REQUIRE_THROWS( expr )      EXPECT_THROWS( expr )
#define CHECK_THROWS(   expr )      EXPECT_THROWS( expr )

#define REQUIRE_THROWS_AS( expr, excpt )    EXPECT_THROWS_AS( expr, excpt )
#define CHECK_THROWS_AS(   expr, excpt )    EXPECT_THROWS_AS( expr, excpt )

// Unimplemented macros:

#define TEST_CASE_METHOD( fixture, name,...)    \
    lest_CASE_METHOD( specification, fixture, name, __VA_ARGS__)

#define lest_CASE_METHOD( specification, fixture, name, ...)    \
    static void lest_FUNCTION( lest::env & ) {}                 \
    namespace {                                                 \
        lest::add_test lest_REGISTRAR( specification, lest::test( name " " __VA_ARGS__, lest_FUNCTION ) );  \
        void lest_FUNCTION() { ex_SYNTAX_WARNING( TEST_CASE_METHOD_not_implemented ); }                     \
        struct lest_UNIQUE(__lest_class__) : public fixture {   \
            void lest_FUNCTION( lest::env & lest_env );         \
        };  \
    }       \
    void lest_UNIQUE(__lest_class__)::lest_FUNCTION( lest::env & lest_env )

#define REQUIRE_THROWS_WITH( expr, text )   ex_SYNTAX_WARNING( REQUIRE_THROWS_WITH_not_implemented )
#define CHECK_THROWS_WITH(   expr, text )   ex_SYNTAX_WARNING( CHECK_THROWS_WITH_not_implemented )

#define REQUIRE_THAT( lhs, matcher_expr )   ex_SYNTAX_WARNING( REQUIRE_THAT_not_implemented )
#define CHECK_THAT(   lhs, matcher_expr )   ex_SYNTAX_WARNING( CHECK_THAT_not_implemented )

#define INFO( message_expression )          ex_SYNTAX_WARNING( INFO_not_implemented )
#define WARN( message_expression )          ex_SYNTAX_WARNING( WARN_not_implemented )
#define FAIL( message_expression )          ex_SYNTAX_WARNING( FAIL_not_implemented )
#define FAIL_CHECK( message_expression )    ex_SYNTAX_WARNING( FAIL_CHECK_not_implemented )
#define CAPTURE( expression )               ex_SYNTAX_WARNING( CAPTURE_not_implemented )

// BDD style, mostly equivalent (see also issue #53):

#undef  SCENARIO
#define SCENARIO( name, ... )   lest_CASE( specification, "Scenario: " name " " __VA_ARGS__ )
// GIVEN( something )
// WHEN( something )
// THEN( something )
// AND_WHEN( something )
// AND_THEN( something )

static lest::tests specification;

TEST_CASE( "A passing test without tag" )
{
    REQUIRE( 42 == 42 );
}

TEST_CASE( "A passing test", "[pass]" )
{
    CHECK(   42 == 42 );
    REQUIRE( 42 == 42 );
    REQUIRE_FALSE( 42 == 7 );
}

TEST_CASE( "A failing test", "[fail]" )
{
    CHECK(   42 == 7 );     // terminates test, unlike in Catch
    REQUIRE( 42 == 7 );
}

TEST_CASE( "Unexpected exception is reported", "[fail]" )
{
    REQUIRE( (throw std::runtime_error("surprise!"), true) );
}

TEST_CASE( "Unspecified expected exception is captured", "[pass]" )
{
    REQUIRE_THROWS( throw std::runtime_error("surprise!") );
}

TEST_CASE( "Specified expected exception is captured", "[pass]" )
{
    REQUIRE_THROWS_AS( throw std::bad_alloc(), std::bad_alloc );
}

TEST_CASE( "Expected exception is reported missing", "[fail]" )
{
    REQUIRE_THROWS( true );
}

TEST_CASE( "Specific expected exception is reported missing", "[fail]" )
{
    EXPECT_THROWS_AS( true, std::runtime_error );
}

TEST_CASE( "A test with unimplemented macros", "[compile-time warning]" )
{
    REQUIRE( "Suppress warning on unused 'lest_env' [-Wunused-parameter]" );

    REQUIRE_THROWS_WITH( expr, "text" );
    REQUIRE_THAT( expr, matcher_expr );

    INFO( message_expression );
    WARN( message_expression );

    FAIL( message_expression );
    FAIL_CHECK( message_expression );

    CAPTURE( expression );
}

struct fixture
{
    int foo() const { return 42; }
};

TEST_CASE_METHOD( fixture, "Fixture test cases compile but are not executed (a)", "[compile-time warning]" )
{
    REQUIRE( foo() == 42 );
}

TEST_CASE_METHOD( fixture, "Fixture test cases compile but are not executed (b)", "[compile-time warning]" )
{
    REQUIRE( foo() == 42 );
}

SCENARIO( "Vectors can be sized and resized", "[vector]" )
{
    GIVEN( "A vector with some items" )
    {
        std::vector<int> v( 5 );

        REQUIRE( v.size() == 5u );
        REQUIRE( v.capacity() >= 5u );

        WHEN( "the size is increased" )
        {
            v.resize( 10 );

            THEN( "the size and capacity change" )
            {
                REQUIRE( v.size() == 10u );
                REQUIRE( v.capacity() >= 10u );
            }
        }
        WHEN( "the size is reduced" )
        {
            v.resize( 0 );

            THEN( "the size changes but not capacity" )
            {
                REQUIRE( v.size() == 0u );
                REQUIRE( v.capacity() >= 5u );
            }
        }
        WHEN( "more capacity is reserved" )
        {
            v.reserve( 10 );

            THEN( "the capacity changes but not the size" )
            {
                REQUIRE( v.size() == 5u );
                REQUIRE( v.capacity() >= 10u );
            }
        }
        WHEN( "less capacity is reserved" )
        {
            v.reserve( 0 );

            THEN( "neither size nor capacity are changed" )
            {
                REQUIRE( v.size() == 5u );
                REQUIRE( v.capacity() >= 5u );
            }
        }
    }
}

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv /*, std::cout */ );
}

// cl -nologo -W3 -EHsc -Dex_WARN_IF_NOT_IMPLEMENTED=1 -Dlest_FEATURE_AUTO_REGISTER=1 -I../include -Fe17-syntax-catch.exe 17-syntax-catch.cpp && 17-syntax-catch
// g++ -Wall -Wextra -std=c++11 -Dex_WARN_IF_NOT_IMPLEMENTED=1 -Dlest_FEATURE_AUTO_REGISTER=1 -I../include -o 17-syntax-catch.exe 17-syntax-catch.cpp && 17-syntax-catch
// g++ -Wall -Wextra -std=c++11 -Dex_WARN_IF_NOT_IMPLEMENTED=1 -Dlest_FEATURE_AUTO_REGISTER=1 -I../include -o 17-syntax-catch.exe 17-syntax-catch.cpp 2>&1 | grep "^17.*warning" && 17-syntax-catch
