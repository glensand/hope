#include <iostream>
#include <type_traits>
#include <functional>

#include "Function/Function.h"
#include "Tuple/Tuple2.h"

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

template <typename T>
struct Add
{
    
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
		std::cout << "My custom std::function realization" << std::endl;
	};

	f2(1);

	TestFunction2 func3(a);
	TestFunction2 func4(b);

	func3(1);

	Tuple<int, float, double> tuple{};
	Tuple<int, float, double> tuple2(1, 1.f, 1.0);

	// TODO:: implement completely constexpr compile-time tuple
	//static_assert(tuple2.Get<1>() == 1.f);

	auto&& floatValue = tuple.Get<float>();
	floatValue = 11.0;

	const auto& sec = tuple.Get<1>();
	std::cout << sec << std::endl;
	static_assert(std::is_same_v<decltype(tuple.Get<1>()), float&>);

	auto&& floatValue2 = tuple.Get<float>();
	std::cout << floatValue2 << std::endl;

	return 0;
}