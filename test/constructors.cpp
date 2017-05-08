
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

BOOST_AUTO_TEST_CASE(default_constructor)
{
    {
        static_assert(std::is_nothrow_default_constructible_v<iorate::variant<int>>);
        constexpr iorate::variant<int> v;
        static_assert(iorate::get<0>(v) == 0);
    }
    {
        iorate::variant<std::string> v;
        BOOST_TEST(iorate::get<0>(v) == "");
    }
    {
        static_assert(std::is_nothrow_default_constructible_v<iorate::variant<susp_copy_move>>);
        iorate::variant<susp_copy_move> v;
        BOOST_TEST(iorate::get<0>(v).value == 0);
    }
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
    {
        iorate::variant<int> v1 = 42;
        auto v2 = v1;
        BOOST_TEST(iorate::get<0>(v2) == 42);
    }
    {
        iorate::variant<std::string> v1 = "foo";
        auto v2 = v1;
        BOOST_TEST(iorate::get<0>(v2) == "foo");
    }
    {
        iorate::variant<susp_copy_move> v1(std::in_place_index<0>, 42);
        auto v2 = v1;
        BOOST_TEST(iorate::get<0>(v2).value == 42);
    }
    {
        // From backup state
        iorate::variant<susp_copy_move, int> v1(std::in_place_index<0>, 42);
        try { v1.emplace<1>(bad_emplace()); } catch (int) {}
        auto v2 = v1;
        BOOST_TEST(iorate::get<0>(v2).value == 42);
    }
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
    {
        static_assert(std::is_nothrow_move_constructible_v<iorate::variant<int>>);
        iorate::variant<int> v1 = 42;
        auto v2 = std::move(v1);
        BOOST_TEST(iorate::get<0>(v2) == 42);
    }
    {
        static_assert(std::is_nothrow_move_constructible_v<iorate::variant<std::string>>);
        iorate::variant<std::string> v1 = "foo";
        auto v2 = std::move(v1);
        BOOST_TEST(iorate::get<0>(v2) == "foo");
    }
    {
        static_assert(!std::is_nothrow_move_constructible_v<iorate::variant<susp_copy_move>>);
        iorate::variant<susp_copy_move> v1(std::in_place_index<0>, 42);
        auto v2 = std::move(v1);
        BOOST_TEST(iorate::get<0>(v2).value == 42);
        BOOST_TEST(iorate::get<0>(v1).value == 0);
    }
    {
        // From backup state
        static_assert(!std::is_nothrow_move_constructible_v<iorate::variant<susp_copy_move, int>>);
        iorate::variant<susp_copy_move, int> v1;
        force_backup(v1, 42);
        auto v2 = std::move(v1);
        BOOST_TEST(iorate::get<0>(v2).value == 42);
        BOOST_TEST(iorate::get<0>(v1).value == 0);
    }
}

BOOST_AUTO_TEST_CASE(universal_constructor)
{
    {
        static_assert(std::is_nothrow_constructible_v<iorate::variant<int, double>, int>);
        constexpr iorate::variant<int, double> v = 42;
        static_assert(iorate::get<0>(v) == 42);
    }
    {
        static_assert(!std::is_nothrow_constructible_v<iorate::variant<int, std::string>, char const (&)[4]>);
        iorate::variant<int, std::string> v = "foo";
        BOOST_TEST(iorate::get<1>(v) == "foo");
    }
}

BOOST_AUTO_TEST_CASE(in_place_constructor)
{
    {
        constexpr iorate::variant<char, int, double> v(std::in_place_index<1>, 42);
        static_assert(iorate::get<1>(v) == 42);
    }
    {
        iorate::variant<int, std::string> v(std::in_place_index<1>, {'f','o','o'});
        BOOST_TEST(iorate::get<1>(v) == "foo");
    }
    {
        constexpr iorate::variant<char, int, double> v(std::in_place_type<int>, 42);
        static_assert(iorate::get<1>(v) == 42);
    }
    {
        iorate::variant<int, std::string> v(std::in_place_type<std::string>, {'f','o','o'});
        BOOST_TEST(iorate::get<1>(v) == "foo");
    }
}
