Expect abort assertions
=======================
This extension lets you assert assertions and other calls to `abort()`.
It works by substituting abort() from the standard library with a version of our own.

There are two versions of this extension:

- lest_expect_abort.hpp - for C++11
- lest_expect_abort_cpp03.hpp - for C++98/03


Assertion macros
----------------
**lest_EXPECT_NO_ABORT(** _expr_ **)**
Evaluate the expression and expect that abort is not called. If abort() *is* called or if an exception (of any type) is thrown it reported and counted as a failure.

**lest_EXPECT_ABORTS(** _expr_ **)**
Evaluate the expression and expect that abort is called. If abort() is not called or if an exception (of any type) is thrown it is reported and counted as a failure.

Example usage
-------------

```Cpp
#include "lest_expect_abort.hpp"
#include <cassert>

#define CASE( name ) lest_CASE( specification, name )

static lest::tests specification;

CASE( "Expect_aborts succeeds for assert(false) " "[pass]" )
{
    EXPECT_ABORTS( assert( false ) );
}

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv /*, std::cout */ );
}
```

### Compile and run

Using g++:
```
prompt>g++ -Wall -std=c++11 -Dlest_FEATURE_AUTO_REGISTER=1 -I../ -I../../../ -o 00_basic.exe 00_basic.cpp && 00_basic.exe --pass
00_basic.cpp:10: passed: aborted: Expect_aborts succeeds for assert(false) [pass]: assert( false )
All 1 selected test passed.
```
With VC14 (VS2015):
```
prompt>cl -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I../ -I../../../ 00_basic.cpp && 00_basic.exe --pass
...
00_basic.cpp(15): passed: aborted: Expect_aborts succeeds for assert(false) [pass]: assert( false )
All 1 selected test passed.
```
Compiling with a pre-VC14 compiler requires linker option [`/FORCE:MULTIPLE`](https://msdn.microsoft.com/en-us/library/70abkas3.aspx) to accept the multiply defined `abort` symbol.

```
prompt>cl -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I../ -I../../../ 00_basic.cpp /link /FORCE:MULTIPLE && 00_basic.exe --pass
...
LIBCMT.lib(abort.obj) : warning LNK4006: _abort already defined in 00_basic.obj; second definition ignored
00_basic.exe : warning LNK4088: image being generated due to /FORCE option; image may not run
00_basic.cpp(10): passed: aborted: Expect_aborts succeeds for assert(false) [pass]: assert( false )
All 1 selected test passed.
```

**Important**: do not use `'c'` (extern "C" defaults to nothrow) in `-EHsc` in combination with optimisation `-O1`, `-O2` or `-Ox`.

For example, with VC14 (VS2015):
```
prompt>cl -O2 -EHs -Dlest_FEATURE_AUTO_REGISTER=1 -I../ -I../../../ 00_basic.cpp && 00_basic.exe --pass
...
00_basic.cpp(15): passed: aborted: Expect_aborts succeeds for assert(false) [pass]: assert( false )
All 1 selected test passed.
```

Appendix A: Test specification
------------------------------
Issuing command `example\00-specification.exe -l @` gives the following listing of the test specification:

```
Expect_aborts succeeds for std::abort() [pass]
Expect_aborts succeeds for assert(false) [pass]
Expect_aborts reports assert(true) [fail]
Expect_aborts reports an unexpected standard exception [fail]
Expect_aborts reports an unexpected non-standard exception [fail]
Expect_no_abort succeeds for assert(true) [pass]
Expect_no_abort reports std::abort() [fail]
Expect_no_abort reports assert(false) [fail]
Expect_no_abort reports an unexpected standard exception [fail]
Expect_no_abort reports an unexpected non-standard exception [fail]
```
