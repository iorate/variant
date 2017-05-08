
// iorate::variant
//
// Copyright iorate 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <type_traits>
#include <variant>
#include "common.hpp"

BOOST_AUTO_TEST_CASE(variant_helper_classes)
{
    using variant_t = iorate::variant<int, double>;

    static_assert(std::variant_size_v<variant_t> == 2);
    static_assert(std::variant_size_v<variant_t const> == 2);
    static_assert(std::variant_size_v<variant_t volatile> == 2);
    static_assert(std::variant_size_v<variant_t const volatile> == 2);

    static_assert(std::is_same_v<std::variant_alternative_t<1, variant_t>, double>);
    static_assert(std::is_same_v<std::variant_alternative_t<1, variant_t const>, double const>);
    static_assert(std::is_same_v<std::variant_alternative_t<1, variant_t volatile>, double volatile>);
    static_assert(std::is_same_v<std::variant_alternative_t<1, variant_t const volatile>, double const volatile>);
}
