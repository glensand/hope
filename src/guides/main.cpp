#include <iostream>
#include <type_traits>
#include <functional>
#include "Function.h"

// TODO:: tests
template <typename T>
constexpr std::add_rvalue_reference_t<T> Declval() noexcept;

template <typename T>
constexpr auto __RemoveReference(T)
{
	return Declval<T>();
}

template <typename T>
using RemoveReference = decltype(__RemoveReference(Declval<T>()));

struct DefaultLessConstructor
{
	DefaultLessConstructor() = delete;
};

int main()
{
	using IntRef = int&;
	using IntRefRef = int&&;
	using CustomRef = DefaultLessConstructor&;
	
	//std::cout << std::is_reference_v<RemoveReference<IntRef>> << std::endl;
	//std::cout << std::is_reference_v<RemoveReference<IntRefRef>> << std::endl;
	//std::cout << std::is_reference_v<RemoveReference<DefaultLessConstructor>> << std::endl;

	using TestFunc = std::function<void(int)>;
	TestFunc a = [](int b) { return 1; };
	TestFunc b = [](float ) { return 1; };

	using TestFunction2 = Function<void(int)>;
	TestFunction2 f = [](int) {};

    const TestFunction2 f2 = [](int)
	{
		std::cout << "My custom std::function realization";
	};

	f2(1);

	TestFunction2 func3(a);
	TestFunction2 func4(b);

	func3(1);
	return 0;
}