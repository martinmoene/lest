// C++11 - report a user-defined data type.

#include "lest/lest.hpp"

namespace ns {

struct book
{
    using text = std::string;

    text author;
    text title;
    text isbn;

    book( text author, text title, text isbn_ )
    : author( author ), title( title ), isbn( isbn_ ) {}

    friend bool operator==( book const & lhs, book const & rhs )
    {
        return lhs.author == rhs.author
            && lhs.title == rhs.title
            && lhs.isbn == rhs.isbn;
    }

    friend bool operator!=( book const & lhs, book const & rhs )
    {
        return ! ( lhs == rhs );
    }
};

// provide stream operator for ns::book:

std::ostream & operator<<( std::ostream & os, book const & b )
{
    using lest::to_string;
    return os << "[book: " << to_string(b.author) << ", " << to_string(b.title) << ", " << to_string(b.isbn) << "]";
}

} // namespace ns

ns::book atocpp{ "Bjarne Stroustrup", "A Tour of C++.", "978-0-321-95831-0" };
ns::book tcpppl{ "Bjarne Stroustrup", "The C++ Programming Language.", "978-0-321-56384-2" };

const lest::test specification[] =
{
    CASE( "A book reports via the book-specific operator<<()" )
    {
        EXPECT( atocpp != tcpppl );
        EXPECT( atocpp == tcpppl );
    },

    CASE( "A collection of books reports via the book-specific operator<<()" )
    {
        std::vector<ns::book> less = { atocpp }, more = { tcpppl };

        EXPECT( less == more );
    },
};

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv );
}

// cl -nologo -W3 -EHsc -I../include 07-udt.cpp && 07-udt
// g++ -Wall -Wextra -Wmissing-include-dirs -std=c++11 -I../include -o 07-udt.exe 07-udt.cpp && 07-udt

// 07-udt.cpp:49: failed: A book reports via the book-specific operator<<(): atocpp == tcpppl for [book: "Bjarne Stroustrup", "A Tour of C++.", "978-0-321-95831-0"] == [book: "Bjarne Stroustrup", "The C++ Programming Language.", "978-0-321-56384-2"]
// 07-udt.cpp:56: failed: A collection of books reports via the book-specific operator<<(): less == more for { [book: "Bjarne Stroustrup", "A Tour of C++.", "978-0-321-95831-0"], } == { [book: "Bjarne Stroustrup", "The C++ Programming Language.", "978-0-321-56384-2"], }
// 2 out of 2 selected tests failed.
