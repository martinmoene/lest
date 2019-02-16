Expect assert assertions
========================
This extension lets you assert assertions. It works by substituting `assert` from the standard library with a version of our own and throwing an exception when the assertion failed [[1]](#notes).

There are two versions of this extension:

- lest_expect_assert.hpp - for C++11 and higher
- lest_expect_assert_cpp03.hpp - for C++98/03 and higher

Please note the following:
- For C++11 and higher, throwing an *assertion-failed* exception makes it impossible to verify assertions that live in or beneath functions that are specified as `noexcept`. If your code uses a macro to specify `noexcept`, you may be able to substitute nothing for it for the tests [[2]](#notes). Otherwise this extension is of limited use with C++11 and higher.
- For Visual C++, if the *assertion-failed* exception travels through a C function, the code must be compiled without the 'c' (extern "C" defaults to nothrow) in option `-EHsc`.


Instrument code in .cpp files
-----------------------------
To activate *lest*'s assertion mechanism in your .cpp code, you must include
`lest_assert.hpp` to provide `assert()`. The assertion mechanism of *lest* is active whether `NDEBUG` is defined or not. See also [Feature control macros](#feature-control-macros).


Assertion macros
----------------
**lest_EXPECT_NO_ASSERT(** _expr_ **)**
Evaluate the expression and expect that assert is not called. If assert() *is* called or if an exception (of any type) is thrown it reported and counted as a failure.

**lest_EXPECT_ASSERTS(** _expr_ **)**
Evaluate the expression and expect that assert is called. If assert() is not called or if an exception (of any type) is thrown it is reported and counted as a failure.

Feature control macros
----------------------
**lest_FEATURE_ASSERT**=1
Define this to 0 to disable *lest*'s assertion mechanism and let `lest_assert.hpp` provide `<cassert>`'s mechanism instead. Default, `lest_FEATURE_ASSERT`is 1.


Example usage
-------------

```Cpp
#if __cplusplus >= 201103 || _MSC_VER >= 1800
# include "lest/lest_expect_assert.hpp"
#else
# include "lest/lest_expect_assert_cpp03.hpp"
#endif

#define CASE( name ) lest_CASE( specification, name )

static lest::tests specification;

CASE( "Expect_asserts succeeds for assert(false) " "[pass]" )
{
    EXPECT_ASSERTS( assert( false ) );
}

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv /*, std::cout */ );
}
```

### Compile and run

Using g++:
```
prompt>g++ -Wall -Wextra -std=c++11 -Dlest_FEATURE_AUTO_REGISTER=1 -I.. -I../../../include -o 00_basic.exe 00_basic.cpp && 00_basic.exe --pass
00_basic.cpp:15: passed: asserted: Expect_asserts succeeds for assert(false) [pass]: assert( false )
All 1 selected test passed.
```
With VC14 (VS2015):
```
prompt>cl -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I.. -I../../../include 00_basic.cpp && 00_basic.exe --pass
...
00_basic.cpp(15): passed: asserted: Expect_asserts succeeds for assert(false) [pass]: assert( false )
All 1 selected test passed.
```

Notes
-----
[1] A previous version of this extension used `setjmp()` and `longjmp()` for flow control. However this mechanism [doesn't mix well with C++](http://stackoverflow.com/a/1376099/437272) as it completely subverts stack unwinding and thus exception handling and object desctruction.

[2] Visual C++ 14 (Visual Studio 2015) does not let you `#define noexcept /*emty*/`. Besides it would be hard to also hide noexcept expressions.


Appendix A: Test specification
------------------------------
Issuing command `example\00-specification.exe -l @` gives the following listing of the test specification:

```
Expect_asserts succeeds for assert(false) [pass]
Expect_asserts reports assert(true) [fail]
Expect_asserts succeeds for assert(false) in non-noexcept function [pass]
Expect_asserts terminates for assert(false) in noexcept function [.pass]
Expect_asserts reports an unexpected standard exception [fail]
Expect_asserts reports an unexpected non-standard exception [fail]
Expect_no_assert succeeds for assert(true) [pass]
Expect_no_assert reports assert(false) [fail]
Expect_no_assert reports an unexpected standard exception [fail]
Expect_no_assert reports an unexpected non-standard exception [fail]
```
