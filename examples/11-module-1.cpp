// C++11 - use multiple source files.

#include "lest.hpp"

lest::tests & specifications() 
{ 
    static lest::tests tests; 
    return tests; 
}

int main( int argc, char * argv[] )
{
    return lest::run( specifications(), lest::texts( argv + 1, argv + argc ) /*, std::cout */ );
}

// cl -nologo -W3 -EHsc -I.. -Fe11-module.exe 11-module-1.cpp 11-module-2.cpp 11-module-3.cpp && 11-module
// g++ -Wall -Wextra -std=c++11 -I.. -o 11-module.exe 11-module-1.cpp 11-module-2.cpp 11-module-3.cpp && 11-module
