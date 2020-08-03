#include <iostream>
#include <type_traits>
#include <functional>

#include "function/function.h"
#include "tuple/flat_tuple.h"
#include "tuple/flat_tuple_dummy.h"
#include "tuple/detect_fields_count.h"
#include "variant/variant.h"
#include "typelist/typelistsort.h"
#include "tuple/tuple_from_struct.h"
#include "tuple/generated.h"
#include "tuple/flat_sorted_tuple.h"
#include "core/static_string.h"

#include <string_view>
#include <variant>

int main()
{
	using TestFunc = std::function<void(int)>;
	TestFunc a = [](int b) { return 1; };
	TestFunc b = [](float ) { return 1; };

	using TestFunction2 = hope::function<void(int)>;
	TestFunction2 f = [](int) {};

    const TestFunction2 f2 = [](int)
	{
		std::cout << "My custom std::function realization" << std::endl;
	};

	f2(1);

	TestFunction2 func3(a);
	TestFunction2 func4(b);

	func3(1);

    hope::flat_tuple_dummy<int, float, double> tuple{};
    hope::flat_tuple_dummy<int, float, double> tuple2(10, 15.f, 20.0);

	// TODO:: implement completely constexpr compile-time tuple
	//static_assert(tuple2.get<1>() == 1.f);

	auto&& floatValue = tuple.get<float>();
	floatValue = 11.0;

	const auto& sec = tuple.get<1>();
	std::cout << sec << std::endl;
	static_assert(std::is_same_v<decltype(tuple.get<1>()), float&>);

	auto&& floatValue2 = tuple.get<float>();
	std::cout << floatValue2 << std::endl;

	auto expandedTuple = push_front(std::string_view("const"), tuple2);
	std::cout << expandedTuple.get<float>() << std::endl;
	std::cout << expandedTuple.get<int>() << std::endl;
	std::cout << expandedTuple.get<double>() << std::endl;
	std::cout << expandedTuple.get<0>() << std::endl;

	std::cout << tuple << std::endl
    << tuple2 << std::endl
    << expandedTuple << std::endl;

    std::variant<std::string, float, double> variant = 1.0;
	variant = "lol";
	std::cout << std::get<0>(variant) << std::endl;

	hope::flat_tuple<int, float, double> flatTuple(1, 1.2f, 1.0);
	std::cout << "FlatTuple: " << flatTuple.get<1>() << std::endl;
	std::cout << "FlatTuple: " << flatTuple.get<2>() << std::endl;
	std::cout << "FlatTuple: " << flatTuple.get<0>() << std::endl;
	std::cout << "FlatTuple: " << flatTuple << std::endl;

	hope::variant<std::string, double, float> myVariant(1.34534534f);

	
	myVariant = std::string("Lol");
	std::cout << myVariant.get<std::string>() << std::endl;

    const hope::type_list<int, float, double> list;
	constexpr hope::type_list sortedList = sort(list);
	using FirstType = typename decltype(hope::get_nth_type<0>(sortedList))::Type;
	static_assert(std::is_same_v<FirstType, double>);

	struct SimpleStruct
	{
		int x;
		double y;
		float z;
		float zz;
	};

	std::cout << sizeof(SimpleStruct) << std::endl;
	std::cout << sizeof(hope::flat_tuple<int, double, float, float>) << std::endl;
	
    constexpr SimpleStruct ss = { 1, 1., 1.f, 2222.f};

	constexpr auto fieldsCount = hope::detect_fields_count(ss);
	/*const auto tupleFromStruct = */
   // hope::tuple_from_struct_unsafe(ss);
	//constexpr std::size_t fieldsCount = DetectFieldsCount(ss);
	//std::cout << fieldsCount << std::endl;

 //   struct ZeroFields
 //   {
 //       
 //   };

	std::size_t types_[4];
	using Unic = hope::detail::unsafe::any_convertible<1>;

	//constexpr SimpleStruct ss2{ Unic(types_) };

	constexpr auto strTuple = hope::tuple_from_struct(ss);

	std::cout << strTuple << std::endl;

	constexpr auto sortedTuple = hope::make_sorted_tuple(false, false, 'c', 1.0, false, 4);
	constexpr auto nonSortedTuple = hope::make_flat_tuple(false, false, 'c', 1.0, false, 4);

	std::cout << sizeof(sortedTuple) << " " << sizeof(nonSortedTuple) << std::endl;
	std::cout << sortedTuple << " " << nonSortedTuple << std::endl;

	constexpr auto static_string = gl::make_static_string("static_string");
	constexpr auto static_string2 = gl::make_static_string("static_string2");
	constexpr auto string2 = gl::concat(static_string, static_string2);
	std::cout << string2;
	return 0;
} 