// C++11 - use multiple source files.

#include "lest.hpp"

lest::tests & specification() 
{ 
    static lest::tests tests; 
    return tests; 
}

int main( int argc, char * argv[] )
{
    return lest::run( specification(), argc, argv /*, std::cout */ );
}

// cl -nologo -W3 -EHsc -I.. -Fe11-module.exe 11-module-1.cpp 11-module-2.cpp 11-module-3.cpp && 11-module
// g++ -Wall -Wextra -std=c++11 -I.. -o 11-module.exe 11-module-1.cpp 11-module-2.cpp 11-module-3.cpp && 11-module
