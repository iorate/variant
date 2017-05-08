
// iorate::variant
//
// Copyright iorate 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <string>
#include <variant>
#include "common.hpp"

BOOST_AUTO_TEST_CASE(hash)
{
    std::variant<int, std::string> v1 = "Hello, world!";
    iorate::variant<int, std::string> v2 = "Hello, world!";
    BOOST_TEST(std::hash<decltype(v1)>()(v1) == std::hash<decltype(v2)>()(v2));
}
