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
example5_select.cpp:20: failed: got unexpected exception with message "surprise!": Unexpected exception is reported: (th
row std::runtime_error("surprise!"), true)
example5_select.cpp:35: failed: didn't get exception: Expected exception is reported missing: true
example5_select.cpp:40: failed: didn't get exception of type std::runtime_error: Specific expected exception is reported
 missing: true
4 out of 7 selected tests failed.```

Synopsis
--------

###Command line
Usage: test [spec ...]
where _spec_ can be:
- "*": all tests, except excluded tests.
- "_text_": select tests that contain _text_ (case sensitive).
- "!_text_": omit tests that contain _text_ (case sensitive).

Omission of tests takes precedence over inclusion of tests.

Note: when regular expression selection has been enabled (and is available), spec can use the regular expression syntax of `std::regex_search()`. See also `lest_USE_REGEX_SEARCH` in section [Other Macros](#other-macros).

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

Note: The standard library of GCC 4.8.1 doesn't yet support this use of regular expressions. The library of VC13 and VC14 CTP 3 *can* use regular expressions, but cannot compile the latest variant of lest.

### Namespace
namespace **lest** { }  
Types and functions are located in namespace lest.

### Tests
struct **test**  
{  
&emsp;const std::string name;  
&emsp;const std::function\<void()\> behaviour;  
};

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

- lest.hpp - lest's latest development, this project [1].
- lest_basic.hpp - lest at it's very basic, this project [1].
- lest_decompose.hpp - lest with expression decomposition, this project [1].
- lest_cpp03.hpp - lest with expression decomposition for C++03, this project [1].
- [hamlest](https://github.com/martinmoene/hamlest) - matchers for lest.
- [lest with groups](https://github.com/pmed/lest/tree/test_group) - Pavel Medvedev


###Features

Feature / variant             | latest | decompose | basic | cpp03 |
------------------------------|--------|-----------|-------|-------|
Expression decomposition      | +      | +         | -     | +     |
Test selection (include/omit) | +      | -         | -     | -     |
List available tests          | -      | -         | -     | -     |
Report succeeeding tests      | -      | -         | -     | -     |


Reported to work with
---------------------

Variant / compiler     | clang | GCC   | VC    |
-----------------------|-------|-------|-------|
lest (latest)          |  3.2  | 4.8.1 | ?     |
lest_basic             |  ?    | ?     | ?     |
lest_decompose         |  3.2  | 4.6   | 13 Preview |
lest_cpp03 (decompose) |  ?    | ?     | 6 SP6 |


Notes and References
--------------------

[1] [Kevlin Henney on Rethinking Unit Testing in C++](http://accu.org/index.php/accu_branches/accu_london/accu_london_may_2010)  ([Video](http://skillsmatter.com/podcast/agile-testing/kevlin-henney-rethinking-unit-testing-in-c-plus-plus)).

[2] Martin Moene. [Elefant C++11 test setup](http://lists.accu.org/mailman/private/accu-general/2013-June/040594.html) on the [ACCU](http://accu.org/) mailing list accu-general (requires login). It mentions the C++11 test appoach Andrzej Krzemie&#x144;ski uses for [Optional. A library for optional (nullable) objects for C++11](https://github.com/akrzemi1/Optional/).

[3] Phil Nash. [CATCH, an automated test framework for C, C++ and Objective-C](http://builds.catch-lib.net/).

[4] A more technically informed name: lest - lambda engaged small tester.

[![Build Status](https://travis-ci.org/martinmoene/lest.png?branch=master)](https://travis-ci.org/martinmoene/lest)
