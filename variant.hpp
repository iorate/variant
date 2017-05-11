
// iorate::variant
//
// Copyright iorate 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef IORATE_VARIANT_HPP
#define IORATE_VARIANT_HPP

#include <cstddef>
#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

namespace iorate {

namespace variant_detail {

/* Variadic Utilities */

constexpr std::size_t npos = static_cast<std::size_t>(-1);

template <std::size_t N>
using size_t_ = std::integral_constant<std::size_t, N>;

// at
template <std::size_t I, class ...Types>
struct at {};

template <class Type, class ...Types>
struct at<npos, Type, Types...> {};

template <class Type, class ...Types>
struct at<0, Type, Types...> { using type = Type; };

template <std::size_t I, class Type, class ...Types>
struct at<I, Type, Types...> : at<I - 1, Types...> {};

template <std::size_t I, class ...Types>
using at_t = typename at<I, Types...>::type;

// count
template <std::size_t I, class T, class ...Types>
struct count_impl : size_t_<I> {};

template <std::size_t I, class T, class Type, class ...Types>
struct count_impl<I, T, Type, Types...> :
    count_impl<I + (std::is_same_v<T, Type> ? 1 : 0), T, Types...>
{};

template <class T, class ...Types>
constexpr std::size_t count_v = count_impl<0, T, Types...>::value;

// find
template <std::size_t I, class T, class ...Types>
struct find_impl : size_t_<npos> {};

template <std::size_t I, class T, class Type, class ...Types>
struct find_impl<I, T, Type, Types...> :
    std::conditional_t<std::is_same_v<T, Type>, size_t_<I>, find_impl<I + 1, T, Types...>>
{};

template <class T, class ...Types>
constexpr std::size_t find_v = find_impl<0, T, Types...>::value;

/* Constexpr Invoke */

// is_member_of
template <class F, class T1>
struct is_member_of : std::false_type {};

template <class R, class T, class T1>
struct is_member_of<R T::*, T1> : std::is_base_of<T, T1> {};

template <class F, class T1>
constexpr bool is_member_of_v = is_member_of<F, T1>::value;

// is_reference_wrapper
template <class T>
struct is_reference_wrapper : std::false_type {};

template <class T>
struct is_reference_wrapper<std::reference_wrapper<T>> : std::true_type {};

template <class T>
constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value;

// c_invoke
template <class F, class T1, class ...Ts>
constexpr decltype(auto) c_invoke(F &&f, T1 &&t1, Ts &&...ts)
{
    using FD = std::decay_t<F>;
    using T1D = std::decay_t<T1>;
    if constexpr (std::is_member_function_pointer_v<FD>) {
        if constexpr (is_member_of_v<FD, T1D>) {
            return (std::forward<T1>(t1).*f)(std::forward<Ts>(ts)...);
        } else if constexpr (is_reference_wrapper_v<T1D>) {
            return (t1.get().*f)(std::forward<Ts>(ts)...);
        } else {
            return ((*std::forward<T1>(t1)).*f)(std::forward<Ts>(ts)...);
        }
    } else if constexpr (sizeof...(Ts) == 0 && std::is_member_object_pointer_v<FD>) {
        if constexpr (is_member_of_v<FD, T1D>) {
            return std::forward<T1>(t1).*f;
        } else if constexpr (is_reference_wrapper_v<T1D>) {
            return t1.get().*f;
        } else {
            return (*std::forward<T1>(t1)).*f;
        }
    } else {
        return std::forward<F>(f)(std::forward<T1>(t1), std::forward<Ts>(ts)...);
    }
}

/* Special Member Function Enablers */

// enable_default_constructor
template <bool>
struct enable_default_constructor
{
    constexpr enable_default_constructor() = delete;
};

template <>
struct enable_default_constructor<true>
{
    constexpr enable_default_constructor() = default;
};

// enable_copy_move
template <bool, bool, bool, bool>
struct enable_copy_move
{
    constexpr enable_copy_move() = default;
    constexpr enable_copy_move(enable_copy_move const &) = delete;
    constexpr enable_copy_move(enable_copy_move &&) = delete;
    enable_copy_move &operator=(enable_copy_move const &) = delete;
    enable_copy_move &operator=(enable_copy_move &&) = delete;
};

template <>
struct enable_copy_move<false, false, false, true>
{
    constexpr enable_copy_move() = default;
    constexpr enable_copy_move(enable_copy_move const &) = delete;
    constexpr enable_copy_move(enable_copy_move &&) = delete;
    enable_copy_move &operator=(enable_copy_move const &) = delete;
    enable_copy_move &operator=(enable_copy_move &&) = default;
};

template <>
struct enable_copy_move<false, false, true, false>
{
    constexpr enable_copy_move() = default;
    constexpr enable_copy_move(enable_copy_move const &) = delete;
    constexpr enable_copy_move(enable_copy_move &&) = delete;
    enable_copy_move &operator=(enable_copy_move const &) = default;
    enable_copy_move &operator=(enable_copy_move &&) = delete;
};

template <>
struct enable_copy_move<false, false, true, true>
{
    constexpr enable_copy_move() = default;
    constexpr enable_copy_move(enable_copy_move const &) = delete;
    constexpr enable_copy_move(enable_copy_move &&) = delete;
    enable_copy_move &operator=(enable_copy_move const &) = default;
    enable_copy_move &operator=(enable_copy_move &&) = default;
};

template <>
struct enable_copy_move<false, true, false, false>
{
    constexpr enable_copy_move() = default;
    constexpr enable_copy_move(enable_copy_move const &) = delete;
    constexpr enable_copy_move(enable_copy_move &&) = default;
    enable_copy_move &operator=(enable_copy_move const &) = delete;
    enable_copy_move &operator=(enable_copy_move &&) = delete;
};

template <>
struct enable_copy_move<false, true, false, true>
{
    constexpr enable_copy_move() = default;
    constexpr enable_copy_move(enable_copy_move const &) = delete;
    constexpr enable_copy_move(enable_copy_move &&) = default;
    enable_copy_move &operator=(enable_copy_move const &) = delete;
    enable_copy_move &operator=(enable_copy_move &&) = default;
};

template <>
struct enable_copy_move<false, true, true, false>
{
    constexpr enable_copy_move() = default;
    constexpr enable_copy_move(enable_copy_move const &) = delete;
    constexpr enable_copy_move(enable_copy_move &&) = default;
    enable_copy_move &operator=(enable_copy_move const &) = default;
    enable_copy_move &operator=(enable_copy_move &&) = delete;
};

template <>
struct enable_copy_move<false, true, true, true>
{
    constexpr enable_copy_move() = default;
    constexpr enable_copy_move(enable_copy_move const &) = delete;
    constexpr enable_copy_move(enable_copy_move &&) = default;
    enable_copy_move &operator=(enable_copy_move const &) = default;
    enable_copy_move &operator=(enable_copy_move &&) = default;
};

template <>
struct enable_copy_move<true, false, false, false>
{
    constexpr enable_copy_move() = default;
    constexpr enable_copy_move(enable_copy_move const &) = default;
    constexpr enable_copy_move(enable_copy_move &&) = delete;
    enable_copy_move &operator=(enable_copy_move const &) = delete;
    enable_copy_move &operator=(enable_copy_move &&) = delete;
};

template <>
struct enable_copy_move<true, false, false, true>
{
    constexpr enable_copy_move() = default;
    constexpr enable_copy_move(enable_copy_move const &) = default;
    constexpr enable_copy_move(enable_copy_move &&) = delete;
    enable_copy_move &operator=(enable_copy_move const &) = delete;
    enable_copy_move &operator=(enable_copy_move &&) = default;
};

template <>
struct enable_copy_move<true, false, true, false>
{
    constexpr enable_copy_move() = default;
    constexpr enable_copy_move(enable_copy_move const &) = default;
    constexpr enable_copy_move(enable_copy_move &&) = delete;
    enable_copy_move &operator=(enable_copy_move const &) = default;
    enable_copy_move &operator=(enable_copy_move &&) = delete;
};

template <>
struct enable_copy_move<true, false, true, true>
{
    constexpr enable_copy_move() = default;
    constexpr enable_copy_move(enable_copy_move const &) = default;
    constexpr enable_copy_move(enable_copy_move &&) = delete;
    enable_copy_move &operator=(enable_copy_move const &) = default;
    enable_copy_move &operator=(enable_copy_move &&) = default;
};

template <>
struct enable_copy_move<true, true, false, false>
{
    constexpr enable_copy_move() = default;
    constexpr enable_copy_move(enable_copy_move const &) = default;
    constexpr enable_copy_move(enable_copy_move &&) = default;
    enable_copy_move &operator=(enable_copy_move const &) = delete;
    enable_copy_move &operator=(enable_copy_move &&) = delete;
};

template <>
struct enable_copy_move<true, true, false, true>
{
    constexpr enable_copy_move() = default;
    constexpr enable_copy_move(enable_copy_move const &) = default;
    constexpr enable_copy_move(enable_copy_move &&) = default;
    enable_copy_move &operator=(enable_copy_move const &) = delete;
    enable_copy_move &operator=(enable_copy_move &&) = default;
};

template <>
struct enable_copy_move<true, true, true, false>
{
    constexpr enable_copy_move() = default;
    constexpr enable_copy_move(enable_copy_move const &) = default;
    constexpr enable_copy_move(enable_copy_move &&) = default;
    enable_copy_move &operator=(enable_copy_move const &) = default;
    enable_copy_move &operator=(enable_copy_move &&) = delete;
};

template <>
struct enable_copy_move<true, true, true, true>
{
    constexpr enable_copy_move() = default;
    constexpr enable_copy_move(enable_copy_move const &) = default;
    constexpr enable_copy_move(enable_copy_move &&) = default;
    enable_copy_move &operator=(enable_copy_move const &) = default;
    enable_copy_move &operator=(enable_copy_move &&) = default;
};

/* Variant Internals */

// wrapper
template <std::size_t I, class T, bool = std::is_nothrow_move_constructible_v<T>>
struct wrapper
{
    constexpr wrapper() = default;

    wrapper(wrapper const &rhs) :
        value(std::in_place_index<0>, rhs.unwrap())
    {}

    wrapper(wrapper &&rhs) :
        value(std::in_place_index<0>, std::move(rhs.unwrap()))
    {}

    wrapper(T const &v) :
        value(std::in_place_index<0>, v)
    {}

    wrapper(T &&v) :
        value(std::in_place_index<0>, std::move(v))
    {}

    template <class ...Args>
    wrapper(std::in_place_t, Args &&...args) :
        value(std::in_place_index<0>, std::forward<Args>(args)...)
    {}

    // restore
    wrapper(std::unique_ptr<T> &&p) :
        value(std::in_place_index<1>, std::move(p))
    {}

    std::unique_ptr<T> backup() &&
    {
        return value.index() == 0 ? std::make_unique<T>(std::move(std::get<0>(value))) : std::move(std::get<1>(value));
    }

    T &unwrap() &
    {
        return value.index() == 0 ? std::get<0>(value) : *std::get<1>(value);
    }

    T const &unwrap() const &
    {
        return value.index() == 0 ? std::get<0>(value) : *std::get<1>(value);
    }

    T &&unwrap() &&
    {
        return std::move(value.index() == 0 ? std::get<0>(value) : *std::get<1>(value));
    }

    T const &&unwrap() const &&
    {
        return std::move(value.index() == 0 ? std::get<0>(value) : *std::get<1>(value));
    }

private:
    std::variant<T, std::unique_ptr<T>> value;
};

template <std::size_t I, class T>
struct wrapper<I, T, true>
{
    constexpr wrapper() = default;

    wrapper(T const &v) :
        value(v)
    {}

    // restore
    wrapper(T &&v) :
        value(std::move(v))
    {}

    template <class ...Args>
    constexpr wrapper(std::in_place_t, Args &&...args) :
        value(std::forward<Args>(args)...)
    {}

    T backup() &&
    {
        return std::move(value);
    }

    constexpr T &unwrap() &
    {
        return value;
    }

    constexpr T const &unwrap() const &
    {
        return value;
    }

    constexpr T &&unwrap() &&
    {
        return std::move(value);
    }

    constexpr T const &&unwrap() const &&
    {
        return std::move(value);
    }

private:
    T value;
};

#define DEFINE_WRAPPER_RELATION(OP) \
template <std::size_t I, class T> \
constexpr bool operator OP(wrapper<I, T> const &l_wrapper, wrapper<I, T> const &r_wrapper) \
{ \
    return l_wrapper.unwrap() OP r_wrapper.unwrap(); \
} \
/**/
DEFINE_WRAPPER_RELATION(==)
DEFINE_WRAPPER_RELATION(!=)
DEFINE_WRAPPER_RELATION(< )
DEFINE_WRAPPER_RELATION(<=)
DEFINE_WRAPPER_RELATION(> )
DEFINE_WRAPPER_RELATION(>=)
#undef DEFINE_WRAPPER_RELATION

// variant_base
template <class Indices, class ...Types>
struct variant_base_impl;

template <std::size_t ...Indices, class ...Types>
struct variant_base_impl<std::index_sequence<Indices...>, Types...>
{
    using type = std::variant<wrapper<Indices, Types>...>;
};

template <class ...Types>
using variant_base_t = typename variant_base_impl<std::index_sequence_for<Types...>, Types...>::type;

/* Visitors for Member Functions */

template <class Base>
struct copy_assign
{
    Base &l_base;

    template <std::size_t LI, class LT, std::size_t RI, class RT>
    void operator()(wrapper<LI, LT> &l_wrapper, wrapper<RI, RT> const &r_wrapper)
    {
        if constexpr (LI == RI) {
            l_wrapper.unwrap() = r_wrapper.unwrap();
        } else if constexpr (std::is_nothrow_copy_constructible_v<RT>) {
            l_base.template emplace<RI>(r_wrapper.unwrap());
        } else if constexpr (std::is_nothrow_move_constructible_v<RT>) {
            l_base.template emplace<RI>(RT(r_wrapper.unwrap()));
        } else {
            auto backup = std::move(l_wrapper).backup();
            try {
                l_base.template emplace<RI>(r_wrapper.unwrap());
            } catch (...) {
                l_base.template emplace<LI>(std::move(backup));
                throw;
            }
        }
    }
};

template <class Base>
struct move_assign
{
    Base &l_base;

    template <std::size_t LI, class LT, std::size_t RI, class RT>
    void operator()(wrapper<LI, LT> &l_wrapper, wrapper<RI, RT> &&r_wrapper)
    {
        if constexpr (LI == RI) {
            l_wrapper.unwrap() = std::move(r_wrapper).unwrap();
        } else if constexpr (std::is_nothrow_move_constructible_v<RT>) {
            l_base.template emplace<RI>(std::move(r_wrapper).unwrap());
        } else {
            auto backup = std::move(l_wrapper).backup();
            try {
                l_base.template emplace<RI>(std::move(r_wrapper).unwrap());
            } catch (...) {
                l_base.template emplace<LI>(std::move(backup));
                throw;
            }
        }
    }
};

template <class Base, std::size_t RI, class RT, class T>
struct assign
{
    Base &l_base;
    T &&t;

    template <std::size_t LI, class LT>
    void operator()(wrapper<LI, LT> &l_wrapper)
    {
        if constexpr (LI == RI) {
            l_wrapper.unwrap() = std::forward<T>(t);
        } else if constexpr (std::is_nothrow_constructible_v<RT, T>) {
            l_base.template emplace<RI>(std::in_place, std::forward<T>(t));
        } else if constexpr (std::is_nothrow_move_constructible_v<RT>) {
            l_base.template emplace<RI>(RT(std::forward<T>(t)));
        } else {
            auto backup = std::move(l_wrapper).backup();
            try {
                l_base.template emplace<RI>(std::in_place, std::forward<T>(t));
            } catch (...) {
                l_base.template emplace<LI>(std::move(backup));
                throw;
            }
        }
    }
};

template <class Base, std::size_t RI, class RT, class ...Args>
struct do_emplace
{
    Base &l_base;
    std::tuple<Args &&...> args;

    template <std::size_t LI, class LT>
    RT &operator()(wrapper<LI, LT> &l_wrapper)
    {
        if constexpr (std::is_nothrow_constructible_v<RT, Args...>) {
            return std::apply(
                [this](Args &&...args) -> RT &
                {
                    return l_base.template emplace<RI>(std::in_place, std::forward<Args>(args)...).unwrap();
                },
                std::move(args)
                );
        } else {
            auto backup = std::move(l_wrapper).backup();
            try {
                return std::apply(
                    [this](Args &&...args) -> RT &
                    {
                        return l_base.template emplace<RI>(std::in_place, std::forward<Args>(args)...).unwrap();
                    },
                    std::move(args)
                    );
            } catch (...) {
                l_base.template emplace<LI>(std::move(backup));
                throw;
            }
        }
    }
};

template <class Base>
struct do_swap
{
    Base &l_base;
    Base &r_base;

    template <std::size_t LI, class LT, std::size_t RI, class RT>
    void operator()(wrapper<LI, LT> &l_wrapper, wrapper<RI, RT> &r_wrapper)
    {
        if constexpr (LI == RI) {
            using std::swap;
            swap(l_wrapper.unwrap(), r_wrapper.unwrap());
        } else if constexpr (std::is_nothrow_move_constructible_v<LT>) {
            auto l_tmp = std::move(l_wrapper).unwrap();
            if constexpr (std::is_nothrow_move_constructible_v<RT>) {
                l_base.template emplace<RI>(std::move(r_wrapper).unwrap());
            } else {
                try {
                    l_base.template emplace<RI>(std::move(r_wrapper).unwrap());
                } catch (...) {
                    l_base.template emplace<LI>(std::move(l_tmp));
                    throw;
                }
            }
            r_base.template emplace<LI>(std::move(l_tmp));
        } else if constexpr (std::is_nothrow_move_constructible_v<RT>) {
            auto r_tmp = std::move(r_wrapper).unwrap();
            try {
                r_base.template emplace<LI>(std::move(l_wrapper).unwrap());
            } catch (...) {
                r_base.template emplace<RI>(std::move(r_tmp));
                throw;
            }
            l_base.template emplace<RI>(std::move(r_tmp));
        } else {
            auto backup = std::move(l_wrapper).backup();
            try {
                l_base.template emplace<RI>(std::move(r_wrapper).unwrap());
            } catch (...) {
                l_base.template emplace<LI>(std::move(backup));
                throw;
            }
            r_base.template emplace<LI>(std::move(backup));
        }
    }
};

/* Construct from Various Types */

// overload
template <std::size_t I, class T>
struct overload_base
{
    std::integral_constant<std::size_t, I> operator()(T) const;
};

template <class Indices, class ...Types>
struct overload_impl;

template <std::size_t ...Indices, class ...Types>
struct overload_impl<std::index_sequence<Indices...>, Types...> :
    overload_base<Indices, Types>...
{
    using overload_base<Indices, Types>::operator()...;
};

template <class ...Types>
using overload = overload_impl<std::index_sequence_for<Types...>, Types...>;

// accepted_index
template <class Void, class T, class ...Types>
struct accepted_index_impl : size_t_<npos> {};

template <class T, class ...Types>
struct accepted_index_impl<std::void_t<decltype(overload<Types...>()(std::declval<T>()))>, T, Types...> :
    decltype(overload<Types...>()(std::declval<T>()))
{};

template <class T, class ...Types>
constexpr std::size_t accepted_index_v = accepted_index_impl<void, T, Types...>::value;

/* Miscellaneous */

// index_of
template <class T, class ...Types>
constexpr std::size_t index_of_v = count_v<T, Types...> == 1 ? find_v<T, Types...> : npos;

// is_in_place
template <class T>
struct is_in_place : std::false_type {};

template <class T>
struct is_in_place<std::in_place_type_t<T>> : std::true_type {};

template <std::size_t I>
struct is_in_place<std::in_place_index_t<I>> : std::true_type {};

template <class T>
constexpr bool is_in_place_v = is_in_place<T>::value;

/* Value Access */

template <std::size_t I>
struct get_impl
{
    template <class Variant>
    constexpr auto &&operator()(Variant &&v) const
    {
        return std::get<I>(std::forward<Variant>(v).base).unwrap();
    }
};

/* Visitation */

template <class Visitor>
struct do_visit
{
    Visitor &&visitor;

    template <class ...Wrappers>
    constexpr decltype(auto) operator()(Wrappers &&...wrappers)
    {
        return c_invoke(std::forward<Visitor>(visitor), std::forward<Wrappers>(wrappers).unwrap()...);
    }
};

struct visit_impl
{
    template <class Visitor, class ...Variants>
    constexpr decltype(auto) operator()(Visitor &&vis, Variants &&...vars) const
    {
        return std::visit(
            do_visit<Visitor>{std::forward<Visitor>(vis)},
            std::forward<Variants>(vars).base...
            );
    }
};

} // namespace variant_detail

template <class ...Types>
class variant;

} // namespace iorate

namespace std {

template <class ...Types>
struct variant_size<iorate::variant<Types...>> :
    iorate::variant_detail::size_t_<sizeof...(Types)>
{};

template <size_t I, class ...Types>
struct variant_alternative<I, iorate::variant<Types...>> :
    iorate::variant_detail::at<I, Types...>
{};

} // namespace std

namespace iorate {

#define DEFINE_VARIANT_RELATION(OP) \
template <class ...Types> \
constexpr bool operator OP(variant<Types...> const &lhs, variant<Types...> const &rhs) \
{ \
    return lhs.base OP rhs.base; \
} \
/**/
DEFINE_VARIANT_RELATION(==)
DEFINE_VARIANT_RELATION(!=)
DEFINE_VARIANT_RELATION(< )
DEFINE_VARIANT_RELATION(<=)
DEFINE_VARIANT_RELATION(> )
DEFINE_VARIANT_RELATION(>=)
#undef DEFINE_VARIANT_RELATION

template <class ...Types>
class variant :
    variant_detail::enable_default_constructor<
        std::is_default_constructible_v<variant_detail::at_t<0, Types...>>
        >,
    variant_detail::enable_copy_move<
        (std::is_copy_constructible_v<Types> && ...),
        (std::is_move_constructible_v<Types> && ...),
        ((std::is_copy_constructible_v<Types> && std::is_copy_assignable_v<Types>) && ...),
        ((std::is_move_constructible_v<Types> && std::is_move_assignable_v<Types>) && ...)
        >
{
    static constexpr std::size_t npos = variant_detail::npos;

    template <std::size_t I>
    using at_t = variant_detail::at_t<I, Types...>;

    template <class T>
    static constexpr std::size_t accepted_index_v = variant_detail::accepted_index_v<T, Types...>;

    template <class T>
    static constexpr std::size_t index_of_v = variant_detail::index_of_v<T, Types...>;

    using base_t = variant_detail::variant_base_t<Types...>;
    
    base_t base;

    template <std::size_t I, class ...Args>
    at_t<I> &emplace_impl(Args &&...args)
    {
        return std::visit(variant_detail::do_emplace<base_t, I, at_t<I>, Args...>{base, std::forward_as_tuple(std::forward<Args>(args)...)}, base);
    }

    template <std::size_t I>
    friend struct variant_detail::get_impl;

    friend struct variant_detail::visit_impl;

#define FRIEND_VARIANT_RELATION(OP) \
    friend constexpr bool operator OP <>(variant const &, variant const &); \
    /**/
    FRIEND_VARIANT_RELATION(==)
    FRIEND_VARIANT_RELATION(!=)
    FRIEND_VARIANT_RELATION(< )
    FRIEND_VARIANT_RELATION(<=)
    FRIEND_VARIANT_RELATION(> )
    FRIEND_VARIANT_RELATION(>=)
#undef FRIEND_VARIANT_RELATION

    friend struct std::hash<variant>;

public:
    constexpr variant() = default;

    variant(variant const &) = default;

    variant(variant &&) = default;

    template <
        class T,
        std::size_t I = accepted_index_v<T>,
        std::enable_if_t<
            !std::is_same_v<std::decay_t<T>, variant> &&
            !variant_detail::is_in_place_v<std::decay<T>> &&
            std::is_constructible_v<at_t<I>, T>
            > * = nullptr
        >
    constexpr variant(T &&t) noexcept(std::is_nothrow_constructible_v<at_t<I>, T>) :
        base(std::in_place_index<I>, std::in_place, std::forward<T>(t))
    {}

    template <
        class T, class ...Args,
        std::size_t I = index_of_v<T>,
        std::enable_if_t<
            std::is_constructible_v<T, Args...>
            > * = nullptr
        >
    constexpr explicit variant(std::in_place_type_t<T>, Args &&...args) :
        base(std::in_place_index<I>, std::in_place, std::forward<Args>(args)...)
    {}

    template <
        class T, class U, class ...Args,
        std::size_t I = index_of_v<T>,
        std::enable_if_t<
            std::is_constructible_v<T, std::initializer_list<U> &, Args...>
            > * = nullptr
        >
    constexpr explicit variant(std::in_place_type_t<T>, std::initializer_list<U> il, Args &&...args) :
        base(std::in_place_index<I>, std::in_place, il, std::forward<Args>(args)...)
    {}

    template <
        std::size_t I, class ...Args,
        std::enable_if_t<
            std::is_constructible_v<at_t<I>, Args...>
            > * = nullptr
        >
    constexpr explicit variant(std::in_place_index_t<I>, Args &&...args) :
        base(std::in_place_index<I>, std::in_place, std::forward<Args>(args)...)
    {}

    template <
        std::size_t I, class U, class ...Args,
        std::enable_if_t<
            std::is_constructible_v<at_t<I>, std::initializer_list<U> &, Args...>
            > * = nullptr
        >
    constexpr explicit variant(std::in_place_index_t<I>, std::initializer_list<U> il, Args &&...args) :
        base(std::in_place_index<I>, std::in_place, il, std::forward<Args>(args)...)
    {}

    ~variant() = default;

    variant &operator=(variant const &rhs)
    {
        std::visit(variant_detail::copy_assign<base_t>{base}, base, rhs.base);
        return *this;
    }

    variant &operator=(variant &&rhs)
    noexcept(((std::is_nothrow_move_constructible_v<Types> && std::is_nothrow_move_assignable_v<Types>) && ...))
    {
        std::visit(variant_detail::move_assign<base_t>{base}, base, std::move(rhs.base));
        return *this;
    }

    template <
        class T,
        std::size_t I = accepted_index_v<T>,
        std::enable_if_t<
            !std::is_same_v<std::decay_t<T>, variant> &&
            std::is_constructible_v<at_t<I>, T> &&
            std::is_assignable_v<at_t<I> &, T>
            > * = nullptr
        >
    variant &operator=(T &&t)
    noexcept(std::is_nothrow_constructible_v<at_t<I>, T> && std::is_nothrow_assignable_v<at_t<I> &, T>)
    {
        std::visit(variant_detail::assign<base_t, I, at_t<I>, T>{base, std::forward<T>(t)}, base);
        return *this;
    }

    template <
        class T, class ...Args,
        std::size_t I = index_of_v<T>,
        std::enable_if_t<
            std::is_constructible_v<T, Args...> &&
            I != npos
            > * = nullptr
        >
    T &emplace(Args &&...args)
    {
        return emplace_impl<I>(std::forward<Args>(args)...);
    }

    template <
        class T, class U, class ...Args,
        std::size_t I = index_of_v<T>,
        std::enable_if_t<
            std::is_constructible_v<T, std::initializer_list<U> &, Args...> &&
            I != npos
            > * = nullptr
        >
    T &emplace(std::initializer_list<U> il, Args &&...args)
    {
        return emplace_impl<I>(il, std::forward<Args>(args)...);
    }

    template <
        std::size_t I, class ...Args,
        std::enable_if_t<
            std::is_constructible_v<at_t<I>, Args...>
            > * = nullptr
        >
    std::variant_alternative_t<I, variant> &emplace(Args &&...args)
    {
        return emplace_impl<I>(std::forward<Args>(args)...);
    }

    template <
        std::size_t I, class U, class ...Args,
        std::enable_if_t<
            std::is_constructible_v<at_t<I>, std::initializer_list<U> &, Args...>
            > * = nullptr
        >
    std::variant_alternative_t<I, variant> &emplace(std::initializer_list<U> il, Args &&...args)
    {
        return emplace_impl<I>(il, std::forward<Args>(args)...);
    }

    constexpr bool valueless_by_exception() const noexcept
    {
        return false;
    }

    constexpr std::size_t index() const noexcept
    {
        return base.index();
    }

    void swap(variant &rhs)
    noexcept(((std::is_nothrow_swappable_v<Types> && std::is_nothrow_move_constructible_v<Types>) && ...))
    {
        std::visit(variant_detail::do_swap<base_t>{base, rhs.base}, base, rhs.base);
    }
};

template <class T, class ...Types>
constexpr bool holds_alternative(variant<Types...> const &v) noexcept
{
    constexpr std::size_t I = variant_detail::index_of_v<T, Types...>;
    static_assert(I != variant_detail::npos);
    return v.index() == I;
}

template <std::size_t I, class ...Types>
constexpr std::variant_alternative_t<I, variant<Types...>> &get(variant<Types...> &v)
{
    return variant_detail::get_impl<I>()(v);
}

template <std::size_t I, class ...Types>
constexpr std::variant_alternative_t<I, variant<Types...>> const &get(variant<Types...> const &v)
{
    return variant_detail::get_impl<I>()(v);
}

template <std::size_t I, class ...Types>
constexpr std::variant_alternative_t<I, variant<Types...>> &&get(variant<Types...> &&v)
{
    return variant_detail::get_impl<I>()(std::move(v));
}

template <std::size_t I, class ...Types>
constexpr std::variant_alternative_t<I, variant<Types...>> const &&get(variant<Types...> const &&v)
{
    return variant_detail::get_impl<I>()(std::move(v));
}

template <class T, class ...Types>
constexpr T &get(variant<Types...> &v)
{
    constexpr std::size_t I = variant_detail::index_of_v<T, Types...>;
    return get<I>(v);
}

template <class T, class ...Types>
constexpr T const &get(variant<Types...> const &v)
{
    constexpr std::size_t I = variant_detail::index_of_v<T, Types...>;
    return get<I>(v);
}

template <class T, class ...Types>
constexpr T &&get(variant<Types...> &&v)
{
    constexpr std::size_t I = variant_detail::index_of_v<T, Types...>;
    return get<I>(std::move(v));
}

template <class T, class ...Types>
constexpr T const &&get(variant<Types...> const &&v)
{
    constexpr std::size_t I = variant_detail::index_of_v<T, Types...>;
    return get<I>(std::move(v));
}

template <std::size_t I, class ...Types>
constexpr std::add_pointer_t<std::variant_alternative_t<I, variant<Types...>>>
get_if(variant<Types...> *v) noexcept
{
    return v != nullptr && v->index() == I ? std::addressof(get<I>(*v)) : nullptr;
}

template <std::size_t I, class ...Types>
constexpr std::add_pointer_t<std::variant_alternative_t<I, variant<Types...>> const >
get_if(variant<Types...> const *v) noexcept
{
    return v != nullptr && v->index() == I ? std::addressof(get<I>(*v)) : nullptr;
}

template <class T, class ...Types>
constexpr std::add_pointer_t<T> get_if(variant<Types...> *v) noexcept
{
    constexpr std::size_t I = variant_detail::index_of_v<T, Types...>;
    return get_if<I>(v);
}

template <class T, class ...Types>
constexpr std::add_pointer_t<T const> get_if(variant<Types...> const *v) noexcept
{
    constexpr std::size_t I = variant_detail::index_of_v<T, Types...>;
    return get_if<I>(v);
}

template <
    class ...Types,
    std::enable_if_t<
        ((std::is_move_constructible_v<Types> && std::is_swappable_v<Types>) && ...)
        > * = nullptr
    >
inline void swap(variant<Types...> &lhs, variant<Types...> &rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

template <class Visitor, class ...Variants>
constexpr decltype(auto) visit(Visitor &&vis, Variants &&...vars)
{
    return variant_detail::visit_impl()(std::forward<Visitor>(vis), std::forward<Variants>(vars)...);
}

} // namespace iorate

namespace std {

template <size_t I, class T>
struct hash<iorate::variant_detail::wrapper<I, T>>
{
    size_t operator()(iorate::variant_detail::wrapper<I, T> const &wrapper) const
    {
        return std::hash<T>()(wrapper.unwrap());
    }
};

template <class ...Types>
struct hash<iorate::variant<Types...>>
{
    size_t operator()(iorate::variant<Types...> const &v) const
    {
        return std::hash<typename iorate::variant<Types...>::base_t>()(v.base);
    }
};

} // namespace std

#endif
