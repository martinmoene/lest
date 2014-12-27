// C++11 - auto-register test cases.

#include "lest.hpp"

#define CASE( name ) lest_CASE( specification(), name )

lest::tests & specification() 
{ 
    static lest::tests tests; 
    return tests; 
}

int main( int argc, char * argv[] )
{
    return lest::run( specification(), argc, argv /*, std::cout */ );
}

// cl -nologo -W3 -EHsc -Dlest_FEATURE_AUTO_REGISTER=1 -I.. -Fe12-module-auto-reg.exe 12-module-auto-reg-1.cpp 12-module-auto-reg-2.cpp 12-module-auto-reg-3.cpp && 12-module-auto-reg
// g++ -Wall -Wextra -std=c++11 -Dlest_FEATURE_AUTO_REGISTER=1 -I.. -o 12-module-auto-reg.exe 12-module-auto-reg-1.cpp 12-module-auto-reg-2.cpp 12-module-auto-reg-3.cpp && 12-module-auto-reg
