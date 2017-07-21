// Copyright 2016 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LEST_LEST_ASSERT_HPP_INCLUDED
#define LEST_LEST_ASSERT_HPP_INCLUDED

#ifndef  lest_FEATURE_ASSERT
# define lest_FEATURE_ASSERT  1
#endif

#if lest_FEATURE_ASSERT
# undef  assert
# define assert( expr )  ( !!(expr) ? ((void)0) : ::lest::assertion_failed( #expr, lest___FUNC__, __FILE__, __LINE__) )
#else
# include <cassert>
#endif

#if __cplusplus >= 201103 || _MSC_VER >= 1800
# define lest___FUNC__  __func__
#else
# define lest___FUNC__  "(unknown)"
#endif

namespace lest
{
    void assertion_failed( char const * expr, char const * function, char const * file, long line );
}

#endif // LEST_LEST_ASSERT_HPP_INCLUDED
