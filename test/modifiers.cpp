
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

BOOST_AUTO_TEST_CASE(emplace_by_index)
{
    {
        iorate::variant<int, double> v;
        static_assert(std::is_same_v<decltype(v.emplace<1>(12.0)), double &>);
        BOOST_TEST(v.emplace<1>(12.0) == 12.0);
    }
    {
        iorate::variant<int, std::string> v;
        static_assert(std::is_same_v<decltype(v.emplace<1>("foo")), std::string &>);
        BOOST_TEST(v.emplace<1>("foo") == "foo");
        static_assert(std::is_same_v<decltype(v.emplace<1>({'b','a','r'})), std::string &>);
        BOOST_TEST(v.emplace<1>({'b','a','r'}) == "bar");
    }
    {
        iorate::variant<int, double> v = 42;
        BOOST_CHECK_THROW(v.emplace<1>(bad_emplace()), int);
        BOOST_TEST(iorate::get<0>(v) == 42);
    }
    {
        iorate::variant<susp_copy_move, double> v;
        static_assert(std::is_same_v<decltype(v.emplace<1>(12.0)), double &>);
        BOOST_TEST(v.emplace<1>(12.0) == 12.0);
    }
    {
        iorate::variant<susp_copy_move, std::string> v;
        static_assert(std::is_same_v<decltype(v.emplace<1>("foo")), std::string &>);
        BOOST_TEST(v.emplace<1>("foo") == "foo");
        static_assert(std::is_same_v<decltype(v.emplace<1>({'b','a','r'})), std::string &>);
        BOOST_TEST(v.emplace<1>({'b','a','r'}) == "bar");
    }
    {
        iorate::variant<susp_copy_move, double> v(std::in_place_index<0>, 42);
        BOOST_CHECK_THROW(v.emplace<1>(bad_emplace()), int);
        BOOST_TEST(iorate::get<0>(v).value == 42);
    }
}

BOOST_AUTO_TEST_CASE(emplace_by_type)
{
    {
        iorate::variant<int, double> v;
        static_assert(std::is_same_v<decltype(v.emplace<double>(12.0)), double &>);
        BOOST_TEST(v.emplace<double>(12.0) == 12.0);
    }
    {
        iorate::variant<int, std::string> v;
        static_assert(std::is_same_v<decltype(v.emplace<std::string>("foo")), std::string &>);
        BOOST_TEST(v.emplace<std::string>("foo") == "foo");
        static_assert(std::is_same_v<decltype(v.emplace<std::string>({'b','a','r'})), std::string &>);
        BOOST_TEST(v.emplace<std::string>({'b','a','r'}) == "bar");
    }
    {
        iorate::variant<int, double> v = 42;
        BOOST_CHECK_THROW(v.emplace<double>(bad_emplace()), int);
        BOOST_TEST(iorate::get<0>(v) == 42);
    }
    {
        iorate::variant<susp_copy_move, double> v;
        static_assert(std::is_same_v<decltype(v.emplace<double>(12.0)), double &>);
        BOOST_TEST(v.emplace<double>(12.0) == 12.0);
    }
    {
        iorate::variant<susp_copy_move, std::string> v;
        static_assert(std::is_same_v<decltype(v.emplace<std::string>("foo")), std::string &>);
        BOOST_TEST(v.emplace<std::string>("foo") == "foo");
        static_assert(std::is_same_v<decltype(v.emplace<std::string>({'b','a','r'})), std::string &>);
        BOOST_TEST(v.emplace<std::string>({'b','a','r'}) == "bar");
    }
    {
        iorate::variant<susp_copy_move, double> v(std::in_place_index<0>, 42);
        BOOST_CHECK_THROW(v.emplace<double>(bad_emplace()), int);
        BOOST_TEST(iorate::get<0>(v).value == 42);
    }
}
