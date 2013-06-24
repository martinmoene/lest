lest &ndash; lest errors escape testing
=======================================

This tiny C++11 test helper is based on ideas and examples by Kevlin Henney [1,2] and on the CATCH test framework by Phil Nash [3].

Let writing tests become irresistibly easy and attractive.

Example usage
-------------

```Cpp
#include "lest.hpp"
#include <string>

using namespace std;

const lest::test specification[] =
{
    "Empty string has length zero (succeed)", []()
    {
        EXPECT( 0 == string(  ).length() );
        EXPECT( 0 == string("").length() );
    },

    "Text compares lexically (fail)", []()
    {
        EXPECT( string("hello") > string("world") );
    },

    "Unexpected exception is reported", []()
    {
        EXPECT( (throw std::runtime_error("surprise!"), true) );
    },

    "Unspecified expected exception is captured", []()
    {
        EXPECT_THROWS( (throw std::runtime_error("surprise!"), true) );
    },

    "Specified expected exception is captured", []()
    {
        EXPECT_THROWS_AS( (throw std::bad_alloc(), true), std::bad_alloc );
    },

    "Expected exception is reported missing", []()
    {
        EXPECT_THROWS( true );
    },

    "Specific expected exception is reported missing", []()
    {
        EXPECT_THROWS_AS( true, std::runtime_error );
    },
};

int main()
{
    return lest::run( specification );
}
```

Compile and run
---------------

```
prompt>g++ -Wall -Wextra -Weffc++ -std=c++11 -o example1.exe example1.cpp && example1
example1.cpp:16: failed: Text compares lexically (fail): string("hello") > string("world")
example1.cpp:21: failed: got unexpected exception with message "surprise!": Unexpected exception is reported: (throw std::runtime_error("surprise!"), true)
example1.cpp:36: failed: didn't get exception: Expected exception is reported missing: true
example1.cpp:41: failed: didn't get exception of type std::runtime_error: Specific expected exception is reported missing: true
4 out of 7 tests failed.
```

Notes and References
--------------------

[1] [Kevlin Henney on Rethinking Unit Testing in C++](http://accu.org/index.php/accu_branches/accu_london/accu_london_may_2010)  ([Video](http://skillsmatter.com/podcast/agile-testing/kevlin-henney-rethinking-unit-testing-in-c-plus-plus)).

[2] Martin Moene. [Elefant C++11 test setup](http://lists.accu.org/mailman/private/accu-general/2013-June/040594.html) on the [ACCU](http://accu.org/) mailing list accu-general (requires login). It mentions the C++11 test appoach Andrzej Krzemie&#x144;ski uses for [Optional. A library for optional (nullable) objects for C++11](https://github.com/akrzemi1/Optional/).

[3] Phil Nash. [CATCH, an automated test framework for C, C++ and Objective-C](http://builds.catch-lib.net/).

[4] A more technically informed name: lest - lambda engaged small tester.
