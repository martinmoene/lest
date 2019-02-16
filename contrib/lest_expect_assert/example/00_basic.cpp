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

#if 0
// VC:
cl -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I.. -I../../../include 00_basic.cpp && 00_basic.exe --pass
// suppress portability/security warnings
cl -W3 -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -I.. -I../../../include 00_basic.cpp && 00_basic.exe --pass
// Note: omit 'c' (extern "C" defaults to nothrow) in -EHsc when you throw through C functions
cl -O2 -W3 -EHs -Dlest_FEATURE_AUTO_REGISTER=1 -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -I.. -I../../../include 00_basic.cpp && 00_basic.exe --pass

// GNUC:
g++ -Wall -Wextra -std=c++03 -Dlest_FEATURE_AUTO_REGISTER=1 -I.. -I../../../include -o 00_basic.exe 00_basic.cpp && 00_basic.exe --pass
g++ -Wall -Wextra -std=c++11 -Dlest_FEATURE_AUTO_REGISTER=1 -I.. -I../../../include -o 00_basic.exe 00_basic.cpp && 00_basic.exe --pass
#endif
