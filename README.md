lest &ndash; lest errors escape testing &ensp; [![Build Status](https://travis-ci.org/martinmoene/lest.png?branch=master)](https://travis-ci.org/martinmoene/lest) [#10](https://github.com/martinmoene/lest/issues/10)
=======================================

This tiny C++11 test framework is based on ideas and examples by Kevlin Henney [1,2] and on ideas found in the CATCH test framework by Phil Nash [3].

Let writing tests become irresistibly easy and attractive.

**Contents**  
- [Example usage](#example-usage)
- [In a nutshell](#in-a-nutshell)
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Synopsis](#synopsis)
- [Variants of lest](#variants-of-lest)
- [Reported to work with](#reported-to-work-with)
- [Compile-time performance](#compile-time-performance)
- [Other test frameworks](#other-test-frameworks)
- [Notes and references](#notes-and-references)


Example usage
-------------

```Cpp
#include "lest.hpp"

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

### Compile and run

```
prompt>g++ -Wall -Wextra -std=c++11 -I.. -o 05_select.exe 05_select.cpp && 05_select
05_select.cpp:17: failed: Text compares lexically (fail): string("hello") > string("world") for "hello" > "world"
05_select.cpp:22: failed: got unexpected exception with message "surprise!": Unexpected exception is reported: (throw std::runtime_error("surprise!"), true)
05_select.cpp:37: failed: didn't get exception: Expected exception is reported missing: true
05_select.cpp:42: failed: didn't get exception of type std::runtime_error: Specific expected exception is reported missing: true
4 out of 7 selected tests failed.
```

In a nutshell
-------------

**lest** is a small C++11 test framework for unit testing, regression testing, [Test-driven development (TDD)](http://en.wikipedia.org/wiki/Test-driven_development) and [Behaviour-driven design (BDD)](http://dannorth.net/introducing-bdd/). It replicates innovative ideas in C++ testing from the [Catch test framework](https://github.com/philsquared/Catch) such as [function-level fixtures](#fixture-macros) and [expression-decomposing assertion macros](#assertion-macros) in a form that is compact enough to read in five minutes. The lest_cpp03 variant provides similar capabilities to use with C++98/03 compilers.

**Features and properties of lest** are ease of installation (single header), no boilerplate code,  traditional unit test cases and BDD style scenarios, strings as test names, function-level fixtures, expression-decomposing assertion macros, support for floating point comparison, test selection from commandline, test duration timing, test randomisation and sorting, display of passing tests, colourised output (compile-time option), C++11 code and a C++98/03 variant with comparable features (compilable with [VC6](http://en.wikipedia.org/wiki/Visual_C%2B%2B) and as C++11), 

**Not provided** are things present in [other test frameworks](#other-test-frameworks), such as suites of tests, parameterised tests, templated tests, built-in hamcrest matchers (see [variants of lest](#variants-of-lest)), customisable reporting, easy logging of extra information, breaking into a debugger, concurrent execution of tests, Visual Studio Test Adapter.


Dependencies
------------
*lest* has no other dependencies than the C++ standard library.

Installation
------------

*lest* is a single-file header-only library. Put `lest.hpp`, or a variant of it such as `lest_cpp03.hpp` directly into the project source tree or somewhere reachable from your project.


Synopsis
--------

**Contents**  
- [Command line](#command-line)
- [Test case macro](#test-case-macro)
- [Fixture macros](#fixture-macros)
- [Assertion macros](#assertion-macros)
- [BDD style macros](#bdd-style-macros)
- [Module registration macro](#module-registration-macro)
- [Other macros](#other-macros)
- [Namespace](#namespace)
- [Tests](#tests)
- [Functions](#functions)
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
- `-t, --time`, list duration of selected tests
- `--order=declared`, use source code test order
- `--order=lexical`, use lexical sort test order
- `--order=random`, use random test order
- `--random-seed=n`, use *n* for random generator seed
- `--random-seed=time`, use time for random generator seed
- `--repeat=n`, repeat selected tests *n* times (-1: indefinite)
- `--version`, report lest version and compiler used
- `--`, end options

Test specification:
- `"@"`, `"*"`: all tests, unless excluded
- _empty_: all tests, unless tagged [hide] or [.]
- `"text"`: select tests that contain _text_ (case insensitive)
- `"!text"`: omit tests that contain _text_ (case insensitive)

Tests that contain the tag `[hide]` or `[.]` in their description are skipped, unless they are specifically selected by specifying [hide], [.], "@" or "*".

Test specifications can be combined and are evaluated left-to-right. For example: `a !ab abc` selects all tests that contain 'a', except those that contain 'ab', but include those that contain 'abc'.

When regular expression selection has been enabled (and works), test specifications can use the regular expression syntax of `std::regex_search()`. See also `lest_FEATURE_REGEX_SEARCH` in section [Other Macros](#other-macros).

### Test case macro
A *lest* test specification can consist of a) one or more arrays of test cases that use lambdas, or b) auto-registered test cases that use free functions. See also macro [lest_FEATURE_AUTO_REGISTER](#other-macros). 

**CASE(** "_proposition_", ...**) {** _code_ **}** &emsp; *(array of cases)*  
Describe the expected behaviour to test for and specify the actions and expectations. After the description you can add a lambda capture list to refer to symbols in the enclosing scope. See also section [Module registration macro](#module-registration-macro) &ndash; [Code example](examples/11-module-1.cpp).

**TEST(** "_proposition_", ...**) {** _code_ **}** &emsp; *(array of cases)*  
This macro is an alias for CASE(). It may be deprecated.

**lest_CASE(** _specification_, "_proposition_" **) {** _code_ **}** &emsp; *(auto-registered cases)*  
Provide the collection of test cases, describe the expected behaviour to test for and specify the actions and expectations. Consider defining macro CASE(_proposition_) that hides the collection of test cases and define it in terms of lest_CASE(...)  &ndash; [Code example](examples/12-module-auto-reg-1.cpp). 

### Fixture macros
*lest* provides function-level fixtures. Fixtures are stack-based and their setup and teardown occurs at the block scope of SETUP and (nested) SECTIONs &ndash; [Code example](examples/09-fixture.cpp).

**SETUP(** "_context_" **) {** _code_ **}**  
Describe and setup the context to use afresh in each enclosed section. 

**SECTION(** "_proposition_" **) {** _code_ **}**  
Describe the expected behaviour to test for using the enclosing context and specify the actions and expectations. The objects in the enclosing setup or section come into existence and go out of scope for each section. A section must be enclosed in setup or in another section. 

### Assertion macros
*lest* has expression-decomposing assertion macros. An expression with strings such as `hello > world` may be reported with code and expansion as `hello > world ("hello" > "world")`. As a consequence, only a few macro variants are needed &ndash; [Code example](examples/05-select.cpp).

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
*lest* provides several macros to write [Behaviour-Driven Design (BDD)](http://dannorth.net/introducing-bdd/) style scenarios &ndash; [Code example](examples/10-bdd.cpp).

**SCENARIO(** "_sketch_", ...**) {** _code_ **}**  

**GIVEN(** "_context_" **) {** _code_ **}**  

**WHEN(** "_action_" **) {** _code_ **}**  

**THEN(** "_result_" **) {** _code_ **}**  

**AND_WHEN(** "_action_" **) {** _code_ **}**  

**AND_THEN(** "_result_" **) {** _code_ **}**  

These macros simply map to macros CASE(), SETUP() and SECTION().

### Module registration macro
When using arrays of test cases written across multiple files, you can use macro MODULE() to add a module's test cases to the overall specification &ndash; [Code example](examples/11-module-1.cpp).  

**MODULE(** _overall-specification_, _module-specification_ **)**  
Register this module's test specification with the overall specification.

Note that with *lest* using [auto test registration](#other-macros) there's no need for macro MODULE(), see the [auto-registration module example](examples/12-module-auto-reg-1.cpp). The same holds for *lest_cpp03*, see the [cpp03 module example](examples/13-module-cpp03-1.cpp).

### Other macros
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

### Functions
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

### Floating point comparison
*lest* provides `class approx` to compare floating point values &ndash; [Code example](examples/06-approx.cpp).

class **approx** { };  

Use `approx` as follows:

EXPECT( 1.23 == approx( 1.23 ) );  
EXPECT( 1.23 != approx( 1.24 ) );  

EXPECT( 1.23 != approx( 1.231 ) );  
EXPECT( 1.23 == approx( 1.231 ).epsilon( 0.1 ) );  

approx custom = approx::custom().epsilon( 0.1 );  
    
EXPECT( approx( 1.231 ) != 1.23 );  
EXPECT( custom( 1.231 ) == 1.23 );  

### Reporting a user-defined type
*lest* allows you to report a user-defined type via operator<<() &ndash; [Code example](examples/07-udt.cpp).

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


Variants of lest
----------------

Various variants of *lest* are kept here. The simple ones, such as `lest_basic` and `lest_decompose` provide an easy read into the techniques used and remain the tiny test frameworks that are a good fit to include with small projects.

You are encouraged to take it from here and change and expand it as you see fit and publish your variant. If you do, I'd much appreciate to hear from you!

- lest.hpp - lest's latest development, this project.
- lest_basic.hpp - lest at its very basic, this project.
- lest_decompose.hpp - lest with expression decomposition, this project.
- lest_cpp03.hpp - lest with expression decomposition for C++03, this project.
- [hamlest](https://github.com/martinmoene/hamlest) - matchers for lest.
- [lest with groups](https://github.com/pmed/lest/tree/test_group) - Pavel Medvedev


### Features

Feature / variant             | latest | cpp03 | decompose | basic |
------------------------------|:------:|:-----:|:---------:|:-----:|
Expression decomposition      | +      | modest| modest    | -     |
Literal suffix u, l, f        | +      | -     | -         | -     |
Colourised output             | +      | +     | -         | -     |
BDD style scenarios           | +      | +     | -         | -     |
Fixtures (sections)           | +      | +     | -         | -     |
Floating point comparison     | +      | +     | -         | -     |
Test selection (include/omit) | +      | +     | -         | -     |
Test selection (regexp)       | +      | +     | -         | -     |
Help screen                   | +      | +     | -         | -     |
Abort at first failure        | +      | +     | -         | -     |
Count selected tests          | +      | +     | -         | -     |
List tags of selected tests   | +      | +     | -         | -     |
List selected tests           | +      | +     | -         | -     |
Report passing tests          | +      | +     | -         | -     |
Time duration of tests        | +      | +     | -         | -     |
Control order of tests        | +      | +     | -         | -     |
Repeat tests                  | +      | +     | -         | -     |
Auto registration of tests    | +      | +     | -         | -     |
Modules of tests              | +      | +     | -         | -     |
&nbsp;                        | &nbsp; |&nbsp; |&nbsp;     |&nbsp; |
Suites of tests               | -      | -     | -         | -     |
Parameterised tests           | -      | -     | -         | -     |
Templated tests               | -      | -     | -         | -     |
[Hamcrest matchers](#variants-of-lest)| +/-| - | -         | -     |
Logging facility              | -      | -     | -         | -     |
Break into debugger           | -      | -     | -         | -     |
Concurrent execution of tests | -      | -     | -         | -     |


Reported to work with
---------------------

The table below mentions the lowest version of a compiler `lest` is reported to work with.
 
Variant / compiler     | clang | GCC   | VC    |
-----------------------|-------|-------|-------|
lest (latest)          |  3.2  | 4.8.1 | 12    |
lest_basic             |  3.2  | 4.6   | 12    |
lest_decompose         |  3.2  | 4.6   | 12    |
lest_cpp03 (decompose) |  ?    | ?     | 6 SP6 |

**Note**: I've made a few concessions to enable compilation of `lest.hpp` with Visual C++:
- Prevent [error C2797](https://connect.microsoft.com/VisualStudio/feedbackdetail/view/917150): replace braced member initialisation with C++98 style initialisation.
- Prevent [error C2144](https://connect.microsoft.com/VisualStudio/feedbackdetail/view/812488): use `enum{ value }` instead of `static constexpr bool` in struct `is_container` (for VC only).


Compile time performance
------------------------

The table below shows relative and actual compile times for two `lest` frameworks and the `Catch` framework [3].

Compile time | GCC 4.8.1 | VC12    | VC6     | GCC 4.8.1 [s] | VC12 [s]   | VC6 [s]    |
:------------|----------:|--------:|--------:|--------------:|-----------:|-----------:|
lest         | 66 (3)    | 111 (1) | -       | 26 (1.0)      | 43 (0.47)  | -          |
lest_cpp03   | 19 (1)    |   3 (1) | 5 (0.3) |  7.3 (0.5)    | 1.2 (0.39) | 2.0 (0.13) |
Catch 1.0b53 | 17 (20)   |   3 (5) | -       | 14 (7.6)      | 2.5 (1.74) | -          |

The table is based on compile times with optimisations disabled for 1000 tests with a single assertion (and compile times for no test). The tests were performed twice in succession and the second result was used (Windows 8.1, Intel i7-4702MQ CPU).

The relative compile times were computed as:
- r<sub>x</sub> = t<sub>x</sub> / t<sub>0-lest-cpp03-vc12</sub>

with the exception of:
- r<sub>1000-Catch</sub> = (t<sub>1000</sub> - t<sub>0</sub> + t<sub>1</sub>) / t<sub>0-lest-cpp03--vc12</sub>

Where 0, 1 and 1000 indicate the number of tests, x is 0 or 1000 and t<sub>1</sub> is  0.35s for VC12 and GCC 4.8.1. 

Compilation of main() for Catch takes a noticeable amount of time. To reduce compilation times with Catch, its main() is compiled separately from the tests. The formula above takes this into account.

Using lambdas as test functions clearly comes at a cost. To keep (re)compile times reasonable for TDD, a source file with *lest* tests should probably have no more than circa 100 assertions. lest_cpp03 has compile times similar to Catch.


Other test frameworks
---------------------
- [Catch](https://github.com/philsquared/Catch) - A modern, C++-native, header-only, framework for unit-tests, TDD and BDD.
- [bandit](http://banditcpp.org/) - Human friendly unit testing for C++11.
- [igloo](http://igloo-testing.org/) - BDD style unit testing for C++.
- [xUnit++](https://bitbucket.org/moswald/xunit/wiki/Home) - A unit testing platform for C++ inspired by xUnit.net.
- [Search for C++11 test frameworks on GitHub](https://github.com/search?utf8=%E2%9C%93&q=c%2B%2B11+test+framework&type=Repositories&ref=searchresults)
- [Search for C++14 test frameworks on GitHub](https://github.com/search?utf8=%E2%9C%93&q=c%2B%2B14+test+framework&type=Repositories&ref=searchresults)
- [List of C++ unit testing frameworks](http://en.wikipedia.org/wiki/List_of_unit_testing_frameworks#C.2B.2B) at Wikipedia


Notes and references
--------------------

[1] [Kevlin Henney on Rethinking Unit Testing in C++](http://accu.org/index.php/accu_branches/accu_london/accu_london_may_2010)  ([Video](http://skillsmatter.com/podcast/agile-testing/kevlin-henney-rethinking-unit-testing-in-c-plus-plus)).

[2] Martin Moene. [Elefant C++11 test setup](http://lists.accu.org/mailman/private/accu-general/2013-June/040594.html) on the [ACCU](http://accu.org/) mailing list accu-general (requires login). It mentions the C++11 test appoach Andrzej Krzemie&#x144;ski uses for [Optional. A library for optional (nullable) objects for C++11](https://github.com/akrzemi1/Optional/).

[3] Phil Nash. [CATCH, an automated test framework for C, C++ and Objective-C](http://builds.catch-lib.net/).

[4] A more technically informed name: lest - lambda engaged small tester.

