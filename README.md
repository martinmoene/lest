lest &ndash; lest errors escape testing
=======================================

[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)  [![Standard](https://img.shields.io/badge/c%2B%2B-11-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![Standard](https://img.shields.io/badge/c%2B%2B-98-orange.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)  [![License](https://img.shields.io/badge/license-BSL-blue.svg)](https://opensource.org/licenses/BSL-1.0) [![Build Status](https://travis-ci.org/martinmoene/lest.svg?branch=master)](https://travis-ci.org/martinmoene/lest) [![Build status](https://ci.appveyor.com/api/projects/status/3777o06o2ni5lww2/branch/master?svg=true)](https://ci.appveyor.com/project/martinmoene/lest/branch/master) [![Version](https://badge.fury.io/gh/martinmoene%2Flest.svg)](https://github.com/martinmoene/lest/releases) [![download](https://img.shields.io/badge/latest-download-blue.svg)](https://raw.githubusercontent.com/martinmoene/lest/master/include/lest/lest.hpp) [![Conan](https://img.shields.io/badge/on-conan-blue.svg)](https://bintray.com/martinmoene/testing/lest%3Atesting/_latestVersion) [![Try it online](https://img.shields.io/badge/on-wandbox-blue.svg)](https://wandbox.org/permlink/80a88nlPralvatNq)

This tiny C++11 test framework is based on ideas and examples by Kevlin Henney [1,2] and on ideas found in the CATCH test framework by Phil Nash [3].

Let writing tests become irresistibly easy and attractive.

**Contents**  
- [Example usage](#example-usage)
- [In a nutshell](#in-a-nutshell)
- [License](#license)
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Usage](#usage)
- [Synopsis](#synopsis)
- [Variants of *lest*](#variants-of-lest)
- [Features of *lest*](#features-of-lest)
- [Reported to work with](#reported-to-work-with)
- [Building tests and examples](#building-tests-and-examples)
- [Contributions to *lest*](#contributions-to-lest)
- [Other test frameworks](#other-test-frameworks)
- [Notes and references](#notes-and-references)


Example usage
-------------

```Cpp
#include "lest/lest.hpp"

using namespace std;

const lest::test specification[] =
{
    CASE( "Empty string has length zero (succeed)" )
    {
        EXPECT( 0 == string(  ).length() );
        EXPECT( 0 == string("").length() );
    },

    CASE( "Text compares lexically (fail)" )
    {
        EXPECT( string("hello") > string("world") );
    },

    CASE( "Unexpected exception is reported" )
    {
        EXPECT( (throw std::runtime_error("surprise!"), true) );
    },

    CASE( "Unspecified expected exception is captured" )
    {
        EXPECT_THROWS( throw std::runtime_error("surprise!") );
    },

    CASE( "Specified expected exception is captured" )
    {
        EXPECT_THROWS_AS( throw std::bad_alloc(), std::bad_alloc );
    },

    CASE( "Expected exception is reported missing" )
    {
        EXPECT_THROWS( true );
    },

    CASE( "Specific expected exception is reported missing" )
    {
        EXPECT_THROWS_AS( true, std::runtime_error );
    },
};

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv );
}
```
Note: besides above table approach, *lest* also supports [auto-registration of tests](#test-case-macro).

### Compile and run

```
prompt>g++ -Wall -Wextra -std=c++11 -I../include -o 05_select.exe 05_select.cpp && 05_select.exe
05_select.cpp:17: failed: Text compares lexically (fail): string("hello") > string("world") for "hello" > "world"
05_select.cpp:22: failed: got unexpected exception with message "surprise!": Unexpected exception is reported: (throw std::runtime_error("surprise!"), true)
05_select.cpp:37: failed: didn't get exception: Expected exception is reported missing: true
05_select.cpp:42: failed: didn't get exception of type std::runtime_error: Specific expected exception is reported missing: true
4 out of 7 selected tests failed.
```

With [Buck](https://buckbuild.com/):
```
prompt> buck run example/:05_select
...
```


In a nutshell
-------------

**lest** is a small C++11 test framework for unit testing, regression testing, [Test-driven development (TDD)](http://en.wikipedia.org/wiki/Test-driven_development) and [Behaviour-driven design (BDD)](http://dannorth.net/introducing-bdd/). It replicates innovative ideas in C++ testing from the [Catch test framework](https://github.com/philsquared/Catch) such as [function-level fixtures](#fixture-macros) and [expression-decomposing assertion macros](#assertion-macros) in a form that is compact enough to read in five minutes. The lest_cpp03 variant provides similar capabilities to use with C++98/03 compilers.

**Features and properties of lest** are ease of installation (single header), no boilerplate code,  traditional unit test cases and BDD style scenarios, strings as test names, function-level fixtures, expression-decomposing assertion macros, support for floating point comparison, test selection from commandline, test duration timing, test randomisation and sorting, display of passing tests, colourised output (compile-time option), C++11 code and a C++98/03 variant with comparable features (also compilable as C++11). 

**Features available via other projects** are mocking (see [integrate Trompeloeil mocking framework](#main-trompeloeil)) and hamcrest matchers (see [variants of lest](#variants-of-lest)), 

**Not provided** are things present in [other test frameworks](#other-test-frameworks), such as suites of tests, value-parameterised tests, type-parameterised tests, test data generators, customisable reporting, easy logging of extra information, breaking into a debugger, concurrent execution of tests, isolated execution of tests, Visual Studio Test Adapter.


License
-------
*lest* uses the [Boost Software License](LICENSE.txt).
 

Dependencies
------------
*lest* has no other dependencies than the [C++ standard library](http://en.cppreference.com/w/cpp/header).


Installation
------------

*lest* is a single-file header-only library. Put `lest.hpp`, or a variant of it such as `lest_cpp03.hpp` directly into the project source tree or somewhere reachable from your project.


Usage
--------
- [Making a case](#test-case-macro) &ndash; [example](example/05-select.cpp#L9)
- [Asserting for success](#assertion-macros) &ndash; [example](example/05-select.cpp#L11)
- [Asserting for failure](#assertion-macros) &ndash; [example](example/05-select.cpp#L13)
- [Asserting for exceptions](#assertion-macros) &ndash; [example](example/05-select.cpp#L28)
- [Asserting for a specific exception](#assertion-macros) &ndash; [example](example/05-select.cpp#L33)
- [Comparing floating point numbers](#floating-point-comparison) &ndash; [example](example/06-approx.cpp).
- [Auto test registration](#test-case-macro) &ndash; [single-file example](example/11-auto-reg.cpp)
- [Auto test registration with multiple source files](#test-case-macro) &ndash; [example part  1](example/13-module-auto-reg-1.cpp), [2](example/13-module-auto-reg-2.cpp), [3](example/13-module-auto-reg-3.cpp). 
- [Non-auto test registration](#test-case-macro) &ndash; [single-file example](example/05-select.cpp) 
- [Non-auto test registration with multiple source files](#module-registration-macro) &ndash; [example part 1](example/12-module-1.cpp), [2](example/12-module-2.cpp), [3](example/12-module-3.cpp).
- [Using fixtures](#fixture-macros) &ndash; [example](example/09-fixture.cpp).
- [Using *lest* assertions in a (reusable) user-defined function or lambda](#tests) &ndash; [function](example/15-extract-function.cpp), [templated function](example/15-extract-template-function.cpp), [lambda](example/15-extract-lambda.cpp).
- [Writing the test main function](#main) &ndash; [single-file auto](example/11-auto-reg.cpp), [non-auto](example/05-select.cpp) &ndash; [multi-file auto part 1](example/13-module-auto-reg-1.cpp), [2](example/13-module-auto-reg-2.cpp), [3](example/13-module-auto-reg-3.cpp), [non-auto part 1](example/12-module-1.cpp), [2](example/12-module-2.cpp), [3](example/12-module-3.cpp).
- [Integrating *lest* with the Trompeloeil mocking framework](#main-trompeloeil) &ndash; [example](example/16-trompeloeil-runtime.cpp).
- [Reporting a user-defined type](#reporting-a-user-defined-type) &ndash; [example](example/07-udt.cpp).
- [Running tests](#command-line)


Synopsis
--------

**Contents**  
- [Command line](#command-line)
- [Test case macro](#test-case-macro)
- [Fixture macros](#fixture-macros)
- [Assertion macros](#assertion-macros)
- [BDD style macros](#bdd-style-macros)
- [Module registration macro](#module-registration-macro)
- [Feature selection macros](#feature-selection-macros)
- [Standard selection macro](#standard-selection-macro)
- [Namespace](#namespace)
- [Tests](#tests)
- [Main](#main)
- [Main (Trompeloeil)](#main-trompeloeil)
- [Floating point comparison](#floating-point-comparison)
- [Reporting a user-defined type](#reporting-a-user-defined-type)

### Command line
Usage: **test** [options] [_test-spec_ ...]  

Options:
- `-h, --help`, this help message
- `-a, --abort`, abort at first failure
- `-c, --count`, count selected tests
- `-g, --list-tags`, list tags of selected tests
- `-l, --list-tests`, list selected tests
- `-p, --pass`, also report passing tests
- `-z, --pass-zen`, ... without expansion
- `-t, --time`, list duration of selected tests
- `-v, --verbose`, also report passing or failing sections
- `--order=declared`, use source code test order (default)
- `--order=lexical`, use lexical sort test order
- `--order=random`, use random test order
- `--random-seed=n`, use *n* for random generator seed
- `--random-seed=time`, use time for random generator seed
- `--repeat=n`, repeat selected tests *n* times (-1: indefinite)
- `--version`, report lest version and compiler used
- `--`, end options

Test specification:
- `"@"`, `"*"`: all tests, unless excluded
- _empty_: all tests, unless tagged [hide] or [.*optional-name*]
- `"text"`: select tests that contain _text_ (case insensitive)
- `"!text"`: omit tests that contain _text_ (case insensitive)

Test descriptions can contain tags such as `[option]`, `[hide]` and `[.integration]`. Tests that contain the tag `[hide]` or a tag that starts with `[.` in their description are skipped, unless they are specifically selected by specifying `"@"`, `"*"`, or by specifying (part of) the tag.

Test specifications can be combined and are evaluated left-to-right. For example: `a !ab abc` selects all tests that contain 'a', except those that contain 'ab', but include those that contain 'abc'.

When regular expression selection has been enabled (and works), test specifications can use the regular expression syntax of `std::regex_search()`. See also `lest_FEATURE_REGEX_SEARCH` in section [Other Macros](#other-macros).

### Test case macro
A *lest* test specification can consist of a) one or more arrays of test cases that use lambdas, or b) auto-registered test cases that use free functions. See also macro [lest_FEATURE_AUTO_REGISTER](#other-macros). 

**CASE(** "_proposition_" **) {** _code_ **}** &emsp; *(array of cases)*  
Describe the expected behaviour to test for and specify the actions and expectations. See also section [Module registration macro](#module-registration-macro) &ndash; [Single-file code example](example/02-basic.cpp) &ndash; [Multi-file code example part 1](example/12-module-1.cpp), [2](example/12-module-2.cpp), [3](example/12-module-3.cpp).

**CASE_ON(** "_proposition_", ...**) {** _code_ **}** &emsp; *(array of cases, since v1.33)*  
Describe the expected behaviour to test for and specify the actions and expectations. After the description you can add a lambda capture list to refer to symbols in the enclosing scope. See also section [Module registration macro](#module-registration-macro) &ndash; [Single-file code example](example/02-basic.cpp) &ndash; [Multi-file code example part 1](example/12-module-1.cpp), [2](example/12-module-2.cpp), [3](example/12-module-3.cpp).

**lest_CASE(** _specification_, "_proposition_" **) {** _code_ **}** &emsp; *(auto-registered cases)*  
Provide the collection of test cases, describe the expected behaviour to test for and specify the actions and expectations. Consider defining macro CASE(_proposition_) to hide the collection of test cases and define it in terms of lest_CASE(...) &ndash; [Single-file code example](example/11-auto-reg.cpp)  &ndash; [Multi-file code example part  1](example/13-module-auto-reg-1.cpp), [2](example/13-module-auto-reg-2.cpp), [3](example/13-module-auto-reg-3.cpp). 

### Fixture macros
*lest* provides function-level fixtures. Fixtures are stack-based and their setup and teardown occurs at the block scope of SETUP and (nested) SECTIONs &ndash; [Code example](example/09-fixture.cpp).

**SETUP(** "_context_" **) {** _code_ **}**  
Describe and setup the context to use afresh in each enclosed section. 

**SECTION(** "_proposition_" **) {** _code_ **}**  
Describe the expected behaviour to test for using the enclosing context and specify the actions and expectations. The objects in the enclosing setup or section come into existence and go out of scope for each section. A section must be enclosed in setup or in another section. 

### Assertion macros
*lest* has expression-decomposing assertion macros. An expression with strings such as `hello > world` may be reported with code and expansion as `hello > world ("hello" > "world")`. As a consequence, only a few assertion macro variants are needed &ndash; [Code example](example/05-select.cpp).

**EXPECT(** _expr_ **)**  
Evaluate the expression and report failure. If an exception is thrown it is caught, reported and counted as a failure.

**EXPECT_NOT(** _expr_ **)**  
Evaluate the expression, record the *logical not* of its result and report failure. If an exception is thrown it is caught, reported and counted as a failure. This macro is a workaround to circumvent ! prefixed expressions as these cannot be decomposed.

**EXPECT_NO_THROW(** _expr_ **)**  
Expect that no exception (of any type) is thrown during evaluation of the expression.

**EXPECT_THROWS(** _expr_ **)**  
Expect that an exception (of any type) is thrown during evaluation of the expression.

**EXPECT_THROWS_AS(** _expr_, _exception_ **)**  
Expect that an exception of the specified type is thrown during evaluation of the expression.

If an assertion fails, the remainder of the test that assertion is part of is skipped.

### BDD style macros
*lest* provides several macros to write [Behaviour-Driven Design (BDD)](http://dannorth.net/introducing-bdd/) style scenarios &ndash; [Code example](example/10-bdd.cpp), [auto-registration](example/10-bdd-auto.cpp).

**lest_SCENARIO(** _specification_, "_sketch_" **) {** _code_ **}** &emsp; *(auto-registered cases)*     

**SCENARIO(** "_sketch_" **) {** _code_ **}** &emsp; *(array of cases)*  

**GIVEN(** "_context_" **) {** _code_ **}**  

**WHEN(** "_action_" **) {** _code_ **}**  

**THEN(** "_result_" **) {** _code_ **}**  

**AND_WHEN(** "_action_" **) {** _code_ **}**  

**AND_THEN(** "_result_" **) {** _code_ **}**  

These macros simply map to macros CASE(), SETUP() and SECTION().

For auto-registered scenarios, consider defining macro SCENARIO(proposition) to hide the collection of scenarios and define it in terms of lest_SCENARIO(...).

### Module registration macro
When using *arrays of  test cases* written across multiple files, you can use macro MODULE() to add a module's test cases to the overall specification &ndash; [Code example part 1](example/12-module-1.cpp), [2](example/12-module-2.cpp), [3](example/12-module-3.cpp).

**MODULE(** _overall-specification_, _module-specification_ **)**  
Register this module's test specification with the overall specification.

Note that with *lest* using [auto test case registration](#other-macros) there's no need for macro MODULE(), see the [auto-registration example part  1](example/13-module-auto-reg-1.cpp), [2](example/13-module-auto-reg-2.cpp), [3](example/13-module-auto-reg-3.cpp). The same holds for *lest_cpp03*, see [cpp03 example part 1](example/14-module-cpp03-1.cpp), [2](example/14-module-cpp03-2.cpp), [3](example/14-module-cpp03-3.cpp).

### Feature selection macros
-D<b>lest_NO_SHORT_MACRO_NAMES</b>  
-D<b>lest_NO_SHORT_ASSERTION_NAMES</b> (deprecated)  
All public API macros of _lest_ exist as lest\_*MACRO* and shorthand _MACRO_ variant. Define this macro to omit the shorthand macros.

-D<b>lest_FEATURE_AUTO_REGISTER</b>=0  
Define this to 1 to enable auto registration of test cases.  Default is 0.

See also section [Test case macro](#test-case-macro).

-D<b>lest_FEATURE_COLOURISE</b>=0  
Define this to 1 to emphasise success and failure with colour. Default is 0.

Note: [ANSI colour codes](http://en.wikipedia.org/wiki/ANSI_escape_code) are used. On Windows versions that [lack support for this](http://stackoverflow.com/questions/16755142/how-to-make-win32-console-recognize-ansi-vt100-escape-sequences) you can use the [ANSICON](https://github.com/adoxa/ansicon) terminal. Executables can be obtained [here](http://ansicon.adoxa.vze.com/).

-D<b>lest_FEATURE_LITERAL_SUFFIX</b>=0  
Define this to 1 to append `u`, `l`, a combination of these, or `f` to numeric literals. Default is 0.

-D<b>lest_FEATURE_REGEX_SEARCH</b>=0  
Define this to 1 to enable regular expressions to select tests. Default is 0.

Note: You have to make sure the compiler's library has a working `std::regex_search()`; not all do currently. GCC 4.8.1's regex search function doesn't work yet. Visual C++ probably has a working regex search function since VC9, Visual Studio 2008 (tested VC10, Visual Studio 2010).

-D<b>lest_FEATURE_TIME_PRECISION</b>=0  
Define this to set the precision of the duration in ms reported with option --time. Default is 0.

-D<b>lest_FEATURE_WSTRING</b>=1  
Define this to 0 to remove references to std::wstring. Default is 1.

-D<b>lest_FEATURE_RTTI</b> (undefined)  
*lest* tries to determine if RTTI is available itself. If that doesn't work out, define this to 1 or 0 to include or remove uses of RTTI (currently a single occurrence of `typeid` used for reporting a type name). Default is undefined.

### Standard selection macro
-D<b>lest_CPLUSPLUS</b>=199711L  
Define this macro to override the auto-detection of the supported C++ standard, or if your compiler does not set the `__cplusplus` macro correctly.

### Namespace
namespace **lest** { }  
Types and functions are located in namespace lest.

### Tests
struct **env** { };  

struct **test**  
{  
&emsp;std::string name;  
&emsp;std::function\<void( env & )\> behaviour;  
};

You'll need type `env` and variable `lest_env` when you have a test case that calls a user-defined function or lambda that uses lest assertions like `EXPECT()` &ndash; [Call reusable function](example/15-extract-function.cpp), [Call reusable templated function](example/15-extract-template-function.cpp), and [Call reusable lambda](example/15-extract-lambda.cpp).

### Main
A typical `main()` function for *lest* may look as follows:
```Cpp
#include "lest/lest.hpp"

const lest::test specification[] = { CASE("..."){} };

int main( int argc, char *argv[] )
{
    if ( int failures = lest::run( specification, argc, argv ) )
        return failures;
    
    return std::cout << "All tests passed\n", EXIT_SUCCESS;
}
```

Compile and run:
```
prompt>g++ -std=c++11 -o main.exe -I../include main.cpp && main.exe
All tests passed
```

Or, if feedback on success is moved to the command line:
```Cpp
#include "lest/lest.hpp"

const lest::test specification[] = { CASE("..."){} };

int main( int argc, char *argv[] )
{
    return lest::run( specification, argc, argv );
}
```

Compile and run with feedback on success:
```
prompt>g++ -std=c++11 -o main.exe -I../include main.cpp && main.exe && echo All tests passed
All tests passed
```

You can use the following variants of *lest*'s `run()` function in main.

inline  
int **run(** std::vector\<test\> _specification_, std::vector\<std::string\> _arguments_, std::ostream & _os_ = std::cout );

inline  
int **run(** std::vector\<test\> _specification_, int _argc_, char \* _argv_[], std::ostream & _os_ = std::cout );


template\<std::size_t N\>  
int **run(** test const (& _specification_ )[N], std::ostream & _os_ = std::cout **)**;

template\<std::size_t N\>  
int **run(** test const (& _specification_ )[N], std::vector\<std::string\> _arguments_, std::ostream & _os_ = std::cout **)**;

template\<std::size_t N\>  
int **run(** test const (& _specification_ )[N], int _argc_, char \* _argv_[], std::ostream & _os_ = std::cout **)**;

- _specification_ - collection of tests
- _arguments_ - options and arguments to select and omit tests
- _argc_, _arcv_ - options and arguments to select and omit tests
- _os_ - stream to report to
- returns number of failing tests

### Main (Trompeloeil)

You can integrate the [Trompeloeil mocking framework](https://github.com/rollbear/trompeloeil) with *lest* by providing a reporter for Trompeloeil &ndash; [Code example](example/16-trompeloeil-runtime.cpp).

```Cpp
#include "lest/lest.hpp"
#include "trompeloeil.hpp"

int main( int argc, char * argv[] )
{
    std::ostream & stream = std::cout;
    
    trompeloeil::set_reporter(
        [&stream]( trompeloeil::severity severity, const char * file, unsigned long line, std::string const & msg )
    {
        if ( severity == trompeloeil::severity::fatal )
        {
            throw lest::message{"", lest::location{ line ? file : "[file/line unavailable]", int(line) }, "", msg };
        }
        else
        {   
            stream << lest::location{ line ? file : "[file/line unavailable]", int(line) } << ": " << msg;
        }
    });

    return lest::run( specification, argc, argv, stream );
}
```

### Floating point comparison
*lest* provides `class approx` to compare floating point values &ndash; [Code example](example/06-approx.cpp).

class **approx** { };  

Use `approx` as follows:

EXPECT( 1.23 == approx( 1.23 ) );  
EXPECT( 1.23 != approx( 1.24 ) );  

EXPECT( 1.23 != approx( 1.231 ) );  
EXPECT( 1.23 == approx( 1.231 ).epsilon( 0.1 ) );  

approx custom = approx::custom().epsilon( 0.1 );  
    
EXPECT( approx( 1.231 ) != 1.23 );  
EXPECT( custom( 1.231 ) == 1.23 );  

Class `approx` also provides *less-than or equal* and *greater-than or equal* operators.

### Reporting a user-defined type
*lest* allows you to report a user-defined type via operator<<() &ndash; [Code example](example/07-udt.cpp).

To report a type not yet supported by *lest*, define a streaming function for it:  

namespace ns {  
&emsp;struct _user-defined-type_ { ... };  
&emsp;std::ostream & **operator<<** ( std::ostream & os, _user-defined-type_ const & _type_ )  
&emsp;{  
&emsp;&emsp;using lest::to_string;  
&emsp;&emsp;return os << ... ;  
&emsp;}  
}

In it, stream the constituent parts of the type via lest's `to_string()` conversion functions.


Variants of *lest*
------------------

Various variants of *lest* are kept here. The simple ones, such as `lest_basic` and `lest_decompose` provide an easy read into the techniques used and remain the tiny test frameworks that are a good fit to include with small projects.

You are encouraged to take it from here and change and expand it as you see fit and publish your variant. If you do, I'd much appreciate to hear from you!

- lest.hpp - lest's latest development, this project.
- lest_basic.hpp - lest at its very basic, this project.
- lest_decompose.hpp - lest with expression decomposition, this project.
- lest_cpp03.hpp - lest with expression decomposition for C++03, this project.
- [hamlest](https://github.com/martinmoene/hamlest) - matchers for lest.
- [lest with groups](https://github.com/pmed/lest/tree/test_group) - Pavel Medvedev


Features of *lest*
------------------

Feature / variant             | latest  | cpp03  | decompose  | basic    |
------------------------------|:-------:|:-------:|:--------:|:--------:|
Assert expressions            | &#10003;| &#10003;| &#10003; | &#10003; |
Assert exceptions             | &#10003;| &#10003;| &#10003; | &#10003; |
Assert abortion (death)       | [contrib](contrib/lest_expect_abort/)  | [contrib](contrib/lest_expect_abort/)  | - | - |
Assert assertions (death)     | [contrib](contrib/lest_expect_assert/) | [contrib](contrib/lest_expect_assert/) | - | - |
Expression decomposition      | &#10003;| modest  | modest    | -     |
Literal suffix u, l, f        | &#10003;| -       | -         | -     |
Colourised output             | &#10003;| &#10003;| -         | -     |
BDD style scenarios           | &#10003;| &#10003;| -         | -     |
Fixtures (sections)           | &#10003;| &#10003;| -         | -     |
Floating point comparison, approx|&#10003;| &#10003;| -       | -     |
Floating point comparison, ulp| -       | -       | -         | -     |
Test selection (include/omit) | &#10003;| &#10003;| -         | -     |
Test selection (regexp)       | &#10003;| &#10003;| -         | -     |
Help screen                   | &#10003;| &#10003;| -         | -     |
Abort at first failure        | &#10003;| &#10003;| -         | -     |
Count selected tests          | &#10003;| &#10003;| -         | -     |
List tags of selected tests   | &#10003;| &#10003;| -         | -     |
List selected tests           | &#10003;| &#10003;| -         | -     |
Report passing tests          | &#10003;| &#10003;| -         | -     |
Time duration of tests        | &#10003;| &#10003;| -         | -     |
Control order of tests        | &#10003;| &#10003;| -         | -     |
Repeat tests                  | &#10003;| &#10003;| -         | -     |
Auto registration of tests    | &#10003;| &#10003;| -         | -     |
Modules of tests              | &#10003;| &#10003;| -         | -     |
&nbsp;                        | &nbsp;  | &nbsp;  |&nbsp;     |&nbsp; |
Suites of tests               | -       | -       | -         | -     |
Value-parameterised tests     | -       | -       | -         | -     |
Type-parameterised tests      | -       | -       | -         | -     |
Test data generators          | -       | -       | -         | -     |
[Hamcrest matchers](#variants-of-lest)| +/-| -    | -         | -     |
Mocking support               | -       | -       | -         | -     |
Logging facility              | -       | -       | -         | -     |
Break into debugger           | -       | -       | -         | -     |
Concurrent execution of tests | -       | -       | -         | -     |
Isolated execution of tests   | -       | -       | -         | -     |


Reported to work with
---------------------

The table below mentions the lowest version of a compiler `lest` is reported to work with.
 
Variant / compiler     | clang | GCC   | VC    |
-----------------------|-------|-------|-------|
lest (latest)          |  3.2  | 4.8.1 | 12    |
lest_basic             |  3.2  | 4.6   | 12    |
lest_decompose         |  3.2  | 4.6   | 12    |
lest_cpp03 (decompose) |  ?    | ?     | 8     |

**Note**: I've made a few concessions to enable compilation of `lest.hpp` with Visual C++:
- Prevent [error C2797](https://connect.microsoft.com/VisualStudio/feedbackdetail/view/917150): replace braced member initialisation with C++98 style initialisation.
- Prevent [error C2144](https://connect.microsoft.com/VisualStudio/feedbackdetail/view/812488): use `enum{ value }` instead of `static constexpr bool` in struct `is_container` (for VC only).


Building tests and examples
---------------------------
Tests and examples can be build with Buck, via Makefiles or by using CMake.

To build the tests and examples as described below you need:

- [Buck](https://buckbuild.com/) or [CMake](http://cmake.org) version 3.5 or later to be installed and in your PATH.
- A [suitable compiler](#reported-to-work-with). 

The following steps assume that the [*lest* source code](https://github.com/martinmoene/lest) has been cloned into a directory named `lest`.

### Buck

```
lest> buck run test:test_lest_basic
lest> buck run test:test_lest_cpp03
lest> buck run test:test_lest_decompose
lest> buck run test:test_lest
```

### CMake

1. Create a directory for the build outputs for a particular architecture.  
Here we use lest/build.

        lest> mkdir build && cd build

2. Configure CMake to use the compiler of your choice (run `cmake --help` for a list) and build the tests for *lest* and the examples.

        lest/build> cmake -G "Unix Makefiles" [see 3. below] ..

3. Optional. You can control above configuration through the following options:
	- `-DLEST_BUILD_TEST=ON`: build the tests for lest, default on
	- `-DLEST_BUILD_EXAMPLE=ON`: build the examples, default on
	- `-DLEST_BUILD_CONTRIB=OFF`: build the contrib folder, default off
	- `-DLEST_EXTRA_WARNINGS=OFF`: build with extra warnings and warnings as errors, default off

4. Build the test suite.    

        lest/build> cmake --build .

5. Run the test suite.    

        lest/build> ctest -V

All tests should pass, indicating your platform is supported and you are ready to use *lest*. Note that quite some examples fail. They do so to demonstrate the usage of things.


Contributions to *lest*
-----------------------
Folder [contrib](contrib) contains extensions to *lest*. These extensions are not part of *lest* itself because for example they use non-standard behaviour, they  are considered to be for a too-specific use case, or they are considered not yet ripe for inclusion in *lest* and we first like to gain more experience with them. 


Other test frameworks
---------------------
This [comparison of Catch, doctest and lest](https://github.com/martinmoene/catch-lest-other-comparison#catchdoctestlest-feature-comparison) in table form may help you to discover similarities and differences of these frameworks.

- [Catch](https://github.com/philsquared/Catch) - A modern, C++-native, header-only, framework for unit-tests, TDD and BDD.
- [doctest](https://github.com/onqtam/doctest) - The lightest feature-rich C++ single-header testing framework for unit tests and TDD. Catch/lest-inspired tests that accompany code.
- [stf](https://github.com/jfalcou/stf) - Simple TDD Framework. Catch/lest-inspired with an emphasis on floating point precision testing.  
- [cute](https://github.com/Kosta-Github/cute) - C++ unit test executor.
- [bandit](http://banditcpp.github.io/bandit/) - Human friendly unit testing for C++11.
- [igloo](http://igloo-testing.org/) - BDD style unit testing for C++.
- [mettle](https://github.com/jimporter/mettle) - A C++14 unit test framework.
- [CppUTest](http://cpputest.github.io/) - Unit testing and mocking framework for C/C++.
- [UnitTest++](https://github.com/unittest-cpp/unittest-cpp) - A lightweight unit testing framework for C++.
- [xUnit++](https://bitbucket.org/moswald/xunit/wiki/Home) - A unit testing platform for C++ inspired by [xUnit.net](http://xunit.github.io/).
- [ZenUnit](https://github.com/NeilJustice/ZenUnit) - Single-header C++ unit testing framework for value- and type-parameterized tests.
- [Search for C++98 test frameworks on GitHub](https://github.com/search?q=language%3AC%2B%2B+98+test+framework&type=Repositories&ref=advsearch)
- [Search for C++11 test frameworks on GitHub](https://github.com/search?q=language%3AC%2B%2B+11+test+framework&type=Repositories&ref=advsearch)
- [Search for C++14 test frameworks on GitHub](https://github.com/search?q=language%3AC%2B%2B+14+test+framework&type=Repositories&ref=advsearch)
- [List of C++ unit testing frameworks](http://en.wikipedia.org/wiki/List_of_unit_testing_frameworks#C.2B.2B) at Wikipedia.


Notes and references
--------------------

[1] [Kevlin Henney on Rethinking Unit Testing in C++](http://accu.org/index.php/accu_branches/accu_london/accu_london_may_2010)  ([Video](http://skillsmatter.com/podcast/agile-testing/kevlin-henney-rethinking-unit-testing-in-c-plus-plus)).

[2] Martin Moene. [Elefant C++11 test setup](http://lists.accu.org/mailman/private/accu-general/2013-June/040594.html) on the [ACCU](http://accu.org/) mailing list accu-general (requires login). It mentions the C++11 test appoach Andrzej Krzemie&#x144;ski uses for [Optional. A library for optional (nullable) objects for C++11](https://github.com/akrzemi1/Optional/).

[3] Phil Nash. [CATCH, an automated test framework for C, C++ and Objective-C](http://builds.catch-lib.net/).

[4] A more technically informed name: lest - lambda engaged small tester.

