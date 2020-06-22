#include <iostream>
#include <type_traits>
#include <functional>

#include "function/Function.h"
#include "tuple/FlatTuple.h"
#include "tuple/FlatTupleUniqueTypes.h"
#include "tuple/DetectFieldsCount.h"
#include "variant/Variant.h"
#include "typelist/typelistsort.h"
#include "tuple/TupleFromStruct.h"
#include "tuple/generated.h"
#include "tuple/FlatSortedTuple.h"

#include <string_view>
#include <variant>

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

template <std::size_t, typename T>
struct type_by_index {
	using type = T;
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

	FlatTupleUniqueTypes<int, float, double> tuple{};
	FlatTupleUniqueTypes<int, float, double> tuple2(10, 15.f, 20.0);

	// TODO:: implement completely constexpr compile-time tuple
	//static_assert(tuple2.Get<1>() == 1.f);

	auto&& floatValue = tuple.Get<float>();
	floatValue = 11.0;

	const auto& sec = tuple.Get<1>();
	std::cout << sec << std::endl;
	static_assert(std::is_same_v<decltype(tuple.Get<1>()), float&>);

	auto&& floatValue2 = tuple.Get<float>();
	std::cout << floatValue2 << std::endl;

	auto expandedTuple = PushFront(std::string_view("const"), tuple2);
	std::cout << expandedTuple.Get<float>() << std::endl;
	std::cout << expandedTuple.Get<int>() << std::endl;
	std::cout << expandedTuple.Get<double>() << std::endl;
	std::cout << expandedTuple.Get<0>() << std::endl;

	std::cout << tuple << std::endl
    << tuple2 << std::endl
    << expandedTuple << std::endl;

    std::variant<std::string, float, double> variant = 1.0;
	variant = "lol";
	std::cout << std::get<0>(variant) << std::endl;

	FlatTuple<int, float, double> flatTuple(1, 1.2f, 1.0);
	std::cout << "FlatTuple: " << flatTuple.Get<1>() << std::endl;
	std::cout << "FlatTuple: " << flatTuple.Get<2>() << std::endl;
	std::cout << "FlatTuple: " << flatTuple.Get<0>() << std::endl;
	std::cout << "FlatTuple: " << flatTuple << std::endl;

	Variant<std::string, double, float> myVariant(1.34534534f);

	
	myVariant = std::string("Lol");
	std::cout << myVariant.Get<std::string>() << std::endl;

    const TypeList<int, float, double> list;
	constexpr TypeList sortedList = Sort(list);
	using FirstType = typename decltype(GetNthType<0>(sortedList))::Type;
	static_assert(std::is_same_v<FirstType, double>);

	struct SimpleStruct
	{
		int x;
		double y;
		float z;
		float zz;
	};

	std::cout << sizeof(SimpleStruct) << std::endl;
	std::cout << sizeof(FlatTuple<int, double, float, float>) << std::endl;
	
    constexpr SimpleStruct ss = { 1, 1., 1.f, 2222.f};

	constexpr auto fieldsCount = DetectFieldsCount(ss);
	/*const auto tupleFromStruct = */TupleFromStructUnsafe(ss);
	//constexpr std::size_t fieldsCount = DetectFieldsCount(ss);
	//std::cout << fieldsCount << std::endl;

 //   struct ZeroFields
 //   {
 //       
 //   };

	std::size_t types_[4];
	using Unic = Detail::Unsafe::AnyConvertible<1>;

	//constexpr SimpleStruct ss2{ Unic(types_) };

	constexpr auto strTuple = TupleFromStruct(ss);

	std::cout << strTuple << std::endl;

	constexpr auto sortedTuple = MakeSortedTuple(false, false, 'c', 1.0, false, 4);
	constexpr auto nonSortedTuple = MakeFlatTuple(false, false, 'c', 1.0, false, 4);

	std::cout << sizeof(sortedTuple) << " " << sizeof(nonSortedTuple) << std::endl;
	std::cout << sortedTuple << " " << nonSortedTuple << std::endl;

	return 0;
} 