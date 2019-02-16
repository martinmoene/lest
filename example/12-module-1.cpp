// C++11 - use multiple source files.

#include "lest/lest.hpp"

lest::tests & specification()
{
    static lest::tests tests;
    return tests;
}

int main( int argc, char * argv[] )
{
    return lest::run( specification(), argc, argv /*, std::cout */ );
}

// cl -nologo -W3 -EHsc -I../include -Fe12-module.exe 12-module-1.cpp 12-module-2.cpp 12-module-3.cpp && 12-module
// g++ -Wall -Wextra -std=c++11 -I../include -o 12-module.exe 12-module-1.cpp 12-module-2.cpp 12-module-3.cpp && 12-module
