// C++14 - use lest with trompeloeil mocking framework (runtime adaptation).

// Trompeloeil, thread-safe single-file header-only C++14 mocking framework,
// by Björn Fahller (@rollbear), https://github.com/rollbear/trompeloeil

#include "lest/lest.hpp"
#include "trompeloeil.hpp"

#define CASE( name ) lest_CASE( specification, name )

static lest::tests specification;

// Non-interface mock:

class widget_t
{
public:
    MAKE_MOCK1( method, bool(std::string) );
};

void function_to_test( widget_t * object )
{
    object->method( "world" );
}

// lest test specification, second test case with Trompeloeil expectations:

CASE( "Passing case before" )
{
    EXPECT( "Pass" );
}

CASE( "Expect one call to mock.method(std::string)" )
{
    widget_t mock;

    REQUIRE_CALL( mock, method("hello") )
    .TIMES      ( 1 )
    .RETURN     ( false )  
    ;

    function_to_test( &mock );
}

CASE( "Passing case after" ) 
{
    EXPECT( "Pass" );
}

// lest main function, adapted for Trompeloeil:

int main( int argc, char * argv[] )
{
    std::ostream & stream = std::cout;
    
    trompeloeil::set_reporter(
        [&stream]( trompeloeil::severity severity, const char * file, unsigned long line, std::string const & msg )
    {
        if ( severity == trompeloeil::severity::fatal )
        {
            throw lest::message{ "", lest::location{ line ? file : "[file/line unavailable]", int(line) }, "", msg };
        }
        else
        {   
            stream << lest::location{ line ? file : "[file/line unavailable]", int(line) } << ": " << msg;
        }
    });

    return lest::run( specification, argc, argv, stream );
}

// cl -nologo -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I../include -I. 16-trompeloeil-runtime.cpp && 16-trompeloeil-runtime
// g++ -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -std=c++14 -Dlest_FEATURE_AUTO_REGISTER=1 -I../include -I. -o 16-trompeloeil-runtime.exe 16-trompeloeil-runtime.cpp && 16-trompeloeil-runtime
