# iorate::variant
A variant class template with the "Never-Empty" guarantee

## Description
The `iorate::variant` class template is a discriminated union container offering the "Never-Empty" guarantee. It is built on `std::variant` of C\++17, and has almost the same interface as `std::variant` (except that it does not support custom allocators).
`iorate::variant` realizes the "Never-Empty" guarantee by the temporary backup technique. Before a type-changing operation which may throw is performed, the old value is moved for backup. If the operation throws, the old value is restored from backup. Dynamic memory allocation is performed if and only if the old type is not nothrow-move-constructible.

## Example
```cpp
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
```

## Requirements
A complier that supports C\++17. `iorate::variant` is tested under g\++ 7.1.0.

## Usage
Just copy `variant.hpp` to your include directory, and include it. `iorate::variant` can be used just like `std::variant`, except that it does not support custom allocators now.

## Author
[@iorate](https://twitter.com/iorate)

## License
[Boost Software License 1.0](http://www.boost.org/LICENSE_1_0.txt)
