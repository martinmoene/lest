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
    "Empty string has length zero", []()
    {
        REQUIRE( 0 == string(  ).length() );
        REQUIRE( 0 == string("").length() );
    },

    "Text compares lexically", []()
    {
        CHECK( string("hello") < string("world") );
        CHECK( string("hello") > string("world") );
        CHECK( string("skipped") == string("assertion") );
    },
};

int main()
{
    return lest::breaks( specification );
}
```

Note that there are two assertion macro's: CHECK and REQUIRE. `REQUIRE` tests an expression and aborts the test program if it fails. `CHECK` also tests an expression but execution continues *in the next test* if the assertion fails.

Unexpected exceptions are caught and shown. It is not yet possible to test for expected exceptions.

Compile and run
---------------

```
prompt>g++ -Wall -Wextra -std=c++11 -o example1.exe example1.cpp && example1
Error(17): 'Text compares lexically': string("hello") > string("world")
1 out of 2 tests failed.
```

Notes and References
--------------------

[1] [Kevlin Henney on Rethinking Unit Testing in C++](http://accu.org/index.php/accu_branches/accu_london/accu_london_may_2010). [(Video)](http://skillsmatter.com/podcast/agile-testing/kevlin-henney-rethinking-unit-testing-in-c-plus-plus)

[2] Martin Moene. [Elefant C++11 test setup](http://lists.accu.org/mailman/private/accu-general/2013-June/040594.html) on the [ACCU](http://accu.org/) mailing list accu-general (requires login).

[3] Phil Nash. [CATCH, an automated test framework for C, C++ and Objective-C](http://builds.catch-lib.net/).

[4] A more technically informed name: lest - lambda engaged small tester.
