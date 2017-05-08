
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

BOOST_AUTO_TEST_CASE(copy_assignment)
{
    {
        iorate::variant<int, double> v1 = 42;
        iorate::variant<int, double> v2 = 65;
        v1 = v2;
        BOOST_TEST(iorate::get<0>(v1) == 65);
    }
    {
        iorate::variant<int, double> v1 = 42;
        iorate::variant<int, double> v2 = 12.0;
        v1 = v2;
        BOOST_TEST(iorate::get<1>(v1) == 12.0);
    }
    {
        iorate::variant<int, std::string> v1 = 42;
        iorate::variant<int, std::string> v2 = "foo";
        v1 = v2;
        BOOST_TEST(iorate::get<1>(v1) == "foo");
    }
    {
        iorate::variant<int, susp_copy_move> v1 = 42;
        iorate::variant<int, susp_copy_move> v2(std::in_place_index<1>, 65);
        v1 = v2;
        BOOST_TEST(iorate::get<1>(v1).value == 65);
    }
    {
        iorate::variant<int, bad_copy_move> v1 = 42;
        iorate::variant<int, bad_copy_move> v2(std::in_place_index<1>);
        BOOST_CHECK_THROW(v1 = v2, int);
        BOOST_TEST(iorate::get<0>(v1) == 42);
    }
    {
        iorate::variant<susp_copy_move, double> v1(std::in_place_index<0>, 42);
        iorate::variant<susp_copy_move, double> v2 = 12.0;
        v1 = v2;
        BOOST_TEST(iorate::get<1>(v1) == 12.0);
        force_backup(v1, 42);
        v1 = v2;
        BOOST_TEST(iorate::get<1>(v1) == 12.0);
    }
    {
        iorate::variant<susp_copy_move, susp_copy_move> v1(std::in_place_index<0>, 42);
        iorate::variant<susp_copy_move, susp_copy_move> v2(std::in_place_index<1>, 65);
        v1 = v2;
        BOOST_TEST(iorate::get<1>(v1).value == 65);
        force_backup(v1, 42);
        v1 = v2;
        BOOST_TEST(iorate::get<1>(v1).value == 65);
    }
    {
        iorate::variant<susp_copy_move, bad_copy_move> v1(std::in_place_index<0>, 42);
        iorate::variant<susp_copy_move, bad_copy_move> v2(std::in_place_index<1>);
        BOOST_CHECK_THROW(v1 = v2, int);
        BOOST_TEST(iorate::get<0>(v1).value == 42);
        BOOST_CHECK_THROW(v1 = v2, int);
        BOOST_TEST(iorate::get<0>(v1).value == 42);
    }
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
    {
        static_assert(std::is_nothrow_move_assignable_v<iorate::variant<int, double>>);
        iorate::variant<int, double> v1 = 42;
        iorate::variant<int, double> v2 = 65;
        v1 = std::move(v2);
        BOOST_TEST(iorate::get<0>(v1) == 65);
    }
    {
        iorate::variant<int, double> v1 = 42;
        iorate::variant<int, double> v2 = 12.0;
        v1 = std::move(v2);
        BOOST_TEST(iorate::get<1>(v1) == 12.0);
    }
    {
        static_assert(std::is_nothrow_move_assignable_v<iorate::variant<int, std::string>>);
        iorate::variant<int, std::string> v1 = 42;
        iorate::variant<int, std::string> v2 = "foo";
        v1 = std::move(v2);
        BOOST_TEST(iorate::get<1>(v1) == "foo");
    }
    {
        static_assert(!std::is_nothrow_move_assignable_v<iorate::variant<int, susp_copy_move>>);
        iorate::variant<int, susp_copy_move> v1 = 42;
        iorate::variant<int, susp_copy_move> v2(std::in_place_index<1>, 65);
        v1 = std::move(v2);
        BOOST_TEST(iorate::get<1>(v1).value == 65);
        BOOST_TEST(iorate::get<1>(v2).value == 0);
    }
    {
        static_assert(!std::is_nothrow_move_assignable_v<iorate::variant<int, bad_copy_move>>);
        iorate::variant<int, bad_copy_move> v1 = 42;
        iorate::variant<int, bad_copy_move> v2(std::in_place_index<1>);
        BOOST_CHECK_THROW(v1 = std::move(v2), int);
        BOOST_TEST(iorate::get<0>(v1) == 42);
    }
    {
        static_assert(!std::is_nothrow_move_assignable_v<iorate::variant<susp_copy_move, double>>);
        iorate::variant<susp_copy_move, double> v1(std::in_place_index<0>, 42);
        iorate::variant<susp_copy_move, double> v2 = 12.0;
        v1 = std::move(v2);
        BOOST_TEST(iorate::get<1>(v1) == 12.0);
        force_backup(v1, 42);
        v1 = std::move(v2);
        BOOST_TEST(iorate::get<1>(v1) == 12.0);
    }
    {
        static_assert(!std::is_nothrow_move_assignable_v<iorate::variant<susp_copy_move, susp_copy_move>>);
        iorate::variant<susp_copy_move, susp_copy_move> v1(std::in_place_index<0>, 42);
        iorate::variant<susp_copy_move, susp_copy_move> v2(std::in_place_index<1>, 65);
        v1 = std::move(v2);
        BOOST_TEST(iorate::get<1>(v1).value == 65);
        BOOST_TEST(iorate::get<1>(v2).value == 0);
        force_backup(v1, 42);
        v2.emplace<1>(65);
        v1 = std::move(v2);
        BOOST_TEST(iorate::get<1>(v1).value == 65);
        BOOST_TEST(iorate::get<1>(v2).value == 0);
    }
    {
        static_assert(!std::is_nothrow_move_assignable_v<iorate::variant<susp_copy_move, bad_copy_move>>);
        iorate::variant<susp_copy_move, bad_copy_move> v1(std::in_place_index<0>, 42);
        iorate::variant<susp_copy_move, bad_copy_move> v2(std::in_place_index<1>);
        BOOST_CHECK_THROW(v1 = std::move(v2), int);
        BOOST_TEST(iorate::get<0>(v1).value == 42);
        BOOST_CHECK_THROW(v1 = std::move(v2), int);
        BOOST_TEST(iorate::get<0>(v1).value == 42);
    }
}

BOOST_AUTO_TEST_CASE(assignment)
{
    {
        iorate::variant<int, double> v = 42;
        static_assert(noexcept(v = 65));
        v = 65;
        BOOST_TEST(iorate::get<0>(v) == 65);
    }
    {
        iorate::variant<int, double> v = 42;
        static_assert(noexcept(v = 12.0));
        v = 12.0;
        BOOST_TEST(iorate::get<1>(v) == 12.0);
    }
    {
        iorate::variant<int, std::string> v = 42;
        static_assert(!noexcept(v = "foo"));
        v = "foo";
        BOOST_TEST(iorate::get<1>(v) == "foo");
    }
    {
        iorate::variant<int, bad_copy_move> v = 42;
        static_assert(!noexcept(v = bad_copy_move()));
        BOOST_CHECK_THROW(v = bad_copy_move(), int);
        BOOST_TEST(iorate::get<0>(v) == 42);
    }
    {
        iorate::variant<susp_copy_move, double> v(std::in_place_index<0>, 42);
        static_assert(noexcept(v = 12.0));
        v = 12.0;
        BOOST_TEST(iorate::get<1>(v) == 12.0);
        force_backup(v, 42);
        v = 12.0;
        BOOST_TEST(iorate::get<1>(v) == 12.0);
    }
    {
        iorate::variant<susp_copy_move, std::string> v(std::in_place_index<0>, 42);
        static_assert(!noexcept(v = "foo"));
        v = "foo";
        BOOST_TEST(iorate::get<1>(v) == "foo");
        force_backup(v, 42);
        v = "foo";
        BOOST_TEST(iorate::get<1>(v) == "foo");
    }
    {
        iorate::variant<susp_copy_move, bad_copy_move> v(std::in_place_index<0>, 42);
        static_assert(!noexcept(v = bad_copy_move()));
        BOOST_CHECK_THROW(v = bad_copy_move(), int);
        BOOST_TEST(iorate::get<0>(v).value == 42);
        BOOST_CHECK_THROW(v = bad_copy_move(), int);
        BOOST_TEST(iorate::get<0>(v).value == 42);
    }
}
