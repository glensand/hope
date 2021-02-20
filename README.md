# Hope
[![Actions Status](https://github.com/glensand/hope/workflows/Hope/badge.svg)](https://github.com/glensand/hope/actions)
![GitHub last commit](https://img.shields.io/github/last-commit/glensand/hope?color=red&style=plastic)
![GitHub stars](https://img.shields.io/github/stars/glensand/hope?style=social)
![GitHub watchers](https://img.shields.io/github/watchers/glensand/hope?style=social)
![GitHub followers](https://img.shields.io/github/followers/glensand?style=social)
## Library purpose
Here are the things that I use or would like to use in my daily development. When I find an interesting technology, I immediately add it here. As a result, this repository will contain all the most interesting, in my opinion, things from the c ++ language. You can find here [my implementation](https://github.com/glensand/hope/blob/master/source/tuple/tuple_from_struct.h) of static reflection (based on magic-get idea) and wich used type loopholes. I also reworked some things from the Loki library, for example the [small object allocator](https://github.com/glensand/hope/tree/master/source/memory/small_object). Of course, here is the most important thing in c ++ in my opinion - the [list of types](https://github.com/glensand/hope/tree/master/source/typelist). With a variety of [examples](https://github.com/glensand/hope/tree/master/samples/typelist).
## System requirements
The bulk of the library runs in c ++ 14, some modules require c ++ 17. The library has no third-party dependencies, and uses stl to a minimum.
## Build
- Just clone the repository (using ``git clone`` for instance)
- Main part of the library is header only, only small object allocator is needed to be build
- Run ``cmake --build`` at the root directory
- Enjoy
## Examples
### Small objects
To use the allocator for small objects, you just need to inherit from small_object; And thats all! See [readme](https://github.com/glensand/hope/tree/master/source/memory/small_object) for more details.
``` c++
// client_side_code.h
#include <hope/memory/small_object/small_object.h>
class realy_small_object : public hope::memory::small_object{
    // ...
}
// source.cpp
auto* object = new really_small_object;
// ...
delete object;
```
### Tuples
To make tuple from struct you may use two variants of methods, *c++14* based and unsafe (used C-style cast) or *c++17* based on structured bindings. NOTE! Only POD objects is currently available, tuple does not guarantee proper work with complicated structs.
```c++
#include <hope/tuple/tuple_from_struct.h>

struct your_struct final {
    double i; float g; int k;
};
// ...
constexpr your_struct st{ 0.1, 0.1f, 11 };
constexpr auto test_tuple = hope::tuple_from_struct(st); // You may use constexpr; safe version
constexpr auto test_tuple_unsafe = hope::tuple_from_struct_unsafe(st); // potentially unsafe version
// You can print out tuples: 
std::cout << test_tuple << std::endl; // the result is:
// { 0.1, 0.1f, 11 };
```
### Typelist
For instance, you want to find out the real type (index) of your polymorphic object using dynamic_cast (Yeah, this is not a good practice...) but:
``` c++
struct base { virtual ~base(){}};
struct der1 : base{};
struct der2 : base{};
struct der3 : base{};

constexpr auto types = hope::typelist<der1, der2, der3>;
constexpr auto size = size(types);

template <typename T, std::size_t... Is>
std::size_t try_cast(T* object, std::index_sequence<Is...> seq){
    bool br[] = { (dynamic_cast<typename decltype(get_nth_type<Is>(types))::Type*>
                (link) != nullptr)... };
    for (std::size_t i{ 0 }; i < holder_size; ++i) {
        if (br[i])
            return i;
    }
    return size;
}

constexpr auto seq = std::make_index_sequence<size>();
const auto index = try_cast(link, seq);
```
[See](https://github.com/glensand/hope/tree/master/samples/typelist) for more typelst examples
