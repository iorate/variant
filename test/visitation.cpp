
// iorate::variant
//
// Copyright iorate 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <functional>
#include <utility>
#include "common.hpp"

BOOST_AUTO_TEST_CASE(visit)
{
    struct A
    {
        constexpr int f(int n) const { return v + n; }
        int v;
    };
    auto g = [](A const &a) constexpr { return a.v; };

    static constexpr A a{42};
    constexpr iorate::variant<A, A> t1_1(std::in_place_index<1>, a);
    constexpr iorate::variant<A, A const *> t1_2(&a);
    iorate::variant<A, std::reference_wrapper<A const>> const t1_3(std::ref(a));
    constexpr iorate::variant<int, int> t2(std::in_place_index<0>, 23);

    static_assert(iorate::visit(&A::f, t1_1, t2) == 65);
    static_assert(iorate::visit(&A::f, t1_2, t2) == 65);
    BOOST_TEST(iorate::visit(&A::f, t1_3, t2) == 65);

    static_assert(iorate::visit(&A::v, t1_1) == 42);
    static_assert(iorate::visit(&A::v, t1_2) == 42);
    BOOST_TEST(iorate::visit(&A::v, t1_3) == 42);

    static_assert(iorate::visit(g, t1_1) == 42);
}
