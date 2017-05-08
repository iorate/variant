
// iorate::variant
//
// Copyright iorate 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <string>
#include <type_traits>
#include <utility>
#include "common.hpp"

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif

BOOST_AUTO_TEST_CASE(relational_operators)
{
    {
        constexpr iorate::variant<int, double> v1 = 42;
        constexpr iorate::variant<int, double> v2 = 42;
        static_assert( (v1 == v2));
        static_assert(!(v1 != v2));
        static_assert(!(v1 <  v2));
        static_assert( (v1 <= v2));
        static_assert(!(v1 >  v2));
        static_assert( (v1 >= v2));
    }
    {
        constexpr iorate::variant<int, double> v1 = 42;
        constexpr iorate::variant<int, double> v2 = 65;
        static_assert(!(v1 == v2));
        static_assert( (v1 != v2));
        static_assert( (v1 <  v2));
        static_assert( (v1 <= v2));
        static_assert(!(v1 >  v2));
        static_assert(!(v1 >= v2));
    }
    {
        constexpr iorate::variant<int, double> v1 = 42;
        constexpr iorate::variant<int, double> v2 = 23;
        static_assert(!(v1 == v2));
        static_assert( (v1 != v2));
        static_assert(!(v1 <  v2));
        static_assert(!(v1 <= v2));
        static_assert( (v1 >  v2));
        static_assert( (v1 >= v2));
    }
    {
        constexpr iorate::variant<int, double> v1 = 42;
        constexpr iorate::variant<int, double> v2 = 3.14;
        static_assert(!(v1 == v2));
        static_assert( (v1 != v2));
        static_assert( (v1 <  v2));
        static_assert( (v1 <= v2));
        static_assert(!(v1 >  v2));
        static_assert(!(v1 >= v2));
    }
    {
        constexpr iorate::variant<int, double> v1 = 3.14;
        constexpr iorate::variant<int, double> v2 = 42;
        static_assert(!(v1 == v2));
        static_assert( (v1 != v2));
        static_assert(!(v1 <  v2));
        static_assert(!(v1 <= v2));
        static_assert( (v1 >  v2));
        static_assert( (v1 >= v2));
    }
}
