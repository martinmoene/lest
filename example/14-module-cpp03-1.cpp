// C++03 - use multiple source files.

#include "lest/lest_cpp03.hpp"

lest::tests & specification()
{
    static lest::tests tests;
    return tests;
}

int main( int argc, char * argv[] )
{
    return lest::run( specification(), argc, argv /*, std::cout */ );
}

// cl -nologo -W3 -EHsc -I../include -Fe14-module-cpp03.exe 14-module-cpp03-1.cpp 14-module-cpp03-2.cpp 14-module-cpp03-3.cpp && 14-module-cpp03
// g++ -Wall -Wextra -std=c++11 -I../include -o 14-module-cpp03.exe 14-module-cpp03-1.cpp 14-module-cpp03-2.cpp 14-module-cpp03-3.cpp && 14-module-cpp03
