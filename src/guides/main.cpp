#include <iostream>
#include <type_traits>

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
	
	std::cout << std::is_reference_v<RemoveReference<IntRef>> << std::endl;
	std::cout << std::is_reference_v<RemoveReference<IntRefRef>> << std::endl;
	std::cout << std::is_reference_v<RemoveReference<DefaultLessConstructor>> << std::endl;

	return 0;
}