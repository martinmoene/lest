lest &ndash; lest errors escape testing
=======================================

This tiny C++11 test helper is based on ideas and examples by Kevlin Henney [1,2] and on ideas found in the CATCH test framework by Phil Nash [3].

Let writing tests become irresistibly easy and attractive.

**Contents**  
- [Example usage](#example-usage)
- [Compile and run](#compile-and-run)
- [Synopsis](#synopsis)
- [Variants of lest](#variants-of-lest)
- [Reported to work with](#reported-to-work-with)
- [Compile time performance](#compile-time-performance)
- [Notes and References](#notes-and-references)


Example usage
-------------

```Cpp
#include "lest.hpp"

using namespace std;

const lest::test specification[] =
{
    "Empty string has length zero (succeed)", []
    {
        EXPECT( 0 == string(  ).length() );
        EXPECT( 0 == string("").length() );
    },

    "Text compares lexically (fail)", []
    {
        EXPECT( string("hello") > string("world") );
    },

    "Unexpected exception is reported", []
    {
        EXPECT( (throw std::runtime_error("surprise!"), true) );
    },

    "Unspecified expected exception is captured", []
    {
        EXPECT_THROWS( (throw std::runtime_error("surprise!"), true) );
    },

    "Specified expected exception is captured", []
    {
        EXPECT_THROWS_AS( (throw std::bad_alloc(), true), std::bad_alloc );
    },

    "Expected exception is reported missing", []
    {
        EXPECT_THROWS( true );
    },

    "Specific expected exception is reported missing", []
    {
        EXPECT_THROWS_AS( true, std::runtime_error );
    },
};

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv );
}
```


Compile and run
---------------

```
prompt>g++ -Wall -Wextra -std=c++11 -I.. -o example5_select.exe example5_select.cpp && example5_select
example5_select.cpp:15: failed: Text compares lexically (fail): string("hello") > string("world") for "hello" > "world"
example5_select.cpp:20: failed: got unexpected exception with message "surprise!": Unexpected exception is reported: (throw std::runtime_error("surprise!"), true)
example5_select.cpp:35: failed: didn't get exception: Expected exception is reported missing: true
example5_select.cpp:40: failed: didn't get exception of type std::runtime_error: Specific expected exception is reported missing: true
4 out of 7 selected tests failed.
```

Synopsis
--------

### Command line
Usage: **test** [options] [_test-spec_ ...]  

Options:
-  `-h, --help`, this help message
-  `-c, --count`, count selected tests
-  `-l, --list`, list selected tests
-  `--`, end options

Test specification:
- _empty_ or <code>"*"</code>: all tests, except excluded tests.
- `"text"`: select tests that contain _text_ (case insensitive).
- `"!text"`: omit tests that contain _text_ (case insensitive).

Omission of tests takes precedence over inclusion of tests.

Note: when regular expression selection has been enabled (and works), spec can use the regular expression syntax of `std::regex_search()`. See also `lest_USE_REGEX_SEARCH` in section [Other Macros](#other-macros).

### Assertions Macros
**EXPECT(** _expr_ **)**  
Evaluate the expression and report failure. If an exception is thrown it is caught, reported and counted as a failure.

**EXPECT_THROWS(** _expr_ **)**  
Expect that an exception (of any type) is thrown during evaluation of the expression.

**EXPECT_THROWS_AS(** _expr_, _exception_ **)**  
Expect that an exception of the specified type is thrown during evaluation of the expression.

If an assertion fails, the remainder of the test that assertion is part of is skipped.

Note that EXPECT(), EXPECT_THROWS() and EXPECT_THROWS_AS() are shortened aliases for lest_EXPECT(), lest_EXPECT_THROWS() and lest_EXPECT_THROWS_AS().

### Other Macros
**lest_NO_SHORT_ASSERTION_NAMES**  
Define this to omit the shortened alias macros for the lest_EXPECT... macros.

**lest_USE_REGEX_SEARCH**  
Define this to enable regular expressions to select tests.

Note: You have to make sure the compiler's library has a working `std::regex_search()`; not all do currently. GCC 4.8.1's regex search function doesn't work yet. Visual C++ probably has a working regex search function since VC9, Visual Studio 2008 (tested VC10, Visual Studio 2010).

### Namespace
namespace **lest** { }  
Types and functions are located in namespace lest.

### Tests
struct **test**  
{  
&emsp;const std::string name;  
&emsp;const std::function\<void()\> behaviour;  
};

### Floating point comparison
class **approx** { };  

Use `approx` to compare floating point values as follows:

EXPECT( 1.23 == approx( 1.23 ) );  
EXPECT( 1.23 != approx( 1.24 ) );  

EXPECT( 1.23 != approx( 1.231 ) );  
EXPECT( 1.23 == approx( 1.231 ).epsilon( 0.1 ) );  

approx custom = approx::custom().epsilon( 0.1 );  
    
EXPECT( approx( 1.231 ) != 1.23 );  
EXPECT( custom( 1.231 ) == 1.23 );  

### Functions
template\<std::size_t N\>  
int **run(** test const (& _specification_ )[N], std::ostream & _os_ = std::cout **)**
- _specification_ - array of tests
- _os_ - stream to report to
- returns number of failing tests

template\<std::size_t N\>  
int **run(** test const (& _specification_ )[N], std::vector\<std::string\> _arguments_, std::ostream & _os_ = std::cout **)**
- _specification_ - array of tests
- _arguments_ - arguments to select and omit tests
- _os_ - stream to report to
- returns number of failing tests

template\<std::size_t N\>  
int **run(** test const (& _specification_ )[N], int _argc_, char \* _argv_[], std::ostream & _os_ = std::cout **)**
- _specification_ - array of tests
- _argc_, _arcv_ - arguments to select and omit tests
- _os_ - stream to report to
- returns number of failing tests


Variants of lest
----------------

Various variants of lest are kept here. The simple ones, such as `lest_basic` and `lest_decompose` provide an easy read into the techniques used and remain the tiny test frameworks that are a good fit to include with small projects.

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
Expression decomposition      | +      | +     | +         | -     |
Floating point comparison     | +      | +     | -         | -     |
Test selection (include/omit) | +      | +     | -         | -     |
Help screen                   | +      | -     | -         | -     |
Count selected tests          | +      | -     | -         | -     |
List selected tests           | +      | -     | -         | -     |
List executing tests (verbose)| -      | -     | -         | -     |
Report succeeding tests       | -      | -     | -         | -     |


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

Compilation of `main()` for Catch takes a noticeable amount of time. To reduce compilation times with Catch, its `main()` is compiled separately from the tests. The formula above takes this into account.

Using lambdas as test functions clearly comes at a cost. To keep (re)compile times reasonable for TDD, a source file with `lest` tests should probably have no more than circa 100 assertions. `lest_cpp03` has compile times similar to `Catch`.


Notes and References
--------------------

[1] [Kevlin Henney on Rethinking Unit Testing in C++](http://accu.org/index.php/accu_branches/accu_london/accu_london_may_2010)  ([Video](http://skillsmatter.com/podcast/agile-testing/kevlin-henney-rethinking-unit-testing-in-c-plus-plus)).

[2] Martin Moene. [Elefant C++11 test setup](http://lists.accu.org/mailman/private/accu-general/2013-June/040594.html) on the [ACCU](http://accu.org/) mailing list accu-general (requires login). It mentions the C++11 test appoach Andrzej Krzemie&#x144;ski uses for [Optional. A library for optional (nullable) objects for C++11](https://github.com/akrzemi1/Optional/).

[3] Phil Nash. [CATCH, an automated test framework for C, C++ and Objective-C](http://builds.catch-lib.net/).

[4] A more technically informed name: lest - lambda engaged small tester.

[![Build Status](https://travis-ci.org/martinmoene/lest.png?branch=master)](https://travis-ci.org/martinmoene/lest)
