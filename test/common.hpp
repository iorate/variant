
// iorate::variant
//
// Copyright iorate 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef TEST_COMMON_HPP
#define TEST_COMMON_HPP

#include "../variant.hpp"

struct susp_copy_move
{
    constexpr susp_copy_move() noexcept :
        value(0)
    {}

    explicit susp_copy_move(int v) noexcept :
        value(v)
    {}

    susp_copy_move(susp_copy_move const &rhs) noexcept(false) :
        value(rhs.value)
    {}

    susp_copy_move(susp_copy_move &&rhs) noexcept(false) :
        value(rhs.value)
    {
        rhs.value = 0;
    }

    susp_copy_move &operator=(susp_copy_move const &rhs) noexcept(false)
    {
        value = rhs.value;
        return *this;
    }

    susp_copy_move &operator=(susp_copy_move &&rhs) noexcept(false)
    {
        value = rhs.value;
        rhs.value = 0;
        return *this;
    }

    int value;
};

struct bad_copy_move
{
    constexpr bad_copy_move() = default;

    bad_copy_move(bad_copy_move const &) noexcept(false) { throw 42; }

    bad_copy_move(bad_copy_move &&) noexcept(false) { throw 42; }

    bad_copy_move &operator=(bad_copy_move const &) noexcept(false) { throw 42; }

    bad_copy_move &operator=(bad_copy_move &&) noexcept(false) { throw 42; }
};

struct bad_emplace
{
    template <class T>
    operator T() const { throw 42; }
};

template <class T>
inline void force_backup(iorate::variant<susp_copy_move, T> &v, int value)
{
    v.template emplace<0>(value);
    try { v.template emplace<1>(bad_emplace()); } catch(int) {}
}

#endif
