
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
#include "common.hpp"

BOOST_AUTO_TEST_CASE(swap_)
{
    static_assert(std::is_nothrow_swappable_v<iorate::variant<int, std::string>>);

    using variant_t = iorate::variant<int, int, susp_copy_move, susp_copy_move, bad_copy_move>;
    variant_t v1, v2;
    static_assert(!std::is_nothrow_swappable_v<variant_t>);

    v1.emplace<0>(23);
    v2.emplace<0>(42);
    swap(v1, v2);
    BOOST_TEST(iorate::get<0>(v1) == 42);
    BOOST_TEST(iorate::get<0>(v2) == 23);

    v1.emplace<0>(23);
    v2.emplace<1>(42);
    swap(v1, v2);
    BOOST_TEST(iorate::get<1>(v1) == 42);
    BOOST_TEST(iorate::get<0>(v2) == 23);

    v1.emplace<0>(23);
    v2.emplace<2>(42);
    swap(v1, v2);
    BOOST_TEST(iorate::get<2>(v1).value == 42);
    BOOST_TEST(iorate::get<0>(v2) == 23);

    v1.emplace<0>(23);
    v2.emplace<4>();
    BOOST_CHECK_THROW(swap(v1, v2), int);
    BOOST_TEST(iorate::get<0>(v1) == 23);

    v1.emplace<2>(23);
    v2.emplace<0>(42);
    swap(v1, v2);
    BOOST_TEST(iorate::get<0>(v1) == 42);
    BOOST_TEST(iorate::get<2>(v2).value == 23);

    v1.emplace<2>(23);
    v2.emplace<2>(42);
    swap(v1, v2);
    BOOST_TEST(iorate::get<2>(v1).value == 42);
    BOOST_TEST(iorate::get<2>(v2).value == 23);

    v1.emplace<2>(23);
    v2.emplace<3>(42);
    swap(v1, v2);
    BOOST_TEST(iorate::get<3>(v1).value == 42);
    BOOST_TEST(iorate::get<2>(v2).value == 23);

    v1.emplace<2>(23);
    v2.emplace<4>();
    BOOST_CHECK_THROW(swap(v1, v2), int);
    BOOST_TEST(iorate::get<2>(v1).value == 23);

    v1.emplace<4>();
    v2.emplace<0>(42);
    BOOST_CHECK_THROW(swap(v1, v2), int);
    BOOST_TEST(iorate::get<0>(v2) == 42);

    v1.emplace<4>();
    v2.emplace<2>(42);
    BOOST_CHECK_THROW(swap(v1, v2), int);
    BOOST_TEST(iorate::get<2>(v2).value == 42);

    v1.emplace<4>();
    v2.emplace<4>();
    BOOST_CHECK_THROW(swap(v1, v2), int);
}
