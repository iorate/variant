
// iorate::variant
//
// Copyright iorate 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <string>
#include <variant>
#include "variant.hpp"

int main()
{
    struct S
    {
        operator int() const { throw 42; }
    };

    std::variant<std::string, int> sv = "Hello, world!";
    try {
        sv.emplace<1>(S());
    } catch (int) {
    }
    if (sv.valueless_by_exception()) {
        std::cout << "sv has no value\n";
    }

    iorate::variant<std::string, int> iv = "Hello, world!";
    try {
        iv.emplace<1>(S());
    } catch (int) {
    }
    std::cout << "iv has a value: " << iorate::get<0>(iv) << '\n';
}
