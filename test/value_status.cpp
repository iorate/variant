
// iorate::variant
//
// Copyright iorate 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "common.hpp"

BOOST_AUTO_TEST_CASE(valueless_by_exception)
{
    {
        constexpr iorate::variant<int, double> v = 3.14;
        static_assert(!v.valueless_by_exception());
    }
    {
        iorate::variant<susp_copy_move, int> v;
        force_backup(v, 42);
        BOOST_TEST(!v.valueless_by_exception());
    }
}

BOOST_AUTO_TEST_CASE(index)
{
    {
        constexpr iorate::variant<int, double> v = 3.14;
        static_assert(v.index() == 1);
    }
    {
        iorate::variant<susp_copy_move, int> v;
        force_backup(v, 42);
        BOOST_TEST(v.index() == 0u);
    }
}
