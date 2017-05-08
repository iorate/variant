
// iorate::variant
//
// Copyright iorate 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <utility>
#include <variant>
#include "common.hpp"

BOOST_AUTO_TEST_CASE(holds_alternative)
{
    constexpr iorate::variant<int, double> v = 42;
    static_assert(iorate::holds_alternative<int>(v));
    static_assert(!iorate::holds_alternative<double>(v));
}

BOOST_AUTO_TEST_CASE(get_by_index)
{
    {
        auto f = []() constexpr {
            iorate::variant<int, double> v = 23;
            static_assert(std::is_same_v<decltype(iorate::get<0>(v)), int &>);
            iorate::get<0>(v) = 42;
            static_assert(std::is_same_v<decltype(iorate::get<0>(std::move(v))), int &&>);
            return iorate::get<0>(std::move(v));
        };
        static_assert(f() == 42);
    }
    {
        iorate::variant<int, double> v = 42;
        BOOST_CHECK_THROW(iorate::get<1>(v), std::bad_variant_access);
        BOOST_CHECK_THROW(iorate::get<1>(std::move(v)), std::bad_variant_access);
    }
    {
        constexpr iorate::variant<int, double> v = 42;
        static_assert(std::is_same_v<decltype(iorate::get<0>(v)), int const &>);
        static_assert(iorate::get<0>(v) == 42);
        static_assert(std::is_same_v<decltype(iorate::get<0>(std::move(v))), int const &&>);
        static_assert(iorate::get<0>(std::move(v)) == 42);
        
        BOOST_CHECK_THROW(iorate::get<1>(v), std::bad_variant_access);
        BOOST_CHECK_THROW(iorate::get<1>(std::move(v)), std::bad_variant_access);
    }
}

BOOST_AUTO_TEST_CASE(get_by_type)
{
    {
        auto f = []() constexpr {
            iorate::variant<int, double> v = 23;
            static_assert(std::is_same_v<decltype(iorate::get<int>(v)), int &>);
            iorate::get<int>(v) = 42;
            static_assert(std::is_same_v<decltype(iorate::get<int>(std::move(v))), int &&>);
            return iorate::get<int>(std::move(v));
        };
        static_assert(f() == 42);
    }
    {
        iorate::variant<int, double> v = 42;
        BOOST_CHECK_THROW(iorate::get<double>(v), std::bad_variant_access);
        BOOST_CHECK_THROW(iorate::get<double>(std::move(v)), std::bad_variant_access);
    }
    {
        constexpr iorate::variant<int, double> v = 42;
        static_assert(std::is_same_v<decltype(iorate::get<int>(v)), int const &>);
        static_assert(iorate::get<int>(v) == 42);
        static_assert(std::is_same_v<decltype(iorate::get<int>(std::move(v))), int const &&>);
        static_assert(iorate::get<int>(std::move(v)) == 42);
        
        BOOST_CHECK_THROW(iorate::get<double>(v), std::bad_variant_access);
        BOOST_CHECK_THROW(iorate::get<double>(std::move(v)), std::bad_variant_access);
    }
}

BOOST_AUTO_TEST_CASE(get_if_by_index)
{
    {
        constexpr iorate::variant<int, double> *v = nullptr;
        static_assert(std::is_same_v<decltype(iorate::get_if<0>(v)), int *>);
        static_assert(iorate::get_if<0>(v) == nullptr);
    }
    {
        auto f = []() constexpr {
            iorate::variant<int, double> v = 42;
            static_assert(std::is_same_v<decltype(iorate::get_if<0>(&v)), int *>);
            return *iorate::get_if<0>(&v);
        };
        static_assert(f() == 42);
    }
    {
        auto f = []() constexpr {
            iorate::variant<int, double> v = 42;
            static_assert(std::is_same_v<decltype(iorate::get_if<1>(&v)), double *>);
            return iorate::get_if<1>(&v);
        };
        static_assert(f() == nullptr);
    }
    {
        constexpr iorate::variant<int, double> v = 42;
        static_assert(std::is_same_v<decltype(iorate::get_if<0>(&v)), int const *>);
        static_assert(iorate::get_if<0>(static_cast<decltype(v) *>(nullptr)) == nullptr);
        static_assert(*iorate::get_if<0>(&v) == 42);
        static_assert(iorate::get_if<1>(&v) == nullptr);
    }
}

BOOST_AUTO_TEST_CASE(get_if_by_type)
{
    {
        constexpr iorate::variant<int, double> *v = nullptr;
        static_assert(std::is_same_v<decltype(iorate::get_if<int>(v)), int *>);
        static_assert(iorate::get_if<int>(v) == nullptr);
    }
    {
        auto f = []() constexpr {
            iorate::variant<int, double> v = 42;
            static_assert(std::is_same_v<decltype(iorate::get_if<int>(&v)), int *>);
            return *iorate::get_if<int>(&v);
        };
        static_assert(f() == 42);
    }
    {
        auto f = []() constexpr {
            iorate::variant<int, double> v = 42;
            static_assert(std::is_same_v<decltype(iorate::get_if<double>(&v)), double *>);
            return iorate::get_if<double>(&v);
        };
        static_assert(f() == nullptr);
    }
    {
        constexpr iorate::variant<int, double> v = 42;
        static_assert(std::is_same_v<decltype(iorate::get_if<int>(&v)), int const *>);
        static_assert(iorate::get_if<int>(static_cast<decltype(v) *>(nullptr)) == nullptr);
        static_assert(*iorate::get_if<int>(&v) == 42);
        static_assert(iorate::get_if<double>(&v) == nullptr);
    }
}
